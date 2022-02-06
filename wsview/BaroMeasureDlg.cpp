// BaroMeasureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wsview.h"
#include "BaroMeasureDlg.h"
#include "BaroPoint.h"
#include "WSBaroCtl.h"
#include "Location.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUM_COLUMNS  3




/////////////////////////////////////////////////////////////////////////////
// CBaroMeasureDlg dialog


CBaroMeasureDlg::CBaroMeasureDlg(	CBaroTrace *pTrace, 
									EUnits		eUnits, 
									EAltUnits	eAltUnits,
									short		uDisplayMask,
									CWnd* pParent /*=NULL*/)
	: CDialog(CBaroMeasureDlg::IDD, pParent),
	m_pTrace(pTrace),
	m_eUnits(eUnits),
	m_eAltUnits(eAltUnits),
	m_uDisplayMask(uDisplayMask)
{
	//{{AFX_DATA_INIT(CBaroMeasureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_lEnd		=0;
	m_lStart	=0;
}


void CBaroMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaroMeasureDlg)
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBaroMeasureDlg, CDialog)
	//{{AFX_MSG_MAP(CBaroMeasureDlg)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



#define ADDITEM( stritem, strname1, strname2 )	{\
  				lvi.iItem = iItem;\
				m_cListCtrl.InsertItem(&lvi);\
				m_cListCtrl.SetItemText(iItem,0,stritem);\
				m_cListCtrl.SetItemText(iItem,1, strname1 );\
				m_cListCtrl.SetItemText(iItem,2, strname2 );\
				iItem++;\
				m_strClip+=stritem+CString("\t");\
				m_strClip+=strname1+CString("\t");\
				m_strClip+=strname2+CString("\r\n");\
			};\

/////////////////////////////////////////////////////////////////////////////
// CBaroMeasureDlg message handlers

