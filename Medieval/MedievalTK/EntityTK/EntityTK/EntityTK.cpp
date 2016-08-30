#include "EntityTK.h"
#include "ui_EntityTK.h"

#include <QMouseEvent>
#include <QFileDialog>
#include <iostream>
#include <stdio.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

QListWidgetItem *currentItem = NULL;

#include "EntityObject.h"

EntityTK::EntityTK(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EntityTK)
{
    ui->setupUi(this);

    ui->allComponents->addItem("RigidBodyComponent [1]");
    ui->allComponents->addItem("VoxelModelComponent [0]");

    connect(ui->actionLoad, &QAction::triggered, this, &EntityTK::loadJSON);
    connect(ui->actionLoad, &QAction::triggered, this, &EntityTK::newJSON);
    ui->pushButton->setEnabled(false);
}

EntityTK::~EntityTK()
{
    delete ui;
}

void EntityTK::newJSON()
{
}

void EntityTK::loadJSON()
{
    QString string = QFileDialog::getOpenFileName(this,
                                 tr("Open Entity (JSON) File."), "I:\\Medieval\\MedievalProject\\Medieval\\data", tr("JSON Files (*.json)"));
    EntityObject object(string.toStdString(), ui);
}

void EntityTK::on_allComponents_itemDoubleClicked(QListWidgetItem *item)
{
    ui->entityComponentList->addItem(item->text());
    if(currentItem == NULL)
        ui->pushButton->setEnabled(false);
}

void EntityTK::on_entityComponentList_itemClicked(QListWidgetItem *item)
{
       currentItem = item;
       ui->pushButton->setEnabled(true);
}

void EntityTK::on_pushButton_clicked()
{
    int row = ui->entityComponentList->row(currentItem);
    QListWidgetItem *item2 = ui->entityComponentList->takeItem(row);
    delete item2;
    item2 = NULL;
    currentItem = NULL;
    ui->pushButton->setEnabled(false);
}
