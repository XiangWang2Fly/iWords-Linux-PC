#include "serverstate.h"
#include "ui_serverstate.h"

ServerState::ServerState(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerState)
{
    ui->setupUi(this);
    ui->labelMessage->setText("Server is not ready, retry?");
    ui->labelMessage->set
}

ServerState::~ServerState()
{
    delete ui;
}
