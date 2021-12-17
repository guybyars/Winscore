//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// TaskTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "TaskTimeDlg.h"
#include "TaskDlg.h"
#include "global_prototypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskTimeDlg dialog


CTaskTimeDlg::CTaskTimeDlg(CWinscoreDoc *pDoc, CTask &cTask, CWnd* pParent /*=NULL*/)
	: CDialog(CTaskTimeDlg::IDD, pParent),
	m_pDoc(pDoc),
	m_cTask(cTask)
{
	//{{AFX_DATA_INIT(CTaskTimeDlg)
	//}}AFX_DATA_INIT
}


void CTaskTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaskTimeDlg)
	DDX_Control(pDX, IDC_POSTTIME_TEXT, m_cPostTimeText);
	DDX_Control(pDX, IDC_TASKOPEN_DATETIMEPICKER, m_cTaskOpenTimePicker);
	DDX_Control(pDX, IDC_MINIMUMTIME_PICKER, m_cMinTimePicker);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaskTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CTaskTimeDlg)
	ON_BN_CLICKED(IDC_MAXPOST_CHECK, OnMaxpostCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskTimeDlg message handlers

void CTaskTimeDlg::OnMaxpostCheck() 
{
//	m_cMaxTimePicker.EnableWindow( m_cMaxPostCheckBox.GetCheck() );
	
}

void CTaskTimeDlg::OnOK() 
{
		UpdateData(true);

		CTime cTime;

		m_cTaskOpenTimePicker.GetTime(cTime);
		m_cTask.m_cGateOpenTime=cTime;
		WriteWinscoreInt( szOpenHours,		cTime.GetHour()		);
		WriteWinscoreInt( szOpenMinutes,		cTime.GetMinute()	);

		m_cMinTimePicker.GetTime(cTime);
		m_cTask.m_cPostTime=CTimeSpan(0, cTime.GetHour(), cTime.GetMinute(), 0 );
		WriteWinscoreInt( szPostHours,		cTime.GetHour()		);
		WriteWinscoreInt( szPostMinutes,		cTime.GetMinute()	);

	CDialog::OnOK();
}

BOOL CTaskTimeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cTaskOpenTimePicker.SetFormat(_T("HH : mm"));
	m_cTaskOpenTimePicker.SetTime( &CTime( m_cTask.m_cGateOpenTime) ); 


	m_cPostTimeText.EnableWindow(	 m_cTask.IsTimedTask()  );
	m_cMinTimePicker.EnableWindow(	 m_cTask.IsTimedTask()  );
	m_cMinTimePicker.SetTime( &CTime(	2000, //m_cTask.m_cDate.GetYear(),
										1, //m_cTask.m_cDate.GetMonth(),
										1, //m_cTask.m_cDate.GetDay(), 
										m_cTask.m_cPostTime.GetHours(),
										m_cTask.m_cPostTime.GetMinutes(),0));
	m_cMinTimePicker.SetFormat(_T("H : mm"));


	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

