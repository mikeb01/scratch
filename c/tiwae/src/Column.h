//
// Created by Michael Barker on 08/08/15.
//

#ifndef TIWAE_COLUMN_H
#define TIWAE_COLUMN_H

#include <cstdint>
#include <memory>
#include <stddef.h>

#pragma pack(push)
#pragma pack(1)
struct ColumnHeaderDefn
{
    int64_t m_position;
    char pad[8];
};
#pragma pack(pop)

class Column
{
public:
    Column(const char* storeRoot, const char* name, size_t initialSize);

    int64_t position()
    {
        return m_header->m_position;
    }

protected:
    uint8_t* memoryPtr()
    {
        return m_memory;
    }

    size_t memorySize()
    {
        return m_size;
    }

    ColumnHeaderDefn* header()
    {
        return m_header;
    }

    template<typename T>
    T* bodyPtr()
    {
        return reinterpret_cast<T*>(&m_memory[sizeof(ColumnHeaderDefn)]);
    }

private:
    std::uint8_t* m_memory;
    size_t m_size;
    ColumnHeaderDefn* m_header;
};


#endif //TIWAE_COLUMN_H
