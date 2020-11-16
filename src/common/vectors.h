//
// vectors.h
//
#pragma once
//
#include <immintrin.h>
//
// Aleksandr Ljwowich (R) (TM) design and code programming (C), 2018.
//
// -----------------------------------------------------------------------------
//
#ifndef ____ALM_NAMESPACE____
   #define ____ALM_NAMESPACE____
   namespace alm {
#endif          
//
// -----------------------------------------------------------------------------
//
extern "C" __m256i _vectors_nMask[8];
extern "C" __m256i _vectors_nMaskd[4]; 

extern "C" __m256 _vectors_nMax[8];
extern "C" __m256 _vectors_nMin[8];

extern "C" __m256d _vectors_nMaxd[4];
extern "C" __m256d _vectors_nMind[4]; 
//
// -----------------------------------------------------------------------------
//
inline __attribute__((always_inline)) float vectors_dp(const float *x, const float *y, int n)
{
   float f[4];
   
   __m256 s, m, v;
   s = _mm256_xor_ps(s, s);
   for ( int i = 0; i < n / 8; ++i ) 
   {
      m = _mm256_loadu_ps(x);
      m = _mm256_mul_ps(m, *(__m256*)y);
      s = _mm256_add_ps(s, m);
      x += 8;
      y += 8;
   }
   n %= 8;

   v = _mm256_maskload_ps(x, _vectors_nMask[n]);
   m = _mm256_maskload_ps(y, _vectors_nMask[n]); 
   m = _mm256_mul_ps(m, v);
   s = _mm256_add_ps(s, m);

   s = _mm256_hadd_ps(s, s);
   s = _mm256_shuffle_ps(s, s, _MM_SHUFFLE(0, 0, 1, 0));
   s = _mm256_hadd_ps(s, s);
   _mm_storeu_ps
   (
      f,
      _mm_add_ss(_mm256_extractf128_ps(s, 0), _mm256_extractf128_ps(s, 1))
   );   
   return f[0];
}
//
// -----------------------------------------------------------------------------
//
inline __attribute__((always_inline)) double vectors_dp(const double *x, const double *y, int n)
{
   double f[2];
   
   __m256d s, m, v;
   s = _mm256_xor_pd(s, s);
   for ( int i = 0; i < n / 4; ++i ) 
   {
      m = _mm256_loadu_pd(x);
      m = _mm256_mul_pd(m, *(__m256d*)y);
      s = _mm256_add_pd(s, m);
      x += 4;
      y += 4;
   }
   n %= 4;

   v = _mm256_maskload_pd(x, _vectors_nMaskd[n]);
   m = _mm256_maskload_pd(y, _vectors_nMaskd[n]); 
   m = _mm256_mul_pd(m, v);
   s = _mm256_add_pd(s, m);

   s = _mm256_hadd_pd(s, s);
   _mm_storeu_pd
   (
      f, 
      _mm_add_sd(_mm256_extractf128_pd(s, 0), _mm256_extractf128_pd(s, 1))
   );
   return f[0];
}
//
// -----------------------------------------------------------------------------
//
inline __attribute__((always_inline)) float vectors_max(const float *x, int n)
{
   float v[4];

   __m256 m;
   m = _mm256_loadu_ps(&_vectors_nMin[0][0]);
   for ( int i = 0; i < n / 8; ++i ) 
   {
      m = _mm256_max_ps(m, *(__m256*)x);
      x += 8;
   }
   n %= 8;
   
   m = _mm256_max_ps
   (
      m, 
      _mm256_add_ps
      (
         _mm256_maskload_ps(x, _vectors_nMask[n]),
         _vectors_nMin[n]
      )   
   );

   m = _mm256_max_ps(m, _mm256_shuffle_ps(m, m, _MM_SHUFFLE(0, 0, 3, 2)));
   m = _mm256_max_ps(m, _mm256_movehdup_ps(m));
   _mm_storeu_ps
   (
     v, 
     _mm_max_ss(_mm256_extractf128_ps(m, 0), _mm256_extractf128_ps(m, 1))
   );   
   return v[0];
}
//
// -----------------------------------------------------------------------------
//
inline __attribute__((always_inline)) double vectors_max(const double *x, int n)
{
   double v[2];
   
   __m256d m;
   m = _mm256_loadu_pd(&_vectors_nMind[0][0]);
   for ( int i = 0; i < n / 4; ++i ) 
   {
      m = _mm256_max_pd(m, *(__m256d*)x);
      x += 4;
   }
   n %= 4;
   
   m = _mm256_max_pd
   (
      m, 
      _mm256_add_pd
      (
         _mm256_maskload_pd(x, _vectors_nMaskd[n]),
         _vectors_nMind[n]
      )   
   );

   m = _mm256_max_pd(m, _mm256_shuffle_pd(m, m, _MM_SHUFFLE(0, 0, 1, 1)));
   _mm_storeu_pd
   (
      v,
      _mm_max_pd(_mm256_extractf128_pd(m, 0), _mm256_extractf128_pd(m, 1))
   );
   return v[0];   
}
//
// -----------------------------------------------------------------------------
//
inline __attribute__((always_inline)) float vectors_min(const float *x, int n)
{
   float v[4];
   
   __m256 m;
   m = _mm256_loadu_ps(&_vectors_nMax[0][0]);
   for ( int i = 0; i < n / 8; ++i ) 
   {
      m = _mm256_min_ps(m, *(__m256*)x);
      x += 8;
   }
   n %= 8;
   
   m = _mm256_min_ps
   (
      m, 
      _mm256_add_ps
      (
         _mm256_maskload_ps(x, _vectors_nMask[n]),
         _vectors_nMax[n]
      )   
   );

   m = _mm256_min_ps(m, _mm256_shuffle_ps(m, m, _MM_SHUFFLE(0, 0, 3, 2)));
   m = _mm256_min_ps(m, _mm256_movehdup_ps(m));

   _mm_storeu_ps
   (
      v, 
      _mm_min_ss(_mm256_extractf128_ps(m, 0), _mm256_extractf128_ps(m, 1))
   );
   return v[0];   
}
//
// -----------------------------------------------------------------------------
//
inline __attribute__((always_inline)) double vectors_min(const double *x, int n)
{
   double v[2];
   
   __m256d m;
   m = _mm256_loadu_pd(&_vectors_nMaxd[0][0]);
   for ( int i = 0; i < n / 4; ++i ) 
   {
      m = _mm256_min_pd(m, *(__m256d*)x);
      x += 4;
   }
   n %= 4;
   
   m = _mm256_min_pd
   (
      m, 
      _mm256_add_pd
      (
         _mm256_maskload_pd(x, _vectors_nMaskd[n]),
         _vectors_nMaxd[n]
      )   
   );

   m = _mm256_min_pd(m, _mm256_shuffle_pd(m, m, _MM_SHUFFLE(0, 0, 1, 1)));
   _mm_storeu_pd
   (
      v,
      _mm_min_pd(_mm256_extractf128_pd(m, 0), _mm256_extractf128_pd(m, 1))
   );   
   return v[0];
}
//
// -----------------------------------------------------------------------------
//
// X[i] = (X[i] - mean) * sigma (i = 0, n)
//
inline __attribute__((always_inline)) void vectors_nrm(float *x, float mean, float sigma, int n)
{
   __m256 m, s, v;
   m = _mm256_set1_ps(-mean);
   s = _mm256_set1_ps(sigma);
   for ( int i = 0; i < n / 8; ++i ) 
   {
      v = _mm256_add_ps(m, *(__m256*)x);
      v = _mm256_mul_ps(s, v);
      _mm256_storeu_ps(x, v);
      x += 8;
   }
   n %= 8;
   
   v = _mm256_maskload_ps(x, _vectors_nMask[n]),
   v = _mm256_add_ps(m, v);
   v = _mm256_mul_ps(s, v);
   _mm256_maskstore_ps(x, _vectors_nMask[n], v);
}
//
// -----------------------------------------------------------------------------
//
// X[i] = (X[i] - mean) * sigma (i = 0, n)
//
inline __attribute__((always_inline)) void vectors_nrm(double *x, double mean, double sigma, int n)
{
   __m256d m, s, v;
   m = _mm256_set1_pd(-mean);
   s = _mm256_set1_pd(sigma);
   for ( int i = 0; i < n / 4; ++i ) 
   {
      v = _mm256_add_pd(m, *(__m256d*)x);
      v = _mm256_mul_pd(s, v);
      _mm256_storeu_pd(x, v);
      x += 4;
   }
   n %= 4;
   
   v = _mm256_maskload_pd(x, _vectors_nMaskd[n]),
   v = _mm256_add_pd(m, v);
   v = _mm256_mul_pd(s, v);
   _mm256_maskstore_pd(x, _vectors_nMaskd[n], v);
}
//
// -----------------------------------------------------------------------------
//
#ifdef ____ALM_NAMESPACE____
};
#undef ____ALM_NAMESPACE____
#endif          
//
// -----------------------------------------------------------------------------
//
