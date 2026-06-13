#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/icon.svg"));
    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
