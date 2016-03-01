#include <QApplication>
#include <QtGui>
#include <QLabel>

#include "paintwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QPixmap pm(100,100);

    QRect rect(0, 0, 100, 100);
    QPixmap pm(rect.size());
    pm.fill();

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::blue, 2);
    p.setPen(pen);

    QBrush brush(Qt::green);
    p.setBrush(brush);


    QRect r = rect.adjusted(10, 10, -10, -10);
    p.drawEllipse(r);
//    p.drawEllipse(10, 10, 50, 80);

    QLabel label;
    label.setPixmap(pm);
//    label.show();

    PaintWidget paintWdg;
    paintWdg.show();

    return a.exec();
}
