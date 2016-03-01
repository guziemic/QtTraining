#include "okienko.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QCoreApplication>
#include <QPalette>
#include <QThread>
#include <QPaintEvent>
#include <QMouseEvent>

#include "colorevent.h"
#include "mousepresseater.h"

okienko::okienko(QWidget *parent)
    : QWidget(parent)
{
    m_pushButton1 = new QPushButton(tr("Send Post - green"), this);
    m_pushButton2 = new QPushButton(tr("Send Event - red"), this);
    QPushButton *buttonInstall = new QPushButton(tr("Instal Events Filter"), this);
    QPushButton *buttonRemove = new QPushButton(tr("Remove Events Filter"), this);
    QPushButton *button2 = new QPushButton(tr("Quit"), this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_pushButton1);
    layout->addWidget(m_pushButton2);
    layout->addWidget(buttonInstall);
    layout->addWidget(buttonRemove);
    layout->addWidget(button2);
    setLayout(layout);

    setMinimumWidth(400);
    setMinimumHeight(300);

    connect(m_pushButton1, &QPushButton::clicked, this, &okienko::sendPostedEvent);
    connect(m_pushButton2, &QPushButton::clicked, this, &okienko::sendEventNormal);
    connect(button2, &QPushButton::clicked, qApp, &QCoreApplication::quit);
    connect(buttonInstall, &QPushButton::clicked, this, &okienko::doInstallEventFilter);
    connect(buttonRemove, &QPushButton::clicked, this, &okienko::doRemoveEventFilter);
}

void okienko::doInstallEventFilter() {
    m_eater = new MousePressEater(this);
    m_pushButton1->installEventFilter(m_eater);
}

void okienko::doRemoveEventFilter() {
    m_pushButton1->removeEventFilter(m_eater);
}

bool okienko::event(QEvent * event) {
//    qDebug() << "event" << event->type() << this->thread()->currentThreadId();;

    if (event->type() == 1012) {
        qDebug() << "Color event" << event->type() << this->thread()->currentThreadId();;
        ColorEvent *colorEv = static_cast<ColorEvent *>(event);
        QPalette pal(palette());
        pal.setColor(QPalette::Background, colorEv->getColor());
        this->setPalette(pal);

        return true;
    }

    return QWidget::event(event);
}

void okienko::paintEvent(QPaintEvent * event) {
//    qDebug() << "       paint event" << event->type() << this->thread()->currentThreadId();
}

void okienko::mouseDoubleClickEvent(QMouseEvent *event) {
    qDebug() << "       mouseDoubleClickEvent" << event->type() << this->thread()->currentThreadId();
    event->setAccepted(true);
}


void okienko::sendPostedEvent() {
    qDebug() << "sendPostedEvent" << this->thread()->currentThreadId();

    ColorEvent *event2 = new ColorEvent(Qt::green);
    QCoreApplication::postEvent(this, event2); //green
    qDebug() << "Event post was send";
}

void okienko::sendEventNormal() {
    qDebug() << "sendEventNormal" << this->thread()->currentThreadId();

    ColorEvent event(Qt::red);
    QCoreApplication::sendEvent(this, &event);
    qDebug() << "Event normal was send";
}
