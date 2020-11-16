//
// GA.h
//
// ---------------------------------------------------------------------------------------
//
class AlmGA
{
public:
    AlmGA(int nGeneCount, int nChromosomeCount);
   ~AlmGA();        

   void setRange(double nRangeMinimum, double nRangeMaximum);
   void setRange(double nRangeMinimum, double nRangeMaximum, double nPrecision);
   void setPrecision(double nPrecision);
   void setEpoch(int nEpoch);
   void setMethod(short nMethod);
   void setService
   (
      void (*pServiceFunction)
      (
         double nFitnessValue, 
         const double *pnChromosome,
         short nTheBestSolutionFound
      )
   );   
   double *getChromosome();
   int     getHistorySize();

   double *run
   (
      double (*pFitnessFunction)(const double *pnChromosome), // Фитнес функция
      unsigned short nReplicationPortion,  // Доля Репликации.
      unsigned short nMutationPortion,     // Доля Естественной мутации.
      unsigned short nArtificialMutation,  // Доля Искусственной мутации.
      unsigned short nGenoMergingPortion,  // Доля Заимствования генов.
      unsigned short nCrossingOverPortion, // Доля Кроссинговера.
      double nReplicationOffset,   // Коэффициент смещения границ интервала
      double nMutationProbability  // Вероятность мутации каждого гена в %
   );
protected:
   void preparePopulation();
   void selectTwoParents
   (
      double **ppnAddressMama,
      double **ppnAddressPapa
   );
   double *naturalSelection();
   double  selectInDiscreteSpace(double nMin, double nMax);

   double  *m_pnChromosome;      // Эталонная хромосома

   const int m_nGeneCount;       // Количество генов в хромосоме
   const int m_nGeneCountP1;
   const int m_nChromosomeCount; // Максимально возможное количество хромосом в колонии
   int       m_nPopulationSize;  // Текущее количество хромосом в полуляции

   double   m_nRangeMinimum;    // Минимум диапазона поиска
   double   m_nRangeMaximum;    // Максимум диапазона поиска
   double   m_nPrecision;       // Шаг поиска
   int      m_nEpoch;           // Количество эпох без улучшения  
   short    m_nMethod;          // 1-минимум, любое другое - максимум
   
   const size_t m_nChromosomeSize;  // Длина хромосомы в байтах
   double   *m_pnFit;               // Рабочий массив для естественного отбора
   double  **m_ppIndexes;           // Массив индексов для сортировки
   double  **m_ppIndexesTemp;       // Массив для удаления дубликатов
   double   *m_pnPopulation;        // Популяция
   double   *m_pnColony;            // Колония потомков

   void    (*m_pfServiceFunction)   // Сервисная функция
   (
      double nFitnessValue, 
      const double *pnChromosome,
      short nTheBestSolutionFound
   );
};

inline void AlmGA::setRange(double nRangeMinimum, double nRangeMaximum)
{
   m_nRangeMinimum = nRangeMinimum;
   m_nRangeMaximum = nRangeMaximum;
};

inline void AlmGA::setRange(double nRangeMinimum, double nRangeMaximum, double nPrecision)
{
   m_nRangeMinimum = nRangeMinimum;
   m_nRangeMaximum = nRangeMaximum;
   m_nPrecision = nPrecision;
};

inline void AlmGA::setPrecision(double nPrecision)
{
   m_nPrecision = nPrecision;
};

inline void AlmGA::setEpoch(int nEpoch)
{
   m_nEpoch = nEpoch;
}

inline void AlmGA::setMethod(short nMethod)
{
   m_nMethod = nMethod;
}

inline void AlmGA::setService
(
   void (*pServiceFunction)
   (
      double nFitnessValue, 
      const double *pnChromosome,
      short nTheBestSolutionFound
   )
)
{
   m_pfServiceFunction = pServiceFunction;
}

inline double *AlmGA::getChromosome()
{
   return m_pnChromosome;
};
//
// ---------------------------------------------------------------------------------------
//