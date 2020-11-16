//
// datetime.cpp
//
#include "datetime.h" 
#include <stdlib.h>
#include <string.h>
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
static const uint16_t p_days[2][13] =
{
   {
      0, 
      31, 
      31 + 29,
      31 + 29 + 31,
      31 + 29 + 31 + 30,
      31 + 29 + 31 + 30 + 31,
      31 + 29 + 31 + 30 + 31 + 30,
      31 + 29 + 31 + 30 + 31 + 30 + 31 ,
      31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,
      31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
      31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
      31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
      31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31
   },   
   {
      0, 
      31, 
      31 + 28,
      31 + 28 + 31,
      31 + 28 + 31 + 30,
      31 + 28 + 31 + 30 + 31,
      31 + 28 + 31 + 30 + 31 + 30,
      31 + 28 + 31 + 30 + 31 + 30 + 31 ,
      31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
      31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
      31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
      31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
      31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31
   }   
};

static const uint32_t p_years[] =
{
  0,
  366,
  366 + 365,
  366 + 365 + 365
};

static const char szDateDelimiters[] = "./";
static char p_szBuffer[17];
//
// -----------------------------------------------------------------------------
//
void date::setDate(uint16_t nYear, uint16_t nMonth, uint16_t nDay)
{
   if ( nMonth > 12 ) // Very robust joke!
   {
      nYear += nMonth / 12;
      nMonth = (nMonth-1) % 12 + 1;
   }
  
   m_nDate  = nDay - 1;
   nYear   -= 2000;
   m_nDate += (nYear / 4) * (365 * 3 + 366); 
   nYear   %= 4;
   m_nDate += p_days[nYear == 0 ? 0 : 1][nMonth-1] + p_years[nYear];
}
//
// -----------------------------------------------------------------------------
//
void date::getDate(uint16_t *pnYear, uint16_t *pnMonth, uint16_t *pnDay) const
{
   uint16_t nDay  = m_nDate;
   uint16_t nDiv  = nDay / (365 * 3 + 366);

   const uint16_t *p_upper_day;
   if ( (nDay -= nDiv * (365 * 3 + 366)) < 366 )
   {
      p_upper_day = p_days[0];
     *pnYear = nDiv * 4 + 2000;   
   }
   else 
   {
      p_upper_day = p_days[1];
      if ( nDay < 366 + 365 )
      {
         nDay   -= 366;
         *pnYear = nDiv * 4 + 2001;   
      }
      else if ( nDay < 366 + 365 + 365 )
      {
         nDay   -= 366 + 365;
         *pnYear = nDiv * 4 + 2002;   
      }
      else if ( nDay < 366 + 365 + 365 + 365 )
      {
         nDay   -= 366 + 365 + 365;
         *pnYear = nDiv * 4 + 2003;   
      }
   }  

   for ( uint16_t m = 1; m <= 12; ++m )
   {
      if ( nDay < p_upper_day[m] )
      {
         *pnMonth = m;  
         *pnDay   = 1 + nDay - p_upper_day[m - 1]; 
         break; 
      }
   }
}
//
// -----------------------------------------------------------------------------
//
void date::setQuikDate(uint32_t nQuikDate) // YYYYMMDD
{
   uint16_t nMMDD = nQuikDate % 10000;
   setDate(nQuikDate / 10000, nMMDD / 100, nMMDD % 100); 
}   
//
// -----------------------------------------------------------------------------
//
uint32_t date::getQuikDate() const
{
   uint16_t nYear;
   uint16_t nMonth;
   uint16_t nDay;   

   getDate(&nYear, &nMonth, &nDay);
   return nYear * 10000 + nMonth * 100 + nDay; 
}
//
// -----------------------------------------------------------------------------
//
bool date::setDate(const char *pszDate)
{
   char szDate[50];
   strncpy(szDate, pszDate, sizeof(szDate)/sizeof(szDate[0])-1);  

   uint16_t nYear;
   uint16_t nMonth;
   uint16_t nDay;   

   const char *p;
   if ( (p = strtok(szDate, szDateDelimiters)) != NULL )
   {
      nDay = atoi(p);
      if ( nDay >= 1 && nDay <= 31 && (p = strtok(NULL, szDateDelimiters)) != NULL )
      {
         nMonth = atoi(p);
         if ( nMonth >= 1 && nMonth <= 12 && (p = strtok(NULL, szDateDelimiters)) != NULL )
         {
            nYear = atoi(p);
            if ( nYear <= 99U )
               nYear += 2000U;
            setDate(nYear, nMonth, nDay);
            return true;
         }
      }         
   }
   return false;
}
//
// -----------------------------------------------------------------------------
//
char *date::getDate(char szBuffer[], bool bNullTerminate, bool bYear2) const
{
   uint16_t nYear;
   uint16_t nMonth;   
   uint16_t nDay;

   getDate(&nYear, &nMonth, &nDay);

   char *pszBuffer = szBuffer;

   *szBuffer++ = (char) (nDay / 10 + '0');
   *szBuffer++ = (char) (nDay % 10 + '0');
   *szBuffer++ = '.';

   *szBuffer++ = (char) (nMonth / 10 + '0');
   *szBuffer++ = (char) (nMonth % 10 + '0');
   *szBuffer++ = '.';

   if ( !bYear2 )
   {   
      *szBuffer++ = (char) (nYear / 1000 + '0'); nYear %= 1000;
      *szBuffer++ = (char) (nYear / 100  + '0'); 
   }
   nYear %= 100;
   *szBuffer++ = (char) (nYear / 10 + '0');
   *szBuffer++ = (char) (nYear % 10 + '0');

   if ( bNullTerminate ) 
      *szBuffer = '\0';

   return pszBuffer;
}   
//
// -----------------------------------------------------------------------------
//
char *date::tostring() const
{
   return getDate(p_szBuffer, true);
}      
//
// -----------------------------------------------------------------------------
//
// time 
//
void time::setQuikTime(uint32_t nQuikTime) // HHMM || HHMMSS
{
   if ( nQuikTime < 10000 ) // HHMM
      setTime(nQuikTime / 100, nQuikTime % 100);
   else // HHMMSS
      setQuikTime(nQuikTime / 100);
}   
//
// -----------------------------------------------------------------------------
//
char *time::getTime(char szBuffer[], bool bNullTerminate) const
{
   char *pszBuffer = szBuffer;

   unsigned char ch;
   ch = m_nTime / 60;
   *szBuffer++ = (char) (ch / 10 + '0');
   *szBuffer++ = (char) (ch % 10 + '0');
   *szBuffer++ = ':';

   ch = m_nTime % 60;
   *szBuffer++ = (char) (ch / 10 + '0');
   *szBuffer++ = (char) (ch % 10 + '0');

   if ( bNullTerminate ) 
      *szBuffer = '\0';

   return pszBuffer;
}   
//
// -----------------------------------------------------------------------------
//
char *time::tostring() const
{
   return getTime(p_szBuffer, true);
}      
//
// -----------------------------------------------------------------------------
//
// datetime
//
char *datetime::getDateTime(char szBuffer[], bool bNullTerminate, bool bYear2) const
{
   getDate(szBuffer, false, bYear2); 
   auto szTime = szBuffer + (bYear2 ? 8 : 10);  
   *szTime++ = ' ';
   getTime(szTime, bNullTerminate);
   return szBuffer;
}   
//
// -----------------------------------------------------------------------------
//
char *datetime::tostring() const
{
   return getDateTime(p_szBuffer, true);
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
