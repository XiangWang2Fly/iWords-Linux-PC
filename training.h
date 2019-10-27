#ifndef IWORDS_TRAINING_H_
#define IWORDS_TRAINING_H_
#include "word.h"


class Training
{
public:
    Training();

    // load from server and local xml file
    void Load();
    Word* GetNext();
    QDate ResetDate();
    void Save();
    void Calc();
    bool Modified;
    int NumConfirmed;
    int FindIndex(QString meaning);
    QList<Word> Words;
private:

};

#endif // IWORDS_TRAINING_H_
