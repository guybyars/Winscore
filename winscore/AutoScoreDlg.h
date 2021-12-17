#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "WinscoreDoc.h"
#include "MainFrm.h"
#include "ResizingDialog.h"
#include "IGCDirectories.h"

#define	IDT_TIMER_0	WM_USER + 200

// CAutoScoreDlg dialog

class CAutoScoreDlg : public CResizingDialog
{
	DECLARE_DYNAMIC(CAutoScoreDlg)

public:
	CAutoScoreDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAutoScoreDlg();


	UINT	m_uTimerVal;
	int		m_iTimeRemaining;
	int		m_iItem;
	CWinscoreDoc* m_pDocument;
	CView*	m_pView;
	CTime	m_cDate;
	EClass	m_eClass;
	CString	m_strUsername;
	CString	m_strPassword;
	int		m_iSSA_ID;
	CSemaphore m_cSem_AddStatusRecord, m_cSem_OnBnClickedStop;

// Dialog Data
	enum { IDD = IDD_AUTOMATIC_SCORING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CStringArray m_strStatusArrayTime, m_strStatusArrayData;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedEmailSettingsButton();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void PostNcDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	CString m_strCountdown;
	int m_iDuration;
	int m_iTimerCount;
	afx_msg void OnClose();

	CWinThread* m_pAutoScoreThread;

	CButton m_cEmailCheck;
	CButton m_cAnalyzeCheck;
	CButton m_cSendScoreslCheck;
	CButton m_cStartButton;
	CButton m_cStopButton;
	CComboBox m_cSSACombo;
	CComboBox m_cDateCombo;
	CEdit m_cNumSecondsEdit;
	CButton m_cEmailSettings;
	CButton m_cTransferSettings;
	CButton m_cMediaCheck;
	CButton m_cAutoSaveCheck;
	bool	m_bRunning;
	CMainFrame* m_pFrame;
	CIGCDirectories m_cIGCDirs;

	CListCtrl m_cListCtrl;
	afx_msg void OnClear();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	int AddStatusRecord(CString strText, int iItem=-1);
	afx_msg void OnTransferSettingsButton();
	void UpdateDate(void);

	afx_msg void OnCbnSelchangeDateCombo();
	afx_msg void OnCbnSelchangeSsa();

	int ScanEmailForLogs( CTime cDate, CString strIMAPServer, CString strUsername, CString strPassword, CString strMailbox, int iPortID, BOOL bUseSSL,CString strLogDir, CString &strError, bool bTestOnly=false );


	afx_msg void OnCancel();
};
