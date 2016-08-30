#ifndef ENTITYOBJECT_H
#define ENTITYOBJECT_H

#include <rapidjson/document.h>

#include "ui_EntityTK.h"
#include "component.h"

#include <gdtl/scoped_ptr.h>

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
    QHBoxLayout *positionLayout;
    QHBoxLayout *scaleLayout;
private:
public:
    EntityObject(const std::string& pathToDoc, Ui::EntityTK* ui);
};

#endif // ENTITYOBJECT_H
