#ifndef ENTITYTK_H
#define ENTITYTK_H

#include <QMainWindow>
#include <QListWidgetItem>


namespace Ui {
class EntityTK;
}

class EntityTK : public QMainWindow
{
    Q_OBJECT

public:
    explicit EntityTK(QWidget *parent = 0);
    ~EntityTK();

private slots:
    void on_allComponents_itemDoubleClicked(QListWidgetItem *item);
    void on_entityComponentList_itemClicked(QListWidgetItem *item);
    void on_pushButton_clicked();

    void loadJSON();
    void newJSON();
private:
    Ui::EntityTK *ui;
};

#endif // ENTITYTK_H
