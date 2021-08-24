#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <string>

using namespace std;

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr, string path_to_entries = "");
    ~Settings();

private slots:
    void handle_path_click();
    void handle_apply_click();

signals:
    void settings_updated();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
