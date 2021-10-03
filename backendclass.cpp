#include "backendclass.h"

BackEndClass::BackEndClass(QObject *parent) : QObject(parent)
{
    timer_SPDTool = new QTimer(this);
    timer_SPDTool->setSingleShot(true);
    connect(timer_SPDTool, SIGNAL(timeout()), this, SLOT(rx_timer_SPDTool_elapsed()));
    singleShotIndex = 0;


    QDir chckRequiredFolder(m3FolderPath);
    if(!chckRequiredFolder.exists()){
        QMessageBox::critical(nullptr, "M3 Error", "Please Install the Mototool Software.", QMessageBox::Ok);
        qDebug()<<" Folder not exist: "<<m3FolderPath;
        qDebug()<<" Folder not exist: "<<adbFolderPath;
        qDebug()<<" Folder not exist: "<<ChinoeIMEICMToolFolderPath_Data1;
    }
    chckRequiredFolder.setPath(adbFolderPath);
    if(!chckRequiredFolder.exists()){
        QMessageBox::critical(nullptr, "ADB Error", "Please Install the ADB Software.", QMessageBox::Ok);
    }
    chckRequiredFolder.setPath(ChinoeIMEICMToolFolderPath_Data1);
    if(!chckRequiredFolder.exists()){
        QMessageBox::critical(nullptr, "ADB Chinoe Error", "Please Install the Chineo Software.", QMessageBox::Ok);
    }

    //qDebug()<<" M3: "<<m3FolderPath;
    //qDebug()<<" ADB: "<<adbFolderPath;

    /*
    int currentProcessIndx = 0;
    metaProcess[currentProcessIndx] = new QProcess(this); metaProcess[currentProcessIndx]->setProgram(cmdShellPath); metaProcess[currentProcessIndx]->setWorkingDirectory(m3FolderPath);
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardOutput()), this, SLOT(rx_CommandLine_OutPut_Device_1()));
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardError()), this, SLOT(rx_CommandLine_Error_Device_1()));
    connect(metaProcess[currentProcessIndx], SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(rx_CommandLine_Error_Device_1()));
    currentProcessIndx = 1;
    metaProcess[currentProcessIndx] = new QProcess(this); metaProcess[currentProcessIndx]->setProgram(cmdShellPath); metaProcess[currentProcessIndx]->setWorkingDirectory(m3FolderPath);
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardOutput()), this, SLOT(rx_CommandLine_OutPut_Device_2()));
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardError()), this, SLOT(rx_CommandLine_Error_Device_2()));
    connect(metaProcess[currentProcessIndx], SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(rx_CommandLine_Error_Device_2()));
    currentProcessIndx = 2;
    metaProcess[currentProcessIndx] = new QProcess(this); metaProcess[currentProcessIndx]->setProgram(cmdShellPath); metaProcess[currentProcessIndx]->setWorkingDirectory(m3FolderPath);
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardOutput()), this, SLOT(rx_CommandLine_OutPut_Device_3()));
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardError()), this, SLOT(rx_CommandLine_Error_Device_3()));
    connect(metaProcess[currentProcessIndx], SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(rx_CommandLine_Error_Device_3()));
    currentProcessIndx = 3;
    metaProcess[currentProcessIndx] = new QProcess(this); metaProcess[currentProcessIndx]->setProgram(cmdShellPath); metaProcess[currentProcessIndx]->setWorkingDirectory(m3FolderPath);
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardOutput()), this, SLOT(rx_CommandLine_OutPut_Device_4()));
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardError()), this, SLOT(rx_CommandLine_Error_Device_4()));
    connect(metaProcess[currentProcessIndx], SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(rx_CommandLine_Error_Device_4()));
    currentProcessIndx = 4;
    metaProcess[currentProcessIndx] = new QProcess(this); metaProcess[currentProcessIndx]->setProgram(cmdShellPath); metaProcess[currentProcessIndx]->setWorkingDirectory(m3FolderPath);
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardOutput()), this, SLOT(rx_CommandLine_OutPut_Device_5()));
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardError()), this, SLOT(rx_CommandLine_Error_Device_5()));
    connect(metaProcess[currentProcessIndx], SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(rx_CommandLine_Error_Device_5()));
    currentProcessIndx = 5;
    metaProcess[currentProcessIndx] = new QProcess(this); metaProcess[currentProcessIndx]->setProgram(cmdShellPath); metaProcess[currentProcessIndx]->setWorkingDirectory(m3FolderPath);
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardOutput()), this, SLOT(rx_CommandLine_OutPut_Device_6()));
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardError()), this, SLOT(rx_CommandLine_Error_Device_6()));
    connect(metaProcess[currentProcessIndx], SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(rx_CommandLine_Error_Device_6()));
    currentProcessIndx = 6;
    metaProcess[currentProcessIndx] = new QProcess(this); metaProcess[currentProcessIndx]->setProgram(cmdShellPath); metaProcess[currentProcessIndx]->setWorkingDirectory(m3FolderPath);
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardOutput()), this, SLOT(rx_CommandLine_OutPut_Device_7()));
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardError()), this, SLOT(rx_CommandLine_Error_Device_7()));
    connect(metaProcess[currentProcessIndx], SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(rx_CommandLine_Error_Device_7()));
    currentProcessIndx = 7;
    metaProcess[currentProcessIndx] = new QProcess(this); metaProcess[currentProcessIndx]->setProgram(cmdShellPath); metaProcess[currentProcessIndx]->setWorkingDirectory(m3FolderPath);
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardOutput()), this, SLOT(rx_CommandLine_OutPut_Device_8()));
    connect(metaProcess[currentProcessIndx], SIGNAL(readyReadStandardError()), this, SLOT(rx_CommandLine_Error_Device_8()));
    connect(metaProcess[currentProcessIndx], SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(rx_CommandLine_Error_Device_8()));
    */




    adbProcess = new QProcess(this);
    adbProcess->setProgram(cmdShellPath);
    adbProcess->setWorkingDirectory(adbFolderPath);

    connect(adbProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(rx_ADB_OutPutReceived()));
    //connect(adbProcess, SIGNAL(finished(int)), this, SLOT(rx_ADB_OutPutReceived()));
    connect(adbProcess, SIGNAL(readyReadStandardError()), this, SLOT(rx_ADB_ErrorReceived()));
    connect(adbProcess, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(rx_ADB_ErrorReceived()));


}

