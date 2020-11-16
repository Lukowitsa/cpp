//
// GA.cpp
//
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <algorithm>

#include "GA.h"
//
// Генератор случайных чисел для заданного интервала
//
inline double getRand(double nMinimum, double nMaximum) 
{ 
   return (nMinimum + ((nMaximum-nMinimum)*rand()) / RAND_MAX);
}
//
// Генератор случайных чисел от 0 до nMaximum
//
inline double getRand(double nMaximum) 
{ 
   return ((nMaximum*rand()) / RAND_MAX);
}
//
// Конструктор
//
AlmGA::AlmGA(int nGeneCount, int nChromosomeCount)
: m_nGeneCount(nGeneCount), 
  m_nGeneCountP1(nGeneCount+1),
  m_nChromosomeSize((nGeneCount+1)*sizeof(double)),
  m_nChromosomeCount(nChromosomeCount), 
  m_pfServiceFunction(NULL)
{
   double  *pnPopulation;
   double  *pnPopulationEnd; 
   double **ppIndex;
   // ---------------------------------
   size_t nSize = 2 * nChromosomeCount;
   m_ppIndexes = new double*[nSize];
   m_ppIndexesTemp = new double*[nSize];
   m_pnFit = new double[nSize+1];
   m_pnFit[0] = 0.0;
   // ---------------------------------
   m_pnChromosome = new double[m_nGeneCountP1]; 
   // ---------------------------------
   nSize = nChromosomeCount * m_nGeneCountP1;
   m_pnColony  = new double[nSize]; 
   // ---------------------------------
   nSize = 2 * nSize;
   m_pnPopulation = new double[nSize]; 
   pnPopulationEnd = m_pnPopulation + nSize;
   //
   // Заполним индексы для сортировки
   //
   ppIndex = m_ppIndexes;
   for ( pnPopulation = m_pnPopulation; pnPopulation != pnPopulationEnd; pnPopulation += m_nGeneCountP1 )
      *(ppIndex++) = pnPopulation;
};

