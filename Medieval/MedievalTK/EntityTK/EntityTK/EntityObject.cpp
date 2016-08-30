#include "EntityObject.h"

#include <rapidjson/filereadstream.h>
#include <QString>
#include <QStringList>
#include <iostream>
#include <QLayout>
#include <QLineEdit>

EntityObject::EntityObject(const std::string& pathToDoc, Ui::EntityTK* ui)
{
    form = ui->formLayout;

    FILE* file = fopen(pathToDoc.c_str(), "rb");
    char buffer[65536];
    rapidjson::FileReadStream readStream(file, buffer, sizeof(buffer));
    jsonDoc.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(readStream);
    fclose(file);

    std::string name = getFileNameFromPath(pathToDoc, "json");
    rapidjson::Value& components = jsonDoc[name.c_str()]["components"];
    for (rapidjson::Value::ConstMemberIterator component_it = components.MemberBegin(); component_it != components.MemberEnd(); ++component_it)
    {
        // Map this
        if(component_it->name == "0")
            ui->entityComponentList->addItem("VoxelModelComponent");
        if(component_it->name == "1")
            ui->entityComponentList->addItem("RigidBodyComponent");
    }

   QLabel *positionLabel = new QLabel("Position");
   form->addWidget(positionLabel);

   QLineEdit *x = new QLineEdit("");
   QLineEdit *y = new QLineEdit("");
   QLineEdit *z = new QLineEdit("");
   form->addWidget(x);
   form->addWidget(y);
   form->addWidget(z);

}

std::string EntityObject::getFileNameFromPath(const std::string& path, const std::string& extension)
{
    QString tmp = path.c_str();
    QStringList list = tmp.split("/");
    QString nameWithExtension = list[list.size() - 1];
    QStringList nameWithoutExtension = nameWithExtension.split(".");
    return nameWithoutExtension[0].toStdString();
}

EntityObject::~EntityObject()
{
}


