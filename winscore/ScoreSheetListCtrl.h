#if !defined(AFX_SCORESHEETLISTCTRL_H__FD2E12B6_C4D5_48FD_8FBD_6ECFEED2FEE5__INCLUDED_)
#define AFX_SCORESHEETLISTCTRL_H__FD2E12B6_C4D5_48FD_8FBD_6ECFEED2FEE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScoreSheetListCtrl.h : header file
//

#include "tasklist.h"
#include "WinscoreDefs.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CScoreSheetListCtrl window

class CScoreSheetListCtrl : public CListCtrl
{
// Construction
public:
	CScoreSheetListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScoreSheetListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void CheckAll();
	void CheckToday();
	CTime GetCheckedDate(int i);
	void LoadContestDates(int nDates, CTime caTimes[]);
	EClass GetCheckedClass(int i);
	CTask* GetCheckedTask(int i);
	int GetNumChecked();
	void LoadAvailableClasses(  );
	void LoadTasks(CTaskList &cTaskList);
	void AddColumns(bool bClassesOnly=false);
	virtual ~CScoreSheetListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScoreSheetListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCORESHEETLISTCTRL_H__FD2E12B6_C4D5_48FD_8FBD_6ECFEED2FEE5__INCLUDED_)
