//
// candle.h
//
#pragma once
//
#include <cstdint>
#include <cmath>
#include "datetime.h"
//
// Aleksandr Ljwowich (R) (TM) design and code programming (C), 2016.
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
struct hloc
{
   inline _Tf a() const
   {
      return 0.5 * (m_nHigh + m_nLow);
   }

   inline _Tf u() const
   {
      return std::max(m_nOpen, m_nClose);
   }     

   inline _Tf d() const
   {
      return std::min(m_nOpen, m_nClose);
   }     

   inline _Tf body() const
   {
      if ( m_nHigh != m_nLow )
         return (m_nClose - m_nOpen) / (m_nHigh - m_nLow);   
      return 0.0;
   }

   inline _Tf ps() const
   {
      return std::min(a(), m_nClose);   
   }

   inline _Tf pb() const
   {
      return std::max(a(), m_nClose);   
   }

   bool like(uint16_t iOpen, uint16_t iClose, const uint16_t n = 5) const;
   bool like(const hloc<_Tf> &c, const uint16_t n = 5) const;
         
   _Tf m_nHigh;
   _Tf m_nLow;
   _Tf m_nOpen;
   _Tf m_nClose;
};   

//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
bool hloc<_Tf>::like(uint16_t iOpen, uint16_t iClose, const uint16_t n) const
{
   const _Tf nSize = m_nHigh - m_nLow;
   return
   (
      iOpen  == ((m_nOpen  == m_nHigh) ?  n-1 : std::floor(n * (m_nOpen  - m_nLow) / nSize)) &&    
      iClose == ((m_nClose == m_nLow)  ?  n-1 : std::floor(n * (m_nClose - m_nLow) / nSize))
   );
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
bool hloc<_Tf>::like(const hloc<_Tf> &c, const uint16_t n) const
{
   const _Tf nSize1 = m_nHigh - m_nLow;
   const _Tf nSize2 = c.m_nHigh - c.m_nLow;

   return
   (
      ((m_nOpen    == m_nHigh)   ?  n-1 : std::floor(n * (m_nOpen    - m_nLow)   / nSize1)) ==
      ((c.m_nOpen  == c.m_nHigh) ?  n-1 : std::floor(n * (c.m_nOpen  - c.m_nLow) / nSize2)) &&      
      ((m_nClose   == m_nHigh)   ?  n-1 : std::floor(n * (m_nClose   - m_nLow)   / nSize1)) ==
      ((c.m_nClose == c.m_nHigh) ?  n-1 : std::floor(n * (c.m_nClose - c.m_nLow) / nSize2))          
   );
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf, typename _Tv>
struct hlocv: public hloc<_Tf>
{
   _Tv m_nVolume;
};   
//
// -----------------------------------------------------------------------------
//
template <typename _Tf, typename _Tv>
struct candle: public hlocv<_Tf, _Tv>, public datetime
{
};   
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
