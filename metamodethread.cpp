#include "metamodethread.h"

MetaModeThread::MetaModeThread(QObject *parent) : QObject(parent)
{
    timer_Meta_singleShot = new QTimer(this);
    timer_Meta_singleShot->setSingleShot(true);
    connect(timer_Meta_singleShot, SIGNAL(timeout()), this, SLOT(rx_timer_Meta_singleShot()));

    metaProcess = new QProcess(this);
    metaProcess->setProgram(cmdShellPath);
    metaProcess->setWorkingDirectory(m3FolderPath);
    connect(metaProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(rx_CommandLine_OutPutReceived()));
    connect(metaProcess, SIGNAL(readyReadStandardError()), this, SLOT(rx_CommandLine_ErrorReceived()));
    connect(metaProcess, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(rx_CommandLine_ErrorReceived()));



}

void MetaModeThread::setGeneralIndex_metaMode(int idx)
{
    generalIndex = idx;
}
void MetaModeThread::rx_StartRepairing_metaMode(int idx, bool startStop, TOOL_TYPE tool)
{
    if(generalIndex != idx) {
        //qDebug()<<" Error in Signal Slot:: setIndx:"<<generalIndex<<" rec: "<<idx;
        return;
    }

    currentToolType = tool;
    showTheConsole = false;
    qDebug()<<" Signal received:  rx_StartRepairing "<<startStop<<" process: "<<idx;
    processStartStop = startStop;
    currentCommand = MMM_Idle;
    if(startStop){
        commandlineOutPut = "";
        commandError = "";
        //qDebug()<<"Serial Port Name: "<<GlobalVars::serialPortName ;


        if(currentToolType == Tool_MTK){
            if(!getServerIMEInumber_updateGlobal()) return;

            if(GlobalVars::meta_dual_imei_bool[generalIndex]){
                commandStr = QString("MMM.exe -k " + GlobalVars::meta_serialPortName[generalIndex]  + " -odm1 ontim -project " + GlobalVars::meta_projectName[generalIndex] + " -m user -t write_imei ds " + GlobalVars::meta_imei_1[idx] + " " + GlobalVars::meta_imei_2[idx] + " dummy_security_key -skip");
            }else {
                commandStr = QString("MMM.exe -k " + GlobalVars::meta_serialPortName[generalIndex] + " -odm1 ontim -project " + GlobalVars::meta_projectName[generalIndex] + " -m user -t write_imei ss " + GlobalVars::meta_imei_1[idx] + " dummy_security_key -skip");
            }
            qDebug()<<" MTK MMM_Repair Command: "<<commandStr;
            emit tx_miscOperations_metaMode(currentToolType, generalIndex, 5, "");
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Start process Write MTK ImeI: OK", false, false, GlobalVars::txtOutPutColor);
        }
        else if(currentToolType == Tool_MTK_UnLock){
            //commandStr = QString("MMM.exe -k "+GlobalVars::meta_serialPortName[generalIndex]+" -odm1 ontim -project "+GlobalVars::meta_projectName[generalIndex] +" -m user -t write_simlock "+ simUnLock_G8PowerLite +" -skip").toUtf8();
            commandStr = QString("MMM.exe -k "+GlobalVars::meta_serialPortName[generalIndex]+" -odm1 ontim -project "+GlobalVars::meta_projectName[generalIndex] +" -m user -t write_simlock_unlock N/A -skip");
            qDebug()<<" MTK-UnLock Command: "<<commandStr;
            emit tx_miscOperations_metaMode(currentToolType, generalIndex, 5, "");
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Start process Unlock: OK", false, false, GlobalVars::txtOutPutColor);
        }




        shellArguments.clear();
        shellArguments.append("/C");
        //shellArguments.append("ipconfig");
        shellArguments.append(commandStr);

        metaProcess->setArguments(shellArguments);

        qDebug()<<"\n\n \t Process Opening: "<<metaProcess->open(QIODevice::ReadWrite);

        if (metaProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug() << "external program running";

        metaProcess->waitForReadyRead(15000);
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 10, "");

    }
    if(processStartStop ==  false){
        try {
            currentCommand = MMM_Exit;
            metaProcess->close();
            if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
            timer_Meta_singleShot->start(500);

        } catch (const std::bad_alloc &) { }
        return;
    }
}

