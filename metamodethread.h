#ifndef METAMODETHREAD_H
#define METAMODETHREAD_H

#include <QObject>
#include "globalvars.h"

class MetaModeThread : public QObject
{
    Q_OBJECT
public:
    explicit MetaModeThread(QObject *parent = nullptr);

signals:
    void tx_ProcessCompleted_metaMode(int idx, bool complt);
    void tx_TextBoxOutput_metaMode(TOOL_TYPE tool, int idx, QString s, bool isBold, bool newline, QColor color);
    void tx_miscOperations_metaMode(TOOL_TYPE tool, int idx, int value, QString str);


public slots:
    void setGeneralIndex_metaMode(int idx);
    void rx_StartRepairing_metaMode(int idx, bool startStop, TOOL_TYPE tool);
    void rx_CommandLine_OutPutReceived();
    void rx_CommandLine_ErrorReceived();
    void rx_timer_Meta_singleShot();

private:
    QSerialPort *serialport;
    QProcess *metaProcess;
    QTimer *timer_Meta_singleShot;

    int generalIndex = 10;
    QString commandlineOutPut = "";
    QString commandError = "";
    QString commandStr = "";

    void getAuthenticationFromServer(QString strData, QString strIMEI);
    bool getServerIMEInumber_updateGlobal();
    QByteArray authenReply;
    QNetworkReply *reply;
    QNetworkAccessManager *nam;

    QString powerShellPath =  "C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe";
    QString cmdShellPath =  "C:/windows/system32/cmd.exe";
    QString m3FolderPath = QString(QDir::currentPath() + "/M3");
    QString adbFolderPath = QString(QDir::currentPath() + "/ADB");
    QString ChinoeIMEICMToolFolderPath_Data1 = QString(QDir::currentPath() + "/DATA/DATA1");
    QString ChinoeIMEICMToolFolderPath_Data2 = QString(QDir::currentPath() + "/DATA/DATA2");
    QString ChinoeIMEICMToolFolderPath_Data3 = QString(QDir::currentPath() + "/DATA/DATA3");

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

    MMM_Commands currentCommand = MMM_Idle;
    TOOL_TYPE currentToolType = Tool_MTK;

    bool showTheConsole = false;


    void check_Repair_Cmd_outPut(QString str);
    void check_UnLock_Cmd_outPut(QString str);

};

#endif // METAMODETHREAD_H
