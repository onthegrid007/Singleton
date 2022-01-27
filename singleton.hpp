#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "noncopyable.h"
#include "nonmoveable.h"

template<typename T>
class Singleton : public std::NonMovable, public std::NonCopyable {
    public:
    static T& GetInstance() { return Instance; }

    private:
    static T Instance;
};

#define _SINGLETON_CHILD_DEFINITIONS(T) typedef T::Singleton<T> S; inline T S::Instance;

#endif