//
// candle.h
//
#pragma once
//
#include <cstdint>
#include <common/daytime.h>
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
// candle
//
struct candle_hloc
{
   float m_nHigh;
   float m_nLow;
   float m_nOpen;
   float m_nClose;
};   
//
// -----------------------------------------------------------------------------
//
struct candle_hlocv: public candle_hloc
{
   uint32_t m_nVolume;
};   
//
// -----------------------------------------------------------------------------
//
struct candle: public candle_hlocv
{
   daytime m_nTime;
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