void MetaModeThread::rx_CommandLine_ErrorReceived()
{
    commandError = metaProcess->readAllStandardError();
    qDebug()<<"\n\n ###################################################### \n rx_CommandLine_ErrorReceived:: "<<commandError;
}
void MetaModeThread::rx_CommandLine_OutPutReceived()
{
    if(processStartStop ==  false){
        return;
    }

    commandlineOutPut = metaProcess->readAllStandardOutput();
    //if(showTheConsole)
    //qDebug()<<"\n\n ---------------------------------------------------- \n rx_CommandLine_OutPutReceived: "<<commandlineOutPut;

    if(currentToolType == Tool_MTK)
    {
        check_Repair_Cmd_outPut(commandlineOutPut);
    }
    else if(currentToolType == Tool_MTK_UnLock)
    {
        check_UnLock_Cmd_outPut(commandlineOutPut);
    }

}
void MetaModeThread::check_Repair_Cmd_outPut(QString str)
{

    if (commandlineOutPut.contains("SEND_HASH_GPSODM"))
    {
        //qDebug()<<" \n\n\n Out:: "<<commandlineOutPut.length();
        int ind = commandlineOutPut.lastIndexOf("SEND_HASH_GPSODM");
        commandlineOutPut = commandlineOutPut.right(commandlineOutPut.length() - ind);
        //qDebug()<<" \n\n\n Out:: "<<commandlineOutPut;
        //qDebug()<<" \n\n\n ";
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 15, "");
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Getting Authorization From Server: OK", false, false, GlobalVars::txtOutPutColor);
        QStringList strList = commandlineOutPut.split('"');
        //qDebug()<<" \n\n : StrList: "<<strList;

        QString data = strList[3];
        qDebug()<<" \n\n : Data for Authentication: "<<data;

        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 50, "");
        getAuthenticationFromServer(data, GlobalVars::meta_imei_1[generalIndex]);
        //showThisinDebug = true;


        currentCommand = MMM_AuthenticationBytes;
        timer_Meta_singleShot->start(500);
    }
    else if (commandlineOutPut.contains("Motorola Modem Meta Tool Started"))
    {
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, "Processing ", false, false, GlobalVars::txtOutPutColor);
    }
    else if (commandlineOutPut.contains("Motorola Modem Meta Tool Passed."))
    {

        currentCommand = MMM_OK_AfterAuthenticationBytes;
        timer_Meta_singleShot->start(500);

    }
    else if (commandlineOutPut.contains("Motorola Modem Meta Tool Failed"))
    {
        currentCommand = MMM_Exit;
        if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
        timer_Meta_singleShot->start(500);
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, "Task Failed", true, false, QColor::fromRgb(255, 192, 192));

    }
    else if (commandlineOutPut.contains("Motorola Modem Meta Tool Output"))
    {
        qDebug()<<"Motorola Modem Meta Tool Output::  ";
    }
    else if(commandlineOutPut.contains("Motorola Modem Meta Tool LogInfo. exit")) // || commandlineOutPut.contains("Motorola Modem Meta Tool LogInfo. Trying to Communicate to ComPort"))
    {
        qDebug()<<"\n\n\n Motorola Modem Meta Tool LogInfo. exit \n\n ";

        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex,"Error in COM", true, false , Qt::red);
        emit tx_miscOperations_metaMode(currentToolType,  generalIndex, 100, "");
        currentCommand = MMM_Exit;
        if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
        timer_Meta_singleShot->start(500);
    }
    else if(commandlineOutPut.contains("A device which does not exist was specified")) {
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex,"Device is not specified.", true, false , Qt::red);
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
        currentCommand = MMM_Exit;
        if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
        timer_Meta_singleShot->start(500);
    }
    else if(commandlineOutPut.contains("FrameComm-> Error in writing COM") || \
            commandlineOutPut.contains("SP_META_InternalConnectByUSB_r(COM4):CreateFile") || \
            commandlineOutPut.contains("ERROR: <0> SP_META_InternalConnectByUSB_r(COM4)"))
    {
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex,"Comm port is not working properly.", true, false , Qt::red);
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
        currentCommand = MMM_Exit;
        if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
        timer_Meta_singleShot->start(500);
    }
    /*
    else if(commandlineOutPut.contains("IMetaFrameCommBase::Disconnect: FrameCommModule stopped")) // || commandlineOutPut.contains("OS & Port Version End======================================================"))
    {
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex,"Phone Disconnected. Please plugin again and start the process.", true, false , Qt::red);
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
        currentCommand = MMM_Exit;
        if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
        timer_Meta_singleShot->start(500);
    }
*/
}
void MetaModeThread::check_UnLock_Cmd_outPut(QString str)
{

    if (commandlineOutPut.contains("SEND_HASH_GPSODM"))
    {
        //qDebug()<<" \n\n\n Out:: "<<commandlineOutPut.length();
        int ind = commandlineOutPut.lastIndexOf("SEND_HASH_GPSODM");
        commandlineOutPut = commandlineOutPut.right(commandlineOutPut.length() - ind);
        //qDebug()<<" \n\n\n Out:: "<<commandlineOutPut;
        //qDebug()<<" \n\n\n ";
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 15, "");
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Getting Authorization Unlock Data: OK", false, false, GlobalVars::txtOutPutColor);
        QStringList strList = commandlineOutPut.split('"');
        //qDebug()<<" \n\n : StrList: "<<strList;

        QString data = strList[3];
        qDebug()<<" \n\n : Data for Authentication: "<<data;

        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 50, "");
        getAuthenticationFromServer(data, GlobalVars::meta_imei_1[generalIndex]);
        //showThisinDebug = true;


        currentCommand = MMM_AuthenticationBytes;
        timer_Meta_singleShot->start(500);
    }
    else if (commandlineOutPut.contains("Motorola Modem Meta Tool Started"))
    {
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, "Processing ", false, false, GlobalVars::txtOutPutColor);
    }
    else if (commandlineOutPut.contains("Motorola Modem Meta Tool Passed."))
    {
        currentCommand = MMM_OK_AfterAuthenticationBytes;
        timer_Meta_singleShot->start(500);
    }
    /*
    else if (commandlineOutPut.contains("Motorola Modem Meta Tool Failed"))
    {
        currentCommand = MMM_Exit;
        if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
        timer_Meta_singleShot->start(500);
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, "Task Failed", true, false, QColor::fromRgb(255, 192, 192));

    }  */
    else if (commandlineOutPut.contains("Motorola Modem Meta Tool Output"))
    {
        qDebug()<<"\n\n\n\n\n\n Motorola Modem Meta Tool Output::  \n\n\n\n\n";
    }
    else if(commandlineOutPut.contains("Motorola Modem Meta Tool LogInfo. exit")) // || commandlineOutPut.contains("Motorola Modem Meta Tool LogInfo. Trying to Communicate to ComPort"))
    {
        qDebug()<<"\n\n\n Motorola Modem Meta Tool LogInfo. exit \n\n ";

        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex,"Error in COM", true, false , Qt::red);
        emit tx_miscOperations_metaMode(currentToolType,  generalIndex, 100, "");
        currentCommand = MMM_Exit;
        if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
        timer_Meta_singleShot->start(500);
    }
    else if(commandlineOutPut.contains("A device which does not exist was specified")) {
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex,"Device is not specified.", true, false , Qt::red);
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
        currentCommand = MMM_Exit;
        if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
        timer_Meta_singleShot->start(500);
    }
    else if(commandlineOutPut.contains("FrameComm-> Error in writing COM") || \
            commandlineOutPut.contains("SP_META_InternalConnectByUSB_r(COM4):CreateFile") || \
            commandlineOutPut.contains("ERROR: <0> SP_META_InternalConnectByUSB_r(COM4)"))
    {
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex,"Comm port is not working properly.", true, false , Qt::red);
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
        currentCommand = MMM_Exit;
        if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
        timer_Meta_singleShot->start(500);
    }
}

