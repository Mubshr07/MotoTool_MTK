#ifndef CLASS_LOGFILE_H
#define CLASS_LOGFILE_H

#include <QObject>
#include <globalvars.h>

class Class_LogFile : public QObject
{
    Q_OBJECT
public:
    explicit Class_LogFile(QObject *parent = nullptr);

signals:

public slots:
    void rx_initialFileFolders();
    void writeToLog(QByteArray ba);


public:

private:
    QFile *logFile;
    QDir logDir;
    QString filePath;
    QString appPath;





};

#endif // CLASS_LOGFILE_H
