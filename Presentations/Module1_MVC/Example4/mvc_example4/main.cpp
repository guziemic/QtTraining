#include "spinboxdelegate.h"

#include <QApplication>
#include <QTableView>
#include <QStandardItemModel>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStandardItemModel *model = new QStandardItemModel(4, 3);

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column) {
            QModelIndex index = model->index(row, column, QModelIndex());
            model->setData(index, QVariant((row + 1) * (column + 1)));
        }
    }

    QTableView *table = new QTableView;
    table->setWindowTitle("Spin box delegate example");

    SpinBoxDelegate delegate;
    table->setItemDelegate(&delegate);
    table->setModel(model);
    table->show();

    return a.exec();
}
