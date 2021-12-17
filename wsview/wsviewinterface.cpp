//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "wsviewinterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// _DWSView properties

long _DWSView::GetDisplayLength()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void _DWSView::SetDisplayLength(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

BOOL _DWSView::GetPointDisplay()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void _DWSView::SetPointDisplay(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// _DWSView operations

void _DWSView::SetTimeRange(long lStartTime, long lEndTime)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lStartTime, lEndTime);
}

short _DWSView::AddPilot(LPCTSTR strContestNo, LPCTSTR strLastName, LPCTSTR strFirstName, short sClass)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strContestNo, strLastName, strFirstName, sClass);
	return result;
}

BOOL _DWSView::SetCurrentTimeIndex(short sTime)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		sTime);
	return result;
}

void _DWSView::SetPilotVisibility(LPCTSTR strContestNo, BOOL bVisible)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strContestNo, bVisible);
}

void _DWSView::SetViewScale(double dScale)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dScale);
}

void _DWSView::SetViewCenter(float dLat, float dLong)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dLat, dLong);
}

void _DWSView::AddControlPoint(short sID, LPCTSTR strName, float fLatitude, float fLongitude, short sAltitude, short sAttribute)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_R4 VTS_R4 VTS_I2 VTS_I2;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sID, strName, fLatitude, fLongitude, sAltitude, sAttribute);
}

void _DWSView::SetStartGateCylinder(float fGateLatitude, float fGateLongitude, float fGateSize, short sGateHeight, short sGateBase)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_I2 VTS_I2;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 fGateLatitude, fGateLongitude, fGateSize, sGateHeight, sGateBase);
}

void _DWSView::SetFinishGateCylinder(float fGateLatitude, float fGateLongitude, float fGateSize, short sGateHeight, short sGateBase)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_I2 VTS_I2;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 fGateLatitude, fGateLongitude, fGateSize, sGateHeight, sGateBase);
}

void _DWSView::ClearTask()
{
	InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DWSView::AddTaskPoint(short sControlPoint)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sControlPoint);
}

void _DWSView::SetDisplayOption(long lOption, short sValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lOption, sValue);
}

void _DWSView::AddPosition(short sPilotIndex, long lTimeIndex, float fLatitude, float fLongitude, long lAltitude, short sMotor)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_R4 VTS_R4 VTS_I4 VTS_I2;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sPilotIndex, lTimeIndex, fLatitude, fLongitude, lAltitude, sMotor);
}

void _DWSView::SetWorldLimits(float fLatitudeBottom, float fLatitudeTop, float fLongitudeLeft, float fLongidudeRight, long lMinAltitude, long lMaxAltitude)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4 VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 fLatitudeBottom, fLatitudeTop, fLongitudeLeft, fLongidudeRight, lMinAltitude, lMaxAltitude);
}

void _DWSView::SetColor(short sOption, long lColor)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sOption, lColor);
}

void _DWSView::SetPilotTraceColor(LPCTSTR strContestNo, long lColor)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strContestNo, lColor);
}

void _DWSView::ResetAll()
{
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DWSView::ResetContestants()
{
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DWSView::Autoscale()
{
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DWSView::SetDisplayOptionD(long lOption, double dValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_R8;
	InvokeHelper(0x30, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lOption, dValue);
}

void _DWSView::InitSite(float fLatitude, float fLongitude, short iAltitude)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_I2;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 fLatitude, fLongitude, iAltitude);
}

void _DWSView::SetStartGateLine(float fLat1, float flong1, float fLat2, float fLong2, long iStartCeiling)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 fLat1, flong1, fLat2, fLong2, iStartCeiling);
}

void _DWSView::SetFinishGateLine(float fLat1, float flong1, float fLat2, float fLong2, long iFinishGateBase)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_I4;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 fLat1, flong1, fLat2, fLong2, iFinishGateBase);
}

void _DWSView::SetPilotTimes(short sPilotID, long lSTime, long lFTime)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sPilotID, lSTime, lFTime);
}

void _DWSView::SetFinishTime(short sPilotID, long lTime)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sPilotID, lTime);
}

void _DWSView::AddPositionByName(LPCTSTR strContestNo, long lTime, float fLatitude, float fLongitude, short sAltitude)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_R4 VTS_R4 VTS_I2;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strContestNo, lTime, fLatitude, fLongitude, sAltitude);
}

void _DWSView::SetCurrentTime(long lTime)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lTime);
}

void _DWSView::AddFixes(const VARIANT& vlTime, const VARIANT& vstrContestNo, const VARIANT& vfLatitude, const VARIANT& vfLongidude, const VARIANT& vsAltitude)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &vlTime, &vstrContestNo, &vfLatitude, &vfLongidude, &vsAltitude);
}

