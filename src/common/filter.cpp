//
// filter.cpp
//
#include "filter.h"
#include <algorithm>
#include <math.h>
//
//
// -----------------------------------------------------------------------------
//
double firf(const double *pV, const double *pH, int nLength)
{
   const double *pS = pV - nLength;

   double nResult = 0.0;
   while ( pV > ++pS )
      nResult += (*pS + *pV--) * *pH++;
   if ( nLength % 2 ) 
      nResult += (*pS + *pV) * *pH;

   return nResult; 
} 
//
// -----------------------------------------------------------------------------
//
double fird(const double *pV, const double *pH, int nLength)
{
   const double *pS = pV - nLength;

   double nResult = 0.0;
   while ( pV > ++pS )
      nResult += (*pS - *pV--) * *pH++;

   return nResult; 
} 
//
// -----------------------------------------------------------------------------
//
AlmAMA::AlmAMA(const int nPeriod, const int nFast, const int nSlow) 
:  m_nPeriod(nPeriod),
   m_nS(2.0 / ( nSlow + 1)),
   m_nR(2.0 / ( nFast + 1) - m_nS)
{
   m_nResult = NAN;  
}

void AlmAMA::reset()
{
   m_nResult = NAN;
}

double AlmAMA::process(const double nV[]) 
{
   const double *pV;
   double nSignal;
   double nSC;
    
   if ( std::isnan(m_nResult) )
   {
      m_nNoise = 0.0;
      for ( pV = nV - m_nPeriod; pV < nV; ++pV )
         m_nNoise += fabs(pV[1] - pV[0]);

      m_nResult = nV[-1];
   }
   else
      m_nNoise += fabs(nV[0] - nV[-1]) - 
                  fabs(nV[0-m_nPeriod] - nV[0-m_nPeriod-1]);
                         
   if ( m_nNoise != 0 )
      nSC = fabs(nV[0] - nV[0-m_nPeriod]) / m_nNoise * m_nR + m_nS;
   else
      nSC = m_nS;    

   m_nResult += nSC*nSC * (nV[0] - m_nResult);      
   
   return m_nResult;
}
//
// -----------------------------------------------------------------------------
//
AlmParabolic::AlmParabolic(const double nStep, const double nMaxStep)
:  m_nStep(nStep),
   m_nMaxStep(nMaxStep)
{
   m_nResult = NAN;
   m_nAF = 0.0;
}

void AlmParabolic::reset(const double nPrice)
{
   m_nResult = nPrice;
   m_nEP = nPrice;
   m_nAF = 0.0;
}

double AlmParabolic::process(const int nPosition, const double nPrice)
{
   if
   ( 
      ( (nPosition > 0) && (nPrice > m_nEP) ) || 
      ( (nPosition < 0) && (nPrice < m_nEP) )
   )   
   {
      m_nEP  = nPrice;
      m_nAF += m_nStep;
      if ( m_nAF > m_nMaxStep )
         m_nAF = m_nMaxStep;
   }      

   if ( m_nAF == 0 )
      m_nAF = m_nStep;

   return m_nResult += m_nAF * (m_nEP - m_nResult);
}
//
// -----------------------------------------------------------------------------
//
