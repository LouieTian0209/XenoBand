#ifndef BINARYUTIL_H
#define BINARYUTIL_H

#include "core.h"

class BinaryUtil
{
public:
    BinaryUtil();

    static string toBinary(char c);
    static int toInt(int argc, char *buffer);
    static int toShort(int argc, char *buffer);
};

#endif // BINARYUTIL_H
