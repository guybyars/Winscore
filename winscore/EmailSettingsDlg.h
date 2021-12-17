#pragma once


// CEmailSettingsDlg dialog

class CEmailSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEmailSettingsDlg)

public:
	CEmailSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEmailSettingsDlg();

	CDialog *m_pParentDlg;

// Dialog Data
	enum { IDD = IDD_EMAIL_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strServer;
	CString m_strUsername;
	CString m_strPassword;
	CString m_strMailbox;
	afx_msg void OnTest();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

	int m_iPortID;
	BOOL m_bUseSSL;
};
