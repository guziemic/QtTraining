#include <QApplication>
#include <QTableView>
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStandardItemModel *model = new QStandardItemModel(8, 4);

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column) {
            QModelIndex index = model->index(row, column);
            model->setData(index, QVariant((row + column)));
        }
    }

    QTableView *table = new QTableView;
    table->setModel(model);
    table->resize(440, 280);
    table->show();

    QItemSelectionModel *selectionModel = table->selectionModel();
    QModelIndex topLeft = model->index(0, 0, QModelIndex());
    QModelIndex bottomRight = model->index(5, 2, QModelIndex());

    QItemSelection selection(topLeft, bottomRight);
    selectionModel->select(selection, QItemSelectionModel::Select);

    // update item - toggle selection
//    QItemSelection toggleSelection;
//    topLeft = model->index(2, 1, QModelIndex());
//    bottomRight = model->index(7, 3, QModelIndex());
//    toggleSelection.select(topLeft, bottomRight);
//    selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);

    // update item - column selection
//    QItemSelection columnSelection;
//    topLeft = model->index(0, 1, QModelIndex());
//    bottomRight = model->index(0, 2, QModelIndex());
//    columnSelection.select(topLeft, bottomRight);
//    selectionModel->select(columnSelection, QItemSelectionModel::ClearAndSelect
//                                            | QItemSelectionModel::Columns);


    // update item - row selection
//    QItemSelection rowSelection;
//    topLeft = model->index(1, 1, QModelIndex());
//    bottomRight = model->index(3, 2, QModelIndex());
//    rowSelection.select(topLeft, bottomRight);
//    selectionModel->select(rowSelection, QItemSelectionModel::ClearAndSelect
//                                            | QItemSelectionModel::Rows);

    // selection of all items in model
//    QModelIndex parent = QModelIndex();
//    QItemSelection allSelection;
//    topLeft = model->index(0, 0, QModelIndex());
//    bottomRight = model->index(model->rowCount(parent) - 1,
//                               model->columnCount(parent) - 1, parent);
//    allSelection.select(topLeft, bottomRight);
//    selectionModel->select(allSelection, QItemSelectionModel::Select);

    return a.exec();
}
