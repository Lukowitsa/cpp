//
// datetime.h
//
#pragma once
//
#include <cstdint>
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
// date
//
struct date // No TZ. You will get what you put!  
{
   inline uint16_t year() const
   {
      uint16_t nYear, nMonth, nDay;
      getDate(&nYear, &nMonth, &nDay);
      return nYear;
   }

   inline uint16_t month() const
   {
      uint16_t nYear, nMonth, nDay;
      getDate(&nYear, &nMonth, &nDay);
      return nMonth;
   }

   inline uint16_t day() const
   {
      uint16_t nYear, nMonth, nDay;
      getDate(&nYear, &nMonth, &nDay);
      return nDay;
   }

   inline uint16_t wday() const // 1-Monday 7-Sunday
   {
      return (m_nDate + 6) % 7;
   }

   void setDate(uint16_t nYear, uint16_t nMonth, uint16_t nDay);
   void getDate(uint16_t *pnYear, uint16_t *pnMonth, uint16_t *pnDay) const; 
   void setQuikDate(uint32_t nQuikDate); // YYYYMMDD
   uint32_t getQuikDate() const; // // YYYYMMDD

// "DD.MM.YYYY" or "DD.MM.YY" or "DD/MM/YYYY" or "DD/MM/YY"
   bool setDate(const char *pszDate);
// Buffer size 11:  DD.MM.YYYY\0 or 10: DD.MM.YYYY
   char *getDate(char szBuffer[], bool bNullTerminate = true, bool bYear2 = false) const;
   char *tostring() const;

   uint16_t m_nDate;  // days since 01.01.2000
};

inline bool operator < (date d1, date d2) 
{   
   return d1.m_nDate < d2.m_nDate;
}   
//
// -----------------------------------------------------------------------------
//
#define ALM_TIME(h, m) ((h)*60+(m))
//
struct time // No TZ. You will get what you put! 
{
   inline uint16_t hour() const
   {
      return m_nTime / 60;
   }
   inline uint16_t minute() const
   {
      return m_nTime % 60; 
   }
   inline void setTime(uint16_t nHour, uint16_t nMinute)
   {
      m_nTime = ALM_TIME(nHour, nMinute);
   }
   inline void getTime(uint16_t *pnHour, uint16_t *pnMinute) const
   {
      *pnHour   = m_nTime / 60;
      *pnMinute = m_nTime % 60; 
   }
   void setQuikTime(uint32_t nQuikTime); // HHMMSS || HHMM
   inline void setQuikTime(uint16_t nQuikTime)  // HHMM
   {
      setTime(nQuikTime / 100, nQuikTime % 100);
   }   
   inline uint16_t getQuikTime() const // HHMM
   {
      return (m_nTime / 60) * 100 + m_nTime % 60;
   }
   
// Buffer size 6:  HH:MM\0 or 5: HH:MM    
   char *getTime(char szBuffer[], bool bNullTerminate = true) const;
   char *tostring() const;

   uint16_t m_nTime; // minutes in today (starts 00:00)
};

inline bool operator < (time t1, time t2) 
{   
   return t1.m_nTime < t2.m_nTime;
}   
//
// -----------------------------------------------------------------------------
//
struct datetime: public date, public time
{
// Buffer size 17: DD.MM.YYYY HH:MM\0 or 16: DD.MM.YYYY HH:MM
   char *getDateTime(char szBuffer[], bool bNullTerminate = true, bool bYear2 = false) const; 
   char *tostring() const;
};

inline bool operator < (datetime d1, datetime d2)
{   
   return d1.m_nDate < d2.m_nDate ||
   (
      d1.m_nDate == d2.m_nDate && d1.m_nTime < d2.m_nTime
   );
}   

inline bool operator == (datetime d1, datetime d2)
{
   return d1.m_nTime == d2.m_nTime && d1.m_nDate == d2.m_nDate;
}

inline bool operator != (datetime d1, datetime d2)
{
   return d1.m_nTime != d2.m_nTime || d1.m_nDate != d2.m_nDate;
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
