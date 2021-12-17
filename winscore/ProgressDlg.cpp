/****************************************************************/
/*																*/
/*  ProgressDlg.cpp												*/
/*																*/
/*  Implementation of the CProgressDlg class.					*/
/*																*/
/*  Programmed by Pablo van der Meer							*/
/*  Copyright Pablo Software Solutions 2002						*/
/*	http://www.pablovandermeer.nl								*/
/*																*/
/*  Last updated: 5 may 2002									*/
/*																*/
/****************************************************************/

#include "stdafx.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


CProgressDlg::CProgressDlg(UINT nCaptionID)
{
	m_nLower = 0;
	m_nUpper = 100;
	m_nStep = 1;
    //{{AFX_DATA_INIT(CProgressDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_bParentDisabled = FALSE;
}


CProgressDlg::~CProgressDlg()
{
    if(m_hWnd != NULL)
      DestroyWindow();
}

BOOL CProgressDlg::DestroyWindow()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}

void CProgressDlg::ReEnableParent()
{
//    if(m_bParentDisabled && (m_pParentWnd!=NULL))
  //    m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled=FALSE;
}

BOOL CProgressDlg::Create(CWnd *pParent, HANDLE hEventKill)
{
    // Get the true parent of the dialog
 //   m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.

 /*   if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
    {
      m_pParentWnd->EnableWindow(FALSE);
      m_bParentDisabled = TRUE;
    }
*/
	m_hEventKill = hEventKill;
    if(!CDialog::Create(CProgressDlg::IDD, pParent))
    {
		ReEnableParent();
		return FALSE;
    }
//	SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    return TRUE;
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, IDC_ANIMATION, m_AnimateCtrl);
    DDX_Control(pDX, IDC_PROGRESSBAR, m_Progress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
    //{{AFX_MSG_MAP(CProgressDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/********************************************************************/
/*																	*/
/* Function name : OnInitDialog										*/
/* Description   : Initialize dialog.								*/
/*																	*/
/********************************************************************/
BOOL CProgressDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    m_Progress.SetRange(m_nLower,m_nUpper);
    m_Progress.SetStep(m_nStep);
    m_Progress.SetPos(m_nLower);

	// open the avi resource.
	m_AnimateCtrl.Open(m_nAnimationID); 
    m_AnimateCtrl.Play(0, -1, -1);
    return TRUE;  
}

void CProgressDlg::SetStatus(LPCTSTR lpszMessage)
{
    ASSERT(m_hWnd); // Don't call this _before_ the dialog has
                    // been created. Can be called from OnInitDialog
    CWnd *pWndStatus = GetDlgItem(IDC_STATUS);

    // Verify that the static text control exists
    ASSERT(pWndStatus!=NULL);
    pWndStatus->SetWindowText(lpszMessage);
}

void CProgressDlg::SetSecondStatus(LPCTSTR lpszMessage)
{
    ASSERT(m_hWnd); // Don't call this _before_ the dialog has
                    // been created. Can be called from OnInitDialog
    CWnd *pWndStatus = GetDlgItem(IDC_SECONDSTATUS);

    // Verify that the static text control exists
    ASSERT(pWndStatus!=NULL);
    pWndStatus->SetWindowText(lpszMessage);
}


void CProgressDlg::OnCancel()
{
	// cancel transfer
	SetEvent(m_hEventKill);
}

void CProgressDlg::SetUpper(unsigned int nUpper)
{
    m_nLower = 0;
    m_nUpper = nUpper;//this is the file size
}
  
void CProgressDlg::SetPos(unsigned int so_far)
{
    PumpMessages();
    UpdatePercent(so_far);
    return;
}

void CProgressDlg::PumpMessages()
{
    // Must call Create() before using the dialog
    ASSERT(m_hWnd!=NULL);

    MSG msg;
    // Handle dialog messages
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if(!IsDialogMessage(&msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  
      }
    }
}


void CProgressDlg::UpdatePercent(int nNewPos)
{
	if (m_nUpper == 0) return;//Do nothing.

//    CWnd *pWndPercent = GetDlgItem(IDC_SECONDSTATUS);
    int nPercent;
    
    int nDivisor = m_nUpper - m_nLower;
    ASSERT(nDivisor>0);  // m_nLower should be smaller than m_nUpper

    int nDividend = (nNewPos - m_nLower);
    ASSERT(nDividend>=0);   // Current position should be greater than m_nLower

    nPercent = nDividend * 100 / nDivisor;

    // Since the Progress Control wraps, we will wrap the percentage
    // along with it. However, don't reset 100% back to 0%
    if(nPercent!=100)
      nPercent %= 100;
    m_Progress.SetPos(nPercent);//Lets move the progress bar according to percentage

    // Display the percentage
    CString strBuf;
    strBuf.Format(_T("%d%c"), nPercent, '%');

	CString strCur; // get current percentage
//    pWndPercent->GetWindowText(strCur);
    GetWindowText(strCur);
	strBuf += _T(" ") + m_strTitle;

//	if (strCur != strBuf)
//		pWndPercent->SetWindowText(strBuf);

	if (strCur != strBuf)
		SetWindowText(strBuf);
}
    
void CProgressDlg::SetWindowTitle(LPCTSTR lpszText)
{
	m_strTitle = lpszText;
}



void CProgressDlg::OnDestroy() 
{
	m_AnimateCtrl.Stop();

	CDialog::OnDestroy();
}
