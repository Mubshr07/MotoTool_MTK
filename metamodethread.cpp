#include "metamodethread.h"

MetaModeThread::MetaModeThread(QObject *parent) : QObject(parent)
{
    timer_Meta_singleShot = new QTimer(this);
    timer_Meta_singleShot->setSingleShot(true);
    connect(timer_Meta_singleShot, SIGNAL(timeout()), this, SLOT(rx_timer_Meta_singleShot()));
    timer_AppMaxTime = new QTimer(this);
    timer_AppMaxTime->setSingleShot(true);
    connect(timer_AppMaxTime, SIGNAL(timeout()), this, SLOT(on_timer_AppMaxTime_Elapsed()));

    //qDebug()<<" Timer AppMaTime::: "<<timer_AppMaxTime->isActive();

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
        else if(currentToolType == Tool_META_CarrierFix){



            QByteArray postdata;
            QJsonObject obj;
            obj["OperationName"] = "CarrierFix";
            obj["DeviceName"] = GlobalVars::meta_modelStr[generalIndex];
            QJsonDocument doc(obj);
            postdata = doc.toJson();
            //qDebug()<<" \n\n ByteArray:"<<postdata<<" Base64::"<<postdata.toBase64();
            if(processStartStop ==  false){ return; }
            QEventLoop loop;
            QNetworkAccessManager nam;
            QNetworkRequest req;
            req.setUrl(QUrl(GlobalVars::api_MTKcharges));
            req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            req.setRawHeader(QByteArray("Authorization"), GlobalVars::authorizedToken.toUtf8());

            QNetworkReply *reply = nam.post(req, postdata.toBase64());
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            if(processStartStop ==  false){
                return;
            }
            QByteArray buffer = reply->readAll();
            //qDebug()<<" reply:: "<<buffer;
            authenReply = QByteArray::fromBase64(buffer);
            reply->deleteLater();
            qDebug()<<"\n\n\n CarrierFix response_data::"<<authenReply;
            qDebug()<<"\n\n";

            QJsonDocument document = QJsonDocument::fromJson(authenReply);
            QJsonObject jsonBuffer = document.object();

            qDebug()<<"Server Authentication reply:: "<<jsonBuffer;
            qDebug()<<" isError: "<<jsonBuffer.value("isError");
            qDebug()<<" IsError: "<<jsonBuffer.value("IsError");
            qDebug()<<" Model: "<<jsonBuffer.value("Model");
            qDebug()<<" Message: "<<jsonBuffer.value("Message");
            qDebug()<<"\n\n ";


            if(jsonBuffer.contains("isError")){
                if (jsonBuffer.value("isError").toBool() == false)
                {
                    QJsonValue buffer1 = jsonBuffer.value("Model");
                    //qDebug()<<" QJsonValue Model: "<<buffer1;
                    QJsonObject bufferModel = buffer1.toObject();
                    GlobalVars::operationID = bufferModel.value("OperationId").toDouble();
                    GlobalVars::userInfo_creditDetails = bufferModel.value("Credit").toDouble();
                    qDebug()<<" operationID : "<<GlobalVars::operationID;
                    qDebug()<<"\n\n ";
                    emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("OperationID: "+QString::number(GlobalVars::operationID, 'f', 0)), false, false, GlobalVars::txtOutPutColor);
                }
            } else if(jsonBuffer.contains("IsError")) {
                if (jsonBuffer.value("IsError").toBool() == true)
                {
                    qDebug()<<"Error Message: "<<jsonBuffer.value("Message");
                    emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Server Process Failed", true, false, QColor::fromRgb(255, 192, 192));
                    emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("Error Msg:"+jsonBuffer.value("Message").toString()), true, false, QColor::fromRgb(255, 192, 192));

                    emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
                    return;
                }
            }


                                 //MMM.exe -k 4 -odm1 ontim -project bj -m user -t write_country_code retbr_retbr -reboot -skipMD
            commandStr = QString("MMM.exe -k "+GlobalVars::meta_serialPortName[generalIndex]+" -odm1 ontim -project "+GlobalVars::meta_projectName[generalIndex] +" -m user -t write_country_code retbr_retbr -reboot -skipMD");
            qDebug()<<" Serial Port: "<<GlobalVars::meta_serialPortName;
            qDebug()<<" MTK-Carrier Fix Command: "<<commandStr;
            emit tx_miscOperations_metaMode(currentToolType, generalIndex, 5, "");
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Start process Carrier Fix: OK", false, false, GlobalVars::txtOutPutColor);
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
        timer_AppMaxTime->start(APP_MAXTIME);

        qDebug()<<" 2 Timer AppMaTime::: "<<timer_AppMaxTime->isActive()<<" time: "<<timer_AppMaxTime->remainingTime();

    }
    if(processStartStop ==  false){
        if(timer_AppMaxTime->isActive()) timer_AppMaxTime->stop();
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

    qDebug()<<" 3 Timer AppMaTime::: "<<timer_AppMaxTime->isActive()<<" time: "<<timer_AppMaxTime->remainingTime();
    if(timer_AppMaxTime->isActive()) timer_AppMaxTime->stop();
    commandError = metaProcess->readAllStandardError();
    qDebug()<<"\n\n ###################################################### \n rx_CommandLine_ErrorReceived:: "<<commandError;
}
void MetaModeThread::rx_CommandLine_OutPutReceived()
{

    if(processStartStop ==  false){ return; }

    commandlineOutPut = metaProcess->readAllStandardOutput();
    //if(showTheConsole)
    if(currentCommand == MMM_MDM)
        qDebug()<<"\n\n ---------------------------------------------------- \n rx_CommandLine_OutPutReceived: "<<commandlineOutPut;

    if(currentToolType == Tool_MTK)
    {
        check_Repair_Cmd_outPut(commandlineOutPut);
    }
    else if(currentToolType == Tool_MTK_UnLock)
    {
        check_UnLock_Cmd_outPut(commandlineOutPut);
    }
    else if(currentToolType == Tool_META_CarrierFix)
    {
        check_CarrierFix_Cmd_outPut(commandlineOutPut);
    }

}
void MetaModeThread::check_CarrierFix_Cmd_outPut(QString str)
{
    qDebug()<<"\n\n ---------------------------------------------------- \n check_CarrierFix_Cmd_outPut: "<<str;

    if(str.contains("Motorola Modem Meta Tool Passed."))
    {


        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex,"Getting Authorization From Server: OK", false, false , GlobalVars::txtOutPutColor);
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, QString("Model: " + GlobalVars::meta_modelStr[generalIndex]), false, false , GlobalVars::txtOutPutColor);
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex,"Invalid Carrier Fixed : OK", false, false , GlobalVars::txtOutPutColor);
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex,"Rebooting: OK", true, false , GlobalVars::txtOutPutColor);
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex,"Process Successfull Completed", true, false , GlobalVars::txtOutPutColor);

        emit tx_miscOperations_metaMode(currentToolType,  generalIndex, 100, "");
    }
    else if(str.contains("FrameComm-> Error in writing COM"))
    {
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex,"Error in ComPort Communication", true, false , Qt::red);
        emit tx_miscOperations_metaMode(currentToolType,  generalIndex, 100, "");
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
        if(currentCommand == MMM_AuthenticationBytes){
            currentCommand = MMM_OK_AfterAuthenticationBytes;
            timer_Meta_singleShot->start(500);
        }
        else if(currentCommand == MMM_MDM){

            if (GlobalVars::meta_reboot_bool[generalIndex]) {
                currentCommand = MMM_Rebooting;
                timer_Meta_singleShot->start(3000);
            }else{
                emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, "Process Successfull Completed", true, false , GlobalVars::txtOutPutColor);
                emit tx_miscOperations_metaMode(currentToolType,  generalIndex, 100, "Success");
                currentCommand = MMM_Exit;
                timer_Meta_singleShot->start(3000);
            }
        }
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


        qDebug()<<" 2 Timer AppMaTime::: "<<timer_AppMaxTime->isActive()<<" time: "<<timer_AppMaxTime->remainingTime();
        if(timer_AppMaxTime->isActive()) timer_AppMaxTime->stop();

        currentCommand = MMM_AuthenticationBytes;
        timer_Meta_singleShot->start(500);
    }
    else if (commandlineOutPut.contains("Motorola Modem Meta Tool Started"))
    {
        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, "Processing ", false, false, GlobalVars::txtOutPutColor);
    }
    else if (commandlineOutPut.contains("Motorola Modem Meta Tool Passed."))
    {
        qDebug()<<" 2 Timer AppMaTime::: "<<timer_AppMaxTime->isActive()<<" time: "<<timer_AppMaxTime->remainingTime();
        if(timer_AppMaxTime->isActive()) timer_AppMaxTime->stop();

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
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex,"Comm port is not working properly.", true, false , Qt::magenta);
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
        qDebug()<<" writing Authentication bytes ";
        metaProcess->write(authenReply);
        metaProcess->write(" \n");
        metaProcess->closeWriteChannel();
        metaProcess->waitForBytesWritten(1000);
        metaProcess->waitForReadyRead(15000);

        break;
    }
    case MMM_OK_AfterAuthenticationBytes:{
        qDebug()<<" writing **OK** after Authentication bytes ";
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

        if (GlobalVars::meta_MDM_bool[generalIndex] && currentToolType == Tool_MTK)
        {
            currentCommand = MMM_MDM;
            timer_Meta_singleShot->start(2000);
        }else{

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
        }
        break;
    }
    case MMM_MDM:{
                                //mmm.exe -k COMPORT            -odm1 ontim -project malta -m user -factory_reset -skipMD
        QByteArray mdmCmd(QString("MMM.exe -k " + GlobalVars::meta_serialPortName[generalIndex] + " -odm1 ontim -project " + GlobalVars::meta_projectName[generalIndex] + " -m user -factory_reset -skipMD").toUtf8());
        qDebug()<<" MDM Command: "<<mdmCmd;




        shellArguments.clear();
        shellArguments.append("/C");
        //shellArguments.append("ipconfig");
        shellArguments.append(mdmCmd);

        metaProcess->setArguments(shellArguments);

        qDebug()<<"\n\n \t Process Opening: "<<metaProcess->open(QIODevice::ReadWrite);

        if (metaProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug() << " \n\n\n\n\n external program running";

        metaProcess->waitForReadyRead(15000);



        /*
        metaProcess->write(mdmCmd);
        metaProcess->write(" \n");
        metaProcess->closeWriteChannel();
        metaProcess->waitForBytesWritten(1000);
        metaProcess->waitForReadyRead(15000);
        */

        emit tx_TextBoxOutput_metaMode(currentToolType,  generalIndex, "MDM Removed : OK", true, false , GlobalVars::txtOutPutColor);
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 90, "Success");

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
        if(timer_AppMaxTime->isActive()) timer_AppMaxTime->stop();
        if(metaProcess->isWritable()){
            metaProcess->write("exit ");
            metaProcess->write("\n");
            metaProcess->closeWriteChannel();
            metaProcess->waitForBytesWritten(1000);
            metaProcess->waitForReadyRead(100);
        }
        qDebug()<<" Process killed";
        metaProcess->close();
        break;
    }
    } // end of sub switch statements

    if(currentCommand != MMM_Exit && currentCommand != MMM_Idle) {
        if(timer_AppMaxTime->isActive()) timer_AppMaxTime->stop();
        timer_AppMaxTime->start(APP_MAXTIME);
    }

}