void MetaModeThread::rx_timer_Meta_singleShot()
{
    switch (currentCommand) {
    case MMM_Idle:{
        break;
    }
    case MMM_AuthenticationBytes:{
        metaProcess->write(authenReply);
        metaProcess->write(" \n");
        metaProcess->closeWriteChannel();
        metaProcess->waitForBytesWritten(1000);
        metaProcess->waitForReadyRead(15000);

        break;
    }
    case MMM_OK_AfterAuthenticationBytes:{
        metaProcess->write("OK ");
        metaProcess->write("\n");
        metaProcess->closeWriteChannel();
        metaProcess->waitForBytesWritten(1000);
        metaProcess->waitForReadyRead(15000);
        emit tx_miscOperations_metaMode(currentToolType,  generalIndex, 80, "");
        if(currentToolType == Tool_MTK) {
            emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, QString("Verify ImeI: OK"), false, false, GlobalVars::txtOutPutColor);
        } else if(currentToolType == Tool_MTK_UnLock) {
            emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, QString("Verify Unlock: OK"), false, false, GlobalVars::txtOutPutColor);
        }

        if (GlobalVars::meta_reboot_bool[generalIndex])
        {
            currentCommand = MMM_Rebooting;
            timer_Meta_singleShot->start(3000);
        }else{
            emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, "Process Successfull Completed", true, false , GlobalVars::txtOutPutColor);
            emit tx_miscOperations_metaMode(currentToolType,  generalIndex, 100, "Success");
            currentCommand = MMM_Exit;
            timer_Meta_singleShot->start(3000);
        }
        break;
    }
    case MMM_Rebooting:{
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, "Rebooting: OK", true, false, Qt::white);
        QByteArray rebootCmd(QString("MMM.exe -k " + GlobalVars::meta_serialPortName[generalIndex] + " -odm1 ontim -project " + GlobalVars::meta_projectName[generalIndex] + " -m user -reboot -skipMD").toUtf8());

        //process->write(QByteArray(QString("MMM.exe -k " + GlobalVars::serialPortName + "  -odm1 ontim -project  " + GlobalVars::projectName + "  -m user -reboot -skipMD")));
        metaProcess->write(rebootCmd);
        metaProcess->write(" \n");
        metaProcess->closeWriteChannel();
        metaProcess->waitForBytesWritten(1000);
        metaProcess->waitForReadyRead(15000);


        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, "Process Successfull Completed", true, false , GlobalVars::txtOutPutColor);
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "Success");
        currentCommand = MMM_Exit;
        timer_Meta_singleShot->start(3000);
        break;
    }
    case MMM_Exit:{
        currentCommand = MMM_Idle;
        if(metaProcess->isWritable()){
            metaProcess->write("exit ");
            metaProcess->write("\n");
            metaProcess->closeWriteChannel();
            metaProcess->waitForBytesWritten(1000);
            metaProcess->waitForReadyRead(100);
        }
        metaProcess->terminate();
        metaProcess->kill();
        metaProcess->close();
        break;
    }
    } // end of sub switch statements

}

