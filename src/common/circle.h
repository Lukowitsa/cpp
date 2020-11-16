//
// circle.h   
//
#pragma once
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
template <typename _T>
class circle
{
public:
   inline circle(int nSize): m_nSize(nSize)
   {
      m_pData = new _T[nSize];
      m_bFull = false;
      m_pLast = m_pData - 1;
   }

   inline ~circle()
   {
      if ( m_pData != NULL )
         delete[] m_pData;
   }

   inline int size()
   {
      return m_nSize;
   }

   inline int number()
   {
      if ( m_bFull )
         return m_nSize;
      return m_pLast >= m_pData ? m_pLast - m_pData + 1 : 0;
   }

   inline _T &get(int i)
   {
      if ( m_bFull )
      {
         i = i + 1;
         if ( m_pLast + i < m_pData + m_nSize )
            return m_pLast[i];
         return m_pLast[i-m_nSize];
      }
      return m_pData[i];
   }

   inline void put(_T &p)
   {
      if ( ++m_pLast < m_pData + m_nSize  )            
         *m_pLast = p;
      else
      {
         *(m_pLast = m_pData) = p;
         m_bFull = true;               
      }
   }

protected:
   _T *m_pData;
   _T *m_pLast;

   bool m_bFull;
   const int m_nSize;
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