AlmGA::~AlmGA()
{
   delete m_ppIndexes;
   delete m_ppIndexesTemp;
   delete m_pnFit;
   delete m_pnColony;
   delete m_pnPopulation;
};
//
// Основная функция UGA
// 
double *AlmGA::run
(
   double (*pFitnessFunction)(const double *pnChromosome), // Фитнес функция
   unsigned short nReplicationPortion,  // Доля Репликации.
   unsigned short nMutationPortion,     // Доля Естественной мутации.
   unsigned short nArtificialMutation,  // Доля Искусственной мутации.
   unsigned short nGenoMergingPortion,  // Доля Заимствования генов.
   unsigned short nCrossingOverPortion, // Доля Кроссинговера.
   double nReplicationOffset,   // Коэффициент смещения границ интервала
   double nMutationProbability  // Вероятность мутации каждого гена в %
)
{ 
   // Переменные
   int      nCurrentEpoch = 1;
   int      nGene;
   int      nBorder;
   double   nGene1;
   double   nGene2;
   double   nTemp;
   double   nMaximum;
   double   nMinimum;
   double  *pMama;
   double  *pPapa;
   double  *pnColony;
   double  *pnColonyEnd;
  
   double  *pnFitnessValue;
   double  *pnGeneEnd;
   
   double **ppIndex;
   
   unsigned short nOperation;
   unsigned short nOperationFit[6];
   //
   // Сброс генератора, производится только один раз
   //
   srand((unsigned)time(NULL));
   //
   // Создаём протопопуляцию 
   //
   m_nPopulationSize = 2 * m_nChromosomeCount;
   pnColony    = m_pnPopulation;
   pnColonyEnd = m_pnPopulation + (m_nGeneCountP1*m_nPopulationSize);
   while ( pnColony != pnColonyEnd )
   {
      pnFitnessValue = pnColony++; 
      pnGeneEnd = pnColony + m_nGeneCount;
      while ( pnColony != pnGeneEnd )
         *(pnColony++) = selectInDiscreteSpace(m_nRangeMinimum, m_nRangeMaximum);
      *pnFitnessValue = pFitnessFunction(pnFitnessValue + 1);
   }
   //
   // Подготовим популяцию к размножению          
   //
   preparePopulation();
   //
   // Выделим эталонную хромосому                          
   //
   memcpy(m_pnChromosome, *m_ppIndexes, m_nChromosomeSize);
   if ( m_pfServiceFunction != NULL )
      m_pfServiceFunction(m_pnChromosome[0], m_pnChromosome + 1, 0);
   //
   // Подготовим массив для выбора опертора UGA
   //
   if ( nMutationProbability < 0.0 )
      nMutationProbability = 0.0;
   if ( nMutationProbability > 100.0 )
      nMutationProbability = 100.0;

   nOperationFit[0] = 0;
   nOperationFit[1] = nReplicationPortion;
   nOperationFit[2] = nReplicationPortion + nMutationPortion;
   nOperationFit[3] = nOperationFit[2] + nArtificialMutation;
   nOperationFit[4] = nOperationFit[3] + nGenoMergingPortion;
   nOperationFit[5] = nOperationFit[4] + nCrossingOverPortion;
   //
   // Основной цикл генетического алгоритма
   //
   pnColonyEnd = m_pnColony + (m_nGeneCountP1 * m_nChromosomeCount);      
   while ( nCurrentEpoch <= m_nEpoch)
   {
      // Заполняем новую колонию потомками 
      pnColony = m_pnColony;
      while ( pnColony != pnColonyEnd )    
      {
         pnFitnessValue = pnColony++; 
         pnGeneEnd = pnColony + m_nGeneCount;
           
         //============================
         nTemp = getRand(nOperationFit[5]);
         for (nOperation = 0; nOperation < 5; ++nOperation)
            if ( nOperationFit[nOperation] <= nTemp && nTemp <= nOperationFit[nOperation+1] )
               break;
         //============================
         switch (nOperation)
         {
            case 0: // Репликация
            {
               //-----------------Отбор родителей------------------------
               selectTwoParents(&pMama, &pPapa);

               //-------------------Цикл перебора генов--------------------------------
               while ( pnColony != pnGeneEnd )
               {
                  nGene1 = *(++pMama);
                  nGene2 = *(++pPapa);
                  // если С1>C2, поменяем их местами
                  if ( nGene1 > nGene2 )
                  {
                     nTemp  = nGene1; 
                     nGene1 = nGene2;
                     nGene2 = nTemp;
                  }
                  //--------------------------------------------
                  if ( (nGene2-nGene1) < m_nPrecision )
                  {
                     *(pnColony++) = nGene1; 
                     continue;
                  }
                  //--------------------------------------------
                  // Назначим границы создания нового гена
                  nMinimum = nGene1 - ((nGene2-nGene1)*nReplicationOffset);
                  nMaximum = nGene2 + ((nGene2-nGene1)*nReplicationOffset);
                  //--------------------------------------------
                  // Обязательная проверка, что бы поиск не вышел из заданного диапазона
                  if (nMinimum < m_nRangeMinimum) 
                     nMinimum = m_nRangeMinimum;
                  if (nMaximum > m_nRangeMaximum) 
                     nMaximum = m_nRangeMaximum;
                  //---------------------------------------------------------------
                  *(pnColony++) = selectInDiscreteSpace(nMinimum, nMaximum);
               }
            }
            break;

            case 1: // Естественная мутация
            {
               //-----------------Отбор родителя------------------------
               pPapa = naturalSelection();
               //---------------------------------------
               while ( pnColony != pnGeneEnd )
               {
                  ++pPapa; 
                  if ( getRand(100.0) <= nMutationProbability )
                     *(pnColony++) = selectInDiscreteSpace(m_nRangeMinimum, m_nRangeMaximum);
                  else
                     *(pnColony++) = *pPapa;
               }
            }
            break;

            case 2: // Искусственная мутация
            {
               //-----------------Отбор родителей------------------------
               selectTwoParents(&pMama, &pPapa);
               //--------------------------------------------------------
               //-------------------Цикл перебора генов------------------
               while ( pnColony != pnGeneEnd )
               {
                  nGene1 = *(++pMama);
                  nGene2 = *(++pPapa);
                  //если С1>C2, поменяем их местами
                  if ( nGene1 > nGene2 )
                  {
                     nTemp  = nGene1;
                     nGene1 = nGene2;
                     nGene2 = nTemp;
                  }
                  //--------------------------------------------
                  // Назначим границы создания нового гена
                  nMinimum = nGene1 - ((nGene2-nGene1)*nReplicationOffset);
                  nMaximum = nGene2 + ((nGene2-nGene1)*nReplicationOffset);
                  //--------------------------------------------
                  // Обязательная проверка, что бы поиск не вышел из заданного диапазона
                  if ( nMinimum < m_nRangeMinimum) 
                     nMinimum = m_nRangeMinimum;
                  if ( nMaximum > m_nRangeMaximum) 
                     nMaximum = m_nRangeMaximum;
                  //---------------------------------------------------------------
                  if (rand() < (RAND_MAX / 2))
                     *(pnColony++) = selectInDiscreteSpace(m_nRangeMinimum, nMinimum);
                  else
                     *(pnColony++) = selectInDiscreteSpace(nMaximum, m_nRangeMaximum);
               }
            }
            break;
            //---------------------
            case 3: // Заимствование генов
            {   
               for (nGene = 1; nGene <= m_nGeneCount; ++nGene)
                  *(pnColony++) = naturalSelection()[nGene];
            }
            break;

            default: // Кроссинговер
            {
               //-----------------Отбор родителей------------------------
               selectTwoParents(&pMama, &pPapa);
               //--------------------------------------------------------
               // Определим точку разрыва
               nBorder = ceil(getRand(m_nGeneCount-1));
               //--------------------------------------------------------
               //-------------------Цикл перебора генов------------------
               for (nGene = 1; nGene <= m_nGeneCount; ++nGene)
               {
                  ++pMama;
                  ++pPapa;
                  //----копируем гены матери--------
                  if ( nGene <= nBorder )
                     *(pnColony++) = *pMama;
                  //----копируем гены отца--------
                  else
                     *(pnColony++) = *pPapa;
               }
            }
            break;
         }
         //
         // Определим приспособленность новой особи
         //
         *pnFitnessValue = pFitnessFunction(pnFitnessValue + 1);
      }
      //
      // Поселим потомков в основную популяцию
      //
      if ( m_nPopulationSize >= m_nChromosomeCount )
      {
         nBorder = m_nChromosomeCount;
         m_nPopulationSize = 2 * m_nChromosomeCount;
      }
      else
      {
         nBorder = m_nPopulationSize;
         m_nPopulationSize += m_nChromosomeCount;
      }
      //
      ppIndex = m_ppIndexes + nBorder;
      for ( pnColony = m_pnColony; pnColony != pnColonyEnd; pnColony += m_nGeneCountP1 ) 
         memcpy(*(ppIndex++), pnColony, m_nChromosomeSize);
      //
      // Подготовим популяцию к следующему размножению
      //
      preparePopulation();
      // 
      // Сравнить гены лучшего потомка с генами эталонной хромосомы 
      //
      if ( m_nMethod == 1 )
      {
         //Если лучшая хромосома популяции лучше эталонной
         if ( **m_ppIndexes < *m_pnChromosome )
         {
            // Заменим эталонную хромосому
            memcpy(m_pnChromosome, *m_ppIndexes, m_nChromosomeSize);
            if ( m_pfServiceFunction != NULL )
               m_pfServiceFunction(m_pnChromosome[0], m_pnChromosome + 1, 1);
            nCurrentEpoch = 1; 
         }
         else
            ++nCurrentEpoch;
      }
      //Если режим оптимизации - максимизация
      else
      {
         //Если лучшая хромосома популяции лучше эталонной
         if ( **m_ppIndexes > *m_pnChromosome )
         {
            //Заменим эталонную хромосому
            memcpy(m_pnChromosome, *m_ppIndexes, m_nChromosomeSize);
            if ( m_pfServiceFunction != NULL )
               m_pfServiceFunction(m_pnChromosome[0], m_pnChromosome + 1, 1);
            nCurrentEpoch = 1; 
         }
         else
            ++nCurrentEpoch;
      }
      // Прошла эпоха...
      if ( m_pfServiceFunction != NULL )
         m_pfServiceFunction(m_pnChromosome[0], m_pnChromosome + 1, 0);
   }
   return m_pnChromosome;
}
//
// Сортировка индексов по значению FF,
// удаление дубликатов, 
// заполнение шага для естественного отбора
//
void AlmGA::preparePopulation()
{
   double   nDelta;

   double **ppIndex;
   double **ppIndexTemp;
   double **ppIndexTempEnd;
 
   double  *pnFit;

   double **ppIndexEnd = m_ppIndexes + m_nPopulationSize;
   //
   // Отсортируем массив индексов
   //
   if ( m_nMethod == 1 )
      std::sort
      (
         m_ppIndexes, 
         ppIndexEnd, 
         [] (const double *p1, const double *p2) 
         {
            return (*p1 < *p2);
         }
      );
   else
      std::sort
      (
         m_ppIndexes, 
         ppIndexEnd, 
         [] (const double *p1, const double *p2) 
         {
            return (*p2 < *p1);
         }
      );
   //
   // Шаг для естественного отбора
   //
   --ppIndexEnd;
   nDelta = (**m_ppIndexes - **ppIndexEnd) * 0.01 - **ppIndexEnd;
   //
   // Запишем первый элемент, он точно не дубликат
   //
   ppIndex = m_ppIndexes;
   ppIndexTemp = m_ppIndexesTemp;
   *ppIndexTemp = *ppIndex;
   // --------------------------
   pnFit = m_pnFit;
   pnFit[1] = pnFit[0] + fabs(**ppIndex + nDelta);
   //
   // Удалим из индексов дубликаты хромосом, заодно подсчитаем шаг для 
   // естественного отбора
   //
   ppIndexTempEnd = ppIndexTemp + m_nPopulationSize;
   while ( ++ppIndex <= ppIndexEnd )
   {
      // Проверим на дубликаты
      if 
      ( 
         (**ppIndex != **(ppIndex-1)) ||
         (memcmp(*ppIndex, *(ppIndex-1), m_nChromosomeSize) != 0)
      )   
      {
         *(++ppIndexTemp) = *ppIndex; 
         // Заполним шаг для естественного отбора
         ++pnFit;
         pnFit[1] = pnFit[0] + fabs(**ppIndex + nDelta);
      }
      else // Дубликаты копировать не будем, запишем индекс в конец
         *(--ppIndexTempEnd) = *ppIndex;
   } 
   //
   // Скопируем индексы обратно, дубликаты остались за границей популяции
   //
   memcpy(m_ppIndexes, m_ppIndexesTemp, m_nPopulationSize * sizeof(double*));
   //
   // Новый размер популяции с учётом удалённых дубликатов
   //
   m_nPopulationSize = ppIndexTemp - m_ppIndexesTemp + 1;
};
//
// Отбор двух родителей
//
void AlmGA::selectTwoParents
(
   double **ppnAddressMama,
   double **ppnAddressPapa
)
{
   for (int nCount = 0; nCount < 10; ++nCount)
   {
      *ppnAddressMama = naturalSelection();
      *ppnAddressPapa = naturalSelection();
      if ( *ppnAddressMama != *ppnAddressPapa )
         break;
   }
}
//
// Естественный отбор
//
double *AlmGA::naturalSelection()
{
   int     nDiff;

   double *pnStart = m_pnFit;
   double *pnEnd   = pnStart + m_nPopulationSize;
   double *pnPivot;

   double  nRand = getRand(m_pnFit[m_nPopulationSize]);

   while ( (nDiff = pnEnd - pnStart) > 1 )
   {
      pnPivot = pnStart + nDiff / 2;
      if ( *pnPivot <= nRand )
         pnStart = pnPivot;
      else
         pnEnd = pnPivot;
   }

   return m_ppIndexes[pnStart-m_pnFit];         
}
//
// Выбор в дискретном пространстве
//
double AlmGA::selectInDiscreteSpace(double nMin, double nMax)
{
   double nRand = getRand(nMin, nMax);

   if ( m_nPrecision == 0.0)
      return nRand;

   // при нарушении - вернем нарушенную границу
   if ( nRand < m_nRangeMinimum ) 
      return m_nRangeMinimum;
   if ( nRand > m_nRangeMaximum ) 
      return m_nRangeMaximum;
   if ( m_nRangeMaximum == m_nRangeMinimum || m_nPrecision <= 0.0 ) 
      return m_nRangeMinimum;

   // приведем к заданному масштабу
   double nRange = m_nRangeMaximum - m_nRangeMinimum;
   double nStep  = nRange / ceil(nRange / m_nPrecision);
   double nValue = (nRand - m_nRangeMinimum) / nStep;
   return m_nRangeMinimum + nStep * (fmod(nValue, 1) >= 0.5 ? ceil(nValue) : floor(nValue));
}
//
// ---------------------------------------------------------------------------------
//
