#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonVague_clicked();

    void on_pushButtonUnknown_clicked();

    void on_pushButtonKnown_clicked();

    void on_pushButtonWrong_clicked();

    void on_pushButtonRight_clicked();

    void on_pushButtonAddNew_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
