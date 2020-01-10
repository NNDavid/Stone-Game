#include "kitolaswidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KitolasWidget w;
    w.show();

    return a.exec();
}
