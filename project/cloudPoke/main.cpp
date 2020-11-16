#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("服务器端 ip:127.0.0.1 Port:8888");
    w.show();
    return a.exec();
}
