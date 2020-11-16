//
// normalize.cpp
//
#include "normalize.h"
#include "vectors.h" 
#include "memory.h"
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
void _normalize_oo(float *v,  int n)
{
   float max = vectors_max(v, n);
   float min = vectors_min(v, n);
   if ( max != min )
      vectors_nrm(v, 0.5 * (max + min), 2.0 / (max - min), n); // -1.0 ... 1.0
   else
      memset(v, 0, n * sizeof(float));
}
//
// -----------------------------------------------------------------------------
//
void _normalize_oo(double *v,  int n)
{
   double max = vectors_max(v, n);
   double min = vectors_min(v, n);
   if ( max != min )
      vectors_nrm(v, 0.5 * (max + min), 2.0 / (max - min), n); // -1.0 ... 1.0
   else
      memset(v, 0, n * sizeof(double));
}
//
// -----------------------------------------------------------------------------
//
void _normalize_zo(float *v,  int n)
{
   float max = vectors_max(v, n);
   float min = vectors_min(v, n);
   if ( max != min )
      vectors_nrm(v, min, 1.0 / (max - min), n); //  0.0 ... 1.0
   else
      memset(v, 0, n * sizeof(float));
}
//
// -----------------------------------------------------------------------------
//
void _normalize_zo(double *v,  int n)
{
   double max = vectors_max(v, n);
   double min = vectors_min(v, n);
   if ( max != min )
      vectors_nrm(v, min, 1.0 / (max - min), n); //  0.0 ... 1.0
   else
      memset(v, 0, n * sizeof(double));
}
//
// -----------------------------------------------------------------------------
//
void _normalize_pct(float *v,  int n)
{
   float mean = 0.5 * (vectors_max(v, n) + vectors_min(v, n));
   if ( mean != 0.0 )
      vectors_nrm(v, mean, 1.0 / mean, n);
   else
      memset(v, 0, n * sizeof(float));
}
//
// -----------------------------------------------------------------------------
//
void _normalize_pct(double *v,  int n)
{
   float mean = 0.5 * (vectors_max(v, n) + vectors_min(v, n));
   if ( mean != 0.0 )
      vectors_nrm(v, mean, 1.0 / mean, n);
   else
      memset(v, 0, n * sizeof(float));
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
