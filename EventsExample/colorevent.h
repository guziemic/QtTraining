#ifndef COLOREVENT_H
#define COLOREVENT_H

#include <QEvent>
#include <QColor>

class ColorEvent : public QEvent {
public:
    ColorEvent(QColor color)
     : QEvent(static_cast<QEvent::Type>(QEvent::User + 12))
     ,  m_color(color) {}

    ~ColorEvent() {
    }

    QColor getColor() {
        return m_color;
    }

private:
    QColor m_color;
};

#endif // COLOREVENT_H
