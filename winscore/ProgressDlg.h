#ifndef __PROGRESSDLG_H__
#define __PROGRESSDLG_H__

#include "resource.h"

class CProgressDlg : public CDialog
{
// Construction / Destruction
public:
	UINT m_nAnimationID;
	void SetWindowTitle(LPCTSTR lpszText);
    CProgressDlg(UINT nCaptionID = 0);   // standard constructor
    ~CProgressDlg();

    BOOL Create(CWnd *pParent=NULL, HANDLE hEventKill = NULL);

    // Checking for Cancel button
    // Progress Dialog manipulation
    void SetStatus(LPCTSTR lpszMessage);
	void SetSecondStatus(LPCTSTR lpszMessage);
	void SetUpper(unsigned int nUpper);
    void SetPos(unsigned int so_far);
        
// Dialog Data
    //{{AFX_DATA(CProgressDlg)
	enum { IDD = IDD_PROGRESS };
	CAnimateCtrl	m_AnimateCtrl;
    CProgressCtrl	m_Progress;
	//}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProgressDlg)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strTitle;
	int m_nLower;
    int m_nUpper;
    int m_nStep;
    
    BOOL m_bCancel;
    BOOL m_bParentDisabled;
	
	HANDLE m_hEventKill;

    void ReEnableParent();

    virtual void OnCancel();
    virtual void OnOK() {}; 
    void UpdatePercent(int nCurrent);
    void PumpMessages();

    // Generated message map functions
    //{{AFX_MSG(CProgressDlg)
    virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // __PROGRESSDLG_H__
