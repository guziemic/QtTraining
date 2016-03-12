#include "window.h"
#include <QApplication>
#include <QStandardItemModel>
#include <QTime>

void addHotel(QAbstractItemModel *model, const QString &hotelName, const double &price,
              const QString &email, const QDateTime &date) {
    model->insertRow(0);
    model->setData(model->index(0,0), hotelName);
    model->setData(model->index(0,1), price);
    model->setData(model->index(0,2), email);
    model->setData(model->index(0,3), date);
}

QAbstractItemModel * createHotelModel(QObject *parent) {
    QStandardItemModel *model = new QStandardItemModel(0, 4, parent);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Hotel"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Price"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Last reservation"));

    addHotel(model, "Ramada", 95 , "John B. <aohn.b@example.com>", QDateTime(QDate(2015, 01, 31), QTime(9, 03)));
    addHotel(model, "Ramada", 80 , "Ann C. <ann@yahoooo.com>", QDateTime(QDate(2016, 01, 31), QTime(10, 23)));
    addHotel(model, "Motel One", 72 , "Ralph W. <ralph.w@google.com>", QDateTime(QDate(2015, 10, 27), QTime(16, 13)));
    addHotel(model, "Motel One", 85 , "Radek B <borosrad@bb.top.com>", QDateTime(QDate(2015, 8, 30), QTime(17, 16)));
    addHotel(model, "Hilton", 195 , "Andrzej W. <andrei.w@wow.com>", QDateTime(QDate(2015, 9, 19), QTime(18, 18)));
    addHotel(model, "Orbis", 110 , "Donald Duck <donald.duck@disney.com>", QDateTime(QDate(2015, 10, 14), QTime(11, 01)));
    addHotel(model, "Ibis", 100 , "Tom J. <tom@disney.com>", QDateTime(QDate(2015, 11, 02), QTime(17, 03)));
    addHotel(model, "Ibis", 140 , "Alfred K. <alf.kos@kotarka.com>", QDateTime(QDate(2015, 11, 12), QTime(17, 15)));
    addHotel(model, "Michealine", 200 , "JJ <john.justin@the.best.email.com>", QDateTime(QDate(2015, 12, 10), QTime(21, 03)));
    addHotel(model, "Duke", 50 , "The three pigs<@.com>", QDateTime(QDate(2015, 07, 18), QTime(23, 22)));
//    addHotel(model, "", 95 , "<@.com>", QDateTime(QDate(2015, 01, 31), QTime(17, 03)));

    return model;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Window w;
    w.setSourceModel(createHotelModel(&w));
    w.show();

    return a.exec();
}
