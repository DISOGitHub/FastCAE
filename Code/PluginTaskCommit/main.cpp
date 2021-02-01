#include "TaskProWidget.h"
#include "TaskMgeWidget.h"
#include "DownTaskDialog.h"

#include <QApplication>
#include <QTranslator>

using namespace  Plugins;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator* myTranslator=new QTranslator;
    myTranslator->load("D:/TaskCommit_zh_CN.qm");
    a.installTranslator(myTranslator);
    TaskDataObserver::getInstance()->init();
    Plugins::TaskProWidget w;
    Plugins::TaskMgeWidget w2;
    w.setTMgeWidget(&w2);
    w.show();
    w2.show();
    w.init();
    w2.init();
    return a.exec();
}
