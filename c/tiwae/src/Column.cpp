//
// Created by Michael Barker on 08/08/15.
//

#include <dirent.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <iostream>

#include "Column.h"
#include "Exceptions.h"
#include "ScopeUtil.h"

Column::Column(const char* storeRoot, const char* name, size_t initialSize)
{
    auto dir = ::opendir(storeRoot);

    if (NULL == dir)
    {
        throw IOException{std::string("opendir failed"), SOURCEINFO};
    }

    OnScopeExit tidy1([&]()
    {
        closedir(dir);
    });

    auto dirFd = ::dirfd(dir);
    if (dirFd < 0)
    {
        throw IOException{std::string{strerror(errno)}, SOURCEINFO};
    }

    auto fd = ::openat(dirFd, name, O_RDWR|O_CREAT, 0644);
    if (fd < 0)
    {
        throw IOException{std::string{strerror(errno)}, SOURCEINFO};
    }

    OnScopeExit tidy2([&]()
    {
        close(fd);
    });

    if (ftruncate(fd, initialSize) < 0)
    {
        throw IOException{std::string{strerror(errno)}, SOURCEINFO};
    }

    auto memory = ::mmap(NULL, initialSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    if (MAP_FAILED == memory)
    {
        throw IOException{std::string{strerror(errno)}, SOURCEINFO};
    }

    m_memory = static_cast<uint8_t*>(memory);
    m_size = initialSize;
    m_header = reinterpret_cast<ColumnHeaderDefn*>(m_memory);
}
