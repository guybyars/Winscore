// SelectClassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "SelectClassDlg.h"
#include "afxdialogex.h"


// CSelectClassDlg dialog

IMPLEMENT_DYNAMIC(CSelectClassDlg, CDialogEx)

CSelectClassDlg::CSelectClassDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectClassDlg::IDD, pParent)
{

}

CSelectClassDlg::~CSelectClassDlg()
{
}

void CSelectClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cClassCombo);
}


BEGIN_MESSAGE_MAP(CSelectClassDlg, CDialogEx)
END_MESSAGE_MAP()


// CSelectClassDlg message handlers


BOOL CSelectClassDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pDoc->SetAvailableClasses(&m_cClassCombo);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSelectClassDlg::OnOK()
{
	int iSel=m_cClassCombo.GetCurSel();
	m_eClass = (EClass)m_cClassCombo.GetItemData(iSel);

	CDialogEx::OnOK();
}
