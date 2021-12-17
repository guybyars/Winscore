//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// RListVw.h : interface of the CWinscoreView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#if !defined(AFX_WINSCOREVIEW__INCLUDED_)
#define AFX_WINSCOREVIEW__INCLUDED_

#include <afxtempl.h>
#include "printgriddlg.h"
#include "listvwex.h"
#include "AutoScoreDlg.h"

class CWinscoreView : public CListViewEx
{
protected: // create from serialization only
	CWinscoreView();
	DECLARE_DYNCREATE(CWinscoreView)

// Attributes
public:
	CWinscoreDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinscoreView)
	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual ~CWinscoreView();
#ifdef _DEBUG

#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWinscoreView)
	afx_msg void OnContestInfoContestants();
	afx_msg void OnContestInfoTurnpoint();
	afx_msg void OnContestInfoSetup();
	afx_msg void OnViewTurnpointGrid();
	afx_msg void OnDailyinfoTask();
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDailyinfoCalculatescores();
	afx_msg void OnDailyPenalties();
	afx_msg void OnUpdateContestInfoTurnpoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateContestInfoContestants(CCmdUI* pCmdUI);
	afx_msg void OnUpdateContestInfoSetup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyPenalties(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyinfoCalculatescores(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyinfoLandingcards(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyinfoTask(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyinfoTimes(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExportScoresheet(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTurnpointGrid(CCmdUI* pCmdUI);
	afx_msg void OnFileExportScoresheet();
	afx_msg void OnContestInfoTakeoffGrid();
	afx_msg void OnUpdateContestInfoTakeoffGrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateContestInfoInvalidTpt(CCmdUI* pCmdUI);
	afx_msg void OnFileImportEvents();
	afx_msg void OnUpdateFileImportEvents(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	afx_msg void OnFileExportContestants();
	afx_msg void OnUpdateFileExportContestants(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintDirect(CCmdUI* pCmdUI);
	afx_msg void OnFilePrintDirect();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnDailyTaskplanner();
	afx_msg void OnUpdateDailyTaskplanner(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileImportFlights();
	afx_msg void OnUpdateFileImportFlights(CCmdUI* pCmdUI);
	afx_msg void OnFlightlogsAnalyzeall();
	afx_msg void OnFlightlogsViewigcfile();
	afx_msg void OnUpdateFlightlogsViewigcfile(CCmdUI* pCmdUI);
	afx_msg void OnFlightlogsDelete();
	afx_msg void OnUpdateFlightlogsDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFlightlogsAnalyzeall(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFlightlogsSettings();
	afx_msg void OnFlightlogsViewwarnings();
	afx_msg void OnUpdateFlightlogsViewwarnings(CCmdUI* pCmdUI);
	afx_msg void OnFlightlogsAnalyzeflight();
	afx_msg void OnUpdateFlightlogsAnalyzeflight(CCmdUI* pCmdUI);
	afx_msg void OnFlightlogsEditflightreslts();
	afx_msg void OnUpdateFlightlogsEditflightreslts(CCmdUI* pCmdUI);
	afx_msg void OnFlightlogsAnalyzenew();
	afx_msg void OnUpdateFlightlogsAnalyzenew(CCmdUI* pCmdUI);
	afx_msg void OnFlightlogsViewanalysis();
	afx_msg void OnUpdateFlightlogsViewanalysis(CCmdUI* pCmdUI);
	afx_msg void OnFlightlogsRefresh();
	afx_msg void OnUpdateFlightlogsRefresh(CCmdUI* pCmdUI);
	afx_msg void OnFlightlogsDisplay();
	afx_msg void OnUpdateFlightlogsDisplay(CCmdUI* pCmdUI);
	afx_msg void OnFileExportWinscoreflightfile();
	afx_msg void OnUpdateFileExportWinscoreflightfile(CCmdUI* pCmdUI);
	afx_msg void OnHelpRegister();
	afx_msg void OnUpdateHelpRegister(CCmdUI* pCmdUI);
	afx_msg void OnFlightlogsImportAutomaticcambridgeupload();
	afx_msg void OnUpdateFlightlogsImportAutomaticcambridgeupload(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFlightlogsView();
	afx_msg void OnUpdateFlightlogsView(CCmdUI* pCmdUI);
	afx_msg void OnAnalyzeInfo();
	afx_msg void OnUpdateAnalyzeInfo(CCmdUI* pCmdUI);
	afx_msg void OnAnalyzeSecurityMarkasfailed();
	afx_msg void OnAnalyzeSecurityMarkaspassed();
	afx_msg void OnDestroy();
	afx_msg void OnAnalyzeDetailedinfo();
	afx_msg void OnUpdateAnalyzeDetailedinfo(CCmdUI* pCmdUI);
	afx_msg void OnFlightlogsCheckSecurity();
	afx_msg void OnUpdateFlightlogsCheckSecurity(CCmdUI* pCmdUI);
	afx_msg void OnFileExportCpXml();
	afx_msg void OnFileExportContestantsinxmlformat();
	afx_msg void OnFileImportControlpointsinxmlformat();
	afx_msg void OnFileImportXmlContestants();
	afx_msg void OnScoresheetServer();
	afx_msg void OnFileExportXmlEntirecontestfile();
	afx_msg void OnFileExportAutoexportscoresheettoxml();
	afx_msg void OnExportTaskToLibrary();
	afx_msg void OnFileImportXmlEntirecontest();
	afx_msg void OnImportFlashcard();
	afx_msg void OnUpdateImportFlashcard(CCmdUI* pCmdUI);
	afx_msg void OnSsaServer();
	afx_msg void OnUpdateSsaServer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileImportControlpointsinxmlformat(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExportCpXml(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExportXmlEntirecontestfile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileImportXmlContestants(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExportContestantsinxmlformat(CCmdUI* pCmdUI);
	afx_msg void OnFileImportControlpointsincaidatformat();
	afx_msg void OnUpdateFileImportControlpointsincaidatformat(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// list view image lists
protected:


// view type helpers
public:
	bool RestoreColumnWidths();
	void SaveColumnWidths();
	BOOL IsFlightSelected();
	void* GetSelectedPtr();
	int m_iSortedColumn[NVIEWS];
	bool SetupPrintOptions();
	void ViewTakeoffGrid(EClass eClass);
	void RefreshScoreDisplay();
	void ViewScores(CTime cDate, EClass eClass);
	void ViewLandingCards(CTime cDate, EClass eClass);
	void ViewTasks();
	void ViewTurnpointGrid();
	void ViewContestantList();
	void ViewTurnpointList();
//	void ViewInvalidLegList();
	void ViewFlightLogs(CTime cDate, EClass eClass);
	void UpdateFlightLogs(CMainFrame* pFrame, CTime cDate, EClass eClass, CFlight *pcFlight);

    void InitialUpdate();
	void ContestInfoContestants();
	void ContestInfoTurnpoint();
	void ContestInfoTakeoffGrid();
	void SetNoLogText(CString strReasonText);

	void ViewTimes(EViews eView, CTime cTime);
	void CWinscoreView::DrawTakeoffGrid(CDC*	pDC, 
										CString &cContestName,
										CString &cLocation,
										CTime	&ContestStartDate, 
										int		iPage);


	CWinscoreView* GetWinscoreView();
	BOOL SetViewType(DWORD dwViewType);
	DWORD GetViewType();
	void ShowScores(CTime cDate);
	CString GetStatusLine();
	void ManageTasks();
	CAutoScoreDlg *m_pModlessAutoScoreDlg;
	CSemaphore m_cSem_UpdateFlightLogs;


// flags and states
protected:
	int m_nCheckedItem;
	void CheckItem(int nNewCheckedItem);
	CGridData m_cGridData;

private:
	void OnDblClickFlightView();
	int GetSelectedItem();
	int CWinscoreView::GetSelectedItems(CArray<int,int> &cIntArray);
	bool IsPrintingAllowed();
	void UpdateStatusLine();
public:
	afx_msg void OnImportfromssa();
	afx_msg void OnUpdateImportfromssa(CCmdUI *pCmdUI);
	afx_msg void OnDeleteContestant();
	afx_msg void OnEdit1Contestant();
	afx_msg void OnAddContestant();
	afx_msg void OnImportHandicaplistfromssa();
	afx_msg void OnExportHandicapsummarytoHTML();
	afx_msg void OnDailyAutoscoring();
	afx_msg void OnFlightlogsRechecklateststart();
	afx_msg void OnUpdateFlightlogsRechecklateststart(CCmdUI *pCmdUI);
	afx_msg void OnExportAlltaskstoxmlfile();
	afx_msg void OnChangeClass();
	afx_msg void OnUpdateChangeclass(CCmdUI *pCmdUI);
	afx_msg void OnNologstatusDidnotlaunch();
	afx_msg void OnNologstatusLandoutreported();
	afx_msg void OnNologstatusNologfound();
	afx_msg void OnNologstatusCustom();
	afx_msg void OnUpdateNologstatus(CCmdUI *pCmdUI);
	afx_msg void OnFileCheckfornewversionatstartup();
	afx_msg void OnUpdateFileCheckfornewversionatstartup(CCmdUI *pCmdUI);
	afx_msg void OnAutomaticSaveOptions();
	afx_msg void OnLogstatusCheckedandok();
};

#ifndef _DEBUG  // debug version in RListVw.cpp
inline CWinscoreDoc* CWinscoreView::GetDocument()
   { return (CWinscoreDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

#endif
