#include "sshoter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sshoter w;
    w.show();
    return a.exec();
}
