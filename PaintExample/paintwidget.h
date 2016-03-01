#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);


    virtual void paintEvent(QPaintEvent *ev);
    virtual QSize sizeHint() const;
};

#endif // PAINTWIDGET_H