void BackEndClass::rx_StartRepairing(int idx, bool startStop, TOOL_TYPE tool)
{
    qDebug()<<" Signal received:  rx_StartRepairing "<<startStop;
    processStartStop = startStop;
    showThisinDebug = false;
    currentCommand[idx] = MMM_Idle;
    if(startStop){
        currentToolType = tool;  // Seting Current Tool , => MTK or SPD


        qDebug()<<" SPD Tool -> Sending ADB Commands";
        nowForSecondIMEI = false;
        adb_InfoOnly = false;
        actualADBcommands = false;
        singleShotIndex = 1;

        if(!getServerIMEInumber_updateGlobal()) return;



        /*
            1. "E6I" powershell
            2. "E7I" powelshell
            3. "E6s XT2053" cmd
            4. "G8 Power Lite XT2055"		 cmd
            5. "E7 Play XT2095" cmd
            6. "G20" cmd
            */


        if(GlobalVars::spd_modelStr.contains("E6I")) { adbProcess->setProgram(powerShellPath); }
        else if(GlobalVars::spd_modelStr.contains("E7I")) { adbProcess->setProgram(powerShellPath); }
        else if(GlobalVars::spd_modelStr.contains("E6s XT2053")) { adbProcess->setProgram(cmdShellPath); }
        else if(GlobalVars::spd_modelStr.contains("G8 Power Lite XT2055")) { adbProcess->setProgram(cmdShellPath); }
        else if(GlobalVars::spd_modelStr.contains("E7 Play XT2095")) { adbProcess->setProgram(cmdShellPath); }
        else if(GlobalVars::spd_modelStr.contains("G20")) { adbProcess->setProgram(cmdShellPath); }



        qDebug()<<" Selected Model: "<<GlobalVars::spd_modelStr<<" Program: "<<adbProcess->program();

        emit tx_miscOperations(Tool_SPD, 1, 5, "");
        if(currentToolType == Tool_SPD)
            emit tx_TextBoxOutput(Tool_SPD, 1, "Start process Write ADB ImeI: OK", false, false, GlobalVars::txtOutPutColor);
        else if(currentToolType == Tool_UnLock)
            emit tx_TextBoxOutput(Tool_UnLock, 1, "Start process UnLOCK: OK", false, false, GlobalVars::txtOutPutColor);

        adbProcess->setWorkingDirectory(adbFolderPath);
        spd_commandStr = "adb devices";
        shellArguments.clear();
        shellArguments.append("/C");
        shellArguments.append(spd_commandStr);
        adbProcess->close();
        adbProcess->waitForReadyRead(1000);
        adbProcess->setArguments(shellArguments);
        //qDebug()<<" Arg::"<<shellArguments;
        currentADB = ADB_ConnectedDevice;
        adbProcess->start();
        //qDebug()<<"\n\n \t Process Opening: "<<adbProcess->open(QIODevice::ReadWrite);

        if (adbProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug() << "external program running";

        //adbProcess->setTextModeEnabled(true);
        //adbProcess->waitForReadyRead(1000);
        adbProcess->waitForFinished(1000);
        emit tx_miscOperations(Tool_SPD, 1, 10, "");
        /*}*/
    }
    if(processStartStop ==  false){
        try {
            //currentCommand[idx] = MMM_Exit;
            currentADB = ADB_Idle;
            adbProcess->close();
            singleShotIndex = 0;
            //metaProcess[idx]->close();
            /*
            switch (idx) {
            case 0:{
                if(timer_Device_1->isActive()) timer_Device_1->stop();
                timer_Device_1->start(500);
                break;
            }
            case 1:{
                if(timer_Device_2->isActive()) timer_Device_2->stop();
                timer_Device_2->start(500);
                break;
            }
            case 2:{
                if(timer_Device_3->isActive()) timer_Device_3->stop();
                timer_Device_3->start(500);
                break;
            }
            case 3:{
                if(timer_Device_4->isActive()) timer_Device_4->stop();
                timer_Device_4->start(500);
                break;
            }
            case 4:{
                if(timer_Device_5->isActive()) timer_Device_5->stop();
                timer_Device_5->start(500);
                break;
            }
            case 5:{
                if(timer_Device_6->isActive()) timer_Device_6->stop();
                timer_Device_6->start(500);
                break;
            }
            case 6:{
                if(timer_Device_7->isActive()) timer_Device_7->stop();
                timer_Device_7->start(500);
                break;
            }
            case 7:{
                if(timer_Device_8->isActive()) timer_Device_8->stop();
                timer_Device_8->start(500);
                break;
            }
            }
*/
        } catch (const std::bad_alloc &) { }
        return;
    }
}



// ---------------- QTimer Slots -----------------------
void BackEndClass::rx_timer_SPDTool_elapsed()
{
    qDebug()<<" single Shot Timer_SPDTool :: "<<currentADB;

    switch (currentADB) {
    case ADB_Idle: break;
    case ADB_ConnectedDevice: break;
    case  ADB_Model: {
        processStartStop = true;
        spd_commandStr = "adb shell getprop ro.product.vendor.model";
        shellArguments.clear();
        shellArguments.append("/C");
        shellArguments.append(spd_commandStr);
        adbProcess->close();
        adbProcess->waitForReadyRead(1000);
        adbProcess->setArguments(shellArguments);
        //adbProcess->start();
        //qDebug()<<" Model : command: "<<spd_commandStr;
        adbProcess->open(QIODevice::ReadWrite);
        if (adbProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug()<< "external program running";
        adbProcess->waitForFinished(1000);
        break;
    }
    case  ADB_AndroidVersion:{
        spd_commandStr = "adb shell getprop ro.product.build.version.release";
        shellArguments.clear();
        shellArguments.append("/C");
        shellArguments.append(spd_commandStr);
        adbProcess->close();
        adbProcess->waitForReadyRead(1000);
        adbProcess->setArguments(shellArguments);
        //qDebug()<<" Android Version : command: "<<spd_commandStr;
        adbProcess->open(QIODevice::ReadWrite);
        if (adbProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug() << "external program running";
        adbProcess->waitForFinished(1000);
        break;
    }
    case  ADB_BuildDate:{
        spd_commandStr = "adb shell getprop ro.product.build.date";
        shellArguments.clear();
        shellArguments.append("/C");
        shellArguments.append(spd_commandStr);
        adbProcess->close();
        adbProcess->waitForReadyRead(1000);
        adbProcess->setArguments(shellArguments);
        adbProcess->start();
        if (adbProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug() << "external program running";
        adbProcess->waitForFinished(1000);
        break;
    }
    case  ADB_SerialNumber:{
        spd_commandStr = "adb shell getprop ro.boot.serialno";
        shellArguments.clear();
        shellArguments.append("/C");
        shellArguments.append(spd_commandStr);
        adbProcess->close();
        adbProcess->waitForReadyRead(1000);
        adbProcess->setArguments(shellArguments);
        adbProcess->start();
        if (adbProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug()<<"\n";// << "external program running";
        adbProcess->waitForFinished(1000);
        break;
    }
    case ADB_AssignPortCmd: {
        adbProcess->setWorkingDirectory(ChinoeIMEICMToolFolderPath_Data1);
        spd_commandStr = "ChinoeIMEICMTool.exe -plat SP -catch";
        //commandStr = "ChinoeIMEICMTool.exe help";
        shellArguments.clear();
        shellArguments.append("/C");
        shellArguments.append(spd_commandStr);
        adbProcess->close();
        adbProcess->waitForReadyRead(1000);
        adbProcess->setArguments(shellArguments);
        adbProcess->start();
        if (adbProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug() << " Assigning Port external program running";
        adbProcess->waitForFinished(1000);
        break;
    }
    case  ADB_Original_imei:{
        spd_commandStr = "adb shell \"service call iphonesubinfo 1 | toybox cut -d \"'\" -f2 | toybox grep -Eo '[0-9]' | toybox xargs | toybox sed 's/\ //g'\"";
        //qDebug()<<" \n\n Origional IMEI Command: "<<commandStr<<"\n\n";
        shellArguments.clear();
        shellArguments.append("/C");
        shellArguments.append(spd_commandStr);
        adbProcess->close();
        adbProcess->waitForReadyRead(1000);
        adbProcess->setArguments(shellArguments);
        adbProcess->start();
        if (adbProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug()<< " Origional IMEI external program running";
        adbProcess->waitForFinished(1000);
        break;
    }
    case ADB_Reboot:{
        spd_commandStr = "adb shell reboot -p";
        shellArguments.clear();
        shellArguments.append("/C");
        shellArguments.append(spd_commandStr);
        adbProcess->close();
        adbProcess->waitForReadyRead(1000);
        adbProcess->setArguments(shellArguments);
        adbProcess->start();
        if (adbProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug()<<"Rebooting Mobile external program running";
        adbProcess->waitForFinished(1000);

        if(adb_InfoOnly == false) {
            if(currentToolType == Tool_UnLock) {
                currentADB = ADB_UnLockSIM;
                timer_SPDTool->start(5000);
            } else if(currentToolType == Tool_SPD){
                currentADB = ADB_ChinoeIMEICMTool;
                actualADBcommands = true;
                timer_SPDTool->start(5000);
            }
        } else {
            if(GlobalVars::spd_modelStr.contains("E6I")||GlobalVars::spd_modelStr.contains("E7I")) {
                if(GlobalVars::spd_AssignPort_bool) {
                    currentADB = ADB_AssignPortCmd;
                    timer_SPDTool->start(3500);
                }
            }
        }


        break;
    }
    case ADB_ChinoeIMEICMTool:{
        //qDebug()<<" Current Selected Model : "<<GlobalVars::modelStr;

        if(GlobalVars::spd_modelStr.contains("E6I")||GlobalVars::spd_modelStr.contains("E7I")) {
            adbProcess->setWorkingDirectory(ChinoeIMEICMToolFolderPath_Data1);
            if(!nowForSecondIMEI){
                spd_commandStr = QString("./ChinoeIMEICMTool.exe -plat SP -auth lixinsheng mf2Rqrw% -wi 0 "+GlobalVars::spd_imei_1+" lixinsheng mf2Rqrw%");
            }else if(nowForSecondIMEI) {
                spd_commandStr = QString("./ChinoeIMEICMTool.exe -plat SP -auth lixinsheng mf2Rqrw% -wi 1 "+GlobalVars::spd_imei_2+" lixinsheng mf2Rqrw%");
            }
        }
        else if(GlobalVars::spd_modelStr.contains("E6s XT2053")||GlobalVars::spd_modelStr.contains("G8 Power Lite XT2055")||GlobalVars::spd_modelStr.contains("E7 Play XT2095")) {
            adbProcess->setWorkingDirectory(ChinoeIMEICMToolFolderPath_Data2);
            if(!nowForSecondIMEI){
                spd_commandStr = QString("Chinoeimeicmtool -i 0 "+GlobalVars::spd_imei_1+" lixinsheng mf2Rqrw%");
            }else if(nowForSecondIMEI) {
                spd_commandStr = QString("Chinoeimeicmtool -i 1 "+GlobalVars::spd_imei_2+" lixinsheng mf2Rqrw%");
            }
        }
        else if(GlobalVars::spd_modelStr.contains("G20")) {
            adbProcess->setWorkingDirectory(ChinoeIMEICMToolFolderPath_Data3);

            nowForSecondIMEI = true;
            if(GlobalVars::spd_dual_imei_bool){
                spd_commandStr = QString("WriteIMEIToolV1.1.exe -cTSDCLogin dfs,Dfs@2021dfs -cConnect -cReadTrackId -cReadIMEI1 -cWriteIMEI1 "+GlobalVars::spd_imei_1+"  -cLog -E");
            }else{
                spd_commandStr = QString("WriteIMEIToolV1.1.exe -cTSDCLogin dfs,Dfs@2021dfs -cConnect -cWriteIMEI1 "+GlobalVars::spd_imei_1+" -cWriteIMEI2 "+GlobalVars::spd_imei_2+" -cReadTrackId -cReadIMEI1 -cReadIMEI2  -cLog -E");
            }
        }


        qDebug()<<" \n\n Repair Working Dir: after reboot::"<<adbProcess->workingDirectory()<<"\n comd: "<<spd_commandStr<<"\n\n";

        shellArguments.clear();
        shellArguments.append("/C");
        shellArguments.append(spd_commandStr);
        adbProcess->close();
        adbProcess->waitForReadyRead(1000);
        adbProcess->setArguments(shellArguments);
        //adbProcess->start();
        adbProcess->open(QIODevice::ReadWrite);

        if (adbProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug() << "external program running";
        //adbProcess->waitForFinished(1000);
        adbProcess->waitForReadyRead(15000);
        break;
    }
    case ADB_UnLockSIM: {
        if(GlobalVars::spd_modelStr.contains("E6I")) {
            adbProcess->setWorkingDirectory(ChinoeIMEICMToolFolderPath_Data1);
            spd_commandStr = QString("./ChinoeIMEICMTool.exe -plat SP -auth lixinsheng mf2Rqrw% -ws "+simUnLock_E6I+" lixinsheng mf2Rqrw%");
        }
        else if(GlobalVars::spd_modelStr.contains("E7I")) {
            adbProcess->setWorkingDirectory(ChinoeIMEICMToolFolderPath_Data1);
            spd_commandStr = QString("./Chinoeimeicmtool -s "+simUnLock_E7I+" lixinsheng mf2Rqrw%");
        }
        else if(GlobalVars::spd_modelStr.contains("E6s XT2053")) {
            adbProcess->setWorkingDirectory(ChinoeIMEICMToolFolderPath_Data2);
            spd_commandStr = QString("Chinoeimeicmtool -s "+simUnLock_E6sXT2053+" lixinsheng mf2Rqrw%");
        }
        else if(GlobalVars::spd_modelStr.contains("G8 Power Lite XT2055")) {
            adbProcess->setWorkingDirectory(ChinoeIMEICMToolFolderPath_Data2);
            spd_commandStr = QString("Chinoeimeicmtool -s "+simUnLock_G8PowerLite+" lixinsheng mf2Rqrw%");
        }
        else if(GlobalVars::spd_modelStr.contains("E7 Play XT2095")) {
            adbProcess->setWorkingDirectory(ChinoeIMEICMToolFolderPath_Data2);
            spd_commandStr = QString("Chinoeimeicmtool -s "+simUnLock_E7PlayXT2095+" lixinsheng mf2Rqrw%");
        }
        else if(GlobalVars::spd_modelStr.contains("G20")) {
            adbProcess->setWorkingDirectory(ChinoeIMEICMToolFolderPath_Data3);
            spd_commandStr = QString("WriteIMEIToolV1.1.exe -cTSDCLogin dfs,Dfs@2021dfs -cConnect -cSIMLOCK "+simUnLock_G20+" -cLog -E");
        }

        qDebug() << "\n\n SIM UNLOCK :"<<spd_commandStr<<"\n\n";

        shellArguments.clear();
        shellArguments.append("/C");
        shellArguments.append(spd_commandStr);
        adbProcess->close();
        adbProcess->waitForReadyRead(1000);
        adbProcess->setArguments(shellArguments);
        //adbProcess->start();
        adbProcess->open(QIODevice::ReadWrite);

        if (adbProcess->waitForStarted(1000) == false)
            qDebug() << "Error starting external program";
        else
            qDebug() << "SIM UNLOCK external program running";
        //adbProcess->waitForFinished(1000);
        adbProcess->waitForReadyRead(15000);
        break;
    }
    case ADB_verify: break;
    }
}

// ---------------- ADB + SPD Slots and Methods -----------------------
void BackEndClass::rx_ADB_OutPutReceived()
{
    if(processStartStop ==  false){
        return;
    }
    singleShotIndex =1;
    spd_commandlineOutPut = adbProcess->readAllStandardOutput();
    if(actualADBcommands){
        spd_commandlineOutPut = spd_commandlineOutPut.replace("\r\n","\n");
    }else {
        spd_commandlineOutPut = spd_commandlineOutPut.left(spd_commandlineOutPut.length()-2);
    }
    qDebug()<<" rx_ADB_OutPutReceived: "<<currentADB<<" : "<<spd_commandlineOutPut;


    switch (currentADB) {
    case ADB_Idle: break;
    case ADB_ConnectedDevice: getConnectedDevices_andSendThemGUI(spd_commandlineOutPut.toUtf8()); break;
    case ADB_Model: getADB_Model(spd_commandlineOutPut.toUtf8()); break;
    case ADB_AndroidVersion: getADB_AndroidVersion(spd_commandlineOutPut.toUtf8());  break;
    case ADB_BuildDate: getADB_BuildDate(spd_commandlineOutPut.toUtf8());  break;
    case ADB_SerialNumber: getADB_SerialNumber(spd_commandlineOutPut.toUtf8());  break;
    case ADB_AssignPortCmd: break;
    case ADB_Original_imei: getADB_Original_imei(spd_commandlineOutPut.toUtf8());  break;
    case ADB_Reboot: getADB_rebootCommand(spd_commandlineOutPut.toUtf8()); break;
    case ADB_ChinoeIMEICMTool: getADB_ChinoeIMEICMTool(spd_commandlineOutPut.toUtf8()); break;
    case ADB_UnLockSIM: getADB_UnLockSIMTool(spd_commandlineOutPut.toUtf8());  break;
    case ADB_verify: break;
    }

}

void BackEndClass::getConnectedDevices_andSendThemGUI(QByteArray ba)
{
    //qDebug()<<"getConnectedDevices_andSendThemGUI::  "<<ba;
    int endlineIndx = 0;
    int tabIndx = 0;
    int deviceNumber = 0;
    QString deviceID ="";
    QString authorized ="";
    adbDeviceDetectedandAuthorized = false;
    if(ba.contains("List of devices attached")){
        adbDevicesRaw.clear();
        endlineIndx = ba.indexOf("\n");
        ba = ba.right(ba.length()-(endlineIndx+1));
        //qDebug()<<" New bytes: "<<ba;
        tabIndx = ba.indexOf("\t");
        deviceID = ba.left(tabIndx);

        ba.remove(0, tabIndx+1);
        endlineIndx = ba.indexOf("\r\n");
        authorized = ba.left(endlineIndx);
        if(authorized == "device"){
            adbDevicesRaw.append(deviceID);
            emit tx_miscOperations(Tool_SPD, 11, deviceNumber, deviceID);
            emit tx_TextBoxOutput(Tool_SPD, 1, "Start process Read Info : OK", false, false, GlobalVars::txtOutPutColor);
            emit tx_TextBoxOutput(Tool_SPD, 1, "Processing ", false, false, GlobalVars::txtOutPutColor);
            emit tx_TextBoxOutput(Tool_SPD, 1,"Getting Authorization From Server: OK ", false, false, GlobalVars::txtOutPutColor);
            adbDeviceDetectedandAuthorized = true;
        }
        else if(authorized =="unauthorized")
        {
            adbDevicesRaw.clear();
            emit tx_TextBoxOutput(Tool_SPD, 1, "Device is not authorized. Please enable the usb debuging mode.", false, false, Qt::red);
            emit tx_miscOperations(Tool_SPD,1, 100, "");
            adbProcess->reset();
        }


        currentADB = ADB_Model;
        singleShotIndex=1;
        timer_SPDTool->start(500);
        emit tx_miscOperations(Tool_SPD, 1, 10, "");
    }
    else
    {
        emit tx_TextBoxOutput(Tool_SPD, 1, "Error in Finding Devices, Please re-Plug the device \n ---------------------------------", false, false, Qt::red);
        emit tx_miscOperations(Tool_SPD, 1, 100, "");
    }
}
void BackEndClass::getADB_Model(QByteArray ba)
{
    //qDebug()<<"getADB_Model::  "<<ba;
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("Model: "+ba), false, false, GlobalVars::txtOutPutColor);
    currentADB = ADB_AndroidVersion;
    timer_SPDTool->start(500);
    emit tx_miscOperations(Tool_SPD, 1, 20, "");
}
void BackEndClass::getADB_AndroidVersion(QByteArray ba)
{
    //qDebug()<<"getADB_AndroidVersion::  "<<ba;
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("Android Version: "+ba), false, false, GlobalVars::txtOutPutColor);
    currentADB = ADB_SerialNumber;
    timer_SPDTool->start(500);
    emit tx_miscOperations(Tool_SPD, 1, 30, "");

}
void BackEndClass::getADB_BuildDate(QByteArray ba)
{
    //qDebug()<<"getADB_BuildDate::  "<<ba;
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("BuildDate: "+ba), false, false, GlobalVars::txtOutPutColor);
    currentADB = ADB_SerialNumber;
    timer_SPDTool->start(500);
    emit tx_miscOperations(Tool_SPD, 1, 10, "");

}
void BackEndClass::getADB_SerialNumber(QByteArray ba)
{
    //qDebug()<<"getADB_SerialNumber::  "<<ba;
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("Serial: "+ba), false, false, GlobalVars::txtOutPutColor);
    if(adb_InfoOnly == false){
        if(currentToolType == Tool_SPD)
        {
            if(GlobalVars::spd_dual_imei_bool)
            {
                emit tx_TextBoxOutput(Tool_SPD, 1, QString("IMEI-1: "+GlobalVars::spd_imei_1), false, false, GlobalVars::txtOutPutColor);
                emit tx_TextBoxOutput(Tool_SPD, 1, QString("IMEI-2: "+GlobalVars::spd_imei_2), false, false, GlobalVars::txtOutPutColor);
            } else {
                emit tx_TextBoxOutput(Tool_SPD, 1, QString("IMEI: "+GlobalVars::spd_imei_1), false, false, GlobalVars::txtOutPutColor);
            }
        }

        currentADB = ADB_Reboot;
        timer_SPDTool->start(500);
        emit tx_miscOperations(Tool_SPD, 1,  40, "");
    }
    else{
        //qDebug()<<"\n\n\t assignPort_bool:"<<GlobalVars::spd_AssignPort_bool;
        if(GlobalVars::spd_modelStr.contains("E6I")||GlobalVars::spd_modelStr.contains("E7I")) {
            if(GlobalVars::spd_AssignPort_bool)
            {
                currentADB = ADB_Reboot;
                //currentADB = ADB_AssignPortCmd;
                timer_SPDTool->start(500);
                emit tx_miscOperations(Tool_SPD, 1,  70, "");
            }
            else
            {
                emit tx_miscOperations(Tool_SPD, 1,  100, "");
                emit tx_TextBoxOutput(Tool_SPD, 1, "---------------------------------", false, false, Qt::white);
            }
        }
        else if(GlobalVars::spd_modelStr.contains("E6s XT2053")||GlobalVars::spd_modelStr.contains("G8 Power Lite XT2055")||GlobalVars::spd_modelStr.contains("E7 Play XT2095")) {

            emit tx_miscOperations(Tool_SPD, 1,  100, "");
            emit tx_TextBoxOutput(Tool_SPD, 1, "---------------------------------", false, false, Qt::white);
        }
        else if(GlobalVars::spd_modelStr.contains("G20")) {

            emit tx_miscOperations(Tool_SPD, 1,  100, "");
            emit tx_TextBoxOutput(Tool_SPD, 1, "---------------------------------", false, false, Qt::white);
        }
    }
}
void BackEndClass::getADB_AssignPortCmd(QByteArray ba)
{
    qDebug()<<"getADB_AssignPortCmd::  "<<ba;


    emit tx_TextBoxOutput(Tool_SPD, 1, ba, false, false, GlobalVars::txtOutPutColor);
     if(adb_InfoOnly == false){
         currentADB = ADB_ChinoeIMEICMTool;
         timer_SPDTool->start(1500);
         emit tx_miscOperations(Tool_SPD, 1,  80, "");
     }
     else {
            emit tx_miscOperations(Tool_SPD, 1, 100, "");
     }
}
void BackEndClass::getADB_Original_imei(QByteArray ba)
{
    //qDebug()<<"getADB_Original_imei::  "<<ba;
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("Original IMEI: "+ba), false, false, GlobalVars::txtOutPutColor);
    currentADB = ADB_Reboot;
    timer_SPDTool->start(500);

}
void BackEndClass::getADB_rebootCommand(QByteArray ba)
{
    qDebug()<<"getADB_rebootCommand::  "<<ba;
    ba.replace("\n", "");
    ba.replace("\r", "");
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("Reboot: "+ba), false, false, GlobalVars::txtOutPutColor);
    //qDebug()<<" TootType indx : "<<currentToolType;
    if(timer_SPDTool->isActive()) timer_SPDTool->stop();
    if(adb_InfoOnly == false) {
        if(currentToolType == Tool_UnLock)
        {
            currentADB = ADB_UnLockSIM;
            timer_SPDTool->start(2000);
            emit tx_miscOperations(Tool_SPD, 1, 50, "");
        }
        else if(currentToolType == Tool_SPD){
            currentADB = ADB_ChinoeIMEICMTool;
            actualADBcommands = true;
            timer_SPDTool->start(2000);
            emit tx_miscOperations(Tool_SPD, 1, 50, "");
        }
    } else {
        if(GlobalVars::spd_modelStr.contains("E6I")||GlobalVars::spd_modelStr.contains("E7I")) {
            if(GlobalVars::spd_AssignPort_bool)
            {
                currentADB = ADB_AssignPortCmd;
                timer_SPDTool->start(1500);
                emit tx_miscOperations(Tool_SPD, 1,  70, "");
            }
            else
            {
                emit tx_miscOperations(Tool_SPD, 1,  100, "");
                emit tx_TextBoxOutput(Tool_SPD, 1, "---------------------------------", false, false, Qt::white);
            }
        }
    }
}

void BackEndClass::getADB_ChinoeIMEICMTool(QByteArray ba)
{
    qDebug()<<"getADB_ChinoeIMEICMTool::  "<<ba;

    if(ba.contains("Connect device fail")||ba.contains("Connect To cellphone = Fail"))
    {
        emit tx_TextBoxOutput(Tool_SPD, 1, "Error: Device not connected ", false, false, Qt::red);
        emit tx_miscOperations(Tool_SPD, 1, 100, "");
        return;
    }

    if(GlobalVars::spd_modelStr.contains("E6I")||GlobalVars::spd_modelStr.contains("E7I")) {
        get_ChinoeDirectory1(ba);
    }
    else if(GlobalVars::spd_modelStr.contains("E6s XT2053")||GlobalVars::spd_modelStr.contains("G8 Power Lite XT2055")||GlobalVars::spd_modelStr.contains("E7 Play XT2095")) {
        get_ChinoeDirectory2(ba);
    }
    else if(GlobalVars::spd_modelStr.contains("G20")) {
        get_ChinoeDirectory3(ba);
    }


    /*


    int indx = ba.indexOf("Write IMEI1");
    ba = ba.right(ba.length() - indx);
    indx=0;
    indx = ba.indexOf("Backup ");
    if(indx <3) indx = ba.indexOf("\r\nWork don");
    QString sendToGUI = ba.left(indx);
    sendToGUI.replace("\r", "");
    sendToGUI = sendToGUI.left(sendToGUI.length()-2);
    qDebug()<<"\n\n \t sendToGUI: "<<sendToGUI;
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("Tool: "+sendToGUI), false, false, GlobalVars::txtOutPutColor);
    qDebug()<<" Left over info: "<<ba;
    if(ba.contains("Work don")){

        //qDebug()<<" Dual IMEI: "<<GlobalVars::dual_imei_bool;
        if(GlobalVars::spd_modelStr.contains("E6I")||GlobalVars::spd_modelStr.contains("E7I")) {
            if(!nowForSecondIMEI && GlobalVars::spd_dual_imei_bool){
                nowForSecondIMEI = true; timer_SPDTool->start(1000);
            }
        }
        else if(GlobalVars::spd_modelStr.contains("E6s XT2053")||GlobalVars::spd_modelStr.contains("G8 Power Lite XT2055")||GlobalVars::spd_modelStr.contains("E7 Play XT2095")) {
            if(!nowForSecondIMEI && GlobalVars::spd_dual_imei_bool){
                nowForSecondIMEI = true; timer_SPDTool->start(1000);
            }
        }
        else if(GlobalVars::spd_modelStr.contains("G20")) {

        }

        if(GlobalVars::spd_dual_imei_bool == false) nowForSecondIMEI = true;

        //if(timer_singleShot->isActive()) qDebug()<<" QTimer is started Again: "<<nowForSecondIMEI<<"  Dual IMEI: "<<GlobalVars::dual_imei_bool;
        //else qDebug()<<" QTimer is not Active : "<<nowForSecondIMEI<<"  Dual IMEI: "<<GlobalVars::dual_imei_bool;


        if(nowForSecondIMEI){
            emit tx_TextBoxOutput(Tool_SPD, 1, "Task successfuly Completed", false, false, GlobalVars::txtOutPutColor);
            emit tx_miscOperations(Tool_SPD, 1, 100, "");
        }
        else
        {
            emit tx_TextBoxOutput(Tool_SPD, 1,"Task IMEI-1 Completed.", false, false, GlobalVars::txtOutPutColor);
            emit tx_TextBoxOutput(Tool_SPD, 1,"Writing IMEI-2: wait.", true, false, Qt::white);
            emit tx_miscOperations(Tool_SPD, 1, 75, "");
        }
    } */

}
void BackEndClass::get_ChinoeDirectory1(QByteArray ba)
{
    qDebug()<<"\n\n \t actual Bytes Dir-1: "<<ba;
    int indx = ba.indexOf("Write IMEI1");
    ba = ba.right(ba.length() - indx);
    indx=0;
    indx = ba.indexOf("Backup ");
    if(indx <3) indx = ba.indexOf("\r\nWork don");
    QString sendToGUI = ba.left(indx);
    sendToGUI.replace("\r", "");
    sendToGUI = sendToGUI.left(sendToGUI.length()-2);
    qDebug()<<"\n\n \t sendToGUI: "<<sendToGUI;
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("Tool: "+sendToGUI), false, false, GlobalVars::txtOutPutColor);
    qDebug()<<" Left over info: "<<ba;
    if(ba.contains("Work don")){

            if(!nowForSecondIMEI && GlobalVars::spd_dual_imei_bool){
                nowForSecondIMEI = true; timer_SPDTool->start(1000);
            }
        if(GlobalVars::spd_dual_imei_bool == false) nowForSecondIMEI = true;

        //if(timer_singleShot->isActive()) qDebug()<<" QTimer is started Again: "<<nowForSecondIMEI<<"  Dual IMEI: "<<GlobalVars::dual_imei_bool;
        //else qDebug()<<" QTimer is not Active : "<<nowForSecondIMEI<<"  Dual IMEI: "<<GlobalVars::dual_imei_bool;


        if(nowForSecondIMEI){
            emit tx_TextBoxOutput(Tool_SPD, 1, "Task successfuly Completed", false, false, GlobalVars::txtOutPutColor);
            emit tx_miscOperations(Tool_SPD, 1, 100, "");
        }
        else
        {
            emit tx_TextBoxOutput(Tool_SPD, 1,"Task IMEI-1 Completed.", false, false, GlobalVars::txtOutPutColor);
            emit tx_TextBoxOutput(Tool_SPD, 1,"Writing IMEI-2: wait.", true, false, Qt::white);
            emit tx_miscOperations(Tool_SPD, 1, 75, "");
        }
    }
}
void BackEndClass::get_ChinoeDirectory2(QByteArray ba)
{
    qDebug()<<"\n\n \t actual Bytes Dir-2: "<<ba;

    int indx = ba.indexOf("Write IMEI1");
    ba = ba.right(ba.length() - indx);
    indx=0;
    indx = ba.indexOf("Backup ");
    if(indx <3) indx = ba.indexOf("\r\nWork don");
    QString sendToGUI = ba.left(indx);
    sendToGUI.replace("\r", "");
    sendToGUI = sendToGUI.left(sendToGUI.length()-2);
    qDebug()<<"\n\n \t sendToGUI: "<<sendToGUI;
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("Tool: "+sendToGUI), false, false, GlobalVars::txtOutPutColor);
    qDebug()<<" Left over info: "<<ba;
    if(ba.contains("Work don")){
            if(!nowForSecondIMEI && GlobalVars::spd_dual_imei_bool){
                nowForSecondIMEI = true; timer_SPDTool->start(1000);
            }
        if(GlobalVars::spd_dual_imei_bool == false) nowForSecondIMEI = true;

        //if(timer_singleShot->isActive()) qDebug()<<" QTimer is started Again: "<<nowForSecondIMEI<<"  Dual IMEI: "<<GlobalVars::dual_imei_bool;
        //else qDebug()<<" QTimer is not Active : "<<nowForSecondIMEI<<"  Dual IMEI: "<<GlobalVars::dual_imei_bool;


        if(nowForSecondIMEI){
            emit tx_TextBoxOutput(Tool_SPD, 1, "Task successfuly Completed", false, false, GlobalVars::txtOutPutColor);
            emit tx_miscOperations(Tool_SPD, 1, 100, "");
        }
        else
        {
            emit tx_TextBoxOutput(Tool_SPD, 1,"Task IMEI-1 Completed.", false, false, GlobalVars::txtOutPutColor);
            emit tx_TextBoxOutput(Tool_SPD, 1,"Writing IMEI-2: wait.", true, false, Qt::white);
            emit tx_miscOperations(Tool_SPD, 1, 75, "");
        }
    }
}
void BackEndClass::get_ChinoeDirectory3(QByteArray ba)
{
    qDebug()<<"***************************************\n\n \t G20 Dir-3: "<<ba<<"\n***************************************";

    int indx = ba.indexOf("[ Start Test ]");
    ba = ba.right(ba.length() - indx);
    qDebug()<<"\n\n \t actual Bytes Dir-3: "<<ba;
    indx=0;
    indx = ba.indexOf("Backup ");
    if(indx <3) indx = ba.indexOf("\r\nWork don");
    QString sendToGUI = ba.left(indx);
    sendToGUI.replace("\r", "");
    sendToGUI = sendToGUI.left(sendToGUI.length()-2);
    qDebug()<<"\n\n \t sendToGUI: "<<sendToGUI;
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("Tool: "+sendToGUI), false, false, GlobalVars::txtOutPutColor);
    qDebug()<<" Left over info: "<<ba;
    if(ba.contains("Work don")){

        //qDebug()<<" Dual IMEI: "<<GlobalVars::dual_imei_bool;
        if(GlobalVars::spd_modelStr.contains("E6I")||GlobalVars::spd_modelStr.contains("E7I")) {
            if(!nowForSecondIMEI && GlobalVars::spd_dual_imei_bool){
                nowForSecondIMEI = true; timer_SPDTool->start(1000);
            }
        }
        else if(GlobalVars::spd_modelStr.contains("E6s XT2053")||GlobalVars::spd_modelStr.contains("G8 Power Lite XT2055")||GlobalVars::spd_modelStr.contains("E7 Play XT2095")) {
            if(!nowForSecondIMEI && GlobalVars::spd_dual_imei_bool){
                nowForSecondIMEI = true; timer_SPDTool->start(1000);
            }
        }
        else if(GlobalVars::spd_modelStr.contains("G20")) {

        }

        if(GlobalVars::spd_dual_imei_bool == false) nowForSecondIMEI = true;

        //if(timer_singleShot->isActive()) qDebug()<<" QTimer is started Again: "<<nowForSecondIMEI<<"  Dual IMEI: "<<GlobalVars::dual_imei_bool;
        //else qDebug()<<" QTimer is not Active : "<<nowForSecondIMEI<<"  Dual IMEI: "<<GlobalVars::dual_imei_bool;


        if(nowForSecondIMEI){
            emit tx_TextBoxOutput(Tool_SPD, 1, "Task successfuly Completed", false, false, GlobalVars::txtOutPutColor);
            emit tx_miscOperations(Tool_SPD, 1, 100, "");
        }
        else
        {
            emit tx_TextBoxOutput(Tool_SPD, 1,"Task IMEI-1 Completed.", false, false, GlobalVars::txtOutPutColor);
            emit tx_TextBoxOutput(Tool_SPD, 1,"Writing IMEI-2: wait.", true, false, Qt::white);
            emit tx_miscOperations(Tool_SPD, 1, 75, "");
        }
    }
}



