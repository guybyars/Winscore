//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FlightDisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "flight.h"
#include "winscoredoc.h"
//#include "sxbutton.h"
#include "suautil.h"
#include "sua.h"

#include "ResizingDialog.h"
#include "FlightDisplayDlg.h"
#include "global_prototypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFlightDisplayDlg dialog

BEGIN_EVENTSINK_MAP(CFlightDisplayDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CChildFrame)
	ON_EVENT(CWSBaro, IDC_WSBAROCTRL, 1 /* OnMoveCursor */, OnMoveCursor, VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

CFlightDisplayDlg::CFlightDisplayDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CFlightDisplayDlg::IDD, pParent)
{


    SetControlInfo(IDC_WSVIEWCTRL,    RESIZE_HOR | RESIZE_VER);
    SetControlInfo(IDC_WSBAROCTRL,    RESIZE_HOR );
 	SetControlInfo(IDCANCEL,	ANCHORE_RIGHT|ANCHORE_BOTTOM );
 	SetControlInfo(IDC_LATLONGEDIT, RESIZE_HOR|	ANCHORE_BOTTOM );
 	SetControlInfo(ID_FLIGHT_DISPLAY,  ANCHORE_RIGHT|ANCHORE_BOTTOM );

	//{{AFX_DATA_INIT(CFlightDisplayDlg)
	//}}AFX_DATA_INIT
}

void CFlightDisplayDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CFlightDisplayDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	 CResizingDialog::OnPaint();
}


void CFlightDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlightDisplayDlg)
//	DDX_Control(pDX, IDC_PLAYBUTTON, m_cPlayButton);
	DDX_Control(pDX, IDC_LATLONGEDIT, m_cLatLong);
//	DDX_Control(pDX, IDC_SLIDER, m_cSlider);
	DDX_Control(pDX, IDC_WSVIEWCTRL, m_cWSView);
	DDX_Control(pDX, IDC_WSBAROCTRL, m_cWSBaro);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlightDisplayDlg, CDialog)
	//{{AFX_MSG_MAP(CFlightDisplayDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_FLIGHT_DISPLAY, OnFlightDisplayHelp)
	ON_BN_CLICKED(IDC_ZOOMINBUTTON, OnZoominbutton)
	ON_BN_CLICKED(IDC_ZOOMOUTBUTTON, OnZoomoutbutton)
	ON_BN_CLICKED(IDC_ZOOMTASKBUTTON, OnZoomtaskbutton)
	ON_BN_CLICKED(IDC_DISPLAYOPTIONSBUTTON, OnDisplayoptionsbutton)
	ON_BN_CLICKED(IDC_AUTOSCALE, OnAutoscale)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlightDisplayDlg message handlers

BOOL CFlightDisplayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	BOOL bUsingBGate=false;


	m_bPlaying=false;
//	m_cPlayButtonSX.SetMaskedBitmap( IDB_PLAYBITMAP, 15, 15, RGB(255,255,255) );

	CPosition* pcPrevPos=NULL;

	CString cWinText="Flight Display for ";
	cWinText+=m_pcFlight->m_strCID+" - "+m_pcFlight->m_strPilot;
	cWinText+="    (Press right mouse button for view commands)";


	SetWindowText(cWinText);

	LoadViewControl( m_cWSView, &m_cWSBaro, m_pDoc, m_pcFlight, m_pcTask );

	UpdateTime(  m_pcFlight->GetNumPoints()-1 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlightDisplayDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	ShowWindow( SW_SHOWMAXIMIZED  );
	
	m_cWSView.Autoscale();

	
}

void CFlightDisplayDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	/*
	if( pScrollBar->GetDlgCtrlID()==IDC_SLIDER)
		{
		KillTimer(1);
		m_bPlaying=false;
//		m_cPlayButtonSX.SetMaskedBitmap( IDB_PLAYBITMAP, 15, 15, RGB(255,255,255) );

		int iPos=m_cSlider.GetPos();
		m_cWSView.SetCurrentTimeIndex( iPos );

		UpdateTime( iPos );

		}
		*/
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void  CFlightDisplayDlg::UpdateTime( int iPos )
	{

	CPosition* pcPos=m_pcFlight->GetPosition(iPos);
	if( pcPos )
		{
		CString cText=pcPos->m_cTime.Format( _T("Time: %H:%M:%S, ") );
		CString cAltitude;
		cAltitude.Format( _T("   GPS Alt: %d,   Pres. Alt: %d,   Corrected Alt: %d,   Position:  "), pcPos->m_iGPSAltitude, pcPos->m_iPressureAltitude, pcPos->m_iCorrectedAltitude  );
		cText+=cAltitude;
		cText+=pcPos->GetFormattedInfo();
		cText+=pcPos->CheckStatus(FAN_MOTOR_ON)?(_T(",   Motor: ON")):(_T(" ") ) ;
		m_cLatLong.SetWindowText( cText );
		}


	}
