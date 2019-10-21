#ifndef SERVERSTATE_H
#define SERVERSTATE_H

#include <QDialog>

namespace Ui {
class ServerState;
}

class ServerState : public QDialog
{
    Q_OBJECT

public:
    explicit ServerState(QWidget *parent = nullptr);
    ~ServerState();
    bool GetChoice();


private:
    Ui::ServerState *ui;
    bool retry;
};

#endif // SERVERSTATE_H
