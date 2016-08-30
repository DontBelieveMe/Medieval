/********************************************************************************
** Form generated from reading UI file 'EntityTK.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTITYTK_H
#define UI_ENTITYTK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EntityTK
{
public:
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionLoad;
    QAction *actionClose;
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *actionNew;
    QWidget *centralWidget;
    QListWidget *allComponents;
    QListWidget *entityComponentList;
    QLabel *label;
    QLabel *label_2;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EntityTK)
    {
        if (EntityTK->objectName().isEmpty())
            EntityTK->setObjectName(QStringLiteral("EntityTK"));
        EntityTK->resize(671, 562);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EntityTK->sizePolicy().hasHeightForWidth());
        EntityTK->setSizePolicy(sizePolicy);
        EntityTK->setMinimumSize(QSize(671, 562));
        EntityTK->setMaximumSize(QSize(671, 562));
        actionSave = new QAction(EntityTK);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave_As = new QAction(EntityTK);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionLoad = new QAction(EntityTK);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionClose = new QAction(EntityTK);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionHelp = new QAction(EntityTK);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionAbout = new QAction(EntityTK);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionNew = new QAction(EntityTK);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        centralWidget = new QWidget(EntityTK);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        allComponents = new QListWidget(centralWidget);
        allComponents->setObjectName(QStringLiteral("allComponents"));
        allComponents->setGeometry(QRect(465, 30, 191, 471));
        entityComponentList = new QListWidget(centralWidget);
        entityComponentList->setObjectName(QStringLiteral("entityComponentList"));
        entityComponentList->setGeometry(QRect(10, 30, 191, 471));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(530, 10, 81, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 10, 141, 16));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(210, 30, 251, 481));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(gridLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);

        EntityTK->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EntityTK);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 671, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        EntityTK->setMenuBar(menuBar);
        statusBar = new QStatusBar(EntityTK);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EntityTK->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionSave);
        menuFile->addAction(actionNew);
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionLoad);
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuAbout->addAction(actionHelp);
        menuAbout->addAction(actionAbout);

        retranslateUi(EntityTK);

        QMetaObject::connectSlotsByName(EntityTK);
    } // setupUi

    void retranslateUi(QMainWindow *EntityTK)
    {
        EntityTK->setWindowTitle(QApplication::translate("EntityTK", "EntityTK", 0));
        actionSave->setText(QApplication::translate("EntityTK", "Save", 0));
        actionSave_As->setText(QApplication::translate("EntityTK", "Save As", 0));
        actionLoad->setText(QApplication::translate("EntityTK", "Load", 0));
        actionClose->setText(QApplication::translate("EntityTK", "Close", 0));
        actionHelp->setText(QApplication::translate("EntityTK", "Help", 0));
        actionAbout->setText(QApplication::translate("EntityTK", "About", 0));
        actionNew->setText(QApplication::translate("EntityTK", "New", 0));
        label->setText(QApplication::translate("EntityTK", "All Components", 0));
        label_2->setText(QApplication::translate("EntityTK", "This entitys components", 0));
        pushButton->setText(QApplication::translate("EntityTK", "Remove Component", 0));
        menuFile->setTitle(QApplication::translate("EntityTK", "File", 0));
        menuAbout->setTitle(QApplication::translate("EntityTK", "About", 0));
    } // retranslateUi

};

namespace Ui {
    class EntityTK: public Ui_EntityTK {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTITYTK_H
