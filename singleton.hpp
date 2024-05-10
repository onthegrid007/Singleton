/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2024, Peter Ferranti
*   All rights reserved.
*/

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "noncopyable.h"
#include "nonmoveable.h"
#include "inline_abi_macros.h"

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
    ABI_INLINE static T Instance{};
    ABI_INLINE static bool IsDestructed;
};

#define _SINGLETON_CHILD_DECLORATIONS(T) friend class Singleton;

#endif