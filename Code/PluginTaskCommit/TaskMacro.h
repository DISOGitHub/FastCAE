#ifndef TASKMACRO_H
#define TASKMACRO_H

#define TABLE_CUR_ROW(row, obj) \
do { \
    obj *senderObj=qobject_cast<obj*>(sender()); \
    if(senderObj == nullptr)  \
    { \
       return; \
    } \
    QModelIndex idx = ui->tableWidget->indexAt(QPoint(senderObj->frameGeometry().x(),senderObj->frameGeometry().y())); \
    row = idx.row(); \
} while (0)

#define TABLE_DELETE_CLICK(obj) \
do { \
    int row; \
    TABLE_CUR_ROW(row,obj) \
    ui->tableWidget->removeRow(row); \
} while (0)

#define MS_TO_HOUR(t) ((t)/3600000.0)

#define REGISTER_DATA_FUN(s,f)   TaskDataObserver::getInstance()->regis(s,f)

#define REGISTER_DIS_FUN(f)    Dispatcher::getInstance()->regis(f)

#define DISPATCH_DIS_FUN(p)    Dispatcher::getInstance()->dispatch(p)

#define DISPATCH_CLEAR()       Dispatcher::getInstance()->clear();

#define GetStorePtr            TaskStore::getInstance()

/// #define GetActionImpl          TaskActionImpl::getInstance()

#define TRANSFER_DATA_FUN(ptr , t) \
do{ \
    ptr->setTaskType(t); \
    DISPATCH_DIS_FUN(ptr); \
} while(0)


#define START_FILE_SH(path, exe) \
    do { \
    QString fileName = "start.sh";  \
    QFile fi(path+"/"+fileName); \
    if(!fi.open(QIODevice::ReadWrite)) \
    { \
        qDebug() << "file can not open"; \
    } \
    QString str = QString("#!/bin/bash\nsrun -N %1 -n %2 ./%3\n").arg(dptr->getNodeSize()).arg(dptr->getCpuSize()).arg(#exe); \
    QTextStream out(&fi); \
	out.setCodec("utf-8");\
    out << str; \
    fi.close(); \
    } while (0)

#define PROMPT_MSS "prompt"
#define NOT_LOGIN "do not login"
#define NOT_CREATED "create task failed: sql error"

#define ABOUT "about"
#define INPUT_NAME_WARNING "please intput task name"

#if 0
#define COMMIT_TEXT "commit"
#define CALC_TEXT "calc"
#define STOP_TEXT "stop"
#define DOWN_TEXT "down"
#define DELETE_TEXT "delete"
#define UPDATE_TEXT "query"

#define CREATE_ACTION_TEXT "CreateTask"
#define COMMIT_ACTION_TEXT "Commit"
#define CALC_ACTION_TEXT "Calculate"
#define DOWN_ACTION_TEXT "Download"

#define TASK_ITEM_TEXT "Tasks"
#define RESULT_ITEM_TEXT "Results"

#define TASK_TREE_TITLE "TaskTree"
#endif

#endif // TASKMACRO_H
