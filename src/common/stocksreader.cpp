//
// stocksreader.cpp
//
#include "stocksreader.h" 
#include <limits>
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
void StocksReader::open(const char *pszFileName, uint32_t nBegin)
{
   uint32_t nDate;
   pos_type nPos;
    
   ifstream::open(pszFileName, in);
   precision(10);
   
   do
   {
      nPos = tellg();
      *this >> nDate;
      ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   } while ( !eof() && nDate < nBegin );

   seekg(nPos);
   return;
}
//
// -----------------------------------------------------------------------------
//
int8_t StocksReader::interval()
{
   uint32_t nDate0, nTime0;
   uint32_t nDate1, nTime1;
   int8_t   nInterval = -1;

   pos_type pos = tellg();
   seekg(0, beg);

   for ( int nCount = 0; !eof() && nCount < 5; ++nCount )
   {
      *this >> nDate0;    
      ignore(std::numeric_limits<std::streamsize>::max(), ';');
      *this >> nTime0;
      ignore(std::numeric_limits<std::streamsize>::max(), '\n');
 
      if ( nTime0 > 9999 )
         nTime0 /= 100;
      nTime0 = (nTime0 / 100) * 60 + nTime0 % 100; 

      if ( nCount > 0 )
      {
         if ( nDate0 != nDate1 )
         {   
            if ( nTime0 == 0 && nTime1 == 0 ) 
            { 
               nInterval = 100;
               break;
            }   
         }      
         else
         {
            auto interval = nTime0 - nTime1;
            if ( interval < nInterval ) 
               nInterval = interval;
         }      
      }

      nDate1 = nDate0;
      nTime1 = nTime0;
   }      
   seekg(pos);
   return nInterval;
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
