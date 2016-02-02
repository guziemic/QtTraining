#include "mainwindow.h"

#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QPushButton *button1 = new QPushButton("Quit");
    QPushButton *button2 = new QPushButton("Send");

    QVBoxLayout *layout = new QVBoxLayout(this);

    setCentralWidget(button1);

    this->layout()->addWidget(button1);
    this->layout()->addWidget(button2);

//    this->setLayout(layout);
}

MainWindow::~MainWindow()
{

}
