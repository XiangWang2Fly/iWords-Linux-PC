#ifndef COMMON_H
#define COMMON_H
#include <QString>


class Common
{
public:
    Common();
    static QString version;
    static int GenerateRandomInteger(int min, int max);
};



#endif // COMMON_H
