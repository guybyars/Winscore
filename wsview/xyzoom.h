//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//#include "geomrect.h"

class XYZoom
    {
    public:
    
        XYZoom(CWnd &cwndParam, const CRect &geomrectValidParam);

        void OnMove(const CPoint &point);
        void SetBeginning(const CPoint &point);
        void SetEnd(const CPoint &point);
        void Cancel();
        
        bool IsValid() const;
        CRect GetZoomRect() const;
        
        void Show(CDC &dc);
        void Hide(CDC &dc);
        
    private:
    


        CWnd &cwnd;
        
        CRect geomrectValid;
        
        bool fOnePoint;
        
        bool fSet;
        bool fShown;
        
        CPoint pointBegin;
        CPoint pointEnd;
    };

