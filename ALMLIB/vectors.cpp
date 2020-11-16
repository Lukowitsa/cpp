//
// vectors.cpp
//
#include <float.h>
#include "vectors.h"
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
__m256i _vectors_nMask[8] = 
{
   {0, 0, 0, 0},
   {
      static_cast<long long>(0x00000000FFFFFFFF), 
      0, 0, 0
   },
   {
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      0, 0, 0
   },
   {
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      static_cast<long long>(0x00000000FFFFFFFF), 
      0, 0
   },
   {
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      0, 0
   },
   {
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      static_cast<long long>(0x00000000FFFFFFFF),    
      0
   },
   {
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      static_cast<long long>(0xFFFFFFFFFFFFFFFF),    
      0
   },
   {
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      static_cast<long long>(0xFFFFFFFFFFFFFFFF),    
      static_cast<long long>(0x00000000FFFFFFFF)
   }
};
//
// -----------------------------------------------------------------------------
//
__m256i _vectors_nMaskd[4] = 
{
   {0, 0, 0, 0},
   {
      static_cast<long long>(0xFFFFFFFFFFFFFFFF), 
      0, 0, 0
   },
   {
      static_cast<long long>(0xFFFFFFFFFFFFFFFF),
      static_cast<long long>(0xFFFFFFFFFFFFFFFF),
      0, 0
   },
   {
      static_cast<long long>(0xFFFFFFFFFFFFFFFF),
      static_cast<long long>(0xFFFFFFFFFFFFFFFF),
      static_cast<long long>(0xFFFFFFFFFFFFFFFF),
      0
   }
};
//
// -----------------------------------------------------------------------------
//
__m256 _vectors_nMax[8] = 
{
   { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX },
   { 0.0,     FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX },
   { 0.0,     0.0,     FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX },
   { 0.0,     0.0,     0.0,     FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX },
   { 0.0,     0.0,     0.0,     0.0,     FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX },
   { 0.0,     0.0,     0.0,     0.0,     0.0,     FLT_MAX, FLT_MAX, FLT_MAX },
   { 0.0,     0.0,     0.0,     0.0,     0.0,     0.0,     FLT_MAX, FLT_MAX },
   { 0.0,     0.0,     0.0,     0.0,     0.0,     0.0,     0.0,     FLT_MAX }
};

__m256 _vectors_nMin[8] = 
{
   { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX },
   { 0.0,      -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX },
   { 0.0,      0.0,      -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX },
   { 0.0,      0.0,      0.0,      -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX },
   { 0.0,      0.0,      0.0,      0.0,      -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX },
   { 0.0,      0.0,      0.0,      0.0,      0.0,      -FLT_MAX, -FLT_MAX, -FLT_MAX },
   { 0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      -FLT_MAX, -FLT_MAX },
   { 0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      0.0,      -FLT_MAX }
};   
//
// -----------------------------------------------------------------------------
//
__m256d _vectors_nMaxd[4] = 
{
   { DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX },
   { 0.0,     DBL_MAX, DBL_MAX, DBL_MAX },
   { 0.0,     0.0,     DBL_MAX, DBL_MAX },
   { 0.0,     0.0,     0.0,     DBL_MAX }
};   

__m256d _vectors_nMind[4] = 
{
   { -DBL_MAX, -DBL_MAX, -DBL_MAX, -DBL_MAX },
   { 0.0,      -DBL_MAX, -DBL_MAX, -DBL_MAX },
   { 0.0,      0.0,      -DBL_MAX, -DBL_MAX },
   { 0.0,      0.0,      0.0,      -DBL_MAX }
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
