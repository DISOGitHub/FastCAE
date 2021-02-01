#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <memory>
#include <vector>
#include <functional>

#include "NonCopyable.h"
#include "ActionType.h"
#include "TaskData.h"

namespace Plugins {

/**
 * @brief The Dispatcher class
 * 分配前端action到相应的数据处理区 *
 */
class Dispatcher : public NonCopyable
{
public:
    using Ptr = std::shared_ptr<Dispatcher>;
    using STORE_FUN = std::function<void(TransferData::Ptr)>;
    Dispatcher() = default;
    ~Dispatcher() = default;
    /// waring mul thread
    static auto getInstance()
    {
//       Dispatcher* d = new Dispatcher;
       if(_ptr == nullptr)
           _ptr = CreateInstance::Instance<Dispatcher>();
       return _ptr;
    }

    template<class F>
    void regis(F&& fun)
    {
        actions.emplace_back(std::forward<F>(fun));
    }

    template<class F>
    void regis(const F& fun)
    {
        actions.emplace_back(std::move(fun));
    }

    void clear()
    {
        std::vector<STORE_FUN> v;
        actions.swap(v);
    }
    void dispatch(const TransferData::Ptr &data);

private:
    std::vector<STORE_FUN> actions;
    static Dispatcher::Ptr _ptr;
};
}
#endif // DISPATCHER_H
