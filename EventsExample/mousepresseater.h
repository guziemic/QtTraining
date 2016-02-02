#ifndef MOUSEPRESSEATER_H
#define MOUSEPRESSEATER_H

#include <QObject>
#include <QEvent>

class MousePressEater : public QObject {
    Q_OBJECT

public:
    MousePressEater(QObject * parent) : QObject(parent) {}
    ~MousePressEater() {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) {
        if (event && event->type() == QEvent::MouseButtonPress) {
            return true;
        }

        return QObject::eventFilter(obj, event);
    }
};

#endif // MOUSEPRESSEATER_H
