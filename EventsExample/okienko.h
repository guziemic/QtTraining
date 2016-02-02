#ifndef OKIENKO_H
#define OKIENKO_H

#include <QWidget>

class QPushButton;
class MousePressEater;

class okienko : public QWidget
{
    Q_OBJECT
public:
    explicit okienko(QWidget *parent = 0);

protected:
    bool event(QEvent *);
    void paintEvent(QPaintEvent * event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:

public slots:
    void sendPostedEvent();
    void sendEventNormal();
    void doInstallEventFilter();
    void doRemoveEventFilter();

private:
    QPushButton * m_pushButton1;
    QPushButton * m_pushButton2;
    MousePressEater * m_eater;
};

#endif // OKIENKO_H
