//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_WSBAROCTL_H__49134344_1CE2_11D4_B1D2_54F85E000000__INCLUDED_)
#define AFX_WSBAROCTL_H__49134344_1CE2_11D4_B1D2_54F85E000000__INCLUDED_


#include "barotracegroup.h"
#include "cursor.h"
#include "winscoredefs.h"
#include "barozoom.h"
#include "BaroMeasureDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WSBaroCtl.h : Declaration of the CWSBaroCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CWSBaroCtrl : See WSBaroCtl.cpp for implementation.

class CWSBaroCtrl : public COleControl
{
	DECLARE_DYNCREATE(CWSBaroCtrl)

// Constructor
public:
	CWSBaroCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWSBaroCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation

	CBaroTraceGroup	m_cBaroTraceGroup;
	CTime			m_cStartTime;
	CTime			m_cEndTime;
	CCursor			m_cCursor;
	C2DTransform	m_c2DTransform;
	CFont			*m_pFont18, *m_pFont15, *m_pFont12, *m_pFont10;
	CString			m_strFontName;
	EUnits			m_eUnits;
	EAltUnits		m_eAltUnits;
	CSemaphore		m_cSemaphore;
//	CToolTipCtrl	m_cToolTip;
	short			m_uDisplayMask;

	HCURSOR			m_hMeasureCursor;
	CBaroZoom		*m_pZoom;
	bool			m_bDemo;

protected:
	~CWSBaroCtrl();

	DECLARE_OLECREATE_EX(CWSBaroCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CWSBaroCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CWSBaroCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CWSBaroCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CWSBaroCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
   	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CWSBaroCtrl)
	afx_msg short AddPilot(LPCTSTR strContestNo, LPCTSTR strLastName, LPCTSTR strFirstName, short sClass, long lColorREF);
	afx_msg void SetCursorPos(long lCursorTime);
	afx_msg void AddPoint(short sPilotIndes, long lTimeIndex, short sGPSAltitude, short sPressureAltitude);
	afx_msg void SetTimeRange(long lStartTime, long lEndTime);
	afx_msg void AddPoints(const VARIANT FAR& vlTime, const VARIANT FAR& vstrContestNo, const VARIANT FAR& vsPressureAlt, const VARIANT FAR& vsGPSAlt);
	afx_msg void ResetAll();
	afx_msg void AddPointByName(LPCTSTR strContestNo, long lTimeIndex, short sGPSAltitude, short sPressureAltitude);
	afx_msg void OnControlKey(short nChar);
	afx_msg void SetTaskOpenTime(long lTime);
	afx_msg void RedrawBaro();
	afx_msg void SetPilotVisibility(LPCTSTR strContestNo, BOOL bVisible);
	afx_msg long GetCursorPos();
	afx_msg BOOL IsCursorEnd();
	afx_msg void CursorHome();
	afx_msg void SetDisplayOption(long lOption, short sValue); 
	afx_msg short GetDisplayOption(long lOption); 
	afx_msg void AddFullPoint(short sPilotIndex, long lTimeIndex, short sGPSAltitude, short sPressAltitude, float fLat, float fLong, float fSpeed, float fVSpeed, short sENL);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnZoom();
	afx_msg void OnReset();
	afx_msg void OnMeasure();
	afx_msg void OnUpdateMeasure(CCmdUI* pCmdUI);
	afx_msg void SetTaskCloseTime(long lTime);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CWSBaroCtrl)
	void FireOnMoveCursor(long lNewTime)
		{
		FireEvent(eventidOnMoveCursor,EVENT_PARAM(VTS_I4), lNewTime);
		}
	void FireUpdateText(LPCTSTR strTime, LPCTSTR strPosition, LPCTSTR strGPSAlt, LPCTSTR strPressAlt, LPCTSTR strSpeed, LPCTSTR strVSpeed, LPCTSTR strMotor)
		{FireEvent(eventidUpdateText,EVENT_PARAM(VTS_BSTR  VTS_BSTR  VTS_BSTR  VTS_BSTR  VTS_BSTR  VTS_BSTR VTS_BSTR), strTime, strPosition, strGPSAlt, strPressAlt, strSpeed, strVSpeed, strMotor);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	long GetClosestPoint(CTime &cTime);
	bool m_bDragging;
	CPoint m_cDragPoint;
	int m_iDataMax;
	int m_iDataMin;
	void MoveCursorNext(bool bBack=false);
	CBaroTrace* IsSinglePilotVisible();
	void UpdateText( CDC* pDC );
	CTime m_cTaskOpenTime;
	CTime m_cTaskCloseTime;
	CRect m_cPlotWindow;
	CRect m_cDataWindow;
	CRect m_cInfoWindow;

	bool  m_bMeasureMode;
	bool  m_bZoomMode;
	CBaroMeasureDlg*	m_pMeasuringDlg;

	void DrawCursor( CDC *pdc, int iMode=R2_COPYPEN );
	void DrawGrid(CDC* pdc, C2DTransform &c2DTransform, CRect &cPlotWindow, CRect &cDataWindow, bool bFullscreen);
	void DrawTraces(CDC *pdc, C2DTransform &c2DTransform);
	void GetPlotArea(const CRect &cWindow, CRect &cPlotArea, CRect &cInfoWindow, bool &bFullScreen);
	void GetPlotDimensions(CRect &cRect);
	enum {
	//{{AFX_DISP_ID(CWSBaroCtrl)
	dispidAddPilot = 1L,
	dispidSetCursorPos = 2L,
	dispidAddPoint = 3L,
	dispidSetTimeRange = 4L,
	dispidAddPoints = 5L,
	dispidResetAll = 6L,
	dispidAddPointByName = 7L,
	dispidOnControlKey = 8L,
	dispidSetTaskOpenTime = 9L,
	dispidRedrawBaro = 10L,
	dispidSetPilotVisibility = 11L,
	dispidGetCursorPos = 12L,
	dispidIsCursorEnd = 13L,
	dispidCursorHome = 14L,
	dispidAddFullPoint = 15L,
	dispidSetDisplayOption = 16L,
	dispidGetDisplayOption = 17L,
	dispidSetTaskCloseTime = 18L,
	eventidOnMoveCursor = 1L,
	eventidUpdateText = 2L,
	//}}AFX_DISP_ID
	};
};



float ConvertAltitude(short sAltitude,  EAltUnits eUnitsTo );

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSBAROCTL_H__49134344_1CE2_11D4_B1D2_54F85E000000__INCLUDED)

