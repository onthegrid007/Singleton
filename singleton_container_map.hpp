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
    // SingletonContainerMap() = delete;
    // ~SingletonContainerMap() = delete;
    typedef T Type;
    typedef std::shared_timed_mutex MType;
    typedef MT<std::string, void*> CType;
    typedef std::lock_guard<MType> LType;
    typedef SingletonContainerMap<T> SCMType;
    
    protected:
    // SingletonContainerMap() = 0;
    AINLINE static CType CMap;
    AINLINE static MType MTX;
    std::string m_key;
    
    public:
    template<bool blocking = true>
    static bool Exists(const std::string key) {
        if constexpr(blocking) {
            LType lock(T::SCMType::MTX);
            return ((T::SCMType::CMap.find(key) != T::SCMType::CMap.end()) && !T::SCMType::CMap.empty());
        } else {
            return ((T::SCMType::CMap.find(key) != T::SCMType::CMap.end()) && !T::SCMType::CMap.empty());
        }
    }
    
    template<typename... Args>
    static T& CreateNewInstance(const std::string key, Args ... args) {
        LType lock(T::SCMType::MTX);
        T& rtn{*(!Exists<false>(key) ? (static_cast<T*>(T::SCMType::CMap[key] = std::move(new T(args...)))) : static_cast<T*>(T::SCMType::CMap[key]))};
        if(rtn.m_key != key) rtn.m_key = key;
        return rtn;
    }
    
    template<typename... Args>
    static T& GetInstanceByKey(const std::string key, Args ... args) {
        if(Exists(key)) {
            LType lock(T::SCMType::MTX);
            return (*(static_cast<T*>(T::SCMType::CMap[key])));
        }
        return CreateNewInstance(key, std::move(args)...);
    }
    
    static const void DeleteInstanceByKey(const std::string key) {
        if(Exists(key)) {
            LType lock(T::SCMType::MTX);
            T* destroy{static_cast<T*>(T::SCMType::CMap[key])};
            T::SCMType::CMap.erase(key);
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
#define _SCM_CHILD_DEFINITIONS(T) template<> ABI_INLINE T::template SingletonContainerMap<T>::CType T::template SingletonContainerMap<T>::CMap{}; template<> ABI_INLINE T::template SingletonContainerMap<T>::MType T::template SingletonContainerMap<T>::MTX{};
#endif