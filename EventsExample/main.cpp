#include <QApplication>
#include "okienko.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    okienko o;
    o.show();

    return a.exec();
}
