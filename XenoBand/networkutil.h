#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H

#include "core.h"

class NetworkUtil
{
public:
    NetworkUtil();

    static QString sendMessage(QString name, QString message);
};

#endif // NETWORKUTIL_H
