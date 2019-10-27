#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    this->training.Save();
}

void MainWindow::on_pushButtonVague_clicked()
{
    if (!this->ui->labelMeaning->text().isEmpty())
    {
        this->preSelect = Vague;
        this->ui->labelEnglish->setText(this->word->English);
        this->ui->pushButtonWrong->hide();
        this->ui->pushButtonNext->setHidden(false);
        this->ui->pushButtonRight->hide();
    }
}

void MainWindow::on_pushButtonUnknown_clicked()
{
    if (!this->ui->labelMeaning->text().isEmpty())
    {
        this->preSelect = Unknown;
        this->ui->labelEnglish->setText(this->word->English);
        this->ui->pushButtonWrong->hide();
        this->ui->pushButtonNext->setHidden(false);
        this->ui->pushButtonRight->hide();
    }
}

void MainWindow::on_pushButtonKnown_clicked()
{
    if (!this->ui->labelMeaning->text().isEmpty())
    {
        this->preSelect = Known;
        this->ui->labelEnglish->setText(this->word->English);
        this->ui->pushButtonWrong->setHidden(false);
        this->ui->pushButtonNext->hide();
        this->ui->pushButtonRight->setHidden(false);
    }
}

void MainWindow::on_pushButtonWrong_clicked()
{
    if (!this->ui->labelEnglish->text().isEmpty())
    {
        this->ShowNextWord();
        this->preSelect = Invalid;
        this->ui->pushButtonWrong->hide();
        this->ui->pushButtonRight->hide();
    }
}

void MainWindow::on_pushButtonRight_clicked()
{
    if (!this->ui->labelEnglish->text().isEmpty())
    {
        switch (this->preSelect)
        {
            case Unknown:
            case Vague:
                word->KnowDate = this->training.ResetDate();
                word->ConfirmDate = this->training.ResetDate();
                word->NextDate = QDateTime::currentDateTime().addDays(6).date();
                word->Modified = true;
                this->training.Modified = true;
                break;
            case Known:
                if (word->KnowDate <= this->training.ResetDate())
                {
                    word->KnowDate = QDateTime::currentDateTime().date();
                    word->NextDate = QDateTime::currentDateTime().addDays(31).date();
                }
                else if (word->ConfirmDate <= this->training.ResetDate())
                {
                    word->ConfirmDate = QDateTime::currentDateTime().date();
                    word->NextDate = QDateTime::currentDateTime().addDays(31*2).date();
                }
                else
                {
                    // confirmed before
                    word->NextDate = QDateTime::currentDateTime().addDays(31*4).date();
                }

                word->Modified = true;
                this->training.Modified = true;
                break;
            default:
                break;
        }
        this->ShowNextWord();
        this->ui->pushButtonWrong->hide();
        this->ui->pushButtonRight->hide();
    }
}

void MainWindow::on_pushButtonNext_clicked()
{
    if (!this->ui->labelEnglish->text().isEmpty())
    {
        this->ShowNextWord();
        this->ui->pushButtonWrong->hide();
        this->ui->pushButtonRight->hide();
    }
}

void MainWindow::on_pushButtonAddNew_clicked()
{
int test;
test++;
}

void MainWindow::Init()
{
    this->ui->pushButtonWrong->hide();
    this->ui->pushButtonNext->hide();
    this->ui->pushButtonRight->hide();
    this->training.Load();
    this->training.Calc();
    this->ShowNextWord();
    int numTotal = this->training.GetNumTotal();
    this->ui->labelTotal->setText(QString::number(numTotal, 10));
    this->ui->labelConfirm->setText(QString::number(this->training.NumConfirmed, 10));
    if (numTotal > 0){
        this->ui->labelRate->setText(QString("%1%").arg(static_cast<float>(this->training.NumConfirmed)/numTotal*100));
    }
}

void MainWindow::ShowNextWord()
{
    this->word = this->training.GetNext();
    this->ui->labelMeaning->setText(this->word->Meaning);
    this->ui->labelEnglish->setText("");
}


