#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonVague_clicked()
{

}

void MainWindow::on_pushButtonUnknown_clicked()
{

}

void MainWindow::on_pushButtonKnown_clicked()
{

}

void MainWindow::on_pushButtonWrong_clicked()
{

}

void MainWindow::on_pushButtonRight_clicked()
{

}

void MainWindow::on_pushButtonAddNew_clicked()
{
    int test = 0;
    test++;
}

