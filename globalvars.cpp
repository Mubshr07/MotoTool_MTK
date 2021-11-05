#include "globalvars.h"

QString GlobalVars::motoTool_SettingsFilePath="";

QString GlobalVars::meta_imei_1[8] = {"", "", "", "", "", "", "", ""};
QString GlobalVars::meta_imei_2[8] = {"", "", "", "", "", "", "", ""};
QString GlobalVars::meta_serialPortName[8] = {"", "", "", "", "", "", "", ""};
int GlobalVars::meta_serialIndex[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int GlobalVars::meta_modelIndex[8] = {0, 0, 0, 0, 0, 0, 0, 0};
QString GlobalVars::meta_modelStr[8] = {"", "", "", "", "", "", "", ""};
QString GlobalVars::meta_projectName[8] = {"", "", "", "", "", "", "", ""};
QString GlobalVars::meta_projectDataType[8] = {"", "", "", "", "", "", "", ""};
QString GlobalVars::meta_projectUnlockType[8] = {"", "", "", "", "", "", "", ""};
bool GlobalVars::meta_dual_imei_bool[8] = {false, false, false, false, false, false, false, false};
bool GlobalVars::meta_manual_imei_bool[8] = {false, false, false, false, false, false, false, false};
bool GlobalVars::meta_unLock_bool[8] = {false, false, false, false, false, false, false, false};
bool GlobalVars::meta_MDM_bool[8] = {false, false, false, false, false, false, false, false};
bool GlobalVars::meta_reboot_bool[8] = {false, false, false, false, false, false, false, false};


QString GlobalVars::spd_imei_1 = "";
QString GlobalVars::spd_imei_2 = "";
int GlobalVars::spd_modelIndex = 0;
QString GlobalVars::spd_modelStr = "";
bool GlobalVars::spd_dual_imei_bool = false;
bool GlobalVars::spd_manual_imei_bool = false;
bool GlobalVars::spd_reboot_bool = false;
bool GlobalVars::spd_AssignPort_bool = false;
QString GlobalVars::spd_projectName = "";
QString GlobalVars::spd_projectDataType = "";

QList<QSerialPortInfo> GlobalVars::availablePortsList;

QColor GlobalVars::txtOutPutColor = QColor::fromRgb(255, 153, 0);


QString GlobalVars::authorizedToken = "";
float GlobalVars::userInfo_creditDetails=0.0;
QString GlobalVars::userInfo_HardwareKey = "";
QString GlobalVars::userInfo_UserName = "";
float GlobalVars::operationID = 0.0;




QString GlobalVars::api_MTKcharges = QString("http://phpstack-682336-2247161.cloudwaysapps.com/api/MTK/charge");
QString GlobalVars::api_MTKimeiGenerator = QString("http://phpstack-682336-2247161.cloudwaysapps.com/api/MTK/imei");
QString GlobalVars::api_MTKserverAuthenticity = QString("http://phpstack-682336-2247161.cloudwaysapps.com/api/MTK/auth");
QString GlobalVars::api_logURLQString = QString("http://phpstack-682336-2247161.cloudwaysapps.com/api/login");
QString GlobalVars::api_serverStatusQString = QString("http://phpstack-682336-2247161.cloudwaysapps.com/api/status");
QString GlobalVars::api_VersionQString = QString("http://phpstack-682336-2247161.cloudwaysapps.com/api/version");
QString GlobalVars::api_LogQString = QString("http://phpstack-682336-2247161.cloudwaysapps.com/api/log");
QString GlobalVars::api_PerformQString = QString("http://phpstack-682336-2247161.cloudwaysapps.com/api/perform");


GlobalVars::GlobalVars()
{

}

void GlobalVars::initalizeToZero()
{

    GlobalVars::spd_imei_1 = "";
    GlobalVars::spd_imei_2 = "";
    GlobalVars::spd_modelIndex = 0;
    GlobalVars::spd_modelStr = "";
    GlobalVars::spd_projectName = "";
    GlobalVars::spd_projectDataType = "";
    GlobalVars::spd_dual_imei_bool = false;
    GlobalVars::spd_reboot_bool = false;
    GlobalVars::spd_AssignPort_bool = false;
    GlobalVars::availablePortsList.clear();

    GlobalVars::txtOutPutColor = QColor::fromRgb(255, 153, 0);

    GlobalVars::motoTool_SettingsFilePath=QString(QDir::homePath()+"/motoToolSettings");
}