void MetaModeThread::getAuthenticationFromServer(QString strData, QString strIMEI)
{
    qDebug()<<"getAuthenticationFromServer ::  Data:"<<strData<<" IMEI:"<<strIMEI;

    QByteArray postdata;// = data.toJson();
    postdata.append(QString("IMEI="+ strIMEI+"&").toUtf8());
    postdata.append(QString("DATA="+ strData+"&").toUtf8());
    postdata.append(QString("pname="+ GlobalVars::meta_projectName[generalIndex]+"&").toUtf8());
    if(currentToolType == Tool_MTK){
        postdata.append(QString("TYPE="+ GlobalVars::meta_projectDataType[generalIndex]+"&").toUtf8());
    }
    else if(currentToolType == Tool_MTK_UnLock){
        postdata.append(QString("TYPE="+ GlobalVars::meta_projectUnlockType[generalIndex]+"&").toUtf8());
    }
    //postdata.append("sysID="+QSysInfo::machineUniqueId());

    emit tx_miscOperations_metaMode(currentToolType, generalIndex, 30, "");
    emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("Model: "+GlobalVars::meta_modelStr[generalIndex]), false, false, GlobalVars::txtOutPutColor);
    //qDebug()<<" Json Object: "<<data<<" \n\n ByteArray:"<<postdata;
    qDebug()<<" \n\n ByteArray:"<<postdata;
    if(processStartStop ==  false){
        return;
    }
    QEventLoop loop;
    QNetworkAccessManager nam;
    QNetworkRequest req; //(QUrl("https://server51214110.ngrok.io/auth.php"));
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    //config.setProtocol(QSsl::SslV2);
    //req.setSslConfiguration(config);
    req.setUrl(QUrl("http://server51214110.ngrok.io/auth.php"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = nam.post(req, postdata);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if(processStartStop ==  false){
        return;
    }
    emit tx_miscOperations_metaMode(currentToolType, generalIndex, 45, "");
    if(currentToolType == Tool_MTK){
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("ImeI: " + GlobalVars::meta_imei_1[generalIndex]), false, false, GlobalVars::txtOutPutColor);
        if(GlobalVars::meta_dual_imei_bool[generalIndex]) emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("ImeI-2: "+GlobalVars::meta_imei_2[generalIndex]), false, false, GlobalVars::txtOutPutColor);
    }
    QByteArray buffer = reply->readAll();
    //qDebug()<<" reply:: "<<buffer;
    authenReply = buffer;
    reply->deleteLater();
    if(processStartStop ==  false){
        return;
    }
    qDebug()<<"\n\n\n response_data::"<<authenReply;
    qDebug()<<"\n\n";

    if (authenReply.contains("success"))
    {
        authenReply = authenReply.replace("success: ", "");
    }
    if (authenReply.toLower().contains("error"))
    {
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Process Failed", true, false, QColor::fromRgb(255, 192, 192));
        return;
    }
    if (authenReply.toLower().contains("fail"))
    {
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Process Failed", true, false, QColor::fromRgb(255, 192, 192));
        return;
    }
    emit tx_miscOperations_metaMode(currentToolType, generalIndex, 60, "");
    if(currentToolType == Tool_MTK){
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("Writing ImeI: OK"), false, false, GlobalVars::txtOutPutColor);
    } else if (currentToolType == Tool_MTK_UnLock) {
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("Writing Unlock Data: OK"), false, false, GlobalVars::txtOutPutColor);
    }
    //qDebug()<<" Writing this code in cmd: "<<authenReply;
}

