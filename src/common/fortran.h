//
// fortran.h
//
#pragma once
//
// -----------------------------------------------------------------------------
//
extern "C" void ldsimq_(double *A, double *B, const int *N, int *KR);
extern "C" void ldrluf_(double *A, const int *N, const int *IA, int *IPVT);
extern "C" void ldrlus_(const double *A, const int *N, const int *IA, double *B, const int *IPVT);
extern "C" void ldrluc_(const double *A, const int *N, const int *IA, int *KR);

inline int ldrlu(double *pa, int n, double *pb, int *pnIPVT)
{
   int nError;
   ldrluf_(pa, &n, &n, pnIPVT);
   ldrluc_(pa, &n, &n, &nError);
   if ( nError == 0 )
      ldrlus_(pa, &n, &n, pb, pnIPVT);
   return nError;
}    
//
// -----------------------------------------------------------------------------
//
