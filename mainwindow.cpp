#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "app_info.h"
#include "editor.h"
#include "settings.h"
#include <filesystem>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include <iostream>

using namespace std;
namespace fs = std::filesystem;

// A little func to get extension
string get_extension(string filename) {
    return filename.substr(filename.find_last_of(".") +1);
}

// Main window constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    renew_settings();

    // Setting default path to entries and warning user
    if (path_to_entries == "") {
        set_path_to_entries("/");
        QMessageBox::warning(this, APP_TITLE, "You have no path to entries specified.\nCheck your settings.");
    }

    // Renew entries list for the first time
    renew_entries();
}

void MainWindow::set_path_to_entries(string path) {
    path_to_entries = path;
}

// Function to renew entries
void MainWindow::renew_entries() {
    // Clearing list of menuentries and QList
    ui->entriesList->clear();
    entries_list.clear();

    // Refilling them
    for (const auto &entry : fs::directory_iterator(path_to_entries))
        if (fs::is_regular_file(entry.path()) and get_extension(entry.path()) == "conf") {
                menuentry new_entry(entry.path());
                entries_list.push_back(new_entry);
                ui->entriesList->addItem(new_entry.get_pretty_name().c_str());
            }
}

void MainWindow::renew_settings() {
    // Getting home dir path
    string path_to_data = string(getpwuid(getuid())->pw_dir) + "/beeforgrub2";
    while (true) {
        // Checking if data dir exists and is a dir
        if (fs::exists(path_to_data) and fs::is_directory(path_to_data)) {

            // Checking if config exists and is a file
            if (fs::exists(path_to_data + "/beeforgrub2.conf") and fs::is_regular_file(path_to_data + "/beeforgrub2.conf")) {

                // Reading config file
                ifstream fin;
                fin.open(path_to_data + "/beeforgrub2.conf");
                for (string line; getline(fin, line); ) {
                    string key = line.substr(0, line.find(" "));
                    string value = line.substr(line.find(" ")+1);
                    if (key == "path_to_entries")
                        set_path_to_entries(value);
                }
                fin.close();

                // break infinite cycle when settings are fine
                break;
            }
            else {
                // Create default config file if not found
                ofstream fout;
                fout.open(path_to_data + "/beeforgrub2.conf");
                fout << "path_to_entries /" << endl;
                fout.close();
            }
        } else {
            fs::create_directory(path_to_data);
        }
    }
}

void MainWindow::renew_all() {
    renew_settings();
    renew_entries();
}

// Open editor for item from QList
void MainWindow::open_item(QListWidgetItem * item) {
    // New instance of editor
    Editor editor(this, get_menuentry_by_pretty_name(item->text().toUtf8().constData()));
    editor.setModal(true);

    // Connect editor's entry_updated signal with renew_entries slot
    QObject::connect(&editor, &Editor::entry_updated, this, &MainWindow::renew_entries);
    editor.exec();
}

// Handling menu triggers !!! NEEDS REWORK
void MainWindow::handle_menu_trigger(QAction * item) {
    // View - Renew handler
    if (item == ui->actionRenew)
        renew_entries();

    // About - About handler
    else if (item == ui->actionAbout_beeforgrub2) {
        char help_text[1024];
        sprintf(help_text, "%s\nVersion: %s\nLicense: %s\nAuthor: %s\n\n%s", APP_TITLE, APP_VERSION, APP_LICENSE, APP_AUTHOR, ABOUT_TEXT);
        QMessageBox::about(this, APP_TITLE, help_text);
    }

    // File - New handler
    else if (item == ui->actionNew) {
        Editor editor(this, menuentry(), true);
        editor.setModal(true);
        QObject::connect(&editor, &Editor::entry_updated, this, &MainWindow::renew_entries);
        editor.exec();
    }

    // File - Open handler
    else if (item == ui->actionOpen) {
        string path = QFileDialog::getOpenFileName().toUtf8().constData();
        if (path != "") {
            Editor editor(this, menuentry(path));
            editor.setModal(true);
            QObject::connect(&editor, &Editor::entry_updated, this, &MainWindow::renew_entries);
            editor.exec();
        }
    }

    // File - Settings handler
    else if (item == ui->actionSettings) {
        Settings settings(this, path_to_entries);
        settings.setModal(true);
        QObject::connect(&settings, &Settings::settings_updated, this, &MainWindow::renew_all);
        settings.exec();
    }
}

// Get menuentry from pretty name (may be reworked in future)
menuentry MainWindow::get_menuentry_by_pretty_name(string pretty_name) {
    for (menuentry entry : entries_list)
        if (entry.get_pretty_name() == pretty_name)
            return entry;
    return menuentry();
}

// Main window deconstructor
MainWindow::~MainWindow()
{
    delete ui;
}

