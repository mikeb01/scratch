//
// Created by Michael Barker on 08/08/15.
//

#include <math.h>
#include <tiff.h>
#include <emmintrin.h>
#include <immintrin.h>
#include "ScaledIntColumn.h"

ScaledIntColumn::ScaledIntColumn(const char* storeRoot, const char* name, size_t initialSize, int32_t scale)
    : Column(storeRoot, name, initialSize), m_scale(scale), m_multiplier(pow(10, scale)), m_body(bodyPtr<int32_t>())
{
}

int64_t ScaledIntColumn::append(double value)
{
    int32_t scaledValue = (int32_t) (value * pow(10, m_scale));
    m_body[header()->m_position] = scaledValue;
    header()->m_position++;

    return header()->m_position;
}

double ScaledIntColumn::get(int i)
{
    return bodyPtr<int32_t>()[i] / m_multiplier;
}

float ScaledIntColumn::getF(int i)
{
    return bodyPtr<int32_t>()[i] / (float) m_multiplier;
}

void ScaledIntColumn::get(int i, double (&vec)[4])
{
    int32_t* vals = &bodyPtr<int32>()[i];

    __m128i ivec = _mm_load_si128((const __m128i*) vals);
    __m128 fvec = _mm_cvtepi32_ps(ivec);
    __m256d dvec = _mm256_cvtps_pd(fvec);
    __m256d scale = _mm256_broadcast_sd(&m_multiplier);
    __m256d dVals = _mm256_div_pd(dvec, scale);
    _mm256_store_pd(vec, dVals);
}

double hSum(__m256d v)
{
    __m256d v2 = _mm256_hadd_pd(v, v);
    __m128d vU = _mm256_extractf128_pd(v2, 1);
    _mm256_zeroupper();
    __m128d v3 = _mm_add_pd(vU, _mm256_castpd256_pd128(v2));

    return ((double*)&v3)[0];
}

float hSum(__m256 v)
{
    __m256 v2 = _mm256_hadd_ps(v, v);
    __m256 v3 = _mm256_hadd_ps(v2, v2);
    __m128 v4 = _mm256_extractf128_ps(v3, 1);
    _mm256_zeroupper();
    __m128 v5 = _mm_add_ps(v4, _mm256_castps256_ps128(v3));

    return ((float*)&v5)[0];
}

double ScaledIntColumn::sum()
{
    __m256d sum = _mm256_set_pd(0, 0, 0, 0);
    int64_t len = position();
    int32* pInt = bodyPtr<int32>();

    for (int i = 0; i < len; i += 4)
    {
        int32_t* vals = &pInt[i];
        __m128i ivec = _mm_load_si128((const __m128i*) vals);
        __m128 fvec = _mm_cvtepi32_ps(ivec);
        __m256d dvec = _mm256_cvtps_pd(fvec);
        __m256d scale = _mm256_broadcast_sd(&m_multiplier);
        __m256d dVals = _mm256_div_pd(dvec, scale);
        sum = _mm256_add_pd(sum, dVals);
    }

    return hSum(sum);
}

float ScaledIntColumn::sumFloat()
{
    __m256 sum = _mm256_set_ps(0, 0, 0, 0, 0, 0, 0, 0);
    int64_t len = position();
    int32* pInt = bodyPtr<int32>();
    float m = (float) m_multiplier;

    for (int i = 0; i < len; i += 8)
    {
        __m128i ivec1 = _mm_load_si128((const __m128i*) &pInt[i]);
        __m128i ivec2 = _mm_load_si128((const __m128i*) &pInt[i + 4]);
        __m128 fvec1 = _mm_cvtepi32_ps(ivec1);
        __m128 fvec2 = _mm_cvtepi32_ps(ivec2);

        __m256 vec = _mm256_castps128_ps256(fvec1);
        vec = _mm256_insertf128_ps(vec, fvec2, 1);
        __m256 scale = _mm256_broadcast_ss(&m);
        __m256 fVals = _mm256_div_ps(vec, scale);
        sum = _mm256_add_ps(sum, fVals);
    }

    return hSum(sum);
}
