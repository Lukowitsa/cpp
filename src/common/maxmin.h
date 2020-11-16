//
// maxmin.h
//
#pragma once
//
// -----------------------------------------------------------------------------
//
#ifndef ____ALM_NAMESPACE____
   #define ____ALM_NAMESPACE____
   namespace alm {
#endif          
//
// Aleksandr Ljwowich (R) (TM) design and code programming (C), 2016.
//
// -----------------------------------------------------------------------------
//
// max
//
template <typename _T1, typename _T2, typename _T3>
inline bool max(_T1 a, _T2 b, _T3 c)
{
   return ((a >= b) ? ((a >= c) ? a : c) : ((b >= c) ? b : c));
}
//
// -----------------------------------------------------------------------------
//
template <typename _T1, typename _T2, typename _T3>
inline bool min(_T1 a, _T2 b, _T3 c)
{
   return ((a <= b) ? ((a <= c) ? a : c) : ((b <= c) ? b : c));
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
