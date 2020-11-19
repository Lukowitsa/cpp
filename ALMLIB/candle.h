//
// candle.h
//
#pragma once
//
#include <cstdint>
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

   int16_t code() const;   
   _Tf like(const _Tf nOpenPosition, const _Tf nClosePosition) const;
   
   _Tf m_nHigh;
   _Tf m_nLow;
   _Tf m_nOpen;
   _Tf m_nClose;
};   

template <typename _Tf>
int16_t hloc<_Tf>::code() const
{
   if ( m_nHigh == m_nLow )
      return 0b10000; // 16

   if ( m_nOpen < m_nClose ) // white
   {   
      if ( m_nClose - m_nOpen >= (m_nHigh - m_nLow) * 0.5 )
         return 0b00100; // 4     

      const _Tf a = (m_nHigh + m_nLow) * 0.5;
      if ( m_nOpen > a && m_nClose > a )   
         return 0b00110; // 6   
      if ( m_nOpen < a && m_nClose < a )   
         return 0b00101; // 5   
      else
         return 0b00111; // 7            
   }
   else if ( m_nOpen > m_nClose ) // black
   {   
      if ( m_nOpen- m_nClose >= (m_nHigh - m_nLow) * 0.5 )
         return 0b00000; // 0     

      const _Tf a = (m_nHigh + m_nLow) * 0.5;
      if ( m_nOpen > a && m_nClose > a )   
         return 0b00010; // 2   
      if ( m_nOpen < a && m_nClose < a )   
         return 0b00001; // 1   
      else
         return 0b00011; // 3            
   }
   else // if ( m_nOpen == m_nClose )
   {
      const _Tf a = (m_nHigh + m_nLow) * 0.5;
      if ( m_nOpen > a )
         return 0b01010; // 10
      if ( m_nOpen < a )
         return 0b01001; // 9
      return 0b01011; // 11
   }  
}   
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
_Tf hloc<_Tf>::like(const _Tf nOpenPosition, const _Tf nClosePosition) const
{
   const _Tf nSize = m_nHigh - m_nLow;

   if ( nSize != 0.0 )
   {
      const _Tf nOpen  = (m_nOpen  - m_nLow) / nSize;
      const _Tf nClose = (m_nClose - m_nLow) / nSize;
  
      return 100.0 * std::max  
      (
         std::abs(nOpen  - nOpenPosition)  / std::min(nOpen,  nOpenPosition),
         std::abs(nClose - nClosePosition) / std::min(nClose, nClosePosition) 
      );
   }
     
   return 100.0; 
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
_Tf distance(const hloc<_Tf> *pCandle1, const hloc<_Tf> *pCandle2)
{
   const _Tf nSize1 = pCandle1->m_nHigh - pCandle1->m_nLow;
   const _Tf nSize2 = pCandle2->m_nHigh - pCandle2->m_nLow;

   if ( nSize1 == 0.0 && nSize2 == 0.0 )
      return 100.0;

   if ( nSize1 != 0.0 && nSize2 != 0.0 )
   {   
      const _Tf nOpen1  = (pCandle1->m_nOpen  - pCandle1->m_nLow) / nSize1;
      const _Tf nClose1 = (pCandle1->m_nClose - pCandle1->m_nLow) / nSize1;
      const _Tf nOpen2  = (pCandle2->m_nOpen  - pCandle2->m_nLow) / nSize2;
      const _Tf nClose2 = (pCandle2->m_nClose - pCandle2->m_nLow) / nSize2;
  
      return 100.0 * std::max 
      (
        std::abs(nOpen1  - nOpen2)  / std::min(nOpen1,  nOpen2),
        std::abs(nClose1 - nClose2) / std::min(nClose1, nClose2) 
      );
   }
     
   return 100.0; 
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
