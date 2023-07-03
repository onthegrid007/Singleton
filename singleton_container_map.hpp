/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2023, Peter Ferranti
*   All rights reserved.
*/

#ifndef SINGLETON_CONTAINER_MAP_HPP_
#define SINGLETON_CONTAINER_MAP_HPP_
#include "noncopyable.h"
#include "nonmoveable.h"
#include "inline_abi_macros.h"
#include <shared_mutex>
#include <unordered_map>
#include <iostream>
#include <string>

template<typename T, template<typename...> typename MT = std::unordered_map>
class SingletonContainerMap : public NonMovable, public NonCopyable {
    public:
    typedef T Type;
    typedef std::shared_timed_mutex MType;
    typedef MT<std::string, T*> CType;
    typedef std::lock_guard<MType> LType;
    typedef SingletonContainerMap<T> SCMType;
    
    protected:
    inline static CType CMap;
    inline static MType MTX;
    std::string m_key;
    SingletonContainerMap() {}
    
    public:
    static bool Exists(const std::string key, bool blocking = true) {
        if(blocking) {
            LType lock(SCMType::MTX);
            return ((SCMType::CMap.find(key) != SCMType::CMap.end()) && !SCMType::CMap.empty());
        } else {
            return ((SCMType::CMap.find(key) != SCMType::CMap.end()) && !SCMType::CMap.empty());
        }
    }
    
    template<typename... Args>
    static T& CreateNewInstance(const std::string key, Args ... args) {
        LType lock(SCMType::MTX);
        if(!Exists(key, false)) {
            (SCMType::CMap[key] = std::move(new T(args...)))->m_key = key;
        }
        T& rtn = *(SCMType::CMap[key]);
        // if(rtn.m_key != key) rtn.m_key = key;
        return rtn;
    }
    
    template<typename... Args>
    static T& GetInstanceByKey(const std::string key, Args ... args) {
        if(Exists(key)) {
            LType lock(SCMType::MTX);
            return (*(SCMType::CMap[key]));
        }
        return CreateNewInstance(key, std::move(args)...);
    }
    
    static const void DeleteInstanceByKey(const std::string key) {
        if(Exists(key)) {
            LType lock(SCMType::MTX);
            T* destroy = (T*)SCMType::CMap[key];
            SCMType::CMap.erase(key);
            destroy->~T();
        }
    }
    
    static const std::string GetKeyByInstance(const T& instance) {
        return instance.m_key;
    }
    
    static const void DeleteInstanceByInstance(const T& instance) {
        return DeleteInstanceByKey(GetKeyByInstance(instance));
    }
    
    const void DeleteInstance() {
        DeleteInstanceByInstance(this);
    }
};
#define _SCM_CHILD_DECLORATIONS(T) friend class SingletonContainerMap; friend class SingletonContainerMap<T>;
#define _SCM_CHILD_DEFINITIONS(T) template<> ALWAYS_INLINE T::template SingletonContainerMap<T>::CType T::template SingletonContainerMap<T>::CMap = T::template SingletonContainerMap<T>::CType(); template<> ALWAYS_INLINE T::template SingletonContainerMap<T>::MType T::template SingletonContainerMap<T>::MTX = T::template SingletonContainerMap<T>::MType();
 
#endif