void BackEndClass::getADB_UnLockSIMTool(QByteArray ba)
{
    qDebug()<<"getADB_UnLockSIMTool::  "<<ba;
    if(ba.contains("device fail"))
    {
        emit tx_TextBoxOutput(Tool_SPD, 1,"Error: Device Fail to connect.", true, false, Qt::red);
        return;
    }
    //int idx = ba.indexOf("UnlockCode generated:");
    int idx = ba.indexOf("Unlock");
    ba = ba.right(ba.length()-idx);
    idx = ba.indexOf(":");
    idx += 1;
    ba = ba.right(ba.length()- idx);
    qDebug()<<" right: "<<ba;
    idx = 0;
    idx = ba.indexOf("Backup NVRAM");
    qDebug()<<"idx::"<<idx;
    if(idx <2) idx = ba.indexOf("\r\nWork");
    qDebug()<<"left IDX:"<<idx<<" bytes:"<<ba;
    QString unLockCode = ba.left(idx);
    unLockCode.replace("\r","");
    unLockCode.replace("\n","");
    emit tx_TextBoxOutput(Tool_SPD, 1, QString("unLock Code: "+unLockCode), true, false, Qt::white);
    if(ba.contains("Work do"))
    {
        emit tx_TextBoxOutput(Tool_SPD, 1, "Task successfuly Completed", false, false, GlobalVars::txtOutPutColor);
        emit tx_miscOperations(Tool_SPD, 1, 100, "");
    }
    else
    {
        emit tx_TextBoxOutput(Tool_SPD, 1, "Task Failed", false, false, Qt::red);
        emit tx_miscOperations(Tool_SPD, 1, 100, "");
    }

}

