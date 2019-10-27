#include "word.h"

Word::Word()
{

}

Word::Word(QDomElement de)
{
    if (de.hasAttribute("chinese"))
    {
       this->Meaning = de.attribute("chinese");
    }
    else if (de.hasAttribute("meaning"))
    {
       this->Meaning = de.attribute("meaning");
    }

    this->English = de.attribute("english");
    this->NextDate = QDate::fromString(de.attribute("next"), "yyyy/M/d");
    this->KnowDate = QDate::fromString(de.attribute("know"), "yyyy/M/d");
    this->ConfirmDate = QDate::fromString(de.attribute("confirm"), "yyyy/M/d");
}
