//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_WSVIEWCTL_H__E0748853_5814_11D3_BA96_0010A4043281__INCLUDED_)
#define AFX_WSVIEWCTL_H__E0748853_5814_11D3_BA96_0010A4043281__INCLUDED_

#include "TraceGroup.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WSViewCtl.h : Declaration of the CWSViewCtrl ActiveX Control class.

#include "winscoredefs.h"
#include "ControlPoint.h"
#include "xyzoom.h"
#include "measuringdlg.h"
#include "HitList.h"
#include <afxmt.h>

#define MAXTASKPOINTS	15
#define	MAXGRIDLIST		16
#define MAXMAPS			20

#define WSTINYFONT	0
#define WSSMALLFONT	1
#define WSMEDFONT	2
#define WSLARGEFONT	3
#define WSHUGEFONT	4

#define WSNUMFONTS  5

#define m_to_ft  3.281  /* m to feet. */

/////////////////////////////////////////////////////////////////////////////
// CWSViewCtrl : See WSViewCtl.cpp for implementation.

class CWSViewCtrl : public COleControl
{
	DECLARE_DYNCREATE(CWSViewCtrl)

// Constructor
public:
	CWSViewCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWSViewCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CWSViewCtrl();

	DECLARE_OLECREATE_EX(CWSViewCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CWSViewCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CWSViewCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CWSViewCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CWSViewCtrl)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPopupZoomwindow();
	afx_msg void OnPopupProperties();
	afx_msg void OnPopupCopy();
	afx_msg void OnSaveAS();
	afx_msg void OnPopupAutoscale();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnZoomout();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnAboutBox();
	afx_msg void OnMeasure();
	afx_msg void OnUpdateMeasure(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
// Dispatch maps
	//{{AFX_DISPATCH(CWSViewCtrl)
	long m_displayLength;
	afx_msg void OnDisplayLengthChanged();
	BOOL m_pointDisplay;
	afx_msg void OnPointDisplayChanged();
	afx_msg void SetTimeRange(long lStartTime, long lEndTime);
	afx_msg short AddPilot(LPCTSTR strContestNo, LPCTSTR strLastName, LPCTSTR strFirstName, short sClass);
	afx_msg BOOL SetCurrentTimeIndex(short sTime);
	afx_msg void SetPilotVisibility(LPCTSTR strContestNo, BOOL bVisible);
	afx_msg void SetViewScale(double dScale);
	afx_msg void SetViewCenter(float dLat, float dLong);
	afx_msg void AddControlPoint(short sID, LPCTSTR strName, float fLatitude, float fLongitude, short sAltitude, short sAttribute);
	afx_msg void SetStartGateCylinder(float fGateLatitude, float fGateLongitude,  float fGateSize,  short sGateHeight, short sGateBase);
	afx_msg void SetFinishGateCylinder(float fGateLatitude, float fGateLongitude,  float fGateSize,  short sGateHeight, short sGateBase);
	afx_msg void ClearTask();
	afx_msg void AddTaskPoint(short sControlPoint);
	afx_msg void SetDisplayOption(long lOption, short sValue);
	afx_msg void AddPosition(short sPilotIndex, long lTimeIndex, float fLatitude, float fLongitude, long lAltitude, short sMotor);
	afx_msg void SetWorldLimits(float fLatitudeBottom, float fLatitudeTop, float fLongitudeLeft, float fLongidudeRight, long lMinAltitude, long lMaxAltitude);
	afx_msg void SetColor(short sOption, long lColor);
	afx_msg void SetPilotTraceColor(LPCTSTR strContestNo, long lColor);
	afx_msg void ResetAll();
	afx_msg void ResetContestants();
	afx_msg void Autoscale();
	afx_msg void AutoscaleTask();
	afx_msg void InitSite(float fLatitude, float fLongitude, short iAltitude);
	afx_msg void SetStartGateLine(float fLat1, float flong1, float fLat2, float fLong2, long iStartCeiling);
	afx_msg void SetFinishGateLine(float fLat1, float fLong1, float fLat2, float fLong2, long iFinishGateBase);
	afx_msg void SetPilotTimes(short sPilotID, long lSTime, long lFTime);
	afx_msg void SetFinishTime(short sPilotID, long lTime);
	afx_msg void AddPositionByName(LPCTSTR strContestNo, long lTime, float fLat, float fLong, short sAltitude);
	afx_msg void SetCurrentTime(long lTime);
	afx_msg void AddFixes(const VARIANT FAR& vlTime, const VARIANT FAR& vstrContestNo, const VARIANT FAR& vfLatitude, const VARIANT FAR& vfLongidude, const VARIANT FAR& vsAltitude);
	afx_msg void SetMinMaxAltitudes(short sMinAltitude, short sMaxAltitude);
	afx_msg BOOL IsPilotVisible(LPCTSTR strContestNo);
	afx_msg void SetTraceDisplayLength(long lDisplayLength);
	afx_msg void SetActiveClass(short sClass);
	afx_msg long GetColor(short sOption);
	afx_msg void OnEditProperties();
	afx_msg void SetPilotText(LPCTSTR strContestNo, LPCTSTR strTitle);
	afx_msg void SetRotations(float fRX, float fRY, short fRZ);
	afx_msg void SetZScale(float SetZScale);
	afx_msg void NudgeView(short sViewType);
	afx_msg float GetZScale();
	afx_msg long GetPilotColor(short sPilotIndex);
	afx_msg void ScaleView(float fScale);
	afx_msg short GetDisplayOption(long lOption);
	afx_msg void SetSceneryPath(LPCTSTR strPath);
	afx_msg void InvalidateGrid();
	afx_msg void ZoomWindow();
	afx_msg void AddTaskLocation(short iPointID, float fLat, float fLong, LPCTSTR strComment);
	afx_msg void SetDisplayOptionD(long lOption, double dValue);
	afx_msg double GetDisplayOptionD(long lOption);
	afx_msg void AddSUAirspace(LPCTSTR strAirspace);
	afx_msg void DisplayNextTaskPoint();
	afx_msg void SetTaskPointRadius(short sTaskPointID, float fRadius);
	afx_msg void AddTATLocation(short sPointID, float fLat, float fLong);
	afx_msg void DefineControlPointAttributes(long iTurnpoint, long iAirport, long iLandable, long iStart, long iFinish, long iHome, long iReserved1, long iReserved2, long iReserved3);
	afx_msg BSTR GetVersionString();
	afx_msg BSTR GetBuildDate();
	afx_msg void OnMeasureCursor();
	afx_msg long GetCurrentDisplayTime();
	afx_msg void Set2ndStartGateCylinder(float fGateLatitude, float fGateLongitude, float fGateSize, short sGateHeight, short sGateBase);
	afx_msg BSTR GetDisplayOptionStr(long lOption);
	afx_msg void SetDisplayOptionStr(long lOption, LPCTSTR strValue);
	afx_msg void SetStartTime(short sPilotID, long lTime);
	afx_msg void SetTATLocation(short sTaskPointID, float fLat, float fLong);
	afx_msg boolean WriteBMPFile(LPCTSTR cFileName);	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CWSViewCtrl)
	void FirePilotSelection(LPCTSTR strContestNo)
		{FireEvent(eventidPilotSelection,EVENT_PARAM(VTS_BSTR), strContestNo);}
	void FireOnKeyDown(short nChar)
		{FireEvent(eventidOnKeyDown,EVENT_PARAM(VTS_I2), nChar);}
	void FireUpdateProperties()
		{FireEvent(eventidUpdateProperties,EVENT_PARAM(VTS_NONE));}
	void FirePointSelected(short sOpt, long lTime)
		{FireEvent(eventidPointSelected,EVENT_PARAM(VTS_I2  VTS_I4), sOpt, lTime);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	CFont* SelectFontByScale(CDC* pdc);
	void CreateFonts();
	bool m_b2ndStartGate;
	bool m_bEnableControlPointHighlight;
	CString GetUnitsText();
	void ZoomOnTaskPoint();
	CControlPoint* GetTaskPoint(int iPoint);
	CControlPoint* GetTATPoint(int iPoint);
	void ExpandWindow(float fLatitude, float fLongitude, float &fXMin, float &fXMax, float &fYMin, float &YMax, int iToler=1);

	CControlPoint*	m_pHighlightedControlPoint;
	CControlPoint*	GetClosestControlPoint( CPoint &cPoint, int iToler=3);

	CHitPoint*		m_pcHit;
	EUnits			m_eUnits;
	EAltUnits		m_eAltUnits;
	bool			m_bMeasureMode;

	CHitPoint*		m_pcMeasureStartHitPoint;
	CHitPoint*		m_pcMeasureDoneHitPoint;
	CPoint			m_cMeasureEndPoint;
	CHitList		m_cHitPointList;

	CMeasuringDlg*	m_pMeasuringDlg;
	CMeasuringDlg*	m_pNavpointPropertiesDlg;

	void DrawScale(CDC *pDC);
	void DrawData(CDC *pdc, const CRect &rcBounds);
	void LoadVertex(CDC *pDC, C3DTransformation &c3DTransform, double dLat, double dLong, double dAltitude, int iSize, COLORREF crColor);
	bool IsGridVisible(C3DTransformation &c3DTrans, short sLat, short sLong );

	void LoadGrid(CDC *pDC, C3DTransformation c3DTransformation );
	void LoadAirspace(CDC *pDC, C3DTransformation &c3DTrans );

	EClass m_eActiveClass;
	void PurgeControlPoints();
	short m_sWorldAltMax;
	short m_sWorldAltMin;
	CControlPoint* GetControlPoint(int i);
	void LoadTask(CDC *pDC, C3DTransformation &c3DTrans);
	void LoadDisk(CDC *pDC, C3DTransformation &c3DTrans, float dLat, float dLong, short sElevation,  float dDiameter, int style, COLORREF crColor  );
	float m_fControlPointRadius;
	float m_fControlPointOuterRadius;
	void LoadControlPoints(CDC *pDC, C3DTransformation &c3DTrans );

	CPoint m_cMousePanPoint;

	CTraceGroup m_cTraceGroup;

	CPtrArray m_caAirspaceSections;

	bool	m_bTexturePresent;
	CString m_strSeneryPath;
    bool m_bCopy;
	bool  m_bWrite;
	CString m_strWriteFileName;


	CTime m_cMinTime;
	CTime m_cMaxTime;
	CTime m_cTimeIndex;
	short m_sTimeIndex;

	float m_fWorldXMin;
	float m_fWorldXMax;
	float m_fWorldYMin;
	float m_fWorldYMax;

	float m_fCPXMin;
	float m_fCPXMax;
	float m_fCPYMin;
	float m_fCPYMax;

	float m_fTraceXMin;
	float m_fTraceXMax;
	float m_fTraceYMin;
	float m_fTraceYMax;

	CPtrList   m_ControlPointList;
	CPtrList   m_TaskPointList;
	CPtrList   m_TATPointList;
	int		   m_iTaskPtr;
	float	   m_fTaskPointRadius[MAXTASKPOINTS];

	float m_fStartGateLatitude;
	float m_fStartGateLongitude;
	float m_fStartGateLatitude2;
	float m_fStartGateLongitude2;
	EGateType m_eStartGateType;
	float m_fStartGateSize;
	short m_sStartGateHeight;
	short m_sStartGateBase;

	float m_f2ndStartGateLatitude;
	float m_f2ndStartGateLongitude;
	float m_f2ndStartGateSize;
	short m_s2ndStartGateHeight;
	short m_s2ndStartGateBase;

	float m_fFinishGateLatitude;
	float m_fFinishGateLongitude;
	EGateType m_eFinishGateType;
	float m_fFinishGateLatitude2;
	float m_fFinishGateLongitude2;

	float m_fFinishGateSize;
	short m_sFinishGateHeight;
	short m_sFinishGateBase;

	COLORREF m_crWaypointColor;
	COLORREF m_crClimbColor;
	COLORREF m_crBackgroundColor;
	COLORREF m_crStartRadiusColor;
	COLORREF m_crFinishRadiusColor;
	COLORREF m_crTaskColor;
	COLORREF m_crAirspaceColor;

	int	m_iDisplayOptions;

	short m_sRotatingDefault;

	C3DTransformation m_c3DTransformation;

	CSemaphore		m_cSemaphore;

    XYZoom *pxyzoom;

	HCURSOR   m_hHandCursor, m_hRotateCursor, m_hMeasureCursor;

	int		m_iHighlightedPilot;
	bool	m_bHighlightLock;


	 long m_iTurnpointAttribute;
	 long m_iAirportAttribute;
	 long m_iLandableAttribute;
	 long m_iStartAttribute;
	 long m_iFinishAttribute;
	 long m_iHomeAttribute;

	CString m_strFontName;
	CFont *m_pFonts[WSNUMFONTS];

	enum {

	//{{AFX_DISP_ID(CWSViewCtrl)
	dispidDisplayLength = 1L,
	dispidPointDisplay = 2L,
	dispidSetTimeRange = 3L,
	dispidAddPilot = 4L,
	dispidSetCurrentTimeIndex = 5L,
	dispidSetPilotVisibility = 6L,
	dispidSetViewScale = 7L,
	dispidSetViewCenter = 8L,
	dispidAddControlPoint = 9L,
	dispidSetStartGateCylinder = 10L,
	dispidSetFinishGateCylinder = 11L,
	dispidClearTask = 12L,
	dispidAddTaskPoint = 13L,
	dispidSetDisplayOption = 14L,
	dispidAddPosition = 15L,
	dispidSetWorldLimits = 16L,
	dispidSetColor = 17L,
	dispidSetPilotTraceColor = 18L,
	dispidResetAll = 19L,
	dispidResetContestants = 20L,
	dispidAutoscale = 21L,
	dispidSetDisplayOptionD = 48L,
	dispidInitSite = 22L,
	dispidSetStartGateLine = 23L,
	dispidSetFinishGateLine = 24L,
	dispidSetPilotTimes = 25L,
	dispidSetFinishTime = 26L,
	dispidAddPositionByName = 27L,
	dispidSetCurrentTime = 28L,
	dispidAddFixes = 29L,
	dispidSetMinMaxAltitudes = 30L,
	dispidIsPilotVisible = 31L,
	dispidSetTraceDisplayLength = 32L,
	dispidSetActiveClass = 33L,
	dispidGetDisplayOptionD = 49L,
	dispidGetColor = 34L,
	dispidOnEditProperties = 35L,
	dispidSetPilotText = 36L,
	dispidSetRotations = 37L,
	dispidSetZScale = 38L,
	dispidNudgeView = 39L,
	dispidGetZScale = 40L,
	dispidGetPilotColor = 41L,
	dispidScaleView = 42L,
	dispidGetDisplayOption = 43L,
	dispidSetSceneryPath = 44L,
	dispidInvalidateGrid = 45L,
	dispidZoomWindow = 46L,
	dispidAddTaskLocation = 47L,
	dispidAddSUAirspace = 50L,
	dispidDisplayNextTaskPoint = 51L,
	dispidSetTaskPointRadius = 52L,
	dispidAddTATLocation = 53L,
	dispidDefineControlPointAttributes = 54L,
	dispidGetVersionString = 55L,
	dispidGetBuildDate = 56L,
	dispidOnMeasureCursor = 57L,
	dispidGetCurrentDisplayTime = 58L,
	dispidSet2ndStartGateCylinder = 59L,
	dispidGetDisplayOptionStr = 60L,
	dispidSetDisplayOptionStr = 61L,
	dispidSetStartTime = 62L,
	dispidSetTATLocation = 63L,
	dispidWriteBMPFile = 64L,
	eventidPilotSelection = 1L,
	eventidOnKeyDown = 2L,
	eventidUpdateProperties = 3L,
	eventidPointSelected = 4L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSVIEWCTL_H__E0748853_5814_11D3_BA96_0010A4043281__INCLUDED)

