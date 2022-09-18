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

template<typename T, template<typename...> typename MT = std::unordered_map>
class SingletonContainerMap : public NonMovable, public NonCopyable {
    public:
    typedef T Type;
    typedef std::unique_ptr<T> TPType;
    typedef std::shared_mutex MType;
    typedef std::unique_ptr<MType> MPType;
    typedef MT<std::string, TPType> CType;
    typedef std::lock_guard<MType> LType;
    
    protected:
    static CType CMap;
    static MPType MTX;
    // template<typename _T>
    static MType& GetStaticMTX(MPType& mtx) {
        if(!mtx.get()) mtx.reset(new MType());
        // std::cout << (mtx.get()) << std::endl;
        return *(mtx.get());
    }
    std::string m_key;
    SingletonContainerMap() {}
    // template<auto Args>
    // static constexpr void s_print() {
    //     #if !defined(__GNUC__) || defined(__clang__)
    //         #error "break"
    //         int static_print_is_implemented_only_for_gcc = 0;
    //     #else
    //         int unused = 0;
    //     #endif
    // };
    
    public:
    static bool Exists(const std::string& key, bool blocking = true) {
        static CType CMap;
        static MPType MTX;
        if(blocking) {
            LType lock(GetStaticMTX(MTX));
            return ((CMap.find(key) != CMap.end()) && !CMap.empty());
        } else {
            return ((CMap.find(key) != CMap.end()) && !CMap.empty());
        }
    }
    
    template<typename... Args>
    static T& CreateNewInstance(const std::string key, Args ... args) {
        static CType CMap;
        static MPType MTX;
        LType lock(GetStaticMTX(MTX));
        constexpr bool CNIHasArgs = (bool)((size_t)(sizeof...(args)));
        // s_print<tmp>();
        T& rtn = Exists(key, false) ? *(CMap[key].get()) : *(CMap[key] = std::move(TPType(new T(args...))));
        if(rtn.m_key != key) rtn.m_key = key;
        return rtn;
    }
    
    template<typename... Args>
    static T& GetInstanceByKey(const std::string key, Args ... args) {
        static CType CMap;
        static MPType MTX;
        if(Exists(key)) { 
            LType lock(GetStaticMTX(MTX));
            return *(CMap[key].get());
        }
        // LType lock(GetStaticMTX(MTX));
        return CreateNewInstance(key, std::move(args)...);
    }

    static const void DeleteInstanceByKey(const std::string key) {
        static CType ContainerMap;
        static MPType MTX;
        if(Exists(key)) {
            LType lock(GetStaticMTX(MTX));
            CMap.erase(key);
        }
    }
    
    static const std::string GetKeyByInstance(const T& instance) {
        return instance.m_key;
        // Old way
        /*
        for(auto& pair = CMap.begin(); pair != CMap.end(); pair++)
            if(pair[1] == &instance) { return pair[0]; }
        */
    }

    static const void DeleteInstanceByInstance(const T& instance) {
        return DeleteInstanceByKey(GetKeyByInstance(instance));
    }

    const void DeleteInstance() {
        DeleteInstanceByInstance(&this);
    }

    // Sadly no one-line means of deducing get/del Type, must think of another way...
    /*
    static rtnType& Get(const getType _getObj, Args&& ... args) {
        return (typeid(std::basic_string<char>) == typeid(getType)) ? GetInstanceByKey(_getObj, args...) : GetKeyByInstance(_getObj);
        // return (typeid(T) == typeid(getType)) ? GetKeyByInstance(_getObj) : GetInstanceByKey(_getObj);
        // Also works, but std::basic_string<char> I trust more than a User Def'ed T...
    }

    template<typename delType>
    static const void Del(delType _delObj) {
        return (typeid(std::basic_string<char>) == typeid(delType)) ? DeleteInstanceByKey(_delObj) : DeleteInstanceByInstance(_delObj);
    }
    */
};

#define _SCM_CHILD_DECLORATIONS(T) friend class SingletonContainerMap; friend class SingletonContainerMap<T>; friend typename SingletonContainerMap<T>::MPType; friend typename std::default_delete<T>; //friend class std::__uniq_ptr_data<T, std::default_delete<T>>; friend typename std::__uniq_ptr_impl<T, std::default_delete<T>>::pointer; friend class std::remove_reference<T>;
#define _SCM_CHILD_DEFINITIONS(T) typedef T::SingletonContainerMap<T> SCM; template<> inline SCM::CType SCM::CMap = SCM::CType(); template<> inline SCM::MPType SCM::MTX = std::make_unique<SCM::MType>();
// #define _SCM_CHILD_DESTRUCTOR for(auto pair = SCM::CMap.begin(); pair != SCM::CMap.end(); pair++) DeleteInstanceByKey(((*pair).first));
 
#endif