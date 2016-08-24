#ifndef ENTITYTK_H
#define ENTITYTK_H

#include <QtWidgets/QMainWindow>
#include "ui_entitytk.h"

class EntityTK : public QMainWindow
{
	Q_OBJECT

public:
	EntityTK(QWidget *parent = 0);
	~EntityTK();

private:
	Ui::EntityTKClass ui;
};

#endif // ENTITYTK_H