void MetaModeThread::on_timer_AppMaxTime_Elapsed()
{
    qDebug()<<" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ";
    qDebug()<<" Process Takes more time then usual so we close this process and exit from cmd. id: "<<generalIndex<<" tool"<<currentToolType;
    qDebug()<<" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ";
    emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex,"Comm port is not working properly.", true, false , Qt::yellow);
    emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
    currentCommand = MMM_Exit;
    if(timer_Meta_singleShot->isActive()) timer_Meta_singleShot->stop();
    timer_Meta_singleShot->start(500);
}

void MetaModeThread::getAuthenticationFromServer(QString strData, QString strIMEI)
{
    //qDebug()<<"getAuthenticationFromServer ::  Data:"<<strData<<" IMEI:"<<strIMEI;

    QByteArray postdata;

    QJsonObject obj;
    obj["data"] =  QString(strData);
    obj["imei"] = strIMEI;
    obj["pname"] = GlobalVars::meta_projectName[generalIndex];
    if(currentToolType == Tool_MTK){
        obj["type"] = GlobalVars::meta_projectDataType[generalIndex];
        obj["OperationName"] = "repairmtk";
    }
    else if(currentToolType == Tool_MTK_UnLock){
        obj["type"] = GlobalVars::meta_projectUnlockType[generalIndex];
        obj["OperationName"] = "Unlockmtk";
    }
    obj["DeviceName"] = GlobalVars::meta_modelStr[generalIndex];
    QJsonDocument doc(obj);
    postdata = doc.toJson();


    emit tx_miscOperations_metaMode(currentToolType, generalIndex, 30, "");
    emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("Model: "+GlobalVars::meta_modelStr[generalIndex]), false, false, GlobalVars::txtOutPutColor);
    //qDebug()<<" Json Object: "<<data<<" \n\n ByteArray:"<<postdata;
    //qDebug()<<" \n\n ByteArray:"<<postdata<<" Base64::"<<postdata.toBase64();
    if(processStartStop ==  false){
        return;
    }
    QEventLoop loop;
    QNetworkAccessManager nam;
    QNetworkRequest req;
    req.setUrl(QUrl(GlobalVars::api_MTKserverAuthenticity));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setRawHeader(QByteArray("Authorization"), GlobalVars::authorizedToken.toUtf8());

    QNetworkReply *reply = nam.post(req, postdata.toBase64());
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
    authenReply = QByteArray::fromBase64(buffer);
    reply->deleteLater();
    if(processStartStop ==  false){
        return;
    }


    //qDebug()<<"\n\n\n Server Authentication response_data::"<<authenReply;
    //qDebug()<<"\n\n";

    QJsonDocument document = QJsonDocument::fromJson(authenReply);
    QJsonObject jsonBuffer = document.object();

    //qDebug()<<"Server Authentication reply:: "<<jsonBuffer;
    qDebug()<<" isError: "<<jsonBuffer.value("isError");
    qDebug()<<" IsError: "<<jsonBuffer.value("IsError");
    qDebug()<<" Model: "<<jsonBuffer.value("Model");
    qDebug()<<" Message: "<<jsonBuffer.value("Message");
    qDebug()<<"\n\n ";


    if(jsonBuffer.contains("isError")) {
        if (jsonBuffer.value("isError").toBool() == false)
        {
            QJsonValue buffer1 = jsonBuffer.value("Model");
            //qDebug()<<" QJsonValue Model: "<<buffer1;
            QJsonObject bufferModel = buffer1.toObject();
            GlobalVars::operationID = bufferModel.value("OperationId").toDouble();
            //qDebug()<<" OperationId 444 : "<<bufferModel.value("OperationId");
            authenReply = bufferModel.value("Auth").toString().toUtf8();
            GlobalVars::userInfo_creditDetails = bufferModel.value("Credit").toDouble();
            qDebug()<<"\n\n authenReply : "<<authenReply;
            qDebug()<<" authenticationID : "<<GlobalVars::operationID;
            qDebug()<<"\n\n ";
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("OperationID: "+QString::number(GlobalVars::operationID, 'f', 0)), false, false, GlobalVars::txtOutPutColor);
        }
    }else if(jsonBuffer.contains("IsError")) {
        if (jsonBuffer.value("IsError").toBool() == true)
        {
            qDebug()<<"Error Message: "<<jsonBuffer.value("Message");
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Process Failed", true, false, QColor::fromRgb(255, 192, 192));
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("Error Msg:"+jsonBuffer.value("Message").toString()), true, false, QColor::fromRgb(255, 192, 192));

            emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
            return;
        }
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
        QByteArray postdata;
        postdata.append(QString("model="+GlobalVars::meta_projectName[generalIndex]+"&").toUtf8());
        postdata.append(QString("simCode="+QString(GlobalVars::meta_dual_imei_bool[generalIndex]? "ds":"ss")).toUtf8());

        QJsonObject obj;
        obj["data"] =  QString(QCryptographicHash::hash(postdata, QCryptographicHash::Sha3_256).toUpper().toHex());
        obj["encrypt"] = "2";
        QJsonDocument doc(obj);
        QByteArray data = doc.toJson();
        //qDebug()<<" \n\n IMEI ByteArray:"<<data <<" \n Base64: "<<data.toBase64();

        if(processStartStop ==  false){
            return false;
        }
        QEventLoop loop;
        QNetworkAccessManager nam;
        QNetworkRequest req;
        req.setUrl(QUrl(GlobalVars::api_MTKimeiGenerator));
        //req.setUrl(QUrl("http://server51214110.ngrok.io/imeiGenerator.php"));
        //req.setUrl((QUrl("http://server51214110.ngrok.io/imeiGenerator.php")));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        req.setRawHeader(QByteArray("Authorization"), GlobalVars::authorizedToken.toUtf8());

        QNetworkReply *reply = nam.post(req, data.toBase64());
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if(processStartStop ==  false){
            return false;
        }
        emit tx_miscOperations_metaMode(currentToolType, generalIndex, 40, "");
        emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("Geting IMEI from Server: OK"), false, false, GlobalVars::txtOutPutColor);

        QByteArray buffer = reply->readAll();
        //qDebug()<<" \n\n\n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n reply:: "<<buffer;


        reply->deleteLater();
        if(processStartStop ==  false){ return false; }
        buffer = QByteArray::fromBase64(buffer);
        qDebug()<<" \n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n ";
        qDebug()<<"\n\n\n IMEI response_data::"<<buffer;
        qDebug()<<"\n\n";
        qDebug()<<" \n ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n ";

        if(buffer.contains("Tunnel") && buffer.contains("ERR_NGROK_3200")){
            qDebug()<<" yes returning and emitting signal ";
            emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Server OFFLine.", true, false, QColor::fromRgb(255, 192, 192));
            return false;
        }

        QJsonDocument document = QJsonDocument::fromJson(buffer);
        QJsonObject jsonBuffer = document.object();

        //qDebug()<<" reply:: "<<jsonBuffer;
        qDebug()<<" isError: "<<jsonBuffer.value("isError"); //.value("AccessToken");
        qDebug()<<"\n\n ";

        if (jsonBuffer.value("isError").toBool() == true)
        {
            emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Faided to get IMEI-1 from Server", true, false, QColor::fromRgb(255, 192, 192));
            return false;
        }
        if (jsonBuffer.value("IsError").toBool() == true)
        {
            emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Faided to get IMEI-1 from Server", true, false, QColor::fromRgb(255, 192, 192));


            QJsonValue buffer1 = jsonBuffer.value("Message");
            QString serverMessage = QString(buffer1.toString());
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, serverMessage, true, false, QColor::fromRgb(255, 192, 192));

            return false;
        }

        if (GlobalVars::meta_dual_imei_bool[generalIndex]) //(buffer.toLower().contains("&"))
        {
            QJsonValue buffer1 = jsonBuffer.value("Model");
            GlobalVars::meta_imei_1[generalIndex] = buffer1.toString();
            /*
            req.setUrl(QUrl(GlobalVars::api_MTKimeiGenerator));
            req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            req.setRawHeader(QByteArray("Authorization"), GlobalVars::authorizedToken.toUtf8());
            */
            reply = nam.post(req, data.toBase64());
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            if(processStartStop ==  false){
                return false;
            }
            emit tx_miscOperations_metaMode(currentToolType, generalIndex, 40, "");
            emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, QString("Geting IMEI-2 from Server: OK"), false, false, GlobalVars::txtOutPutColor);
            QByteArray buffer = reply->readAll();
            reply->deleteLater();
            if(processStartStop ==  false){
                return false;
            }
            buffer = QByteArray::fromBase64(buffer);
            qDebug()<<"\n\n\n IMEI-2 response_data::"<<buffer;
            //qDebug()<<"\n\n";

            document = QJsonDocument::fromJson(buffer);
            jsonBuffer = document.object();

            qDebug()<<" reply 2:: "<<jsonBuffer;
            qDebug()<<" isError: "<<jsonBuffer.value("isError"); //.value("AccessToken");
            qDebug()<<"\n\n ";

            if (jsonBuffer.value("isError").toBool() == true) {
                emit tx_miscOperations_metaMode(currentToolType, generalIndex, 100, "");
                emit tx_TextBoxOutput_metaMode(currentToolType, generalIndex, "Faided to get IMEI-2 from Server", true, false, QColor::fromRgb(255, 192, 192));
                return false;
            }
            QJsonValue imei2 = jsonBuffer.value("Model");
            GlobalVars::meta_imei_2[generalIndex] = imei2.toString();
        }
        else
        {            
            QJsonValue buffer1 = jsonBuffer.value("Model");
            GlobalVars::meta_imei_1[generalIndex] = buffer1.toString();
        }

        qDebug()<<"****** IMEI-1: "<<GlobalVars::meta_imei_1[generalIndex]<<" IMEI-2:"<<GlobalVars::meta_imei_2[generalIndex];
        //qDebug()<<" Writing this code in cmd: "<<authenReply;
    }

    return true;
}