void BackEndClass::rx_ADB_ErrorReceived()
{
    spd_commandError = adbProcess->readAllStandardError();
    qDebug()<<" rx_ADB_ErrorReceived Error:: "<<spd_commandError;
    if(currentADB == ADB_Reboot && spd_commandError.length()>2){
        getADB_rebootCommand(spd_commandError.toUtf8());
    }
    if(spd_commandError.contains("adb.exe: no devices/emulators found")){
        emit tx_TextBoxOutput(Tool_SPD, 1, spd_commandError, true, true, Qt::red);
        emit tx_miscOperations(Tool_SPD, 1, 100, "");
    }
}
void BackEndClass::rx_updateADBdevices()
{
    //qDebug()<<"rx_updateADBdevices:: reading ADB Devices ";
    processStartStop = true;
    adb_InfoOnly = true;
    nowForSecondIMEI = false;
    actualADBcommands = false;
    currentToolType = Tool_SPD;
    adbProcess->setWorkingDirectory(adbFolderPath);

    currentADB = ADB_ConnectedDevice;
    spd_commandStr = "adb devices";
    //qDebug()<<" write:"<<adbProcess->write(commandStr.toUtf8());
    //commandStr = "adb shell getprop ro.product.vendor.model";
    shellArguments.clear();
    shellArguments.append("/C");
    shellArguments.append(spd_commandStr);
    adbProcess->close();
    adbProcess->waitForReadyRead(1000);
    adbProcess->setArguments(shellArguments);
    //qDebug()<<" Arg::"<<shellArguments;
    adbProcess->start();
    //qDebug()<<"\n\n \t Process Opening: "<<adbProcess->open(QIODevice::ReadWrite);

    if (adbProcess->waitForStarted(1000) == false)
        qDebug() << "Error starting external program";
    else
        qDebug() << "external program running";

    //adbProcess->setTextModeEnabled(true);
    //adbProcess->waitForReadyRead(1000);
    adbProcess->waitForFinished(1000);
}




