

set(SOURCE
    Column.cpp
    ScaledIntColumn.cpp
    TimeStampColumn.cpp
    MarketDataStore.cpp)

set(HEADER
    Exceptions.h
    ScaledIntColumn.h
    TimeStampColumn.h
    MarketDataStore.h)

add_library(tiwae_lib STATIC ${SOURCE} ${HEADERS})
add_executable(vec main2.cpp)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(tiwae main.cpp)
    target_link_libraries(tiwae ${Boost_LIBRARIES} tiwae_lib)
endif()

