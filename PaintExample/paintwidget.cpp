#include "paintwidget.h"

#include <QPainter>
#include <QBrush>
#include <QPen>

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget(parent)
{

}

void PaintWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);

    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(Qt::blue, 2);
    paint.setPen(pen);
    QBrush brush(Qt::cyan);
    brush.setStyle(Qt::Dense4Pattern);
    paint.setBrush(brush);

    QRect ri = rect().adjusted(10, 10, -10, -10);
    paint.drawEllipse(ri);

    brush.setColor(Qt::red);
    paint.setBrush(brush);
    ri = rect().adjusted(30, 30, -30, -30);
    paint.drawEllipse(ri);

}

QSize PaintWidget::sizeHint() const {
    return QSize(200, 200);
}
