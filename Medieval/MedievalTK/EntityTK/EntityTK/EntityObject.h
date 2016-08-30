#ifndef ENTITYOBJECT_H
#define ENTITYOBJECT_H

#include <rapidjson/document.h>

#include "ui_EntityTK.h"
#include "component.h"

class EntityObject
{
private:
    std::string getFileNameFromPath(const std::string& path, const std::string& extension);

private:
    rapidjson::Document jsonDoc;
    std::string         name;
    std::vector<Component> components;

    float x, y, z;
    float sx, sy, sz;

    QGridLayout *form;

public:
    EntityObject(const std::string& pathToDoc, Ui::EntityTK* ui);
    ~EntityObject();
};

#endif // ENTITYOBJECT_H