bool BackEndClass::getServerIMEInumber_updateGlobal()
{
    if(!GlobalVars::spd_manual_imei_bool)
    {
        QByteArray postdata;// = data.toJson();
        postdata.append(QString("model="+GlobalVars::spd_projectName+"&").toUtf8());
        postdata.append(QString("simCode="+QString(GlobalVars::spd_dual_imei_bool? "ds":"ss")).toUtf8());

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
        //req.setUrl(QUrl("http://server51214110.ngrok.io/imeiGenerator.php"));
        req.setUrl(QUrl("http://107.175.87.146:16777/imeiGenerator.php"));
        //req.setUrl((QUrl("http://111.88.102.45:1500/imeiGenerator.php")));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        //qDebug()<<"URL:: "<<req.url();


        QNetworkReply *reply = nam.post(req, encypted);
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if(processStartStop ==  false){
            return false;
        }
        emit tx_miscOperations(currentToolType, 1, 40, "");
        emit tx_TextBoxOutput(currentToolType, 1, QString("Geting IMEI from Server: OK"), false, false, GlobalVars::txtOutPutColor);

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
            emit tx_miscOperations(currentToolType, 1, 100, "");
            emit tx_TextBoxOutput(currentToolType, 1, "Failed to get IMEI from Server", true, false, QColor::fromRgb(255, 192, 192));
            return false;
        }
        if (GlobalVars::spd_dual_imei_bool) //(buffer.toLower().contains("&"))
        {
            int idx = buffer.indexOf("&");
            if(idx < 5) {
                emit tx_miscOperations(currentToolType, 1, 100, "");
                emit tx_TextBoxOutput(currentToolType, 1, "Faided to get IMEI from Server", true, false, QColor::fromRgb(255, 192, 192));
                return false;
            }
            GlobalVars::spd_imei_1 = buffer.left(idx);
            buffer.remove(0, (idx+1));
            if(buffer.length() > 5){

                GlobalVars::spd_imei_2 = buffer;
            }
        }
        else
        {
            GlobalVars::spd_imei_1 = buffer;
        }
        //qDebug()<<" Writing this code in cmd: "<<authenReply;
    }

    return true;
}















// ---------------- Serial + MTK Slots and Methods -----------------------

void BackEndClass::rx_updateSerialPorts()
{
    //qDebug()<<" Signal received:  rx_updateSerialPorts ";

    QSerialPortInfo info;
    QList<QSerialPortInfo> ll = info.availablePorts();
    /*qDebug()<<" Total Serial Port: "<<ll.length();
    for(int i=0; i<ll.length(); i++)
    {
        qDebug()<<" Available Serial Port: "<<i<<" Name: "<<ll.at(i).portName()<<" manufact:"<<ll.at(i).manufacturer()<<" descript:"<<ll.at(i).description();
    }
    qDebug()<<"\n\n";
*/
    emit tx_newSerialPortDetected(ll);
}
