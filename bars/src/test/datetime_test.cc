//
// test_datetime.cc
//
#include <stdio.h>
#include <string.h>
#include <cstdint>

#include "datetime.h"
//
// -----------------------------------------------------------------------------
//
static uint16_t nLastMonthDay[2][12] =
{
   {31U, 29U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U},
   {31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U}
};

static char szBuffer1[17];
static char szBuffer2[17];

int main(int argc, char *argv[])
{
   printf("Test runing...");

   int nTestNumber = 0;
   for ( uint16_t y = 2000; y <= 2100; ++y ) 
   {   
      for ( uint16_t m = 1; m <= 12; ++m )
      {
         uint16_t nMaxDay = nLastMonthDay[y % 4 == 0 ? 0 : 1][m-1];
         for ( uint16_t d = 1; d <= nMaxDay; ++d )
            for ( uint16_t hour = 0; hour <= 23; ++hour )
               for ( uint16_t minute = 0; minute <= 59; ++minute )
               {
                  alm::datetime dt;
                  dt.setDate(y, m, d); 
                  dt.setTime(hour, minute);
 
                  dt.getDateTime(szBuffer1);
                  sprintf(szBuffer2, "%02d.%02d.%04d %02d:%02d", d, m, y, hour, minute); 
                  
                  if ( strcmp(szBuffer1, szBuffer2) != 0 )
                  {
                     printf("\nError %s must be %s\n", szBuffer1, szBuffer2);                  
                     return 1; 
                  }

                  ++nTestNumber;
               }
      } 
      if ( y % 5 == 0 )
      {
         printf(".");
         fflush(NULL);
      }
   }

   printf("\n%d tests [OK]\n", nTestNumber);  

   return 0;
}
//
// -----------------------------------------------------------------------------
//