bool MetaModeThread::getServerIMEInumber_updateGlobal()
{
    if(!GlobalVars::meta_manual_imei_bool[generalIndex])
    {
        QByteArray postdata;// = data.toJson();
        postdata.append(QString("model="+GlobalVars::meta_projectName[generalIndex]+"&").toUtf8());
        postdata.append(QString("simCode="+QString(GlobalVars::meta_dual_imei_bool[generalIndex]? "ds":"ss")).toUtf8());

        QByteArray encypted;
        encypted.append("encrypt=2&");
        encypted.append(QString("data="+QCryptographicHash::hash(postdata, QCryptographicHash::Sha3_256).toUpper().toHex()).toUtf8());

        qDebug()<<" \n\n IMEI ByteArray:"<<postdata<<" \n Encrypted: "<<encypted;

        if(processStartStop ==  false){
            return false;
        }
        QEventLoop loop;
        QNetworkAccessManager nam;
        QNetworkRequest req;
        req.setUrl(QUrl("http://107.175.87.146:16777/imeiGenerator.php"));
        //req.setUrl((QUrl("http://111.88.11.37:1500/imeiGenerator.php")));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QNetworkReply *reply = nam.post(req, encypted);
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if(processStartStop ==  false){
            return false;
        }
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 40, "");
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("Geting IMEI from Server: OK"), false, false, GlobalVars::txtOutPutColor);

        QByteArray buffer = reply->readAll();
        //qDebug()<<" reply:: "<<buffer;
        reply->deleteLater();
        if(processStartStop ==  false){
            return false;
        }
        qDebug()<<"\n\n\n IMEI response_data::"<<buffer;
        qDebug()<<"\n\n";

        if (buffer.toLower().contains("fail"))
        {
            emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Faided to get IMEI from Server", true, false, QColor::fromRgb(255, 192, 192));
            return false;
        }
        if (GlobalVars::meta_dual_imei_bool[generalIndex]) //(buffer.toLower().contains("&"))
        {
            int idx = buffer.indexOf("&");
            if(idx < 5) {
                emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
                emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Faided to get IMEI from Server", true, false, QColor::fromRgb(255, 192, 192));
                return false;
            }
            GlobalVars::meta_imei_1[generalIndex] = buffer.left(idx);
            buffer.remove(0, (idx+1));
            if(buffer.length() > 5){

                idx = buffer.indexOf("\\");
                qDebug()<<" second IMEI is: "<<buffer<<" idx: "<<idx;
                if(idx <= 0)
                {
                    GlobalVars::meta_imei_2[generalIndex] = buffer;
                }
                else
                {
                    GlobalVars::meta_imei_2[generalIndex] = buffer.left(idx);
                }
            }
        }
        else
        {
            GlobalVars::meta_imei_1[generalIndex] = buffer;
        }
        //qDebug()<<" Writing this code in cmd: "<<authenReply;
    }

    return true;
}




