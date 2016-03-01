#ifndef WATEK_H
#define WATEK_H

#include <QThread>

class QLabel;
class QEvent;

class Watek : public QThread
{
    Q_OBJECT

public:
    Watek();
    ~Watek();

protected:
    void event(QEvent *event);
    void run();

private:

};

#endif // WATEK_H
