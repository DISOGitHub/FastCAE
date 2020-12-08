#ifndef READDATAFILEPROP_H
#define READDATAFILEPROP_H

#include <QObject>
#include "curve_struct.h"
#include <QFile>
#include <QTextCodec>
#include <qcoreapplication.h>
//#pragma execution_character_set("utf-8")
class readDataFileProp : public QObject
{
    Q_OBJECT
public:
    explicit readDataFileProp(QObject *parent = 0);
    bool initNewFileProp(QString fileName,curve_file_prop *tep_file_prop);
signals:

public slots:
private:
    QTextCodec *codec;
};

#endif // READDATAFILEPROP_H
