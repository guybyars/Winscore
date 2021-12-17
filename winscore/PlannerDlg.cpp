//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// PlannerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "winscoredoc.h"
#include "resizingdialog.h"
#include "PlannerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlannerDlg dialog


CPlannerDlg::CPlannerDlg(CWinscoreDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CResizingDialog(CPlannerDlg::IDD, pParent),
	m_pDoc(pDoc)
{
	//{{AFX_DATA_INIT(CPlannerDlg)
	//}}AFX_DATA_INIT

    SetControlInfo(IDC_SLIDER,         ANCHORE_RIGHT | ANCHORE_BOTTOM);
    SetControlInfo(IDC_TASKCONTROL,    RESIZE_HOR | RESIZE_VER);
    SetControlInfo(IDOK,               ANCHORE_BOTTOM);
    SetControlInfo(IDCANCEL,           ANCHORE_BOTTOM);
    SetControlInfo(IDC_RESET,          ANCHORE_BOTTOM);
    SetControlInfo(ID_PLAN_TASKS,      ANCHORE_BOTTOM);
	m_nTaskPoints=0;
	m_cStartGate.SetPointID(0);
	m_cFinishGate.SetPointID(0);
}

CPlannerDlg::CPlannerDlg(CWinscoreDoc* pDoc, CGate &cStartGate, CGate &cFinishGate, CWnd* pParent)
	: CResizingDialog(CPlannerDlg::IDD, pParent),
	m_pDoc(pDoc)
{
    SetControlInfo(IDC_SLIDER,         ANCHORE_RIGHT | ANCHORE_BOTTOM);
    SetControlInfo(IDC_TASKCONTROL,    RESIZE_HOR | RESIZE_VER);
    SetControlInfo(IDOK,               ANCHORE_BOTTOM);
    SetControlInfo(IDCANCEL,           ANCHORE_BOTTOM);
    SetControlInfo(IDC_RESET,          ANCHORE_BOTTOM);
    SetControlInfo(ID_PLAN_TASKS,      ANCHORE_BOTTOM);
	m_nTaskPoints=0;
	m_cStartGate=cStartGate;
	m_cFinishGate=cFinishGate;

}

void CPlannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlannerDlg)
	DDX_Control(pDX, IDC_SLIDER, m_cSliderCtrl);
	DDX_Control(pDX, IDC_TASKCONTROL, m_cTaskControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlannerDlg, CDialog)
	//{{AFX_MSG_MAP(CPlannerDlg)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER, OnReleasedcaptureSlider)
	ON_WM_HSCROLL()
	ON_WM_CANCELMODE()
	ON_WM_VSCROLL()
	ON_WM_CAPTURECHANGED()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_WM_KEYDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_KEYUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(ID_PLAN_TASKS, OnPlanTasks)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlannerDlg message handlers

BOOL CPlannerDlg::OnInitDialog() 
{
	CResizingDialog::OnInitDialog();

	m_cSliderCtrl.SetRangeMin( 0  );
	m_cSliderCtrl.SetRangeMax( 100 );

//	m_cVScroll.SetScrollRange( 1,100); 
//	m_cVScroll.SetScrollPos( 50 ); 

//	m_cHScroll.SetScrollRange( 1,100); 
//	m_cHScroll.SetScrollPos( 50 ); 


	//  Load Turnpoints

	for ( int i=0; i<m_pDoc->m_turnpointArray.GetSize(); i++)
		{ 	    
		CTurnpoint* pcTurnpoint = (CTurnpoint*)m_pDoc->m_turnpointArray.GetAt(i);
		if( pcTurnpoint && pcTurnpoint->IsControlPoint())
			{
			m_cTaskControl.AddControlPoint(  
									   pcTurnpoint->m_iID, 
									   (pcTurnpoint->m_eLatDirection==eNORTH)?(pcTurnpoint->m_iLatDegrees):(-pcTurnpoint->m_iLatDegrees), 
									   (pcTurnpoint->m_eLatDirection==eNORTH)?(pcTurnpoint->m_dLatMinutes):(-pcTurnpoint->m_dLatMinutes), 
									   (pcTurnpoint->m_eLongDirection==eWEST)?(pcTurnpoint->m_iLongDegrees):(-pcTurnpoint->m_iLongDegrees), 
									   (pcTurnpoint->m_eLongDirection==eWEST)?(pcTurnpoint->m_dLongMinutes):(-pcTurnpoint->m_dLongMinutes),
									   pcTurnpoint->m_strName, 
									   pcTurnpoint->m_iAttribute  );
			}
		}
	m_cTaskControl.SetUnits((int)m_pDoc->m_eUnits);


//	Set Start and finish points.
	m_cTaskControl.SetStartPoint(m_cStartGate.GetPointID());
	m_cTaskControl.SetFinishPoint(m_cFinishGate.GetPointID());


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlannerDlg::OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	*pResult = 0;

	m_cTaskControl.Zoom( m_cSliderCtrl.GetPos() );

}

void CPlannerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int iMin, iMax;
	double dPos;
	if( pScrollBar->GetDlgCtrlID()==IDC_SLIDER)
		{
		m_cSliderCtrl.GetRange( iMin, iMax ) ;
		dPos=m_cSliderCtrl.GetPos( );
		dPos/=(double)(iMax-iMin);
		m_cTaskControl.SetZoom( dPos );
		}
	else
		{
		pScrollBar->GetScrollRange(&iMin, &iMax);
		int iCurPos=pScrollBar->GetScrollPos();
		dPos = (double)iCurPos/(double)(iMax-iMin);

		switch (LOBYTE(nSBCode))
		{
		case SB_LEFT:
			dPos = 0;
			break;
		case SB_RIGHT:
			dPos = 1;
			break;
		case SB_LINELEFT:
			dPos-=.01;
			break;
		case SB_LINERIGHT:
			dPos+=.01;
			break;
		case SB_PAGELEFT:
			dPos-=.25;
			break;
		case SB_PAGERIGHT:
			dPos+=.25;
			break;
		case SB_THUMBTRACK:
			dPos= (double)nPos/((double)iMax-iMin);
			break;
		case SB_THUMBPOSITION:
			dPos= (double)nPos/((double)iMax-iMin);
			break;
		}

		m_cTaskControl.SetHScroll( dPos );
		int nNewPos=(int)(dPos*(iMax-iMin));
		pScrollBar->SetScrollPos( nNewPos );
		}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPlannerDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CPlannerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( nSBCode==SB_ENDSCROLL ) return;

	int iMin, iMax;
	pScrollBar->GetScrollRange(&iMin, &iMax);
	int iCurPos=pScrollBar->GetScrollPos();
	double dPos = (double)iCurPos/(double)(iMax-iMin);

	switch (LOBYTE(nSBCode))
	{
	case SB_TOP:
		dPos = 0;
		break;
	case SB_BOTTOM:
		dPos = 1;
		break;
	case SB_LINEUP:
		dPos-=.01;
		break;
	case SB_LINEDOWN:
		dPos+=.01;
		break;
	case SB_PAGEUP:
		dPos-=.25;
		break;
	case SB_PAGEDOWN:
		dPos+=.25;
		break;
	case SB_THUMBTRACK:
		dPos= (double)nPos/((double)iMax-iMin);
		break;
	case SB_THUMBPOSITION:
		dPos= (double)nPos/((double)iMax-iMin);
		break;
	}

	m_cTaskControl.SetVScroll( dPos );
	int nNewPos=(int)(dPos*(iMax-iMin));
	pScrollBar->SetScrollPos( nNewPos );
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPlannerDlg::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CDialog::OnCaptureChanged(pWnd);
}

void CPlannerDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CResizingDialog::OnPaint();
		
}

void CPlannerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);
		
}

void CPlannerDlg::OnReset() 
{	
	m_cSliderCtrl.SetPos(0);
	m_cTaskControl.Reset();	
}

void CPlannerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	CResizingDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BEGIN_EVENTSINK_MAP(CPlannerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CPlannerDlg)
	ON_EVENT(CPlannerDlg, IDC_TASKCONTROL, 1 /* ZoomChanged */, OnZoomChangedTaskcontrol, VTS_R8)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPlannerDlg::OnZoomChangedTaskcontrol(double dZoom) 
{
	m_cSliderCtrl.SetPos( (int)(dZoom*100)  );
	m_cSliderCtrl.Invalidate();
	
}

void CPlannerDlg::OnOK() 
{
	m_nTaskPoints=m_cTaskControl.GetNumPoints();
	for(int i=0; i<m_nTaskPoints; i++ )
		{
		if(i==MAXTASKTPS) break;
		m_aiTaskPoints[i]=m_cTaskControl.GetPoint(i);
		}
	
	CDialog::OnOK();
}

void CPlannerDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	ShowWindow( SW_SHOWMAXIMIZED  );	
}

void CPlannerDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_cTaskControl.OnKeyUP(nChar);	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CPlannerDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_cTaskControl.OnKeyUP(0);	
	CDialog::OnRButtonUp(nFlags, point);
}

void CPlannerDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_cTaskControl.OnKeyUP(0);		
	CDialog::OnLButtonUp(nFlags, point);
}

void CPlannerDlg::OnPlanTasks() 
{
      AfxGetApp()->WinHelp( 0x10000 + ID_PLAN_TASKS,  HELP_CONTEXT);	
	
}

