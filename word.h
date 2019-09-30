#ifndef WORD_H
#define WORD_H
#include <QString>
#include <QXmlStreamReader>


class Word
{
public:
    Word();
    QString Chinese;
    QString English;
    time_t DateKnow;
    time_t DateConfirm;
    time_t DateNext;
};

#endif // WORD_H
