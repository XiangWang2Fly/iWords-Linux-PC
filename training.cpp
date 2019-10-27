#include "training.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QFile>
#include <QtXml/QtXml>
#include <QtXml/QDomDocument>
#include "word.h"
#include "./Database/sql.h"
#include "./Utility/common.h"

Training::Training()
{

}

void Training::Load()
{
    bool serverReady = false;

    // load from server
    if (SqlServer::GetInstance().Open())
    {
        serverReady = true;
        QSqlQuery query("select * from Words where DATEDIFF(day, next_date, GETDATE()) >= 0");
        while(query.next())
        {
            Word wo;
            wo.Meaning = query.value(0).toString();
            wo.English = query.value(1).toString();
            wo.NextDate = query.value(2).toDate();
            wo.KnowDate = query.value(3).toDate();
            wo.ConfirmDate = query.value(4).toDate();
            this->words.append(wo);
        }
    }

    if (this->words.count() < 100)
    {
        // load from local
        QDomDocument doc;
        QFile file("../iWords-Linux-PC/Words.xml");
        if (!file.open(QIODevice::ReadOnly))
        {
            return;
        }

        if (!doc.setContent(&file))
        {
            file.close();
            return;
        }

        file.close();

        QDomElement root = doc.documentElement();
        QDomNode node = root.firstChild();
        while (!node.isNull())
        {
            Word wo(node.toElement());
            if (serverReady) {
                QString command = "select * from Words where meaning = '";
                command.append(wo.Meaning);
                command.append("'");
                QSqlQuery query(command);
                if (query.next())
                {
                    Word onServer;
                    onServer.NextDate = query.value(2).toDate();
                    onServer.KnowDate = query.value(3).toDate();
                    onServer.ConfirmDate = query.value(4).toDate();
                    if (onServer.NextDate > wo.NextDate)
                    {
                        wo.NextDate = onServer.NextDate;
                        wo.Modified = true;
                    }

                    if (onServer.KnowDate > wo.KnowDate)
                    {
                        wo.KnowDate = onServer.KnowDate;
                        wo.Modified = true;
                    }

                    if (onServer.ConfirmDate > wo.ConfirmDate)
                    {
                        wo.ConfirmDate = onServer.ConfirmDate;
                        wo.Modified = true;
                    }
                }
            }

            int index = this->FindIndex(wo.Meaning);
            if (index != -1) {
                this->words[index].NextDate = wo.NextDate;
                this->words[index].KnowDate = wo.KnowDate;
                this->words[index].ConfirmDate = wo.ConfirmDate;
                this->words[index].Modified = wo.Modified;
            }
            else {
                this->words.append(wo);
            }

            if (this->words.count() >= 100)
            {
                break;
            }
            else
            {
                node = node.nextSibling();
            }
        }
    }
}

Word* Training::GetNext() {
    Word* wo;
    int maxTry = this->words.count();
    do {
        int i = Common::GenerateRandomInteger(0, this->words.count());
        wo = &this->words[i];
        if (wo->NextDate > QDateTime().currentDateTime().date()) {
            break;
        }

        maxTry--;
    }while(maxTry > 0);

    return wo;
}

QDate Training::ResetDate(){
    return QDate(1981, 7, 10);
}

int Training::FindIndex(QString meaning) {
    int result = -1;

    for (int i = 0; i < this->words.count(); i++) {
        if (this->words[i].Meaning == meaning)
        {
            result = i;
            break;
        }
    }

    return result;
}

void Training::Save()
{
    if (this->Modified)
    {
        bool serverReady = SqlServer::GetInstance().Open();
        QDomDocument doc;
        QFile file("../iWords-Linux-PC/Words.xml");
        file.open(QIODevice::ReadOnly);
        doc.setContent(&file);
        file.close();
        QDomElement root = doc.documentElement();
        for (int i = 0; i < this->words.count(); i++) {
            if (this->words[i].Modified){
                // update to server
                if (serverReady) {
                    QString command = "select * from Words where meaning = '";
                    command.append(this->words[i].Meaning);
                    command.append("'");
                    QSqlQuery query(command);
                    if (query.next()) {
                        Word onServer;
                        onServer.NextDate = query.value(2).toDate();
                        if (this->words[i].NextDate != onServer.NextDate) {
                            QString command = "update Words set ";
                            command.append("next_date='");
                            command.append(this->words[i].NextDate.toString("yyyy-M-d"));
                            command.append("',know_date='");
                            command.append(this->words[i].KnowDate.toString("yyyy-M-d"));
                            command.append("',confirm_date='");
                            command.append(this->words[i].ConfirmDate.toString("yyyy-M-d"));
                            command.append("'where meaning='");
                            command.append(this->words[i].Meaning);
                            command.append("'");
                            QSqlQuery query(command);
                        }
                    }
                    else {
                        QString command = "insert into Words values ('";
                        command.append(this->words[i].Meaning);
                        command.append("', '");
                        command.append(this->words[i].English);
                        command.append("', '");
                        command.append(this->words[i].NextDate.toString("yyyy-M-d"));
                        command.append("', '");
                        command.append(this->words[i].KnowDate.toString("yyyy-M-d"));
                        command.append("', '");
                        command.append(this->words[i].ConfirmDate.toString("yyyy-M-d"));
                        command.append("')");
                        QSqlQuery query(command);
                    }

                    // delete local
                    QDomNode node = root.firstChild();
                    while (!node.isNull()) {
                        Word wo(node.toElement());
                        if (wo.Meaning == this->words[i].Meaning) {
                            root.removeChild(node);
                            break;
                        }
                        else {
                            node = node.nextSibling();
                        }
                    }
                }
                else {
                    // update local
                    bool find = false;
                    QDomNode node = root.firstChild();
                    while (!node.isNull()) {
                        Word wo(node.toElement());
                        if (wo.Meaning == this->words[i].Meaning) {
                            node.toElement().setAttribute("english", this->words[i].English);
                            node.toElement().setAttribute("next", this->words[i].NextDate.toString());
                            node.toElement().setAttribute("know", this->words[i].KnowDate.toString());
                            node.toElement().setAttribute("confirm", this->words[i].ConfirmDate.toString());
                            find = true;
                            break;
                        }
                    }

                    // creat new
                    if (!find) {
                        QDomElement element =  doc.createElement("word");
                        element.setAttribute("meaning", this->words[i].Meaning);
                        element.setAttribute("english", this->words[i].English);
                        element.setAttribute("next", this->words[i].NextDate.toString());
                        element.setAttribute("know", this->words[i].KnowDate.toString());
                        element.setAttribute("confirm", this->words[i].ConfirmDate.toString());
                        root.appendChild(element);
                    }
                } // if (serverReady)
            } // if (this->words[i].Modified)
        }// for (int i = 0; i < this->words.count(); i++)

        file.open(QIODevice::WriteOnly|QIODevice::Truncate);
        QTextStream out(&file);
        doc.save(out, 4);
        file.close();
    }
}

void Training::Calc() {
    this->NumConfirmed = 0;
    foreach (Word wo, this->words){
        if (wo.ConfirmDate > this->ResetDate())
        {
            this->NumConfirmed++;
        }
    }
}

int Training::GetNumTotal(){
    return this->words.count();
}
