#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    //w.show();

    Widget p;
    p.resize(200, 200);
    p.show();

    return a.exec();
}
