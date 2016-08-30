#ifndef COMPONENT_H
#define COMPONENT_H

#include <map>
#include <gdtl/scoped_ptr.h>

class Component
{
private:
    const char *name;
    int id;

public:
    Component(const char *string)
        : name(string)
    {
        id = Component::typeMap.at(string);
    }

    static void registerComponentType(const char *type, int id)
    {
        Component::typeMap[type] = id;
    }

    static std::map<const char*, int> typeMap;
};

#endif // COMPONENT_H
