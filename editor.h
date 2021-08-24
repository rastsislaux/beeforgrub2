#ifndef EDITOR_H
#define EDITOR_H

#include <QDialog>
#include <menuentry.h>
#include <mainwindow.h>

namespace Ui {
class Editor;
}

class Editor : public QDialog
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = nullptr, menuentry entry = menuentry(), bool isNew = false);
    menuentry entry;
    ~Editor();

private slots:
    void save();
    void saveas();
    void delete_entry();

signals:
    void entry_updated();

private:
    void update();
    Ui::Editor *ui;
};

#endif // EDITOR_H
