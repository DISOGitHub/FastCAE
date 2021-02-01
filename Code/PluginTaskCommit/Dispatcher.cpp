#include "Dispatcher.h"
#include "TaskData.h"
#include <QDebug>

using namespace  Plugins;
Dispatcher::Ptr Dispatcher::_ptr = nullptr;

void Dispatcher::dispatch(const TransferData::Ptr &data)
{
    qDebug() << "void Dispatcher::dispatch(const TransferData &&data)";
    TASK_STATE type = data->taskType();
    switch (type) {
    case    TASK_STATE_LGN: /// login
    case    TASK_STATE_LGNFAILED:
    case    TASK_STATE_LGNCOMPLETE:
    case    TASK_STATE_CREATE:
    case    TASK_STATE_CREATED:
    case    TASK_STATE_CREATEDFAILED:
    case    TASK_STATE_CREATECOMPLETE:
    case    TASK_STATE_DEL:
    case    TASK_STATE_DELFAILED:
    case    TASK_STATE_DELCOMPLETE:
    case    TASK_STATE_UCOMMIT:
    case    TASK_STATE_UCOMMITEDFAILED:
    case    TASK_STATE_UCOMMITEDCOMPLETE:
    case    TASK_STATE_CALC:
    case    TASK_STATE_CALCING:
    case    TASK_STATE_CALCFAILED:
    case    TASK_STATE_CALCOMPLETE:
    case    TASK_STATE_DRESULT:
    case    TASK_STATE_DRESULTFAILED:
    case    TASK_STATE_DRESULTCOMPLETE:
    case    TASK_STATE_SEL:
    case    TASK_STATE_SELFAILED:
    case    TASK_STATE_SELCOMPLETE:
    case  TASK_STATE_STOP:
    case  TASK_STATE_STOPFAILED:
    case  TASK_STATE_STOPCOMPLETE:
    {
        for(const auto& f : actions)
        {
            f(data);
        }
    }
        break;
    default:
        break;
    }

}
