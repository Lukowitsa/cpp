//
// hilbert.cpp
//
#include "fasttransforms.h"
#include "hilbert.h"
//
// -----------------------------------------------------------------------------
//
// Рассчитывает аналитический сигнал, аналогично одноимённой функции в MATHLAB.
//
void hilbert(const double *pRe, double *pIm, int n)
{
   double nTemp;
   int    i, k;

   alglib::complex_1d_array z;
   z.setlength(n);

   for ( i = 0; i < n; ++i )
   {
      z[i].x = pRe[i];
      z[i].y = 0;      
   }
//
// Прямое БПФ 
//
   alglib::fftc1d(z);
//
// Сдвиг "частот"
//
   k = n / 2;
   z[0].x = 0;
   z[0].y = 0;
   z[k].x = 0;
   z[k].y = 0;
   for (i = 1; i < k; ++i)                       
   {
      nTemp  = -z[i].x;
      z[i].x =  z[i].y;
      z[i].y =  nTemp;
   } 
   for (i = k + 1; i < n; ++i)                     
   {
      nTemp  =  z[i].x;
      z[i].x = -z[i].y;
      z[i].y =  nTemp;
   }
//
// Обратное БПФ
//
   alglib::fftc1dinv(z);
//
// Пишем результат во мнимую часть
//
   for ( int i = 0; i < n; ++i)
      pIm[i] = z[i].x;
}
//
// -----------------------------------------------------------------------------
//
