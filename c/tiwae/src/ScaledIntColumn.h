//
// Created by Michael Barker on 08/08/15.
//

#ifndef TIWAE_SCALEDINTCOLUMN_H
#define TIWAE_SCALEDINTCOLUMN_H

#include "Column.h"

class ScaledIntColumn : public Column
{
public:
    ScaledIntColumn(const char* storeRoot, const char* name, size_t initialSize, int32_t scale);

    int64_t append(double value);

    double get(int i);

    float getF(int i);

    void get(int i, double (&vec)[4]);

    double sum();

    float sumFloat();

private:
    int32_t m_scale;
    double m_multiplier;
    int32_t* m_body;
};


#endif //TIWAE_SCALEDINTCOLUMN_H
