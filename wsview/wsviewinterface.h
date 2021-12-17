//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#if !defined(AFX_WSVIEW_H)
#define AFX_WSVIEW_H



/////////////////////////////////////////////////////////////////////////////
// _DWSBaro wrapper class

class _DWSBaro : public COleDispatchDriver
{
public:
	_DWSBaro() {}		// Calls COleDispatchDriver default constructor
	_DWSBaro(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_DWSBaro(const _DWSBaro& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	short AddPilot(LPCTSTR strContestNo, LPCTSTR strLastName, LPCTSTR strFirstName, short sClass, long lColorREF);
	void SetCursorPos(long lCursorTime);
	void AddPoint(short sPilotIndes, long lTimeIndex, short sGPSAltitude, short sPressureAltitude);
	void SetTimeRange(long lStartTime, long lEndTime);
	void AddPoints(const VARIANT& vlTime, const VARIANT& vstrContestNo, const VARIANT& vsPressureAlt, const VARIANT& vsGPSAlt);
	void ResetAll();
	void AddPointByName(LPCTSTR strContestNo, long lTimeIndex, short sGPSAltitude, short sPressureAltitude);
	void OnControlKey(short nChar);
	void SetTaskOpenTime(long lTime);
	void RedrawBaro();
	void SetPilotVisibility(LPCTSTR strContestNo, BOOL bVisible);
	long GetCursorPos();
	BOOL IsCursorEnd();
	void CursorHome();
	void AddFullPoint(short sPilotIndex, long lTimeIndex, short sGPSAltitude, short sPressAltitude, float fLat, float fLong, float fSpeed, float fVSpeed, short sENL);
	void SetDisplayOption(long lOption, short sValue);
	void AboutBox();
};
/////////////////////////////////////////////////////////////////////////////
// _DWSBaroEvents wrapper class

class _DWSBaroEvents : public COleDispatchDriver
{
public:
	_DWSBaroEvents() {}		// Calls COleDispatchDriver default constructor
	_DWSBaroEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_DWSBaroEvents(const _DWSBaroEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void OnMoveCursor(long lNewTime);
	void UpdateText(LPCTSTR strTime, LPCTSTR strPosition, LPCTSTR strGPSAlt, LPCTSTR strPressAlt, LPCTSTR strSpeed, LPCTSTR strVSpeed);
};
/////////////////////////////////////////////////////////////////////////////
// _DWSView wrapper class

class _DWSView : public COleDispatchDriver
{
public:
	_DWSView() {}		// Calls COleDispatchDriver default constructor
	_DWSView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_DWSView(const _DWSView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetDisplayLength();
	void SetDisplayLength(long);
	BOOL GetPointDisplay();
	void SetPointDisplay(BOOL);

// Operations
public:
	void SetTimeRange(long lStartTime, long lEndTime);
	short AddPilot(LPCTSTR strContestNo, LPCTSTR strLastName, LPCTSTR strFirstName, short sClass);
	BOOL SetCurrentTimeIndex(short sTime);
	void SetPilotVisibility(LPCTSTR strContestNo, BOOL bVisible);
	void SetViewScale(double dScale);
	void SetViewCenter(float dLat, float dLong);
	void AddControlPoint(short sID, LPCTSTR strName, float fLatitude, float fLongitude, short sAltitude, short sAttribute);
	void SetStartGateCylinder(float fGateLatitude, float fGateLongitude, float fGateSize, short sGateHeight, short sGateBase);
	void SetFinishGateCylinder(float fGateLatitude, float fGateLongitude, float fGateSize, short sGateHeight, short sGateBase);
	void ClearTask();
	void AddTaskPoint(short sControlPoint);
	void SetDisplayOption(long lOption, short sValue);
	void AddPosition(short sPilotIndex, long lTimeIndex, float fLatitude, float fLongitude, long lAltitude, short sMotor);
	void SetWorldLimits(float fLatitudeBottom, float fLatitudeTop, float fLongitudeLeft, float fLongidudeRight, long lMinAltitude, long lMaxAltitude);
	void SetColor(short sOption, long lColor);
	void SetPilotTraceColor(LPCTSTR strContestNo, long lColor);
	void ResetAll();
	void ResetContestants();
	void Autoscale();
	void SetDisplayOptionD(long lOption, double dValue);
	void InitSite(float fLatitude, float fLongitude, short iAltitude);
	void SetStartGateLine(float fLat1, float flong1, float fLat2, float fLong2, long iStartCeiling);
	void SetFinishGateLine(float fLat1, float flong1, float fLat2, float fLong2, long iFinishGateBase);
	void SetPilotTimes(short sPilotID, long lSTime, long lFTime);
	void SetFinishTime(short sPilotID, long lTime);
	void AddPositionByName(LPCTSTR strContestNo, long lTime, float fLatitude, float fLongitude, short sAltitude);
	void SetCurrentTime(long lTime);
	void AddFixes(const VARIANT& vlTime, const VARIANT& vstrContestNo, const VARIANT& vfLatitude, const VARIANT& vfLongidude, const VARIANT& vsAltitude);
	void SetMinMaxAltitudes(short sMinAltitude, short sMaxAltitude);
	BOOL IsPilotVisible(LPCTSTR strContestNo);
	void SetTraceDisplayLength(long lDisplayLength);
	void SetActiveClass(short sClass);
	double GetDisplayOptionD(long lOption);
	long GetColor(short sOption);
	void OnEditProperties();
	void SetPilotText(LPCTSTR strContestNo, LPCTSTR strTitle);
	void SetRotations(float fRX, float fRY, short fRZ);
	void SetZScale(float SetZScale);
	void NudgeView(short sViewType);
	float GetZScale();
	long GetPilotColor(short sPilotIndex);
	void ScaleView(float fScale);
	short GetDisplayOption(long lOption);
	void SetSceneryPath(LPCTSTR strPath);
	void InvalidateGrid();
	void ZoomWindow();
	void AddTaskLocation(short iPointID, float fLat, float fLong, LPCTSTR strComment);
	void AddSUAirspace(LPCTSTR strAirspace);
	void DisplayNextTaskPoint();
	void SetTaskPointRadius(short sTaskPointID, float fRadius);
	void AddTATLocation(short sPointID, float fLat, float fLong);
	void DefineControlPointAttributes(long iTurnpoint, long iAirport, long iLandable, long iStart, long iFinish, long iHome, long iReserved1, long iReserved2, long iReserved3);
	CString GetVersionString();
	CString GetBuildDate();
	void OnMeasureCursor();
	long GetCurrentDisplayTime();
	void Set2ndStartGateCylinder(float fGateLatitude, float fGateLongitude, float fGateSize, short sGateHeight, short sGateBase);
	CString GetDisplayOptionStr(long lOption);
	void SetDisplayOptionStr(long lOption, LPCTSTR strValue);
	void AutoscaleTask();
};
/////////////////////////////////////////////////////////////////////////////
// _DWSViewEvents wrapper class

class _DWSViewEvents : public COleDispatchDriver
{
public:
	_DWSViewEvents() {}		// Calls COleDispatchDriver default constructor
	_DWSViewEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_DWSViewEvents(const _DWSViewEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void PilotSelection(LPCTSTR strContestNo);
	void OnKeyDown(short nChar);
	void UpdateProperties();
	void PointSelected(short sOpt, long lTime);
};

#endif