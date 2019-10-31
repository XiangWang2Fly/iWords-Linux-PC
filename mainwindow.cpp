#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>

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
                this->UpdateScoreBoard();
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
        this->on_pushButtonRight_clicked();
        this->ui->pushButtonNext->hide();
    }
}

void MainWindow::on_pushButtonAddNew_clicked()
{
    if (!this->ui->lineEditMeaning->text().isEmpty() && !this->ui->lineEditEnglish->text().isEmpty()) {
        Word wo;
        wo.Meaning = this->ui->lineEditMeaning->text();
        wo.English = this->ui->lineEditEnglish->text();
        wo.NextDate = QDateTime::currentDateTime().addDays(6).date();
        if (this->training.FindIndex(wo.Meaning) != -1)
        {
            QMessageBox::information(NULL, "Info", "Meaning exists", QMessageBox::Ok, QMessageBox::No);
        }
        else
        {
            wo.Modified = true;
            this->training.Words.append(wo);
            this->ui->labelTotal->setText(QString::number(this->training.Words.count(), 10));
            this->training.Modified = true;
            this->UpdateScoreBoard();
        }

        this->ui->lineEditMeaning->clear();
        this->ui->lineEditEnglish->clear();
    }
}

void MainWindow::Init()
{
    this->ui->pushButtonWrong->hide();
    this->ui->pushButtonNext->hide();
    this->ui->pushButtonRight->hide();

    // an information to user
    QMessageBox *box = new QMessageBox(QMessageBox::Information,"Info", "Start loading Database");
    QTimer::singleShot(1200, box, SLOT(close()));
    box->exec();
    this->training.Load();
    this->training.Calc();
    this->ShowNextWord();
    this->UpdateScoreBoard();
}

void MainWindow::ShowNextWord()
{
    this->word = this->training.GetNext();
    this->ui->labelMeaning->setText(this->word->Meaning);
    this->ui->labelEnglish->setText("");
}

void MainWindow::UpdateScoreBoard() {
    int numTotal = this->training.Words.count();
    this->ui->labelTotal->setText(QString::number(numTotal, 10));
    this->ui->labelConfirm->setText(QString::number(this->training.NumConfirmed, 10));
    float rate = static_cast<float>(this->training.NumConfirmed)/numTotal*100;
    if (numTotal > 0){
        this->ui->labelRate->setText(QString("%1 %").arg(QString::number(rate, 'f', 1)));
    }
}


