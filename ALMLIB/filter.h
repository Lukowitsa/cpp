//
// filter.h
//
#pragma once
//
#include <algorithm>
//
// -----------------------------------------------------------------------------
//
double firf(const double *pV, const double *pH, int nLength);
double fird(const double *pV, const double *pH, int nLength);
//
// -----------------------------------------------------------------------------
//
// Usage:
//
//    if ( nBar > nPeriod )
//       nMA = object.process(nV);
//
// nBar starts from 0. 
//
class AlmAMA
{
public:
   AlmAMA(const int nPeriod, const int nFast = 2, const int nSlow = 30); 

   void   reset(); 
   double value() { return m_nResult; }  

   double process(const double *pV);
protected:
   const int    m_nPeriod;
   const double m_nS;
   const double m_nR;

   double m_nNoise;
   double m_nResult;
};
//
// -----------------------------------------------------------------------------
//
class AlmParabolic
{
public:
   AlmParabolic(const double nStep = 0.02, const double nMaxStep = 0.2); 

   void   reset(const double nPrice);
   double value() { return m_nResult; }  

   double process(const int nPosition, const double nPrice);

   AlmParabolic &operator = (const AlmParabolic &r)
   {
      m_nResult = r.m_nResult;
      m_nEP = r.m_nEP;
      m_nAF = r.m_nAF;
      
      return *this;
   }
protected:
   const double m_nStep;
   const double m_nMaxStep;
   
   double m_nEP;
   double m_nAF;
   double m_nResult;
};
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
inline _Tf AlmMedian(_Tf nV[], int nPeriod) // nV will be destroyed!
{  
   std::sort(&nV[0], &nV[nPeriod]);
   return (nPeriod > 30 || (nPeriod & 0x01)) ? nV[nPeriod/2] : (nV[nPeriod/2]+nV[nPeriod/2-1]) / 2.0;
}
//
// -----------------------------------------------------------------------------
//
