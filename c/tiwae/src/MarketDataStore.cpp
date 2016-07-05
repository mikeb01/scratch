//
// Created by Michael Barker on 08/08/15.
//

#include "MarketDataStore.h"

MarketDataStore::MarketDataStore(const int64_t instrumentId, const char* storeRoot, size_t initialSize) :
    m_instrumentId(instrumentId),
    m_storeRoot(storeRoot),
    m_primaryKey(storeRoot, "timestamp", initialSize),
    m_bidPriceColumn(storeRoot, "bidPrice", initialSize, PRICE_SCALE),
    m_bidQtyColumn(storeRoot, "bidQty", initialSize, QTY_SCALE),
    m_askPriceColumn(storeRoot, "askPrice", initialSize, PRICE_SCALE),
    m_askQtyColumn(storeRoot, "askQty", initialSize, QTY_SCALE)
{
}

int64_t MarketDataStore::append(TopOfBook &topOfBook)
{
    m_primaryKey.append(topOfBook.m_timestamp);
    m_bidPriceColumn.append(topOfBook.m_bidPrice);
    m_bidQtyColumn.append(topOfBook.m_bidQty);
    m_askPriceColumn.append(topOfBook.m_askPrice);
    m_askQtyColumn.append(topOfBook.m_askQty);

    return 0;
}

void MarketDataStore::foreach(std::function<void(TopOfBook &)> f)
{
    TopOfBook topOfBook{};
    for (int i = 0; i < m_primaryKey.position(); i++)
    {
        topOfBook.fill(
            m_primaryKey.get(i),
            m_bidPriceColumn.get(i),
            m_bidQtyColumn.get(i),
            m_askPriceColumn.get(i),
            m_askQtyColumn.get(i));

        f(topOfBook);
    }
}

void TopOfBook::fill(int64_t timestamp, double bidPrice, double bidQuantity, double askPrice, double askQuantity)
{
    this->m_timestamp = timestamp;
    this->m_bidPrice = bidPrice;
    this->m_bidQty = bidQuantity;
    this->m_askPrice = askPrice;
    this->m_askQty = askQuantity;
}
