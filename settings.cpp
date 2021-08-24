#include "settings.h"
#include "ui_settings.h"
#include <app_info.h>
#include <unistd.h>
#include <pwd.h>
#include <fstream>

#include <QFileDialog>

// Settings constructor
Settings::Settings(QWidget *parent, string path_to_entries) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    // set the button text to path to entries
    ui->path_to_entries_button->setText(path_to_entries.c_str());
}


// Settings button text to path from user
void Settings::handle_path_click() {
    QString path = ui->path_to_entries_button->text();
    QString new_path = QFileDialog::getExistingDirectory(this, APP_TITLE, path);
    if (new_path != "")
        ui->path_to_entries_button->setText(new_path);
}

void Settings::handle_apply_click() {
    string path_to_data = string(getpwuid(getuid())->pw_dir) + "/beeforgrub2/beeforgrub2.conf";
    ofstream fout;
    fout.open(path_to_data);
    fout << "path_to_entries " << ui->path_to_entries_button->text().toUtf8().constData() << endl;
    fout.close();
    emit settings_updated();
    close();
}

Settings::~Settings()
{
    delete ui;
}
