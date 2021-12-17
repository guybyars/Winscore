#pragma once


// CNoLogFoundStatus dialog

class CNoLogFoundStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CNoLogFoundStatus)

public:
	CNoLogFoundStatus(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNoLogFoundStatus();

// Dialog Data
	enum { IDD = IDD_NOLOGFOUNDSTATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strStatusText;
};
