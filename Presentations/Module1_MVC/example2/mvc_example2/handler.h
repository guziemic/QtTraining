#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>

class QFileSystemModel;

class handler : public QObject
{
    Q_OBJECT
public:
    explicit handler(QObject *parent = 0);

signals:

public slots:
    void getInfo(const QString& path);

private:
    QFileSystemModel *m_model;
};

#endif // HANDLER_H
