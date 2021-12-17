// SSAPicker.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "SSAPicker.h"
#include "afxdialogex.h"
#include "Global_Prototypes.h"


// CSSAPicker dialog

IMPLEMENT_DYNAMIC(CSSAPicker, CDialogEx)

CSSAPicker::CSSAPicker(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSSAPicker::IDD, pParent)
	, m_iSSA(0)
{

}

CSSAPicker::~CSSAPicker()
{
}

void CSSAPicker::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cListCtrl);
}


BEGIN_MESSAGE_MAP(CSSAPicker, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CSSAPicker::OnNMClickList1)
END_MESSAGE_MAP()


// CSSAPicker message handlers


BOOL CSSAPicker::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int iTest=LoadContestsFromSSA( NULL, &m_cListCtrl ) ;

	if( iTest==0 ) 
		{
		AfxMessageBox (_T("Unable to download contest information from ssa.org"));
		CDialog::OnCancel();
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSSAPicker::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nItem=pNMItemActivate->iItem;

	if( nItem<0 ) 
		{
		m_iSSA=0;
		CDialog::OnCancel();
		}
	else
		{
		m_iSSA=m_cListCtrl.GetItemData(nItem);
	   	CDialog::OnOK();
		}	
	
	*pResult = 0;

}




