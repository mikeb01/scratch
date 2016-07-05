#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <sys/stat.h>
#include <immintrin.h>
#include "MarketDataStore.h"
#include "Exceptions.h"
#include "timing.h"

std::ostream& operator<<(std::ostream& out, const TopOfBook& f)
{
    return
        out << "timestamp:" << f.m_timestamp
        << ",bidPrice:" << f.m_bidPrice <<  ",bidQty:" << f.m_bidQty
        << ",askPrice:" << f.m_askPrice << ",askQty:" << f.m_askQty;
}

void printTime(const char* name, timespec time, int64_t len)
{
    double time_taken = time.tv_sec + time.tv_nsec / 1000000000.0;
    double ops_sec = time_taken != 0 ? (len) / time_taken : NAN;
    printf("%10s ops/sec: %'.2f\n", name, ops_sec);
}

int main()
{
    std::ifstream file{
        "/Users/mikeb01/sources/java/java-api/LmaxApiSample/marketdata.csv.gz",
        std::ios_base::in | std::ios_base::binary
    };

    boost::iostreams::filtering_istream in;
    in.push(boost::iostreams::gzip_decompressor());
    in.push(file);

    std::string header;
    std::getline(in, header);

    const char* root = "data";
    if (mkdir(root, 0775) < 0)
    {
        if (EEXIST != errno)
        {
            std::cerr << "mkdir[" << root << "] failed - " << strerror(errno) << '\n';
            return -1;
        }
    }

    const char* storeRoot = "data/4001";
    if (mkdir(storeRoot, 0775) < 0)
    {
        if (EEXIST != errno)
        {
            std::cerr << "mkdir[" << storeRoot << "] failed - " << strerror(errno) << '\n';
            return -1;
        }
    }

    try
    {
        MarketDataStore store{4001, storeRoot, 1 << 24};

        int i = 0;
        TopOfBook topOfBook;

        for (std::string str; std::getline(in, str);)
        {
            std::istringstream line{str};
            topOfBook.parse(line, MarketDataStore::PRICE_SCALE, MarketDataStore::QTY_SCALE);

            store.append(topOfBook);
        }

        __declspec(align(16)) double myVec[4];

        struct timespec t0;
        struct timespec t1;
        struct timespec t2;
        struct timespec t3;
        struct timespec t4;
        struct timespec t5;

        int64_t len = store.askPriceColumn().position();

        hdr_gettime(&t0);

        double total = 0;
        for (int i = 0; i < len; i++)
        {
            total += store.askPriceColumn().get(i);
        }

        hdr_gettime(&t1);

        double total2 = 0;
        for (int i = 0; i < len; i += 4)
        {
            store.askPriceColumn().get(i, myVec);

            __m256d v = _mm256_load_pd(myVec);
            __m256d v2 = _mm256_hadd_pd(v, v);
            __m128d vU = _mm256_extractf128_pd(v2, 1);
            _mm256_zeroupper();
            __m128d v3 = _mm_add_pd(vU, _mm256_castpd256_pd128(v2));
            total2 += ((double*)&v3)[0];
//            total += (myVec[0] + myVec[1] + myVec[2] + myVec[3]);
        }

        hdr_gettime(&t2);

        double total3 = store.askPriceColumn().sum();

        hdr_gettime(&t3);

        double total4 = store.askPriceColumn().sumFloat();

        hdr_gettime(&t4);

        double total5 = 0;
        for (int j = 0; j < len; j++)
        {
            total5 += store.askPriceColumn().getF(j);
        }

        hdr_gettime(&t5);

        std::cout << total << ' ' << total2 << ' ' << total3 << ' ' << total4 << ' ' << total5 << '\n';

        printTime("linear", diff(t0, t1), len);
        printTime("vector", diff(t1, t2), len);
        printTime("sum", diff(t2, t3), len);
        printTime("sumFloat", diff(t3, t4), len);
        printTime("linearF", diff(t4, t5), len);

//        double time_taken1 = taken1.tv_sec + taken1.tv_nsec / 1000000000.0;
//        double time_taken2 = taken2.tv_sec + taken2.tv_nsec / 1000000000.0;
//        double ops_sec1 = (len) / time_taken1;
//        double ops_sec2 = time_taken2 != 0 ? (len) / time_taken2 : NAN;
//
//        printf("linear ops/sec: %'.2f\n", ops_sec1);
//        printf("vector ops/sec: %'.2f\n", ops_sec2);

    }
    catch (IOException e)
    {
        std::cout << e.what() << " at: " << e.where() << '\n';
    }


    return 0;
}