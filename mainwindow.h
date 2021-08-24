#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <vector>
#include <menuentry.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    menuentry get_menuentry_by_pretty_name(string pretty_name);

public slots:
    void handle_menu_trigger(QAction * action);
    void renew_entries();
    void renew_all();
    void open_item(QListWidgetItem * item);

private:
    Ui::MainWindow *ui;
    vector <menuentry> entries_list;
    string path_to_entries;
    void set_path_to_entries(string path);
    void renew_settings();
};
#endif // MAINWINDOW_H