void CBaroMeasureDlg::SetRange(long lStart, long lEnd)
	{
	CString ALTFMT;
	if( m_eAltUnits==eMeters )
		{
		ALTFMT="%5.1lf m";
		}
	else
		{
		ALTFMT="%6.0lf ft";
		}

	CString DISTFMT;
	if( m_eUnits==eNautical )
		DISTFMT="%5.1lf Nm";
	else if( m_eUnits==eStatute )
		DISTFMT="%5.1lf mi";
	else if( m_eUnits==eKilometers )
		DISTFMT="%5.1lf km";

	CString SPEEDFMT;
	if( m_eUnits==eNautical )
		SPEEDFMT="%5.1lf kts";
	else if( m_eUnits==eStatute )
		SPEEDFMT="%5.1lf mph";
	else if( m_eUnits==eKilometers )
		SPEEDFMT="%5.1lf km/hr";



	bool bPress= m_uDisplayMask & PRESSURETRACE;


	LV_ITEM lvi;
	int iItem=0;
	lvi.iSubItem = 0;
	lvi.mask = 0;
	CString strAlt;

	if( !m_pTrace ) return;

	m_cListCtrl.DeleteAllItems();
	m_strClip="";

	CTime cStart(lStart);
	CBaroPoint* pPoint1=m_pTrace->GetBaroPoint(cStart);
	if( pPoint1 )
		{
		strAlt.Format( ALTFMT, ConvertAltitude(pPoint1->GetAltitude(bPress), m_eAltUnits));
		ADDITEM( "Point 1", strAlt, cStart.Format("%H:%M:%S") );
		}

	CTime cEnd(lEnd);
	CBaroPoint* pPoint2=m_pTrace->GetBaroPoint(cEnd);
	if( pPoint2 )
		{
		strAlt.Format( ALTFMT, ConvertAltitude(pPoint2->GetAltitude(bPress), m_eAltUnits) );
		ADDITEM( "Point 2", strAlt, cEnd.Format("%H:%M:%S") );
		}

	if( pPoint1 && pPoint2 )
		{
		CTimeSpan cSpan=cEnd-cStart;
		strAlt.Format( ALTFMT, ConvertAltitude(pPoint2->GetAltitude(bPress)-pPoint1->GetAltitude(bPress), m_eAltUnits));
		ADDITEM( "Difference", strAlt, cSpan.Format("%H:%M:%S") );

		CLocation cLoc1( pPoint1->m_fLat, pPoint1->m_fLong );
		CLocation cLoc2( pPoint2->m_fLat, pPoint2->m_fLong );
		double dDist=cLoc1.DistanceTo(&cLoc2,m_eUnits);
		strAlt.Format( DISTFMT, dDist);

		double dSpeed=0;
		double dHour=(double)cSpan.GetTotalSeconds()/3600.;
		if( dHour>0.)
			dSpeed=dDist/dHour;
		CString strTemp;
		strTemp.Format( SPEEDFMT, dSpeed);

		ADDITEM( "Distance/Speed", strAlt, strTemp );
		}


	CBaroPoint* pMinPoint=m_pTrace->GetMinAltitudeInRange( lStart, lEnd, bPress?PRESSURETRACE:GPSTRACE);
	if( pMinPoint )
		{
		strAlt.Format( ALTFMT, ConvertAltitude(pMinPoint->GetAltitude(bPress), m_eAltUnits) );
		ADDITEM( "Minimum Alt", strAlt, pMinPoint->m_cTime.Format("%H:%M:%S") );
		}

	CBaroPoint* pMaxPoint=m_pTrace->GetMaxAltitudeInRange( lStart, lEnd, bPress?PRESSURETRACE:GPSTRACE);
	if( pMaxPoint )
		{
		strAlt.Format( ALTFMT, ConvertAltitude(pMaxPoint->GetAltitude(bPress), m_eAltUnits) );
		ADDITEM( "Maximum Alt", strAlt, pMaxPoint->m_cTime.Format("%H:%M:%S") );
		}

	if( pMinPoint && pMaxPoint )
		{
		CTimeSpan cSpan=pMaxPoint->m_cTime-pMinPoint->m_cTime;
		strAlt.Format( ALTFMT, ConvertAltitude(pMaxPoint->GetAltitude(bPress)-pMinPoint->GetAltitude(bPress), m_eAltUnits) );
		ADDITEM( "Min/Max Diff.", strAlt, cSpan.Format("%H:%M:%S") );

		float fROC=ConvertAltitude(pMaxPoint->GetAltitude(bPress)-pMinPoint->GetAltitude(bPress), m_eAltUnits)/(float)cSpan.GetTotalSeconds();

		if( m_eAltUnits==eMeters )
			strAlt.Format( "%5.1lf m/s", fROC );
		else
			strAlt.Format( "%5d ft/min", (short)(fROC*60) );
		ADDITEM( "Min/Max Climb Rate", strAlt, "" );

		CLocation cLoc1( pMaxPoint->m_fLat, pMaxPoint->m_fLong );
		CLocation cLoc2( pMinPoint->m_fLat, pMinPoint->m_fLong );
		double dDist=cLoc1.DistanceTo(&cLoc2,m_eUnits);
		strAlt.Format( DISTFMT, dDist);
		ADDITEM( "Min/Max Dist", strAlt, "" );

		}


	}

BOOL CBaroMeasureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	CRect crec;
	m_cListCtrl.GetWindowRect( &crec );
	int iWidth=crec.Width()/3;

	//  Insert Columns into list control.
	LV_COLUMN lvc;
	lvc.mask = LVCF_WIDTH | LVCF_SUBITEM;
	for(  int i = 0; i<NUM_COLUMNS; i++)
		{
		lvc.iSubItem = i;
		lvc.cx = iWidth-2;// _gnColumnWidth[i];
		m_cListCtrl.InsertColumn(i,&lvc);
		}

	if( m_uDisplayMask & PRESSURETRACE )
		SetWindowText("Barogram Data (Pressure Altitude)");
	else
		SetWindowText("Barogram Data (GPS Altitude)");


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBaroMeasureDlg::OnCopy() 
{

    HGLOBAL hglobalText = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, m_strClip.GetLength() + 1);
    strcpy_s((char *)GlobalLock(hglobalText), sizeof(hglobalText), m_strClip);
    GlobalUnlock(hglobalText);
    
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_TEXT,hglobalText);
    CloseClipboard();
	
}

