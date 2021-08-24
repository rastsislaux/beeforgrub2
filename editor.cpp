#include "editor.h"
#include "ui_editor.h"
#include "app_info.h"
#include <filesystem>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

// Editor constructor
Editor::Editor(QWidget *parent, menuentry given_entry, bool isNew) :
    QDialog(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    // Setting up a window
    entry = given_entry;
    char buff[1024];
    sprintf(buff, "<html><head/><body><p align=\"center\"><span style=\"font-weight:600;\">%s<br/>%s</span></p></body></html>", entry.get_title().c_str(), entry.path.c_str());

    // Filling textedits (looks kinda stupid so I'll rework that later, I think - 24.08.2021)
    ui->entry_label->setText(buff);
    ui->title_edit->setText(entry.get_parameter("title").c_str());
    ui->version_edit->setText(entry.get_parameter("version").c_str());
    ui->architecture_edit->setText(entry.get_parameter("architecture").c_str());
    ui->devicetree_edit->setText(entry.get_parameter("devicetree").c_str());
    ui->devicetreeoverlay_edit->setText(entry.get_parameter("devicetree-overlay").c_str());
    ui->efi_edit->setText(entry.get_parameter("efi").c_str());
    ui->options_edit->setText(entry.get_parameter("options").c_str());
    ui->initrd_edit->setText(entry.get_parameter("initrd").c_str());
    ui->machineid_edit->setText(entry.get_parameter("machine-id").c_str());
    ui->grub_users_edit->setText(entry.get_parameter("grub_users").c_str());
    ui->grub_arg_edit->setText(entry.get_parameter("grub_arg").c_str());
    ui->grub_class_edit->setText(entry.get_parameter("grub_class").c_str());
    ui->linux_edit->setText(entry.get_parameter("linux").c_str());

    // Disable save and delete buttons if creating a new entry
    if (isNew) {
        ui->delete_button->setDisabled(true);
        ui->save_button->setDisabled(true);
    }
}

// Updating entry with parameters fron textedits
void Editor::update() {
    entry.params["title"] = ui->title_edit->text().toUtf8().constData();
    entry.params["version"] = ui->version_edit->text().toUtf8().constData();
    entry.params["architecture"] = ui->architecture_edit->text().toUtf8().constData();
    entry.params["devicetree"] = ui->devicetree_edit->text().toUtf8().constData();
    entry.params["devicetree-overlay"] = ui->devicetreeoverlay_edit->text().toUtf8().constData();
    entry.params["efi"] = ui->efi_edit->text().toUtf8().constData();
    entry.params["options"] = ui->options_edit->text().toUtf8().constData();
    entry.params["initrd"] = ui->initrd_edit->text().toUtf8().constData();
    entry.params["machine-id"] = ui->machineid_edit->text().toUtf8().constData();
    entry.params["grub_users"] = ui->grub_users_edit->text().toUtf8().constData();
    entry.params["grub_arg"] = ui->grub_arg_edit->text().toUtf8().constData();
    entry.params["grub_class"] = ui->grub_class_edit->text().toUtf8().constData();
    entry.params["linux"] = ui->linux_edit->text().toUtf8().constData();
}

// Handle save button
void Editor::save() {
    update();
    QString no_boot_text = "This entry has empty linux and efi fields therefore it will be invalid.\nDo you really want to save it?";

    // There should be linux or efi otherwise user should be warned
    if ((entry.params["linux"] != "" or entry.params["efi"] != "") or QMessageBox::warning(this, APP_TITLE, no_boot_text, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        entry.export_to_file(entry.path);
        emit entry_updated();
        close();
    }
}

// Handle saveas button
void Editor::saveas() {
    update();
    QString no_boot_text = "This entry has empty linux and efi fields therefore it will be invalid.\nDo you really want to save it?";

    // There should be linux or efi otherwise user should be warned
    if ((entry.params["linux"] != "" or entry.params["efi"] != "") or QMessageBox::warning(this, APP_TITLE, no_boot_text, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        string path = QFileDialog::getSaveFileName().toUtf8().constData();
        if (path != "") {
            entry.export_to_file(path);
            emit entry_updated();
            close();
        }
    }
}


// Handle delete button
void Editor::delete_entry() {
    update();
    QMessageBox::StandardButton reply;

    // Is user sure about what he's doing?
    reply = QMessageBox::warning(this, APP_TITLE, "Are you sure you want to delete this entry?\nThis action is irreversible.", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        std::filesystem::remove(entry.path);
        emit entry_updated();
        close();
    }
}

// Editor deconstructor
Editor::~Editor()
{
    delete ui;
}
