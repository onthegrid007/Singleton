/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2022, Peter Ferranti
*   All rights reserved.
*/

#ifndef SINGLETON_CONTAINER_MAP_HPP_
#define SINGLETON_CONTAINER_MAP_HPP_
#include "noncopyable.h"
#include "nonmoveable.h"
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <iostream>

#if _GLIBCXX_USE_CXX11_ABI
#  define SCMINLINE inline cxx11 __attribute__((abi_tag("cxx11")))
#else
#  define SCMINLINE inline 
#endif

template<typename T, template<typename...> typename MT = std::unordered_map>
class SingletonContainerMap : public NonMovable, public NonCopyable {
    public:
    typedef T Type;
    typedef std::shared_mutex MType;
    typedef MT<std::string, T*> CType;
    typedef std::lock_guard<MType> LType;
    typedef SingletonContainerMap<T> SCMType;
    // typedef T::template SCMType SCM;
    
    protected:
    SCMINLINE static CType CMap;
    SCMINLINE static MType MTX;
    std::string m_key;
    SingletonContainerMap() {}
    
    public:
    static bool Exists(const std::string key, bool blocking = true) {
        if(blocking) {
            LType lock(T::template SingletonContainerMap<T>::MTX);
            return ((T::template SingletonContainerMap<T>::CMap.find(key) != T::template SingletonContainerMap<T>::CMap.end()) && !T::template SingletonContainerMap<T>::CMap.empty());
        } else {
            return ((T::template SingletonContainerMap<T>::CMap.find(key) != T::template SingletonContainerMap<T>::CMap.end()) && !T::template SingletonContainerMap<T>::CMap.empty());
        }
    }
    
    template<typename... Args>
    static T& CreateNewInstance(const std::string key, Args ... args) {
        LType lock(T:: template SingletonContainerMap<T>::MTX);
        if(!Exists(key, false)) {
            T::template SingletonContainerMap<T>::CMap[key] = std::move(new T(args...));
        }
        T& rtn = *(T::template SingletonContainerMap<T>::CMap[key]);
        if(rtn.m_key != key) rtn.m_key = key;
        return rtn;
    }
    
    template<typename... Args>
    static T& GetInstanceByKey(const std::string key, Args ... args) {
        if(Exists(key)) {
            LType lock(T:: template SingletonContainerMap<T>::MTX);
            return (*(T::template SingletonContainerMap<T>::CMap[key]));
        }
        return CreateNewInstance(key, std::move(args)...);
    }

    static const void DeleteInstanceByKey(const std::string key) {
        if(Exists(key)) {
            LType lock(T:: template SingletonContainerMap<T>::MTX);
            T* destroy = (T*)T::template SingletonContainerMap<T>::CMap[key];
            T::template SingletonContainerMap<T>::CMap.erase(key);
            destroy->~T();
        }
    }
    
    static const std::string GetKeyByInstance(const T& instance) {
        return instance.m_key;
    }

    static const void DeleteInstanceByInstance(const T& instance) {
        // I am aware of the violations, this is meerly still here for thinking
        return DeleteInstanceByKey(GetKeyByInstance(instance));
    }

    const void DeleteInstance() {
        DeleteInstanceByInstance(&this);
    }
};
#define _SCM_CHILD_DECLORATIONS(T) friend class SingletonContainerMap; friend class SingletonContainerMap<T>;
#define _SCM_CHILD_DEFINITIONS(T) template<> SCMINLINE T::template SingletonContainerMap<T>::CType T::template SingletonContainerMap<T>::CMap = T::template SingletonContainerMap<T>::CType(); template<> SCMINLINE T::template SingletonContainerMap<T>::MType T::template SingletonContainerMap<T>::MTX = T::template SingletonContainerMap<T>::MType();
 
#endif