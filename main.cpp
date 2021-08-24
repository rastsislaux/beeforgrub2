#include "mainwindow.h"
#include <unistd.h>
#include <stdio.h>
#include <filesystem>

#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    // Getting root privelege if needed
    if (getuid()) {
        string sudo[3] = { "kdesu", "gksu", "sudo" };
        for (string su : sudo) {
            if (filesystem::exists("/usr/bin/" + su)) {
                char buff[256];
                sprintf(buff, "%s %s", su.c_str(), argv[0]);
                system(buff);
                return 0;
            }
        }
    }

    // Creating main window
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
