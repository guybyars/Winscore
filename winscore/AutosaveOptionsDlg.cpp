// AutosaveOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "AutosaveOptionsDlg.h"
#include "afxdialogex.h"


// CAutosaveOptionsDlg dialog

IMPLEMENT_DYNAMIC(CAutosaveOptionsDlg, CDialogEx)

CAutosaveOptionsDlg::CAutosaveOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutosaveOptionsDlg::IDD, pParent)
	, m_iRemindHours(0)
	, m_iAutosaveHours(0)
	, m_iRadioOptions(0)
{

}

CAutosaveOptionsDlg::~CAutosaveOptionsDlg()
{
}

void CAutosaveOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REMIND, m_iRemindHours);
	DDV_MinMaxInt(pDX, m_iRemindHours, 1, 24);
	DDX_Text(pDX, IDC_EDIT_AUTOSAVE, m_iAutosaveHours);
	DDV_MinMaxInt(pDX, m_iAutosaveHours, 1, 24);
    DDX_Radio(pDX, IDC_RADIO_NONE, m_iRadioOptions);
}


BEGIN_MESSAGE_MAP(CAutosaveOptionsDlg, CDialogEx)

	ON_BN_CLICKED(IDC_RADIO_NONE, &CAutosaveOptionsDlg::OnBnClickedRadioNone)
END_MESSAGE_MAP()


// CAutosaveOptionsDlg message handlers




void CAutosaveOptionsDlg::OnBnClickedRadioNone()
{
	// TODO: Add your control notification handler code here
}
