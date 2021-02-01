#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H
#include <type_traits>
#include <memory>

namespace CreateInstance {
    template<class T, class... Args>
    typename std::enable_if<!std::is_array<T>::value, std::shared_ptr<T>>::type
    Instance(Args ...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

class NonCopyable
{
protected:
    NonCopyable() = default;
    virtual  ~NonCopyable() = default;
    NonCopyable(const NonCopyable& ) = delete ;
    NonCopyable& operator = (const NonCopyable&) = delete ;

};

#endif // NONCOPYABLE_H
