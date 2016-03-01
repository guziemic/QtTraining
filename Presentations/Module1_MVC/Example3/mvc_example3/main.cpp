#include <QApplication>
#include <QStringList>
#include <QAbstractItemModel>
#include <QListView>
#include <QStringListModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList fruits;
    fruits << "Orange" << "Apple" << "Banana" << "Lemon" << "Kiwi";

    QAbstractItemModel *model = new QStringListModel(fruits);

    QListView *view = new QListView;
    view->setModel(model);
    view->show();
    view->setWindowTitle("First view");

    QListView *second = new QListView;
    second->setModel(model);
    second->show();
    second->setWindowTitle("Second view");

    // enables the same selection in both views
//    second->setSelectionModel(view->selectionModel());

    return a.exec();
}
