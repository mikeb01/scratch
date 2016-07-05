//
// Created by Michael Barker on 06/08/15.
//

#include <dirent.h>
#include <sys/fcntl.h>
#include "TimeStampColumn.h"

TimeStampColumn::TimeStampColumn(const char* storeRoot, const char* name, size_t initialSize)
    : Column(storeRoot, name, initialSize), m_body(bodyPtr<int64_t>())
{
}

int64_t TimeStampColumn::append(const int64_t timestamp)
{
    m_body[header()->m_position] = timestamp;
    header()->m_position++;

    return header()->m_position;
}

int64_t TimeStampColumn::position()
{
    return header()->m_position;
}

int64_t TimeStampColumn::get(int i)
{
    if (i < 0 || position() <= i)
    {
        return -1;
    }

    return bodyPtr<int64_t>()[i];
}
