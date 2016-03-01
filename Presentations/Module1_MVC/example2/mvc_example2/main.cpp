#include <QApplication>
#include <QFileSystemModel>
#include <QTreeView>

#include "handler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    QModelIndex parentIndex = model->index(QDir::currentPath());

    QTreeView *tree = new QTreeView;
    tree->setModel(model);
    tree->setRootIndex(parentIndex);
    tree->show();

    handler h;

    return a.exec();
}
