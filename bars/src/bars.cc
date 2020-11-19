//
// bars.cc
//
#include <glob.h>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <limits>
#include <string.h>
#include <cstdint>

#include "datetime.h"
#include "basecandle.h"
//
// -----------------------------------------------------------------------------
//
using namespace std;
//
// -----------------------------------------------------------------------------
//
struct Record: alm::base_candle<double, uint32_t>
{
   inline Record(char *pszFileName)
   {
      m_pszFileName = pszFileName;
      auto slash = strrchr(pszFileName, '/');
      if ( slash != NULL ) 
         m_pszFileName = slash + 1;
   }

   inline bool isHyphen() const
   {
      return m_nHigh == m_nLow;
   }

   char *get(char *pszBuffer) const;
   char *m_pszFileName;
};   

char *Record::get(char *pszBuffer) const
{
   sprintf
   (
      pszBuffer, 
      "%s --> \t%s H=%g L=%g O=%g C=%g V=%lu\n",
      m_pszFileName,
      this->alm::datetime::tostring(),
      m_nHigh,
      m_nLow,      
      m_nOpen,
      m_nClose,
      (unsigned long) m_nVolume
   );   
   return pszBuffer;
}

bool read_txt(istream &f, alm::base_candle<double, uint32_t> &record, bool bNoTime = false, bool bOHLC = false)
{
   uint32_t nQuikDate;
   uint32_t nQuikTime;

   f >> nQuikDate; f.ignore(1024, ';');
   record.setQuikDate(nQuikDate);
   
   if ( bNoTime )
      record.setQuikTime(0U);   
   else 
   {
      f >> nQuikTime; f.ignore(1024, ';');
      record.setQuikTime(nQuikTime);
   }      
   if ( bOHLC )
   {
      f >> record.m_nOpen;  f.ignore(1024, ';'); 
      f >> record.m_nHigh;  f.ignore(1024, ';');
      f >> record.m_nLow;   f.ignore(1024, ';');
      f >> record.m_nClose; f.ignore(1024, ';');
   }
   else
   {
      f >> record.m_nHigh;  f.ignore(1024, ';'); 
      f >> record.m_nLow;   f.ignore(1024, ';');
      f >> record.m_nOpen;  f.ignore(1024, ';');
      f >> record.m_nClose; f.ignore(1024, ';');
   }   
/*
   {
      alm::date dSBER;
      dSBER.setDate(2007, 7, 17);
      if ( record.m_nDate <= dSBER.m_nDate && record.m_nHigh > 1000.0)
      {
         record.m_nHigh /= 1000.0; 
         record.m_nLow /= 1000.0;
         record.m_nOpen /= 1000.0;
         record.m_nClose /= 1000.0;
      }      
   }
   */
   auto p = f.tellg();
   f >> record.m_nVolume;
 
   if ( bOHLC )
      record.m_nVolume /= 10;
 
   return p != f.tellg();
}

void save_txt(ostream &f, const alm::base_candle<double, uint32_t> &record)
{
   f << record.getQuikDate() << ';' << setfill('0') << setw(4) << record.getQuikTime() << ';' <<
        record.m_nHigh << ';' << record.m_nLow   << ';' <<
        record.m_nOpen << ';' << record.m_nClose << ';' << record.m_nVolume << endl;
}


void read_dayoff(const char *pszFileName, set<alm::date> &dayOff)
{ 
   ifstream f(pszFileName, ios_base::in);
   while ( f.good() )
   {
      uint32_t nDate;
      f >> nDate;
      alm::date date;
      date.setQuikDate(nDate);
      dayOff.insert(date);
   }     
   f.close();
}

