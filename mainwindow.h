#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "training.h"
#include "word.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Init();

private slots:
    void on_pushButtonVague_clicked();

    void on_pushButtonUnknown_clicked();

    void on_pushButtonKnown_clicked();

    void on_pushButtonWrong_clicked();

    void on_pushButtonRight_clicked();

    void on_pushButtonNext_clicked();

    void on_pushButtonAddNew_clicked();

private:
    enum RememberStatus
    {
        Invalid,
        Unknown,
        Vague,
        Known
    };

    Ui::MainWindow *ui;
    Training training;
    Word* word;
    RememberStatus preSelect;
    void ShowNextWord();
};



#endif // MAINWINDOW_H
