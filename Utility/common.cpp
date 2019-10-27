#include "common.h"
#include <QDateTime>

Common::Common()
{

}

QString Common::version = "Ver.1";

int Common::GenerateRandomInteger(int min, int max)
{
    if (min > max)
    {
        return max;
    }

    qsrand(static_cast<uint>(QDateTime::currentDateTime().time().second()));
    int result = qrand() % (max - min);
    result = min + result;
    return result;
}

