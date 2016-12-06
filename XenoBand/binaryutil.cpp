#include "core.h"
#include "binaryutil.h"

BinaryUtil::BinaryUtil()
{
}

string BinaryUtil::toBinary(char c)
{
    stringstream ss;
    for (int i = 7; i>=4; i--)
        ss << ((c>>i)&1);
    ss << " ";
    for (int i = 3; i>=0; i--)
        ss << ((c>>i)&1);
    return ss.str();
}

int BinaryUtil::toInt(int argc, char *buffer)
{
    int ret = 0;
    for (int i=0; i<argc; i++)
    {
        ret |= ((((unsigned char) buffer[i]) << (8*i)));
    }
    return ret;
}

int BinaryUtil::toShort(int argc, char *buffer)
{
    short ret = 0;
    for (int i=0; i<argc; i++)
    {
        ret |= ((((unsigned char) buffer[i]) << (8*i)));
    }
    return ret;
}
