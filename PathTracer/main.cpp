#include "mainwindow.h"

#include <QApplication>

/**
 * @brief Main
 *
 * The main function creates the  Qt application and the MainWindow of the software.
 *
 * @author Bianca Fragoso
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
