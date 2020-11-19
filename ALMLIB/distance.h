//
// distance.h
//
#pragma once
//
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <set>
#include <limits>
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
template <typename _Tf>
_Tf distance(const _Tf *x1, const _Tf *x2, uint32_t n)
{
   _Tf nDistance = 0.0;

   for ( uint32_t k = 0; k < n; ++k )
      nDistance += (x1[k] - x2[k]) * (x1[k] - x2[k]);   

   return std::sqrt(nDistance) / n;
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
_Tf max_distance(const _Tf *x1, const _Tf *x2, uint32_t n)
{
   _Tf nDistance = 0.0;

   for ( uint32_t k = 0; k < n; ++k )
   {
      const _Tf d = std::abs(x1[k] - x2[k]);   
      if ( d > nDistance )
         nDistance = d; 
   }

   return nDistance;
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
_Tf max_distance(_Tf *x1, _Tf *y1, int16_t *t1, uint32_t n1, _Tf *x2, _Tf *y2, int16_t *t2, uint32_t n2, uint32_t nOmit = 0U)
{
   const uint32_t n = n1 * n2; 

   _Tf  *distance = new _Tf [n];
   _Tf **index    = new _Tf*[n];

   if ( n1 > n2 )
   {
      std::swap(n1, n2);
      std::swap(x1, x2);   
      std::swap(y1, y2);
   }
   
   uint32_t k = 0;
   for ( uint32_t i = 0; i < n1; ++i )
      for ( uint32_t j = 0; j < n2; ++j, ++k )    
      {
         index[k] = distance + k;
         if ( t1 != NULL && t2 != NULL && t1[i] == t2[j] ) 
            distance[k] = std::hypot(x1[i] - x2[j], y1[i] - y2[j]);
         else  
            distance[k] = std::numeric_limits<_Tf>::max();  
      }    
      
   std::sort
   (
      index, 
      index + n, 
      [] (const _Tf *p1, const  _Tf *p2) 
      {
         return (*p1 < *p2);
      }
   );

   _Tf nDistance = 0.0;
   std::set<uint32_t> s1;
   std::set<uint32_t> s2;

   for ( k = 0; k < n; ++k )
   {
      uint32_t i = index[k] - distance;      
      uint32_t j = i % n1;
      i /= n1;
      
      if ( s1.find(i) == s1.end() && s2.find(j) == s2.end() )
      { 
         s1.insert(i);
         s2.insert(j);

         if ( *index[k] > nDistance )
            nDistance = *index[k]; 

         if ( s1.size() == n1 - nOmit )
            break;
      }   
   }

   delete [] distance;
   delete [] index;

   return nDistance;   
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
