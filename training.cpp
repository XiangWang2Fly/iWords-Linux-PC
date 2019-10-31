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
            this->Words.append(wo);
        }
    }

    if (this->Words.count() < 100)
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
                QString command = "select top 100 * from Words where meaning = '";
                command.append(wo.Meaning);
                command.append("' order by next_date");
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
                this->Words[index].NextDate = wo.NextDate;
                this->Words[index].KnowDate = wo.KnowDate;
                this->Words[index].ConfirmDate = wo.ConfirmDate;
                this->Words[index].Modified = wo.Modified;
            }
            else {
                this->Words.append(wo);
            }

            if (this->Words.count() >= 100)
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
    int maxTry = this->Words.count();
    do {
        int i = Common::GenerateRandomInteger(0, this->Words.count());
        wo = &this->Words[i];
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

    for (int i = 0; i < this->Words.count(); i++) {
        if (this->Words[i].Meaning == meaning)
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
        for (int i = 0; i < this->Words.count(); i++) {
            if (this->Words[i].Modified){
                // update to server
                if (serverReady) {
                    QString search = "select * from Words where meaning = '";
                    search.append(this->Words[i].Meaning);
                    search.append("'");
                    int count = 0;
                    QSqlQuery query(search);
                    if (query.next()) {
                        Word onServer;
                        onServer.NextDate = query.value(2).toDate();
                        if (this->Words[i].NextDate != onServer.NextDate) {
                            QString command = "update Words set ";
                            command.append("next_date='");
                            command.append(this->Words[i].NextDate.toString("yyyy-M-d"));
                            command.append("',know_date='");
                            command.append(this->Words[i].KnowDate.toString("yyyy-M-d"));
                            command.append("',confirm_date='");
                            command.append(this->Words[i].ConfirmDate.toString("yyyy-M-d"));
                            command.append("'where meaning='");
                            command.append(this->Words[i].Meaning);
                            command.append("'");
                            query.exec(command);
                        }
                    }
                    else {
                        do{
                            QString insert = "insert into Words values ('";
                            insert.append(this->Words[i].Meaning);
                            insert.append("', '");
                            insert.append(this->Words[i].English);
                            insert.append("', '");
                            insert.append(this->Words[i].NextDate.toString("yyyy-M-d"));
                            insert.append("', '");
                            insert.append(this->Words[i].KnowDate.toString("yyyy-M-d"));
                            insert.append("', '");
                            insert.append(this->Words[i].ConfirmDate.toString("yyyy-M-d"));
                            insert.append("')");
                            query.exec(insert);

                            if (query.exec(search))
                            {
                                if (query.next())
                                {
                                    break;
                                }
                                else
                                {
                                    count++;
                                }
                            }

                        }while(count < 2);
                    }

                    // make sure server has it
                    if (count < 2){
                        // delete local
                        QDomNode node = root.firstChild();
                        while (!node.isNull()) {
                            Word wo(node.toElement());
                            if (wo.Meaning == this->Words[i].Meaning) {
                                root.removeChild(node);
                                break;
                            }
                            else {
                                node = node.nextSibling();
                            }
                        }

                    }

                }
                else {
                    // update local
                    bool find = false;
                    QDomNode node = root.firstChild();
                    while (!node.isNull()) {
                        Word wo(node.toElement());
                        if (wo.Meaning == this->Words[i].Meaning) {
                            node.toElement().setAttribute("english", this->Words[i].English);
                            node.toElement().setAttribute("next", this->Words[i].NextDate.toString());
                            node.toElement().setAttribute("know", this->Words[i].KnowDate.toString());
                            node.toElement().setAttribute("confirm", this->Words[i].ConfirmDate.toString());
                            find = true;
                            break;
                        }
                    }

                    // creat new
                    if (!find) {
                        QDomElement element =  doc.createElement("word");
                        element.setAttribute("meaning", this->Words[i].Meaning);
                        element.setAttribute("english", this->Words[i].English);
                        element.setAttribute("next", this->Words[i].NextDate.toString());
                        element.setAttribute("know", this->Words[i].KnowDate.toString());
                        element.setAttribute("confirm", this->Words[i].ConfirmDate.toString());
                        root.appendChild(element);
                    }
                } // if (serverReady)
            } // if (this->words[i].Modified)
        }// for (int i = 0; i < this->words.count(); i++)

        file.open(QIODevice::WriteOnly|QIODevice::Truncate);
        QTextStream out(&file);
        doc.save(out, 4);
        file.close();
        if (serverReady)
        {
            SqlServer::GetInstance().Close();
        }
    }
}

void Training::Calc() {
    this->NumConfirmed = 0;
    foreach (Word wo, this->Words){
        if (wo.ConfirmDate > this->ResetDate())
        {
            this->NumConfirmed++;
        }
    }
}
