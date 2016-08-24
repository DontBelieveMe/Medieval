/********************************************************************************
** Form generated from reading UI file 'entitytk.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTITYTK_H
#define UI_ENTITYTK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EntityTKClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EntityTKClass)
    {
        if (EntityTKClass->objectName().isEmpty())
            EntityTKClass->setObjectName(QStringLiteral("EntityTKClass"));
        EntityTKClass->resize(600, 400);
        menuBar = new QMenuBar(EntityTKClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        EntityTKClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EntityTKClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        EntityTKClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(EntityTKClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        EntityTKClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(EntityTKClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EntityTKClass->setStatusBar(statusBar);

        retranslateUi(EntityTKClass);

        QMetaObject::connectSlotsByName(EntityTKClass);
    } // setupUi

    void retranslateUi(QMainWindow *EntityTKClass)
    {
        EntityTKClass->setWindowTitle(QApplication::translate("EntityTKClass", "EntityTK", 0));
    } // retranslateUi

};

namespace Ui {
    class EntityTKClass: public Ui_EntityTKClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTITYTK_H
