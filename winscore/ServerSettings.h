#if !defined(AFX_SERVERSETTINGS_H__F02154A5_E51E_4D81_83B7_6896CBD0D24F__INCLUDED_)
#define AFX_SERVERSETTINGS_H__F02154A5_E51E_4D81_83B7_6896CBD0D24F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerSettings dialog

class CServerSettings : public CDialog
{
// Construction
public:
	void MasqueForm(bool bMasque);
	CServerSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerSettings)
	enum { IDD = IDD_SERVER_SETTINGS };
	CButton	m_cPassiveCheck;
	CEdit	m_cUsername;
	CEdit	m_cServer;
	CEdit	m_cPassword;
	CEdit	m_cRootDir;
	CStatic	m_cStatic4;
	CStatic	m_cStatic3;
	CStatic	m_cStatic2;
	CStatic	m_cStatic1;
	CButton	m_cCheckBox;
	CString	m_strRootDir;
	CString	m_strPassword;
	CString	m_strServerName;
	CString	m_strUsername;
	BOOL	m_bCheck;
	BOOL	m_bPassiveMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerSettings)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSETTINGS_H__F02154A5_E51E_4D81_83B7_6896CBD0D24F__INCLUDED_)