/*
void CFlightDisplayDlg::OnPlaybutton() 
{
	if( m_bPlaying )
		{
		KillTimer(1);
//		m_cPlayButtonSX.SetMaskedBitmap( IDB_PLAYBITMAP, 15, 15, RGB(255,255,255) );
		m_bPlaying=false;
		}
	else
		{
		SetTimer( 1, 0, NULL );
		m_bPlaying=true;
//		m_cPlayButtonSX.SetMaskedBitmap( IDB_STOPBITMAP, 15, 15, RGB(255,255,255) );

		int iPos=m_cSlider.GetPos();
		if( iPos==m_pcFlight->GetNumPoints()-1 )
			{
			m_cWSView.SetCurrentTimeIndex( 0 );
			UpdateTime( 0 );	
			m_cSlider.SetPos(0);
			}
		}
}
*/
void CFlightDisplayDlg::OnTimer(UINT nIDEvent) 
{
	/*
	int iPos=m_cSlider.GetPos();
	iPos+=25;

	if( iPos>=m_pcFlight->GetNumPoints() )
		{
		KillTimer(1);
		m_bPlaying=false;
//		m_cPlayButtonSX.SetMaskedBitmap( IDB_PLAYBITMAP, 15, 15, RGB(255,255,255) );
		}

	iPos=min( iPos, m_pcFlight->GetNumPoints() );
	m_cSlider.SetPos(iPos);

	CPosition* pcPos=m_pcFlight->GetPosition( iPos );
//			m_cWSView.SetViewCenter( pcPos->GetLat(), pcPos->GetLong() );
	m_cWSView.SetCurrentTimeIndex( iPos );
	UpdateTime( iPos );	
*/

	CDialog::OnTimer(nIDEvent);
}

void CFlightDisplayDlg::OnFlightDisplayHelp() 
{
     AfxGetApp()->WinHelp( 0x10000 + ID_FLIGHT_DISPLAY,  HELP_CONTEXT);	
}

void CFlightDisplayDlg::OnZoominbutton() 
{
	m_cWSView.ZoomWindow();
}

void CFlightDisplayDlg::OnZoomoutbutton() 
{
	m_cWSView.ScaleView(.75);
}

void CFlightDisplayDlg::OnZoomtaskbutton() 
{
	m_cWSView.DisplayNextTaskPoint();		
}

void CFlightDisplayDlg::OnDisplayoptionsbutton() 
{
	m_cWSView.OnEditProperties();	
}

void CFlightDisplayDlg::OnAutoscale() 
{
	m_cWSView.Autoscale();	
}
bool CFlightDisplayDlg::OnMoveCursor(long lTime)
	{
	for ( int iPoint=0; iPoint<m_pcFlight->GetNumPoints(); iPoint++)
		{
		CPosition* pcPos=m_pcFlight->GetPosition(iPoint);
		if( pcPos->m_cTime>=lTime )
			{
			m_cWSView.SetCurrentTimeIndex(iPoint);
			UpdateTime( iPoint );
			return true;
			}
		}
	return true;
	}

void CFlightDisplayDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CFlightDisplayDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CFlightDisplayDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CFlightDisplayDlg::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message==WM_CHAR ||  message==WM_KEYDOWN )
		{
		return CDialog::OnChildNotify(message, wParam, lParam, pLResult);
		}
	return CDialog::OnChildNotify(message, wParam, lParam, pLResult);
}

BOOL CFlightDisplayDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CFlightDisplayDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message==WM_KEYDOWN )
		{
		if( pMsg->wParam==39 || pMsg->wParam==38 || // right arrow or up arrow
			pMsg->wParam==37 || pMsg->wParam==40 || // left arrow or down arrow
			pMsg->wParam==34 || //pgdn
			pMsg->wParam==33 || //pgup
			pMsg->wParam==36 || //home
			pMsg->wParam==35 )//end
			{
			m_cWSBaro.OnControlKey(pMsg->wParam);
			return true;
			}
		}
	return CDialog::PreTranslateMessage(pMsg);
}