void _DWSView::SetMinMaxAltitudes(short sMinAltitude, short sMaxAltitude)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sMinAltitude, sMaxAltitude);
}

BOOL _DWSView::IsPilotVisible(LPCTSTR strContestNo)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strContestNo);
	return result;
}

void _DWSView::SetTraceDisplayLength(long lDisplayLength)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lDisplayLength);
}

void _DWSView::SetActiveClass(short sClass)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sClass);
}

double _DWSView::GetDisplayOptionD(long lOption)
{
	double result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x31, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		lOption);
	return result;
}

long _DWSView::GetColor(short sOption)
{
	long result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		sOption);
	return result;
}

void _DWSView::OnEditProperties()
{
	InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DWSView::SetPilotText(LPCTSTR strContestNo, LPCTSTR strTitle)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strContestNo, strTitle);
}

void _DWSView::SetRotations(float fRX, float fRY, short fRZ)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_I2;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 fRX, fRY, fRZ);
}

void _DWSView::SetZScale(float SetZScale)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SetZScale);
}

void _DWSView::NudgeView(short sViewType)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sViewType);
}

float _DWSView::GetZScale()
{
	float result;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_R4, (void*)&result, NULL);
	return result;
}

long _DWSView::GetPilotColor(short sPilotIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		sPilotIndex);
	return result;
}

void _DWSView::ScaleView(float fScale)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 fScale);
}

short _DWSView::GetDisplayOption(long lOption)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2b, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		lOption);
	return result;
}

void _DWSView::SetSceneryPath(LPCTSTR strPath)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strPath);
}

void _DWSView::InvalidateGrid()
{
	InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DWSView::ZoomWindow()
{
	InvokeHelper(0x2e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DWSView::AddTaskLocation(short iPointID, float fLat, float fLong, LPCTSTR strComment)
{
	static BYTE parms[] =
		VTS_I2 VTS_R4 VTS_R4 VTS_BSTR;
	InvokeHelper(0x2f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 iPointID, fLat, fLong, strComment);
}

void _DWSView::AddSUAirspace(LPCTSTR strAirspace)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strAirspace);
}

void _DWSView::DisplayNextTaskPoint()
{
	InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DWSView::SetTaskPointRadius(short sTaskPointID, float fRadius)
{
	static BYTE parms[] =
		VTS_I2 VTS_R4;
	InvokeHelper(0x34, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sTaskPointID, fRadius);
}

void _DWSView::AddTATLocation(short sPointID, float fLat, float fLong)
{
	static BYTE parms[] =
		VTS_I2 VTS_R4 VTS_R4;
	InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sPointID, fLat, fLong);
}

void _DWSView::DefineControlPointAttributes(long iTurnpoint, long iAirport, long iLandable, long iStart, long iFinish, long iHome, long iReserved1, long iReserved2, long iReserved3)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 iTurnpoint, iAirport, iLandable, iStart, iFinish, iHome, iReserved1, iReserved2, iReserved3);
}

CString _DWSView::GetVersionString()
{
	CString result;
	InvokeHelper(0x37, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _DWSView::GetBuildDate()
{
	CString result;
	InvokeHelper(0x38, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _DWSView::OnMeasureCursor()
{
	InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _DWSView::GetCurrentDisplayTime()
{
	long result;
	InvokeHelper(0x3a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void _DWSView::Set2ndStartGateCylinder(float fGateLatitude, float fGateLongitude, float fGateSize, short sGateHeight, short sGateBase)
{
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_I2 VTS_I2;
	InvokeHelper(0x3b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 fGateLatitude, fGateLongitude, fGateSize, sGateHeight, sGateBase);
}

CString _DWSView::GetDisplayOptionStr(long lOption)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		lOption);
	return result;
}

void _DWSView::SetDisplayOptionStr(long lOption, LPCTSTR strValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x3d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lOption, strValue);
}


/////////////////////////////////////////////////////////////////////////////
// _DWSViewEvents properties

/////////////////////////////////////////////////////////////////////////////
// _DWSViewEvents operations

void _DWSViewEvents::PilotSelection(LPCTSTR strContestNo)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strContestNo);
}

void _DWSViewEvents::OnKeyDown(short nChar)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nChar);
}

void _DWSViewEvents::UpdateProperties()
{
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DWSViewEvents::PointSelected(short sOpt, long lTime)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sOpt, lTime);
}

void _DWSView::AutoscaleTask()
{
	InvokeHelper(0x41, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}