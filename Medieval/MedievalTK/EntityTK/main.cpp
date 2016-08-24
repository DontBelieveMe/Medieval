#include "EntityTK.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EntityTK w;
	w.show();
	return a.exec();
}
