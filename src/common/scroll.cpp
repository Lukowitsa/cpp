//
// scroll.cpp
//
#include <windows.h>
#include <limits>

#include "scroll.h"
//
// -----------------------------------------------------------------------------
//
void AlmScroll(INT nBar, HWND hWnd, WPARAM wParam, int nStep, int nPageStep)
{
    int nPosition = GetScrollPos(hWnd, nBar);
    int nPreviousPosition = nPosition;

    switch ( LOWORD(wParam) )
    {
       case SB_LEFT:
          nPosition = std::numeric_limits<int>::min();
       break; 

       case SB_RIGHT:
          nPosition = std::numeric_limits<int>::max();            
       break;

       case SB_LINELEFT:        
          nPosition -= nStep; 
       break;
                
       case SB_LINERIGHT:
          nPosition += nStep; 
       break;
               
       case SB_PAGELEFT:
          nPosition -= nPageStep;
       break;    

       case SB_PAGERIGHT:
          nPosition += nPageStep;
       break;
                                    
       case SB_THUMBPOSITION:
       break;
            
       case SB_THUMBTRACK:
       {   
          SCROLLINFO fScrollInfo;
          fScrollInfo.cbSize = sizeof(SCROLLINFO);
          fScrollInfo.fMask  = SIF_TRACKPOS;
          GetScrollInfo(hWnd, SB_VERT, &fScrollInfo);
               
          nPosition = fScrollInfo.nTrackPos; 
       }
       break;
    } 
    if ( nPosition != nPreviousPosition )
    {
       int nMinPos;
       int nMaxPos;

       GetScrollRange(hWnd, nBar, &nMinPos, &nMaxPos);
       if ( nPosition < nMinPos )
          nPosition = nMinPos;
       if ( nPosition > nMaxPos ) 
          nPosition = nMaxPos;

       SetScrollPos(hWnd, nBar, nPosition, TRUE);
       InvalidateRect(hWnd, NULL, TRUE);
   }
}   
//
// -----------------------------------------------------------------------------
//
