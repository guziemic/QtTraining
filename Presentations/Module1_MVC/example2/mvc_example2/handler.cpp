#include "handler.h"
#include <QFileSystemModel>
#include <QDebug>
#include <iostream>

handler::handler(QObject *parent) : QObject(parent)
{
    m_model = new QFileSystemModel;
    m_model->setRootPath(QDir::currentPath());

    QObject::connect(m_model, &QFileSystemModel::directoryLoaded, this, &handler::getInfo);
}

void handler::getInfo(const QString& path) {
    qDebug() << "Directory loaded" << path;

    QModelIndex parentIndex = m_model->index(QDir::currentPath());
    int numRows = m_model->rowCount(parentIndex);
    int numColumns = m_model->columnCount(parentIndex);

    qDebug() << numRows << numColumns;
    for (int row = 0; row < numRows; ++row) {
        QModelIndex index = m_model->index(row, 0, parentIndex);
        QString text = "";
//        QString text = m_model->data(index, Qt::DisplayRole).toString();
        for (int column = 0; column < numColumns; ++column) {
            index = m_model->index(row, column, parentIndex);
            text = text + "\t" + m_model->data(index, Qt::DisplayRole).toString();
        }

        std::cout << "Row " << row << " " << text.toUtf8().toStdString() << std::endl;
    }
}
