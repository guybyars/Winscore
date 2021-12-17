// SSAContestLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "SSAContestLoginDlg.h"
#include "afxdialogex.h"


// CSSAContestLoginDlg dialog

IMPLEMENT_DYNAMIC(CSSAContestLoginDlg, CDialogEx)

CSSAContestLoginDlg::CSSAContestLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSSAContestLoginDlg::IDD, pParent)
	, m_strUsername(_T(""))
	, m_strPassword(_T(""))
	, m_bRemember(FALSE)
{

}

CSSAContestLoginDlg::~CSSAContestLoginDlg()
{
}

void CSSAContestLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
	DDV_MaxChars(pDX, m_strUsername, 40);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 40);
	DDX_Check(pDX, IDC_REMEMBER_CHECK, m_bRemember);
}


BEGIN_MESSAGE_MAP(CSSAContestLoginDlg, CDialogEx)
END_MESSAGE_MAP()


// CSSAContestLoginDlg message handlers
