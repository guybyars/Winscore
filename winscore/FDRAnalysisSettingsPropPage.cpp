//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FDRAnalysisSettingsPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "FDRAnalysisSettingsPropPage.h"
#include "FDRSettingsPropSheet.h"
#include "global_prototypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFDRAnalysisSettingsPropPage property page

IMPLEMENT_DYNCREATE(CFDRAnalysisSettingsPropPage, CPropertyPage)

CFDRAnalysisSettingsPropPage::CFDRAnalysisSettingsPropPage() : CPropertyPage(CFDRAnalysisSettingsPropPage::IDD)
{
	//{{AFX_DATA_INIT(CFDRAnalysisSettingsPropPage)
	m_dInnerRadius = 0.0;
	m_dOuterRadius = 0.0;
	m_cUnitsText1 = _T("");
	m_cUnitsText2 = _T("");
	m_iTimeZoneOption = -1;
	m_iUserTimezone = 0;
	//}}AFX_DATA_INIT
}

CFDRAnalysisSettingsPropPage::~CFDRAnalysisSettingsPropPage()
{
}

void CFDRAnalysisSettingsPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFDRAnalysisSettingsPropPage)
	DDX_Control(pDX, IDC_LOCAL_TIMEZONE_EDIT, m_cUserTimezone);
	DDX_Text(pDX, IDC_INNER_RADIUS, m_dInnerRadius);
	DDV_MinMaxDouble(pDX, m_dInnerRadius, 0., 1000.);
	DDX_Text(pDX, IDC_OUTER_RADIUS, m_dOuterRadius);
	DDV_MinMaxDouble(pDX, m_dOuterRadius, 0., 1000.);
	DDX_Text(pDX, IDC_UNITS1, m_cUnitsText1);
	DDX_Text(pDX, IDC_UNITS2, m_cUnitsText2);
	DDX_Radio(pDX, IDC_SYSTEMRADIO, m_iTimeZoneOption);
	DDX_Text(pDX, IDC_LOCAL_TIMEZONE_EDIT, m_iUserTimezone);
	DDV_MinMaxInt(pDX, m_iUserTimezone, -24, 24);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFDRAnalysisSettingsPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFDRAnalysisSettingsPropPage)
	ON_BN_CLICKED(IDC_USERRADIO, OnUserradio)
	ON_BN_CLICKED(IDC_SYSTEMRADIO, OnSystemradio)
	ON_BN_CLICKED(IDC_LOGRADIO, OnLogradio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFDRAnalysisSettingsPropPage message handlers

BOOL CFDRAnalysisSettingsPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	EUnits		eUnits=((CFDRSettingsPropSheet*)GetParent())->GetUnits();

	m_cUnitsText1=UnitsText(eUnits);
	m_cUnitsText2=UnitsText(eUnits);

	double dInnerRadius	= GetWinscoreDouble(INNERRADIUS, ConvertDistance(DEFAULTINNERRADIUS, eStatute, SYSTEMUNITS) );
	double dOuterRadius	= GetWinscoreDouble(OUTERRADIUS, ConvertDistance(DEFAULTOUTERRADIUS, eStatute, SYSTEMUNITS) );
	
	m_dInnerRadius=(float)ConvertDistance(dInnerRadius, SYSTEMUNITS, eUnits );
	m_dOuterRadius=(float)ConvertDistance(dOuterRadius, SYSTEMUNITS, eUnits );	

	m_iTimeZoneOption=	 GetWinscoreInt( LOGTIMEOPTION, eSystemTime );

    m_cUserTimezone.EnableWindow(m_iTimeZoneOption == eUserTime);
	
  // Get Default time zone
    struct tm newtime;        
	long lTime;
    time_t long_time;
    time( &long_time );               
    localtime_s(&newtime, &long_time ); 
	_get_timezone(&lTime);
	CTimeSpan GMTCorrection(lTime);
	if( newtime.tm_isdst!=0 ) GMTCorrection-=CTimeSpan(3600); 
	int iTimeZone=GMTCorrection.GetHours();
	m_iUserTimezone=	 GetWinscoreInt(  LOGTIMEUSERVALUE, -iTimeZone );

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFDRAnalysisSettingsPropPage::OnOK() 
{
	EUnits		eUnits=((CFDRSettingsPropSheet*)GetParent())->GetUnits();
	UpdateData(true);

	WriteWinscoreDouble(INNERRADIUS, ConvertDistance( m_dInnerRadius, eUnits, SYSTEMUNITS ) );
	WriteWinscoreDouble(OUTERRADIUS, ConvertDistance( m_dOuterRadius, eUnits, SYSTEMUNITS ) );
	
	WriteWinscoreInt(  LOGTIMEOPTION, m_iTimeZoneOption );
	if( m_iTimeZoneOption == eUserTime)
		WriteWinscoreInt(  LOGTIMEUSERVALUE, m_iUserTimezone );




	CPropertyPage::OnOK();
}

void CFDRAnalysisSettingsPropPage::OnUserradio() 
{
	m_cUserTimezone.EnableWindow(true);
}

void CFDRAnalysisSettingsPropPage::OnSystemradio() 
{
	m_cUserTimezone.EnableWindow(false);
}

void CFDRAnalysisSettingsPropPage::OnLogradio() 
{
	m_cUserTimezone.EnableWindow(false);
	
}

