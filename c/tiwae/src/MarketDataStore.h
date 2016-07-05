//
// Created by Michael Barker on 08/08/15.
//

#ifndef TIWAE_MARKETDATASTORE_H
#define TIWAE_MARKETDATASTORE_H

#include <iostream>
#include <sstream>
#include <math.h>
#include "TimeStampColumn.h"
#include "ScaledIntColumn.h"

class TopOfBook
{
public:

    int parse(
        std::istream& line,
        const int32_t priceScale, const int32_t qtyScale)
    {
        std::string cell;

        std::getline(line, cell, ',');
        m_timestamp = std::stoll(cell);

        std::getline(line, cell, ',');
        m_bidPrice = cell.length() > 0  ? std::stod(cell) : 0;

        std::getline(line, cell, ',');
        m_bidQty = cell.length() > 0  ? std::stod(cell) : 0;

        std::getline(line, cell, ',');
        m_askPrice = cell.length() > 0  ? std::stod(cell) : 0;

        std::getline(line, cell, ',');
        m_askQty = cell.length() > 0  ? std::stod(cell) : 0;

        return 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const TopOfBook& dt);

    int64_t m_timestamp;
    double m_bidPrice;
    double m_bidQty;
    double m_askPrice;
    double m_askQty;

    void fill(int64_t timestamp, double bidPrice, double bidQuantity, double askPrice, double askQuantity);
};


class MarketDataStore
{
public:
    static const int32_t PRICE_SCALE = 5;
    static const int32_t QTY_SCALE = 2;

    MarketDataStore(const int64_t instrumentId, const char* storeRoot, size_t initialSize);

    int64_t append(TopOfBook &topOfBook);

    void foreach(std::function<void(TopOfBook&)> f);

    ScaledIntColumn& askPriceColumn()
    {
        return m_askPriceColumn;
    }

private:
    const int64_t m_instrumentId;
    const char* m_storeRoot;
    TimeStampColumn m_primaryKey;
    ScaledIntColumn m_bidPriceColumn;
    ScaledIntColumn m_bidQtyColumn;
    ScaledIntColumn m_askPriceColumn;
    ScaledIntColumn m_askQtyColumn;
};


#endif //TIWAE_MARKETDATASTORE_H
