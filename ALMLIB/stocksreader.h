//
// stocksreader.h
//
#pragma once
//
#include <fstream>
#include <cstdint>
#include "basecandle.h"
//
// -----------------------------------------------------------------------------
//
#ifndef ____ALM_NAMESPACE____
   #define ____ALM_NAMESPACE____
   namespace alm {
   using namespace std;
#endif          
//
// -----------------------------------------------------------------------------
//
// stocks_reader
//
class StocksReader: public ifstream  
{
public:
   inline StocksReader()
   {
   }
   inline StocksReader(const char *pszFileName, uint32_t nBegin = 0)
   {
      open(pszFileName, nBegin);   
   }
   inline ~StocksReader()
   {
      if ( good() )
         close();
   }
   void open(const char *pszFileName, uint32_t nBegin = 0);
   template <typename _Tf, typename _Tv>
   bool read(base_candle<_Tf, _Tv> &record, uint32_t nEnd = 0);
   int8_t interval(); 
};       
//
// -----------------------------------------------------------------------------
//
template <typename _Tf, typename _Tv>
bool StocksReader::read(base_candle<_Tf, _Tv> &record, uint32_t nEnd)
{
   uint32_t nQuikDate;
   uint32_t nQuikTime;

   *this >> nQuikDate; ignore(4096, ';');
   if ( nQuikDate > nEnd )
      return false;
   record.setQuikDate(nQuikDate);

   *this >> nQuikTime; ignore(4096, ';');
   record.setQuikTime(nQuikTime);

   *this >> record.m_nHigh;  ignore(4096, ';'); 
   *this >> record.m_nLow;   ignore(4096, ';');
   *this >> record.m_nOpen;  ignore(4096, ';');
   *this >> record.m_nClose; ignore(4096, ';');
   
   auto p = tellg();
   *this >> record.m_nVolume;
   return p != tellg();
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
