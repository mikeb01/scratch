//
// Created by Michael Barker on 13/08/15.
//

#include <immintrin.h>
#include <iostream>


void print(__m256 v)
{
    std::cout << '{';

    for (int i = 0; i < 7; i++)
    {
        std::cout << ((float*)&v)[i] << ',';
    }

    std::cout << ((float*)&v)[7] << "}\n";
}

void print(__m128 v)
{
    std::cout << '{';

    for (int i = 0; i < 3; i++)
    {
        std::cout << ((float*)&v)[i] << ',';
    }

    std::cout << ((float*)&v)[3] << "}\n";
}
void hSumS()
{
    __declspec(align(16)) float myVec[8];

    myVec[0] = 1.0;
    myVec[1] = 2.0;
    myVec[2] = 3.0;
    myVec[3] = 4.0;
    myVec[4] = 5.0;
    myVec[5] = 6.0;
    myVec[6] = 7.0;
    myVec[7] = 8.0;

    __m256 v = _mm256_load_ps(myVec);
    __m256 v2 = _mm256_hadd_ps(v, v);
    __m256 v3 = _mm256_hadd_ps(v2, v2);
    __m128 v5 = _mm256_extractf128_ps(v3, 1);
    _mm256_zeroupper();
    __m128 v6 = _mm_add_ps(v5, _mm256_castps256_ps128(v3));

    float f = ((float*)&v6)[0];

    std::cout << f << '\n';
//    std::cout << myVec2[0] << ' ' << myVec2[1] << ' ' << myVec2[2] <<' ' << myVec2[3] << '\n';
}


void foo()
{
    __declspec(align(16)) double myVec[4];
    __declspec(align(16)) double myVec2[4];

    // store.askPriceColumn().get(0, myVec);

    myVec[0] = 1.0;
    myVec[1] = 2.0;
    myVec[2] = 3.0;
    myVec[3] = 4.0;

    __m256d v = _mm256_load_pd(myVec);
    __m256d v2 = _mm256_hadd_pd(v, v);
    __m128d vU = _mm256_extractf128_pd(v2, 1);
    _mm256_zeroupper();
    __m128d v3 = _mm_add_pd(vU, _mm256_castpd256_pd128(v2));
    _mm_store_pd(myVec2, v3);


    std::cout << ((double*)&v3)[0] << '\n';
    std::cout << myVec2[0] << ' ' << myVec2[1] << ' ' << myVec2[2] <<' ' << myVec2[3] << '\n';
}

int main()
{
    hSumS();
    foo();
}