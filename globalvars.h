#ifndef GLOBALVARS_H
#define GLOBALVARS_H

enum MMM_Commands{
    MMM_Idle,
    MMM_AuthenticationBytes,
    MMM_OK_AfterAuthenticationBytes,
    MMM_Rebooting,
    MMM_Exit
};
enum TOOL_TYPE{
    Tool_MTK,
    Tool_MTK_UnLock,
    Tool_SPD,
    Tool_UnLock
};
enum ADB_Commands{
    ADB_Idle,
    ADB_ConnectedDevice,
    ADB_Model,
    ADB_AndroidVersion,
    ADB_BuildDate,
    ADB_SerialNumber,
    ADB_AssignPortCmd,
    ADB_Original_imei,
    ADB_Reboot,
    ADB_ChinoeIMEICMTool,
    ADB_UnLockSIM,
    ADB_verify
};


#include <QColor>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QIODevice>
#include <QTextStream>
#include <QStringList>
#include <QVector>
#include <QProcess>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QClipboard>
#include <QEventLoop>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QMouseEvent>
#include <QLabel>
#include <QSysInfo>
#include <QMessageBox>
#include <QSsl>
#include <QSslConfiguration>
#include <QCryptographicHash>
#include <QByteArray>
#include <QRegularExpressionValidator>
//#include <QRegExpValidator>



#include "qdebug.h"

class GlobalVars
{
public:
    GlobalVars();


    static QString motoTool_SettingsFilePath;

    static QString meta_imei_1[8];
    static QString meta_imei_2[8];
    static QString meta_serialPortName[8];
    static int meta_serialIndex[8];
    static int meta_modelIndex[8];
    static QString meta_modelStr[8];
    static QString meta_projectName[8];
    static QString meta_projectDataType[8];
    static QString meta_projectUnlockType[8];
    static bool meta_dual_imei_bool[8];
    static bool meta_manual_imei_bool[8];
    static bool meta_unLock_bool[8];
    static bool meta_reboot_bool[8];

    static QString spd_imei_1;
    static QString spd_imei_2;
    static int spd_modelIndex;
    static QString spd_modelStr;
    static QString spd_projectName;
    static QString spd_projectDataType;
    static bool spd_dual_imei_bool;
    static bool spd_manual_imei_bool;
    static bool spd_reboot_bool;

    static bool spd_AssignPort_bool;

    static QList<QSerialPortInfo> availablePortsList;

    static void initalizeToZero();

    static QColor txtOutPutColor;



};


Q_DECLARE_METATYPE(QList<QSerialPortInfo>);
Q_DECLARE_METATYPE(MMM_Commands);
Q_DECLARE_METATYPE(TOOL_TYPE);
Q_DECLARE_METATYPE(ADB_Commands);

#endif // GLOBALVARS_H