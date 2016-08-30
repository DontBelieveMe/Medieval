#include "EntityObject.h"

#include <rapidjson/filereadstream.h>

#include <QString>
#include <QStringList>
#include <iostream>
#include <QLayout>
#include <QLineEdit>

EntityObject::EntityObject(const std::string& pathToDoc, Ui::EntityTK* ui)
{
    form = ui->gridLayout;

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
    rapidjson::Value& position = jsonDoc[name.c_str()]["position"];
    rapidjson::Value& scale = jsonDoc[name.c_str()]["scale"];

    QLabel *positionLabel = new QLabel("Position");
    QLabel *scaleLabel = new QLabel("Scale");
    QLineEdit *x = new QLineEdit(QString::number(position[0].GetFloat()));
    QLineEdit *y = new QLineEdit(QString::number(position[1].GetFloat()));
    QLineEdit *z = new QLineEdit(QString::number(position[2].GetFloat()));
    QLineEdit *sx = new QLineEdit(QString::number(scale[0].GetFloat()));
    QLineEdit *sy = new QLineEdit(QString::number(scale[1].GetFloat()));
    QLineEdit *sz = new QLineEdit(QString::number(scale[2].GetFloat()));

    positionLayout = new QHBoxLayout;
    scaleLayout = new QHBoxLayout;
    positionLayout->addWidget(positionLabel);
    positionLayout->addWidget(x);
    positionLayout->addWidget(y);
    positionLayout->addWidget(z);
    scaleLayout->addWidget(scaleLabel);
    scaleLayout->addWidget(sx);
    scaleLayout->addWidget(sy);
    scaleLayout->addWidget(sz);

    form->addLayout(positionLayout,form->rowCount(), 0);
    form->addLayout(scaleLayout, form->rowCount(), 0);
}

std::string EntityObject::getFileNameFromPath(const std::string& path, const std::string& extension)
{
    QString tmp = path.c_str();
    QStringList list = tmp.split("/");
    QString nameWithExtension = list[list.size() - 1];
    QStringList nameWithoutExtension = nameWithExtension.split(".");
    return nameWithoutExtension[0].toStdString();
}


