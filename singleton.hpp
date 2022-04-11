#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "noncopyable.h"
#include "nonmoveable.h"

template<typename T>
class Singleton : public NonMovable, public NonCopyable {
    public:
    static T& GetInstance() { return Instance; }
    template<typename... Args>
    Singleton<T>(Args ... args) = delete;

    private:
    static T Instance;
    Singleton<T>() = 0;
};

#define _SINGLETON_CHILD_DECLORATIONS(T) friend class Singleton;
#define _SINGLETON_CHILD_DEFINITIONS(T) typedef T::Singleton<T> S; template<> inline T S::Instance;

#endif