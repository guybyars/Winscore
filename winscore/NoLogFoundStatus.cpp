// NoLogFoundStatus.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "NoLogFoundStatus.h"
#include "afxdialogex.h"


// CNoLogFoundStatus dialog

IMPLEMENT_DYNAMIC(CNoLogFoundStatus, CDialogEx)

CNoLogFoundStatus::CNoLogFoundStatus(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNoLogFoundStatus::IDD, pParent)
	, m_strStatusText(_T(""))
{

}

CNoLogFoundStatus::~CNoLogFoundStatus()
{
}

void CNoLogFoundStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strStatusText);
}


BEGIN_MESSAGE_MAP(CNoLogFoundStatus, CDialogEx)
END_MESSAGE_MAP()


// CNoLogFoundStatus message handlers
