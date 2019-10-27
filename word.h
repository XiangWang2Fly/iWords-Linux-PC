#ifndef WORD_H
#define WORD_H
#include <QString>
#include <QDate>
#include <QXmlStreamReader>
#include <QtXml/QDomDocument>

class Word
{
public:
    Word();
    Word(QDomElement de);
    QString Meaning;
    QString English;
    QDate KnowDate = QDate(1981, 7, 10);
    QDate ConfirmDate = QDate(1981, 7, 10);
    QDate NextDate = QDate(1981, 7, 10);
    bool Modified = false;
private:

};

#endif // WORD_H