int merge(set<Record> &records, set<alm::date> &dayOff, glob_t *p_globbuf, bool bNoInterval)
{
   char szBuffer1[512];
   char szBuffer2[512];

   bool bRTKM = false;
   for ( size_t i = 0; i < p_globbuf->gl_pathc; ++i ) 
   {
      bool bOHLC = strstr(p_globbuf->gl_pathv[i], "finam") != 0;
      bRTKM = bRTKM || strstr(p_globbuf->gl_pathv[i], "RTKM-d1") != 0;

      ifstream f;
      f.open(p_globbuf->gl_pathv[i], ios_base::in);
      f.precision(10);
      
      Record record(p_globbuf->gl_pathv[i]); 
      int nNumberRead = 0;
      int nNumberInserted = 0;

      while ( read_txt(f, record, false, bOHLC) )
      {	
         if
         ( 
            record.m_nOpen  < record.m_nLow  ||
            record.m_nClose < record.m_nLow  ||
            record.m_nOpen  > record.m_nHigh ||
            record.m_nClose > record.m_nHigh ||
            record.m_nLow   > record.m_nHigh
         )                     
         {
            cout << endl << "\033[1;31mНеверное соотношение High Low Open Close в:" << "\033[0m" << endl; 
            cout << record.get(szBuffer1);
            f.close();
            return 2;
         }

         auto found = records.find(record);
         if ( found == records.end() )
         {
            records.insert(record);
            ++nNumberInserted;
         }   
         else if 
         ( 
            found->m_nHigh   != record.m_nHigh  ||
            found->m_nLow    != record.m_nLow   || 
            found->m_nOpen   != record.m_nOpen  || 
            found->m_nClose  != record.m_nClose || 
            found->m_nVolume != record.m_nVolume
         ) 
         {
            cout << "\n\033[1;31mОбнаружены различия в записях:" << "\033[0m" << endl; 
            cout << record.get(szBuffer1) << found->get(szBuffer2);
          //if ( !bOHLC )
          //{
            f.close();
            return 2;
          //}

         }    
         ++nNumberRead;
      }
      f.close();

      if ( i == 0 )
         cout << endl;
      cout << record.m_pszFileName << "\t добавлено " << setw(7) << nNumberInserted << " записей из " << setw(7) << nNumberRead << endl;    
   }

   // Проверка
   int nNumberRead = 0;
   int nInterval = -1;
   int nCandleNo = -1;

   alm::datetime dt1, dt2, dt3;
   alm::time tfirst;
   for ( auto &record: records )
   {
      // Пропуски в данных
      if ( nNumberRead >= 1 )
      {
         // Пропуски в данных
         if ( record.m_nDate - dt1.m_nDate > 1 && !bRTKM )
         {
            alm::date date;
            date.m_nDate = dt1.m_nDate + 1;
            for ( ; date.m_nDate < record.m_nDate; ++date.m_nDate )
               if ( dayOff.find(date) == dayOff.end() )
               {
                  alm::date d1, d2;
                  d1.m_nDate = dt1.m_nDate + 1;
                  d2.m_nDate = record.m_nDate - 1;                   
                  cout << endl << "Найдены пропуски в записях не соответствующие \"выходным\" (dayoff):" <<endl;
                  cout << "\t с " << d1.getDate(szBuffer1) << " по " << d2.getDate(szBuffer2) << " включительно." << endl << endl;
                  return 1;
               }            
         }            
      } 
          
      // Проверяем интервал
      if ( !bNoInterval && nNumberRead >= 3 && record.m_nDate > 7000 )
      {
         int interval =  abs(record.m_nTime - dt1.m_nTime);
         interval =  std::min(interval, abs(dt1.m_nTime - dt2.m_nTime)); 
         interval =  std::min(interval, abs(dt2.m_nTime - dt3.m_nTime));

         if ( nInterval < 0 )              
            nInterval = interval;
         if ( nInterval > 60 && nInterval != interval )
         {
            cout << endl << "Найдена запись соответcтвующая отличному интервалу:" << endl;   
            cout << record.get(szBuffer1);
            return 1;
         }
      }     
      
      // Первая свечка
      if ( nNumberRead >= 1 && dt1.m_nDate != record.m_nDate )
         nCandleNo = 1;

      if ( nCandleNo == 1 && record.m_nTime != 0 && record.m_nTime < ALM_TIME(10, 00) && !record.isHyphen() )
      {               
         cout << endl << "Неверное время или вид (чёрточка) первой свечки торговой сессии:" << endl;   
         cout << record.get(szBuffer1);
         return 1;
      }

      dt3 = dt2;
      dt2 = dt1;
      dt1 = record;

      if ( nCandleNo > 0 )
         ++nCandleNo;
      ++nNumberRead;
   }
   return 0;
}   
//
// -----------------------------------------------------------------------------
//
int main(int argc, char *argv[])
{
   if ( argc == 1 || (argc == 2 && strcmp(argv[1], "--help") == 0) )
   {
      cout << endl <<
         "Использование:" << endl << 
         "\tbars --help" << endl << 
         "\tbars [path/]files1 [[path/]files2... -o file_to" << endl << 
         "\tbars -[-]dayoff [path/]files1 [[path/]files2... -o file_to" << endl << 
         "\tbars -[-]time0  file_from -o file_to" << endl << endl <<
         "Разработано Aleksandr Ljwovich (C), (2018)"  << endl << endl;
      return 0;
   }

   glob_t globbuf;
   globbuf.gl_pathc = 0;
   
   int  nOperation = 1;
   int  nResult = 0;
   bool bInputs = false;
   bool bOutput = false;
   bool bNoInterval = false;
   string outFileName;

   for ( int i = 1; i < argc; ++i )
   {
      if ( (strcmp(argv[i], "--dayoff") == 0 || strcmp(argv[i], "-dayoff") == 0) && i == 1 )
         nOperation = 10;
      else if ( (strcmp(argv[i], "--time0") == 0 || strcmp(argv[i], "-time0") == 0) && i == 1 )
         nOperation = 11;
      else if ( (strcmp(argv[i], "--nointerval") == 0 || strcmp(argv[i], "-nointerval") == 0) && i == 1 )
         bNoInterval = true;
      else if ( strcmp(argv[i], "-o") == 0  )
      {
         if ( ++i < argc )      
         {
            outFileName = argv[i];
            bOutput = true;
         }         
      }
      else
      {
         if ( globbuf.gl_pathc > 0 ) 
            glob(argv[i], GLOB_APPEND, NULL, &globbuf);
         else   
            glob(argv[i], 0, NULL, &globbuf);
         bInputs = true;             
      }
   }
   
   if ( globbuf.gl_pathc < 1 ) 
   {
      cout << "\nНе найдено ни одного входного файла.\n\n";
      return 1;
   }   

   if ( nOperation >= 10 && nOperation != 12 && !bOutput )
   {
      cout << "\nНебходимо указать опцию -o файл_результата.\n\n";
      if ( bInputs )
         globfree(&globbuf);      
      return 1;
   }

   if ( nOperation == 11 && globbuf.gl_pathc > 1 )
   { 
      cout << "\nДля --time0 небходимо указать только один входной файл.\n\n";
      if ( bInputs )
      {
         globfree(&globbuf);      
         return 1;
      }   
   }      

   set<alm::date> dayOff;

   if ( nOperation == 10 ) // -- dayoff
   {
      for ( size_t i = 0; i < globbuf.gl_pathc; ++i )
      {
         ifstream f;
         f.open(globbuf.gl_pathv[i], ios_base::in);
         
         Record record(globbuf.gl_pathv[i]); 
         int nNumberRead = 0;
         int nNumberInserted = 0;

         read_dayoff(outFileName.c_str(), dayOff);

         uint16_t nPrevDay = std::numeric_limits<uint16_t>::max();
         while ( read_txt(f, record) )
         {   
            if ( record.m_nDate - nPrevDay > 1 )
            {
               alm::date date;
               date.m_nDate = nPrevDay + 1;
               for ( ; date.m_nDate < record.m_nDate; ++date.m_nDate )
                  if ( dayOff.find(date) == dayOff.end() )
                  {
                     dayOff.insert(date);
                     ++nNumberInserted;
                  }   
            }
            nPrevDay = record.m_nDate;
            ++nNumberRead;
         }   

         ofstream o(outFileName, ios_base::out | ios_base::trunc);
         for ( auto date: dayOff )
            o << date.getQuikDate() << endl; 
         o.close();
         f.close();
         cout << endl << "Из " << nNumberRead << " записей " << nNumberInserted << " \"выходных\" добавлено в " << outFileName << endl << endl;
      }
   }
   else if ( nOperation == 11 ) // --time0
   {
      ifstream f;
      f.open(globbuf.gl_pathv[0], ios_base::in);

      Record record(globbuf.gl_pathv[0]); 
      int nNumberRead = 0;

      ofstream o(outFileName, ios_base::out | ios_base::trunc);
      while ( read_txt(f, record, true) )
      {   
         ++nNumberRead;
         save_txt(o, record);
      }   
      o.close();
      cout << endl << nNumberRead << " записей из " << record.m_pszFileName <<  
           " преобразовано и сохранено в " << outFileName << endl << endl;
   }
   else if ( nOperation == 1 ) // -- merge
   {
      set<Record> records;

      read_dayoff("/home/aleksandr/.wine/drive_c/stocks/dayoff.dat", dayOff);
      int m = merge(records, dayOff, &globbuf, bNoInterval);   

      if ( m != 0 )
      {
         if ( bOutput ) 
            cout << endl << "Файлы не сохранены! (всё осталось без изменений)" << endl << endl;
         nResult = 1;
      } else if ( bOutput )
      {      
         ofstream o(outFileName, ios_base::out | ios_base::trunc);
         o.precision(10);
         for ( auto &record: records )
            save_txt(o, record);  
         o.close();
         cout << endl << "\033[1;32m" << records.size() << " записей сохранено в " << outFileName << " [OK]\033[0m" << endl << endl; 
      }
   }

   if ( bInputs )
      globfree(&globbuf);

   return nResult;
}
//
// -----------------------------------------------------------------------------
//

