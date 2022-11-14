/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2022, Peter Ferranti
*   All rights reserved.
*/

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "noncopyable.h"
#include "nonmoveable.h"

#if _GLIBCXX_USE_CXX11_ABI
#  define SINLINE inline cxx11 __attribute__((abi_tag("cxx11")))
#else
#  define SINLINE inline
#endif

template<typename T>
class Singleton : public NonMovable, public NonCopyable {
    public:
    typedef T Type;
    static T& GetInstance() { return T::template Singleton<T>::Instance; }
    const bool IsAvaliable() {
        return !IsDestructed;
    }
    static void InstanceDestruction() {
        bool IsDestructed = true;
    }

    protected:
    SINLINE static T Instance;
    SINLINE static bool IsDestructed;
};

#define _SINGLETON_CHILD_DECLORATIONS(T) friend class Singleton;
#define _SINGLETON_CHILD_DEFINITIONS(T) typedef T::template Singleton<T> S; template<> SINLINE T S::Instance = T(); template<> SINLINE bool S::IsDestructed = false;

#endif