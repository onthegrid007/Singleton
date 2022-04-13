/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2022, Peter Ferranti
*   All rights reserved.
*   Other Contributers:
*/

#ifndef SINGLETON_CONTAINER_MAP_HPP_
#define SINGLETON_CONTAINER_MAP_HPP_
#include "noncopyable.h"
#include "nonmoveable.h"

#include <string>
#include <unordered_map>
#include <tuple>
#include <assert.h>

template<typename T, template<typename...> typename _MapType = std::unordered_map>
class SingletonContainerMap : public NonMovable, public NonCopyable {
    public:
    typedef _MapType<std::string, T*> ContainerType;
    static ContainerType ContainerMap;
    template<typename... Args>
    SingletonContainerMap<T>(Args ... args) = delete;

    private:
    std::string m_key;
    friend typename ContainerType::mapped_type;
    static bool exists(const std::string& key) { return (ContainerMap.find(key) != ContainerMap.end()); }

    public:
    SingletonContainerMap<T>() = default;
    template<typename... Args>
    static T& CreateNewInstance(const std::string key, Args ... args) {
        auto& rtn = *(ContainerMap[key] = std::move(new T(args...)));
        if(rtn.m_key != key) rtn.m_key = key;
        return rtn;
    }
    //template<typename... Args>
    //static T& CreateNewInstance(const std::string key, Args&& ... args) {
    //    return CreateNewInstance(key, args...);
    //}

    template<typename... Args>
    static T& GetInstanceByKey(const std::string key, Args ... args) {
        if(exists(key)) {
            return *(ContainerMap[key]);
        }
        assert(false);
    }
    //template<typename... Args>
    //static T& GetInstanceByKey(const std::string key, Args&& ... args) {
    //    return GetInstanceByKey(key, args...);
    //}

    static const void DeleteInstanceByKey(const std::string key) {
        if(exists(key)) {
            return ContainerMap.erase(key);
        }
    }
    static const std::string GetKeyByInstance(const T& instance) {
        return instance.m_key;
        // Old way
        /*
        for(auto& pair = ContainerMap.begin(); pair != ContainerMap.end(); pair++)
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

#define _SCM_CHILD_DECLORATIONS(T) friend class SingletonContainerMap;
#define _SCM_CHILD_DEFINITIONS(T) typedef T::SingletonContainerMap<T> SCM; template<> inline SCM::ContainerType SCM::ContainerMap = SCM::ContainerType();

#endif