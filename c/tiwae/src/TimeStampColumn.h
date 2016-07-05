//
// Created by Michael Barker on 06/08/15.
//

#ifndef TIWAE_TIMESTAMPCOLUMN_H
#define TIWAE_TIMESTAMPCOLUMN_H

#include <stdint.h>
#include "Column.h"

struct TimestampColumnDefn
{
    uint64_t position;
    uint64_t capacity;
    uint64_t baseTimestamp;
};

class TimeStampColumn : public Column
{
private:
    TimestampColumnDefn* data;
    int64_t* m_body;

public:
    TimeStampColumn(const char* storeRoot, const char* name, size_t initialSize);
    int64_t append(const int64_t timestamp);

    int64_t position();

    int64_t get(int i);
};


#endif //TIWAE_TIMESTAMPCOLUMN_H
