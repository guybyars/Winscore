//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#include "stdafx.h"

#include "xyzoom.h"


XYZoom::XYZoom(CWnd &cwndParam, const CRect &geomrectValidParam) :
        cwnd(cwndParam),
        geomrectValid(geomrectValidParam),
        fOnePoint(false),
        fShown(false),
        fSet(false)
    {
    cwnd.SetCapture();

    // Lets the user know its time to use the mouse.
    SetCursor(LoadCursor(NULL, IDC_CROSS));
    }


void XYZoom::OnMove(const CPoint &point)
    {
    SetCursor(LoadCursor(NULL, IDC_CROSS));

    if (fOnePoint)
        {
        CClientDC clientdc(&cwnd);
        Hide(clientdc);

        pointEnd = point;//geomrectValid.Closest(point);

        fSet = true;    

        Show(clientdc);
        }
    }
    

void XYZoom::SetBeginning(const CPoint &point)
    {
    pointBegin = point;//geomrectValid.Closest(point);

    fOnePoint = true;        
    }
    
    
void XYZoom::SetEnd(const CPoint &point)
    {
    pointEnd = point;//geomrectValid.Closest(point);
                
    ReleaseCapture();
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    }
    
    
void XYZoom::Cancel()
    {
    ReleaseCapture();
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    CClientDC clientdc(&cwnd);
    Hide(clientdc);
    }
    
    
bool XYZoom::IsValid() const
    {
    if (pointBegin.x <0 ||
		pointEnd.x   <0 ||
		pointBegin.y <0 ||
		pointEnd.y   <0     ) return false;

    return pointBegin.x != pointEnd.x && pointBegin.y != pointEnd.y;
    }
    
    
CRect XYZoom::GetZoomRect() const
    {
    return CRect(pointBegin, pointEnd);
    }
    

void XYZoom::Show(CDC &dc)
    {
 //   if (fSet && !fShown)
        {
		CRect cRect=GetZoomRect();
		cRect.NormalizeRect();
        dc.DrawFocusRect(cRect);
        fShown = true;
        }
    }
    
    
void XYZoom::Hide(CDC &dc)
    {
    if (fSet && fShown)
        {
		CRect cRect=GetZoomRect();
		cRect.NormalizeRect();
        dc.DrawFocusRect(cRect);

//        dc.DrawFocusRect(CRect(GetZoomRect()));
        fShown = false;
        }
    }

