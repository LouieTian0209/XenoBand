#ifndef SCALE_H
#define SCALE_H

class Scale
{
public:
    Scale();

    static double keyFrequencies[88];

    static double getFrequency(int key);
};

#endif // SCALE_H
