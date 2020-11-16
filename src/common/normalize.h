//
// normalize.h 
//
#pragma once
//
#include <cstdint>
#include <limits>
#include "vectors.h" 
//
// Aleksandr Ljwowich (R) (TM) design and code programming (C), 2019.
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
void _normalize_oo(float *v, int n); // -1.0 ... 1.0
void _normalize_oo(double *v, int n); 

void _normalize_zo(float *v, int n); // 0.0 ... 1.0
void _normalize_zo(double *v, int n); 

void _normalize_pct(float *v, int n); 
void _normalize_pct(double *v, int n);
//
// -----------------------------------------------------------------------------
//
// -1.0 ... 1.0
//
template <typename _Td>
void normalize_oo(_Td *v, const uint32_t n, _Td min = std::numeric_limits<_Td>::max(), _Td max = std::numeric_limits<_Td>::lowest())
{
   uint32_t i;

   if ( n < 16 )
   {
      for ( i = 0; i < n; ++i )
      {
         if ( v[i] > max )
            max = v[i];
         if ( v[i] < min )
            min = v[i];
      }      
      if ( max != min )
      {
         const _Td m = 0.5 * (max + min);
         const _Td s = 2.0 / (max - min);
         for ( uint32_t i = 0; i < n; ++i )
            v[i] = s * (v[i] - m);          
         return;
      }
   }
   else
   {   
      max = vectors_max(v, n);
      min = vectors_min(v, n);
      if ( max != min )
      {
         vectors_nrm(v, 0.5 * (max + min), 2.0 / (max - min), n); 
         return;
      }   
   }         

   for ( i = 0; i < n; ++i )
      v[i] = 0.0;
} 
//
// -----------------------------------------------------------------------------
//
// 0.0 ... 1.0
//
template <typename _Td>
void normalize_zo(_Td *v, const uint32_t n, _Td min = std::numeric_limits<_Td>::max(), _Td max = std::numeric_limits<_Td>::lowest())
{
   uint32_t i;

   if ( n < 16 )
   {
      for ( i = 0; i < n; ++i )
      {
         if ( v[i] > max )
            max = v[i];
         if ( v[i] < min )
            min = v[i];
      }      
      if ( max != min )
      {
         const _Td s = 1.0 / (max - min);
         for ( i = 0; i < n; ++i )
            v[i] = s * (v[i] - min);          
         return;
      }
   }
   else
   {   
      max = vectors_max(v, n);
      min = vectors_min(v, n);
      if ( max != min )
      {
         vectors_nrm(v, min, 1.0 / (max - min), n); 
         return;
      }   
   }         

   for ( i = 0; i < n; ++i )
      v[i] = 0.5;
} 
//
// -----------------------------------------------------------------------------
//
template <typename _Td>
void normalize_pct(_Td *v, const uint32_t n, _Td min = std::numeric_limits<_Td>::max(), _Td max = std::numeric_limits<_Td>::lowest())
{
   uint32_t i;

   if ( n < 16 )
   {
      for ( i = 0; i < n; ++i )
      {
         if ( v[i] > max )
            max = v[i];
         if ( v[i] < min )
            min = v[i];
      }      
      if ( max != min )
      {
         const _Td m = 0.5 * (max + min);
         const _Td s = 1.0 / m;
         for ( i = 0; i < n; ++i )
            v[i] = s * (v[i] - m);          
         return; 
      }
   }
   else
   {   
      const _Td m = 0.5 * (vectors_max(v, n) + vectors_min(v, n));
      if ( m != 0.0 )
      {
         vectors_nrm(v, m, 1.0 / m, n);
         return;
      }
   }

   for ( i = 0; i < n; ++i )
      v[i] = 0.0;
} 
//
// -----------------------------------------------------------------------------
//
template <typename _Td>
void normalize_max(_Td *v,  const uint32_t n, const _Td max) 
{
   for ( int i = 1; i < n; ++i )
      v[i] = v[i] / max;
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
