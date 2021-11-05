#include "class_logfile.h"

Class_LogFile::Class_LogFile(QObject *parent) : QObject(parent)
{
    appPath = QDir::currentPath();
    //qDebug()<<" appPath Path: "<<appPath;
    logDir.setPath(QString(appPath+"/Log/"));
    if(!logDir.exists()) logDir.mkdir(QString(appPath+"/Log/"));

    filePath = QString(QString(appPath+"/Log/MotoTool_MTK_LogFile_"+QDate::currentDate().toString("dd:MM:yyyy").replace(":","_")+".txt"));

    //()<<" Log File Path: "<<filePath;
}


void Class_LogFile::rx_initialFileFolders()
{
    //qDebug()<<"rx_initialFileFolders ";
    if(!QFile::exists(filePath)){
        //qDebug()<<"rx_initialFileFolders ";
        logFile = new QFile(filePath);
        if(logFile->open(QIODevice::WriteOnly))
        {
           // qDebug()<<" New File is generated ";
            QByteArray baa("Welcom to MOTO TOOL MTK Software \n +++++++++++++++++++++++++++++++++++++++++++ \n");
            logFile->write(baa);
            logFile->close();
        }
    }
    else
    {
        logFile = new QFile(filePath);
    }

}

void Class_LogFile::writeToLog(QByteArray ba)
{
    if(logFile->open(QIODevice::Append))
    {
        QByteArray info("System Time: ");
        info.append(QTime::currentTime().toString("HH:mm:ss").toUtf8());
        info.append("\n");
        logFile->write(info);

        logFile->write(ba);
        logFile->write("\n========================================================\n\n");
        logFile->close();
    }





    QByteArray postdata;// = data.toJson();
    postdata.append(QString("OperationId="+ QString::number(58) +"&").toUtf8());
    postdata.append(QString("Log="+ ba).toUtf8());
    qDebug()<<" \n\n ByteArray:"<<postdata<<"\n\n";


    QEventLoop loop;
    QNetworkAccessManager nam;
    QNetworkRequest req;
    req.setUrl(QUrl(GlobalVars::api_LogQString));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setRawHeader(QByteArray("Authorization"), GlobalVars::authorizedToken.toUtf8());

    QNetworkReply *reply = nam.post(req, postdata.toBase64());
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    //qDebug()<<" Reply:: "<<reply->readAll();

    QJsonDocument document = QJsonDocument::fromJson(QByteArray::fromBase64(reply->readAll()));
    QJsonObject buffer = document.object();

    qDebug()<<"Log reply:: "<<buffer;
    qDebug()<<"\n\n ";











}
