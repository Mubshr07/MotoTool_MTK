#ifndef BACKENDCLASS_H
#define BACKENDCLASS_H

#include <QObject>
#include "globalvars.h"


class BackEndClass : public QObject
{
    Q_OBJECT
public:
    explicit BackEndClass(QObject *parent = nullptr);

signals:
    void tx_ProcessCompleted(bool complt);
    void tx_TextBoxOutput(TOOL_TYPE tool, int idx, QString s, bool isBold, bool newline, QColor color);
    void tx_newSerialPortDetected(QList<QSerialPortInfo> sInfo);
    void tx_miscOperations(TOOL_TYPE tool, int idx, int value, QString str);


public slots:
    void rx_StartRepairing(int idx, bool startStop, TOOL_TYPE tool);
    void rx_updateSerialPorts();
    void rx_updateADBdevices();

    void rx_ADB_OutPutReceived();
    void rx_ADB_ErrorReceived();

    void rx_timer_SPDTool_elapsed();



private:

    QSerialPort *serialport;
    QProcess *adbProcess;
    QTimer *timer_SPDTool;
    int singleShotIndex = 0;


    QString spd_commandlineOutPut = "";
    QString spd_commandError = "";
    QString spd_commandStr = "";

    QString powerShellPath =  "C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe";
    QString cmdShellPath =  "C:/windows/system32/cmd.exe";
    QString m3FolderPath = QString(QDir::currentPath() + "/M3");
    QString adbFolderPath = QString(QDir::currentPath() + "/ADB");

    QString ChinoeIMEICMToolFolderPath_Data1 = QString(QDir::currentPath() + "/DATA/DATA1");
    QString ChinoeIMEICMToolFolderPath_Data2 = QString(QDir::currentPath() + "/DATA/DATA2");
    QString ChinoeIMEICMToolFolderPath_Data3 = QString(QDir::currentPath() + "/DATA/DATA3");
    QString fastBootFolderPath = QString(QDir::currentPath() + "/DATA/fastboot");

    //1. "E6I"
    QString simUnLock_E6I = QString(QDir::currentPath() +"/DATA/DATA1/e6isimlock/fijisc_mexico.att.bin");
    //2. "E7I"
    QString simUnLock_E7I = QString(QDir::currentPath() +"/DATA/DATA1/E7Isimlock/maltalsc_guatemala.tigo.bin");
    //3. "E6s XT2053"
    QString simUnLock_E6sXT2053 = QString(QDir::currentPath() +"/DATA/DATA2/e6eSimlock/SIMLock.MTK.Guatemala.Tigo.20191128.SIMLock");
    //4. "G8 Power Lite XT2055"
    QString simUnLock_G8PowerLite = QString(QDir::currentPath() +"/DATA/DATA2/g8simlock/SIMLock.MTK.Guatemala.Claro.20200720.SIMLock");
    //5. "E7 Play XT2095"
    QString simUnLock_E7PlayXT2095 = QString(QDir::currentPath() +"/DATA/DATA2/E7simlock/SIMLock.MTK.Guatemala.Tigo.20201014.SIMLock");
    //6. "G20"
    QString simUnLock_G20 = QString(QDir::currentPath() +"/DATA/DATA3/g20simlock/simlock-attmx_region.bin");

    QStringList shellArguments;
    bool processStartStop = false;
    bool showThisinDebug = false;
    bool adb_InfoOnly = false;
    bool actualADBcommands = false;
    MMM_Commands currentCommand[8] = {MMM_Idle,MMM_Idle,MMM_Idle,MMM_Idle,MMM_Idle,MMM_Idle,MMM_Idle,MMM_Idle};
    TOOL_TYPE currentToolType = Tool_MTK;
    ADB_Commands currentADB = ADB_Idle;

    void getConnectedDevices_andSendThemGUI(QByteArray ba);
    bool getServerIMEInumber_updateGlobal();
    bool getServerFastBoot_File();
    QVector<QString> adbDevicesRaw;
    bool adbDeviceDetectedandAuthorized = false;

    QString frp_ProdName = "", frp_uniqID = "";
    QByteArray frp_PassWord;
    bool frp_infoCompleted = false;
    int frpInfoIndex = 0;
    QString fastBoot_FileName ;
    QString frp_DeviceID = "";
    void getADB_Model(QByteArray ba);
    void getADB_AndroidVersion(QByteArray ba);
    void getADB_BuildDate(QByteArray ba);
    void getADB_SerialNumber(QByteArray ba);
    void getADB_AssignPortCmd(QByteArray ba);
    void getADB_Original_imei(QByteArray ba);
    void getADB_rebootCommand(QByteArray ba);
    void getADB_ChinoeIMEICMTool(QByteArray ba);
    void get_ChinoeDirectory1(QByteArray ba);
    void get_ChinoeDirectory2(QByteArray ba);
    void get_ChinoeDirectory3(QByteArray ba);


    void get_FRP_GetServerFile(QByteArray ba);
    void get_FRP_PushFile(QByteArray ba);
    void get_FRP_FastBootErase(QByteArray ba);
    void get_FRP_Fast_Reboot(QByteArray ba);


    void getFastBoot_info(QByteArray ba);

    void getADB_UnLockSIMTool(QByteArray ba);
    bool nowForSecondIMEI = false;

};

#endif // BACKENDCLASS_H
