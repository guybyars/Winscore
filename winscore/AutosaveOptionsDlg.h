#pragma once


// CAutosaveOptionsDlg dialog

class CAutosaveOptionsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAutosaveOptionsDlg)

public:
	CAutosaveOptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAutosaveOptionsDlg();

// Dialog Data
	enum { IDD = IDD_AUTOSAVE_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_iRemindHours;
	int m_iAutosaveHours;
	int m_iRadioOptions;
	afx_msg void OnBnClickedRadioNone();
};
