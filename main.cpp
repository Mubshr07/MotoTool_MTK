#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QIODevice>
#include <QIcon>

#include "qdebug.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<QList<QSerialPortInfo>>();
    qRegisterMetaType<MMM_Commands>();
    qRegisterMetaType<TOOL_TYPE>();
    qRegisterMetaType<ADB_Commands>();










    QString fullStyleSheetData="";
    QString fileLocation = ":/Incrypt.qss";
    if(QFile::exists(fileLocation))
    {
        QFile *styFile = new QFile(fileLocation);
        if(styFile->open(QIODevice::ReadOnly)){
            fullStyleSheetData = QLatin1String(styFile->readAll());
            //qDebug()<<" File open and read sucess: "<<fullStyleSheetData;
        }
        else {
            qDebug()<<" Error in opening file: ";
        }
        styFile->close();
    }
    else {
        qDebug()<<" File did not found ";
    }
    QIcon icon(":/pics/logo round.png");



    //FirstWindow w;
    MainWindow w;
    w.setStyleSheet(fullStyleSheetData);
    w.setWindowIcon(icon);
    w.show();
    return a.exec();
}
