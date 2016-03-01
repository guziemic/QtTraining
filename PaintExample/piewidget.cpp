#include "piewidget.h"

#include <QPainter>

PieWidget::PieWidget(QWidget *parent) : QWidget(parent)
{

}

QSize PieWidget::sizeHint() const {

}

QSize PieWidget::minimumSizeHint() const {

}

void PieWidget::addEntry(const QString& name, int val) {
    values.insert(name, val);
}

void PieWidget::paintEvent(QPaintEvent *ev) {
    //calculate total
    QHash<QString, int>::const_iterator it;
    int total = 0;
    for (it = values.begin(); it != values.end(); ++it) {
        total += it.value();
    }

    //prepera painter
    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing, true);

    //prepare colors
    QStringList colorNames = QColor::colorNames();
    int colorPos = 13; //pastel color

    int height = rect().height();
    QRect pieRect(0, 0, height, height);

    //dedicated right half to legend
    QRect legendRect = rect();
    legendRect.setLeft(pieRect.width());
    legendRect.adjust(10, 10, -10, -10);
}
