//
// round.h
//
#pragma once
//
#include <cmath>
#include <cstdint>
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
inline _Tf round2(_Tf nValue)
{
   return std::round(nValue * 100) / 100.0;
}
//
// -----------------------------------------------------------------------------
//
/*
floor(2.8) == 2.0
floor2(2.83001) == 2.83
*/
template <typename _Tf>
inline _Tf floor2(_Tf nValue) 
{
   return std::floor(nValue * 100) / 100.0;
}
//
// -----------------------------------------------------------------------------
//
/*
ceil(2.2)  == 3.0
ceil2(2.23001) == 2.24
*/
template <typename _Tf> 
inline _Tf ceil2(_Tf nValue)
{
   return std::ceil(nValue * 100) / 100.0;
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
inline _Tf roundp(_Tf nValue, int16_t nDigits)
{ 
   int16_t  nLog = std::log10(nValue);
   long double r = pow(10.0, nDigits-((nLog > 0) ? nLog+1 : nLog));
   return std::round(nValue * r) / r;
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
