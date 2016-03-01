#include "watek.h"

#include <QEvent>
#include <QDebug>

Watek::Watek() {

}

Watek::~Watek() {
}


void Watek::event(QEvent *event) {
    qDebug() << "Event" << event->type() << "in thread" << QThread::currentThreadId();
}

void Watek::run() {
    qDebug() << "Thread started";

    exec();
}
