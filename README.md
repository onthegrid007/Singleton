# Singleton
Singleton &amp; SingletonContainerMap

Singleton: Single static class instantiation

SingletonContainerMap: Multi-static mapped class instantiations

Both are acheived through the use of class polymorphism, template specalization (partial and full), and hierarchical inheritance.

# Usage
Basic Classes:

foo.h
```
#pragma once
class Foo {
    void printHW();
}
```

foo.cpp
```
#include "foo.h"
#include <iostream>
void Foo::printHW() { std::cout << "Hello World! - From Foo" << std::endl; }
```

bar.h
```
#pragma once
class Bar {
    void printHW();
}
```

bar.cpp
```
#include "bar.h"
#include <iostream>
void Bar::printHW() { std::cout << "Hello World! - From Bar" << std::endl; }
```

Singleton-Based Classes:

foo.h
```
#pragma once
#include "singleton.hpp"
class Foo : public Singleton<Foo> {
    public:
    void printHW();

    private:
    // ALL constructor of child classes must be expicitly declared private
    // There is also an explicit deletion of the template child's constructor, since it can not be deduced untill the full template specalization. It forces the user to manualy define one.
    Foo();
}
```

foo.cpp
```
#include "foo.h"
_SINGLETON_CHILD_DEFINITIONS(Foo)
#include <iostream>
void Foo::printHW() { std::cout << "Hello World! - From Foo" << std::endl; }
```

bar.h
```
#pragma once
#include "singleton_container_map.hpp"
class Bar : public SingletonContainerMap<Bar> {
    public:
    void printHW();

    private:
    // REQUIRED to be private
    _SCM_CHILD_DECLORATIONS(Bar)

    // ALL constructor of child classes must be expicitly declared private
    // There is also an explicit deletion of the template child's constructor, since it can not be deduced untill the full template specalization. It forces the user to manualy define one.
    Bar();
}
```

bar.cpp
```
#include "bar.h"
_SCM_CHILD_DEFINITIONS(Bar)
#include <iostream>
Bar::Bar() { }
void Bar::printHW() { std::cout << "Hello World! - From Bar" << std::endl; }
```

# Example
```
// Main just for example purposes, can be accessed anywhere within the proper defined scopes
#include "foo.h"
#include "bar.h"
int main() {
    // Ex. 1
    auto& foo = Foo::GetInstance();
    foo.printHW();
    // Ex. 2
    Foo::GetInstance().printHW();

    // Ex. 3
    Bar::CreateInstance("key1");
    auto& bar1 = Bar::GetInstanceByKey("key1");
    bar1.printHW();

    // Ex. 4 - You can even grab a new instance on the fly even if the key doesen't already exist, for single or multi-call instances
    Bar::GetInstanceByKey("key2").printHW().DeleteInstance();
    // Note not even needing to call CreateInstance as it is done by default if the key doesen't already exist in the map

    return 0;
}
```