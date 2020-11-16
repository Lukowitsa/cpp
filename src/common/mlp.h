//
// mlp.h   
//
#pragma once
//  
#include <cmath> 
#include <istream>
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
template <typename _Tf>
class mlp
{
public:
   enum 
   { 
      none = 0, 
      relu = 1, 
      sigmoid = 2, 
      hard_sigmoid = 3, 
      tanh = 4 
   };

   mlp();
   ~mlp();
   
   int  getWeightsNumber() const;
   void read(const char *pszFileName);
   void read(std::istream &f);
   const _Tf *predict(const _Tf nInputs[], const _Tf *nWeights) const;   
   inline const _Tf *predict(const _Tf nInputs[]) const
   {
      return predict(nInputs, m_nWeights);
   }
//protected:   
   mlp(short nLayersNumber, const short nLayersDesc[]);
   int  init();
   void free();

   short  m_nLayersNumber2;
   short *m_nLayersDesc;   
   
   _Tf *m_nWeights;
   _Tf *m_nTemp1;
   _Tf *m_nTemp2;
};
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
int mlp<_Tf>::getWeightsNumber() const
{
   int nWeightsNumber = 0; 
   auto end = m_nLayersDesc + m_nLayersNumber2;
   auto nInputsSize = m_nLayersDesc[0];
   for ( auto layer = m_nLayersDesc; layer < end; ++layer )
   {  
      auto nLayerSize = *(++layer);
      nWeightsNumber += (1 + nInputsSize) * nLayerSize;
      nInputsSize = nLayerSize;
   }   
   return nWeightsNumber;
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
int mlp<_Tf>::init() 
{
   int nWeightsNumber = 0; 
   short nLargestLayerSize = 0;

   auto end = m_nLayersDesc + m_nLayersNumber2;
   auto nInputsSize = m_nLayersDesc[0];
   for ( auto layer = m_nLayersDesc; layer < end; ++layer )
   {  
      auto nLayerSize = *(++layer);
      if ( nLayerSize > nLargestLayerSize )
         nLargestLayerSize = nLayerSize;
      nWeightsNumber += (1 + nInputsSize) * nLayerSize;
      nInputsSize = nLayerSize;
   }   
   m_nWeights = new _Tf[nWeightsNumber];   
   m_nTemp1   = new _Tf[nLargestLayerSize];
   m_nTemp2   = new _Tf[nLargestLayerSize];

   return nWeightsNumber;
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
mlp<_Tf>::mlp(short nLayersNumber, const short nLayersDesc[]) // nInputs, l1, a1, l2, a2 ... 
{
   m_nLayersNumber2 = 2 * nLayersNumber;
   int nLayersSize  = 1 + m_nLayersNumber2; 
   m_nLayersDesc  = new short[nLayersSize]; 
   memcpy(m_nLayersDesc, nLayersDesc, nLayersSize * sizeof(m_nLayersDesc[0]));

   init();
}   
//
// -----------------------------------------------------------------------------
//
template <typename _Tf> 
mlp<_Tf>::mlp()
{
   m_nLayersNumber2 = 0;
   m_nLayersDesc = NULL;   
   m_nWeights = NULL;
   m_nTemp1 = NULL;
   m_nTemp2 = NULL;
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
void mlp<_Tf>::read(const char *pszFileName)
{
  std::ifstream f(pszFileName, std::ifstream::in | std::ifstream::binary);
  read(f);
  f.close();
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
void mlp<_Tf>::read(std::istream &f)
{
   free();

   short nLayersNumber;
   f.read(reinterpret_cast<char*>(&nLayersNumber), sizeof(nLayersNumber));
   m_nLayersNumber2 = 2 * nLayersNumber;

   int nLayersSize = 1 + m_nLayersNumber2; 
   m_nLayersDesc  = new short[nLayersSize]; 
   f.read(reinterpret_cast<char*>(m_nLayersDesc), sizeof(m_nLayersDesc[0]) * nLayersSize);

   int nWeightsNumber = init();

   char chFloatTypeSize;
   f.read(reinterpret_cast<char*>(&chFloatTypeSize), sizeof(chFloatTypeSize));
   if ( sizeof(_Tf) != chFloatTypeSize )
   {
      double *nBuffer = new double[nWeightsNumber];       
      f.read(reinterpret_cast<char*>(nBuffer), sizeof(nBuffer[0]) * nWeightsNumber);
      for ( int i = 0; i < nWeightsNumber; ++i )
         m_nWeights[i] = (_Tf) nBuffer[i];
      delete[] nBuffer;    
   }
   else 
      f.read(reinterpret_cast<char*>(m_nWeights), sizeof(m_nWeights[0]) * nWeightsNumber);
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
const _Tf *mlp<_Tf>::predict(const _Tf nInputs[], const _Tf *nWeights) const
{
   auto weight = nWeights; 
   auto layer  = m_nLayersDesc;
   auto end_l  = m_nLayersDesc + m_nLayersNumber2;
   auto input  = nInputs;
   auto output = m_nTemp2;
   auto nInputsSize = layer[0];
   while ( layer < end_l )
   {  
      auto nLayerSize  = *(++layer);
      auto nActivation = *(++layer);
      auto end_o = output + nLayerSize;
      for ( auto o = output; o < end_o; ++o )
      {
         float x = *weight++;
         x += vectors_dp(input, weight, nInputsSize);
         weight += nInputsSize;

         switch ( nActivation )
         {
            case relu:
               if ( x < 0.0 )
                  *o = 0.0;
               else
                  *o = x;   
            break;
            
            case sigmoid:
               *o = 1.0 / (1.0 + std::exp(-x));      
            break;               

            case hard_sigmoid:
               if ( x < -2.5 )
                  *o = 0.0;
               else if ( x > 2.5 )
                  *o = 1.0;
               else
                  *o = 0.2 * x + 0.5;
            break;               

            case tanh:
               *o = std::tanh(x);      
            break;
         }
      }        
 
      if ( output == m_nTemp2 )
      {   
         input  = output;
         output = m_nTemp1;
      }        
      else
      {   
         input  = output;
         output = m_nTemp2;
      }        
      nInputsSize = nLayerSize;
   }   

   return input;      
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
void mlp<_Tf>::free()
{
   if ( m_nLayersDesc != NULL )
      delete[] m_nLayersDesc;
   if ( m_nWeights != NULL )
      delete[] m_nWeights;
   if ( m_nTemp1 != NULL )   
      delete[] m_nTemp1;
   if ( m_nTemp2 != NULL )
      delete[] m_nTemp2;
}
//
// -----------------------------------------------------------------------------
//
template <typename _Tf>
mlp<_Tf>::~mlp()
{
   free();
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
