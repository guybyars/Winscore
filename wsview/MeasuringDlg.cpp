//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// MeasuringDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wsview.h"
#include "WSViewCtl.h"
#include "MeasuringDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeasuringDlg dialog


CMeasuringDlg::CMeasuringDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasuringDlg::IDD, pParent)
	{
	//{{AFX_DATA_INIT(CMeasuringDlg)
	m_strText = _T("");
	m_strSpeed = _T("");
	m_strTime = _T("");
	m_strAveROC = _T("");
	m_strAltitude = _T("");
	m_strTurnpoint = _T("");
	//}}AFX_DATA_INIT

	m_dMeasureDistance	=0.0;
	m_cMeasureTimeSpan	=CTimeSpan(0);
	m_iMeasureAltitude	=0;
	m_bMeasurmentMade	=false;
	}


void CMeasuringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasuringDlg)
	DDX_Text(pDX, IDC_OUTPUT, m_strText);
	DDX_Text(pDX, IDC_SPEED, m_strSpeed);
	DDX_Text(pDX, IDC_TIME, m_strTime);
	DDX_Text(pDX, IDC_AVEROC, m_strAveROC);
	DDX_Text(pDX, IDC_ALTITUDE, m_strAltitude);
	DDX_Text(pDX, IDC_TURNPOINT_TEXT, m_strTurnpoint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasuringDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasuringDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasuringDlg message handlers

void CMeasuringDlg::ResetText()
{
	m_strText="Dist: 0.000";
	m_strSpeed="";
	m_strTime="";
	m_strAveROC="";
	m_strAltitude="";
	UpdateData(false);
}

void CMeasuringDlg::UpdateRunningTotals(CHitPoint *pHit1, CHitPoint *pHit2, EUnits eUnits)
{
	if( !pHit1 || !pHit2 ) return;

	CLocation cLoc1, cLoc2;
	pHit1->GetLocation(cLoc1);
	pHit2->GetLocation(cLoc2);

	m_dMeasureDistance+=cLoc1.DistanceTo(&cLoc2, eUnits);

	long lTime1=pHit1->GetTime();
	long lTime2=pHit2->GetTime();
	if( (lTime1>0 && lTime2>0) && lTime1!=lTime2 )
		{
		CTime cTime1(min(lTime1, lTime2));
		CTime cTime2(max(lTime1, lTime2));
		CTimeSpan cSpan=cTime2-cTime1;
		m_cMeasureTimeSpan+=cSpan;
		}
	m_bMeasurmentMade	=true;
}

void CMeasuringDlg::UpdateTotals(CHitPoint *pStart, CPoint &cEndPoint, C3DTransformation &c3DTransformation, CHitPoint *pEnd, EUnits eUnits, BOOL bAltitudeMeters, CString strUnitsText)
	{

	CLocation cLoc1, cLoc2;
	pStart->GetLocation(cLoc1);

	if( pStart->m_ePointType==CHitPoint::eNavPoint ) 
		{
		m_strTurnpoint=pStart->GetPointName();
		}

	if( !pEnd )
		{
		double dX, dY,dZ;
		c3DTransformation.LogicalToWorld(cEndPoint, dX, dY, dZ );
		cLoc2.SetLat(dY);
		cLoc2.SetLong(dX);
		if( !m_bMeasurmentMade) 
			{
			m_strSpeed="";
			m_strTime="";
			m_strAltitude="";
			m_strAveROC="";
			}
		m_strTurnpoint=pStart->GetPointName();

		}
	else
		{
		pEnd->GetLocation(cLoc2);
		if( pEnd->m_ePointType==CHitPoint::eNavPoint ) 
			m_strTurnpoint=pEnd->GetPointName();

		}

	double dDist=m_dMeasureDistance+cLoc1.DistanceTo(&cLoc2, eUnits);

	CString strText;
	strText.Format( "Dist: %6.3lf %s", dDist, strUnitsText);
	m_strText=strText;

	if( pEnd )
		{
		long lTime1=pStart->GetTime();
		long lTime2=pEnd->GetTime();
		if( (lTime1>0 && lTime2>0) && lTime1!=lTime2 )
			{
			CTime cTime1(min(lTime1, lTime2));
			CTime cTime2(max(lTime1, lTime2));
			CTimeSpan cSpan=cTime2-cTime1;
			cSpan+=m_cMeasureTimeSpan;
			if( lTime1>lTime2 )
				m_strTime=cSpan.Format("Time : -%H:%M:%S");
			else
				m_strTime=cSpan.Format("Time : %H:%M:%S");

			double dSpeed=dDist/((double)cSpan.GetTotalSeconds());
			dSpeed*=60.*60.;
			m_strSpeed.Format("Speed: %5.1lf %s/Hr", dSpeed, strUnitsText);

			float fAlt1=(float)m_iMeasureAltitude;
			float fAlt2=(float)pEnd->GetAltitude();
			float fDelta=0;

			if( bAltitudeMeters )
				{
				fDelta=(float)((fAlt2-fAlt1)/m_to_ft);
				m_strAltitude.Format("Alt Diff.: %4.1lf m", fDelta );
				m_strAveROC.Format("Ave Climb.: %4.1lf m/s", fDelta/cSpan.GetTotalSeconds() );
				}
			else
				{
				fDelta=(fAlt2-fAlt1);
				m_strAltitude.Format("Alt Diff.: %4.0lf ft", fDelta);
				m_strAveROC.Format("Ave Climb.: %4.0lf ft/min", (fDelta/cSpan.GetTotalSeconds())*60. );
				}
			}
		}

	UpdateData(false);
}

void CMeasuringDlg::SetInitialAltitude(int iAlt)
	{
	if( !m_bMeasurmentMade )
		m_iMeasureAltitude=iAlt;

	}

void CMeasuringDlg::UpdateTurnpointText(CHitPoint *pcHit)
	{
	if( pcHit && pcHit->m_ePointType==CHitPoint::eNavPoint )
		{
		m_strTurnpoint=pcHit->GetPointName();
		}
	else
		m_strTurnpoint="";
	UpdateData(false);
	}


void CMeasuringDlg::DisplayControlPointInfo(CControlPoint *pPoint, EAltUnits eAltUnits)
	{
	CString strLat;
	CString strLon;
	double dMin;

	if( !pPoint ) return;

	m_strText.Format("%d - %s", pPoint->m_iID,  pPoint->m_strName);

	if( eAltUnits==eMeters )
		m_strTime.Format("Elevaton : %4.1lf m", (float)(pPoint->m_iAltitude)/m_to_ft );
	else
		m_strTime.Format("Elevaton : %d ft", pPoint->m_iAltitude );

	double dDeg=modf( pPoint->m_fLat, &dMin );
	(pPoint->m_fLat<0) ? (strLat="S") : (strLat="N");
	m_strSpeed.Format("%2.0f° %06.3lf %s", 
		fabs(dMin),
		fabs(dDeg*60.), 
		strLat);

	dDeg=modf( pPoint->m_fLong, &dMin );
	(pPoint->m_fLong<0) ? (strLon="W") : (strLon="E");
	m_strAltitude.Format("%3.0f° %06.3lf %s", 
		fabs(dMin),
		fabs(dDeg*60.), 
		strLon);

	m_strAveROC="Attributes: "+pPoint->m_strAttributeText;

	UpdateData(false);
	}
