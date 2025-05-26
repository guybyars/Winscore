//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
    // WinscoreDoc.h : interface of the CWinscoreDoc class
    //
    /////////////////////////////////////////////////////////////////////////////
    
    #if !defined(AFX_WINSCOREDOC_H__BAD124E0_4095_11D1_ACCF_000000000000__INCLUDED_)
    #define AFX_WINSCOREDOC_H__BAD124E0_4095_11D1_ACCF_000000000000__INCLUDED_
    
    #include "TurnpointArray.h"	// Added by ClassView
    #if _MSC_VER >= 1000
    #pragma once
    #endif // _MSC_VER >= 1000
    
    #include "ContestantList.h"
    #include "EventList.h"
    #include "TaskList.h"
    #include "ScoreRecordList.h"
    #include "ClassTabCtrl.h"
    #include "PenalityList.h"
    #include "FlightList.h"
	#include "wsclass.h"
	#include "GliderInfoList.h"
    
    class CWinscoreDoc : public CDocument
    {
    protected: // create from serialization only
    	CWinscoreDoc();
    	DECLARE_DYNCREATE(CWinscoreDoc)

    // Attributes
    public:
    
    // Operations
    public:
    
    // Overrides
    	// ClassWizard generated virtual function overrides
    	//{{AFX_VIRTUAL(CWinscoreDoc)
    	public:
    	virtual void DeleteContents();
    	virtual BOOL OnNewDocument();
    	virtual void OnCloseDocument();
    	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    	//}}AFX_VIRTUAL
    
    // Implementation
    public:

	BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);
	void ImportXML		(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMWinscoreNode);
	void GetXML			(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pIDOMWinscoreNode);
    void ImportXMLClasses(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr pWinscore );
    void ImportXMLGridPositions(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr pWinscore );
	bool ExportCumulativeXML( CString &strFile, int nClasses, EClass aeClasses[] );


  	CString GetTextTrailer(	CString &cNewLineChar, CString cComment, EExportType	eExportType );
  	CString CWinscoreDoc::GetTextHeader( CTime& cDate,
  								     EScoresheetType eType,
  									 CString   &cNewLineChar );
  
  	CString GetCumulativeText(	EClass		eClass, 
  								CString		&cNewLine,
  								bool		bTabs );
  
  	CString CWinscoreDoc::GetDayText(	EClass			eClass, 
  										CTime&			cDate, 
  										ESortBy		eSortBy,
  										CString		&cNewLineChar,
  										bool		bTabs			 );
  
    	void DrawScoresheetTrailer(CDC *pDC);
        void ExportTextTrailer(CStdioFile& cStdioFile, CString &cNewLineChar, CString cComment="", EExportType	eExportType=eText);
    
    
    	void CWinscoreDoc::ExportTextHeader(CStdioFile& cStdioFile, 
    									CTime& cDate,
    								    EScoresheetType eType,
    									CString     &cNewLineChar);
    	
    									
    	BOOL ExportCumulative(	CStdioFile& cStdioFile,
    							EClass		eClass );
    
    	void CWinscoreDoc::DrawDay(	CDC*			  pDC,
    								EClass			  eClass, 
    								CTime&			  cDate,
    								POSITION		 &posScoreLine,
    								CScoreRecordList *pcScoreRecordList,
    								int				 &nRecordsPrinted,
    								int				 &iYptr,
    								double			 &dPrevScore,
    								int				 &iPrevStanding,
    								int				 &iStanding,
    								int				 &nCodes,
    								CStringArray     &cCodeArray, 
    								CStringArray     &cReasonArray	);
    
		BOOL CWinscoreDoc::ExportDayText(	CStdioFile& cStdioFile,
    										EClass			eClass, 
    										CTime&			cDate, 
    										ESortBy		eSortBy,
    										CString     &cNewLineChar);
    
    
    	void UpdateCumScores(EClass eClass);
    	BOOL Valid();
    	int GetNumClasses();
    	int GetContestDay(CTime cDate, EClass eClass);
    	BOOL NoContestDay(CTime cDate, EClass eClass);
    	CTime FirstContestDay();
//    	double GetMinTaskDistance(EUnits eUnits, EClass eClass);
//    	CTimeSpan GetMinTaskTime(EClass eClass);
    	void LoadDateTabs(CTabCtrl& cTabCtrl);
    	void CalculateScores( CTime cDate, EClass eClass,  CWinscoreListCtrl& cStatus, CSummary& cSummary, CXMLMgr *pMgr=NULL, MSXML2::IXMLDOMNodePtr pClassNode=NULL );
    	void CalculateDayScores(CTime cDate, EClass eClass, CWinscoreListCtrl& cStatus, CSummary& cSummary, CXMLMgr *pMgr=NULL, MSXML2::IXMLDOMNodePtr pClassNode=NULL );
    	BOOL ExportDay(	CStdioFile& cFile,
    					EClass			eClass, 
    					CTime&			cDate, 
    					ESortBy		eSortBy);
    
    	void FormatPenalityLine(			EClass			eClass,
											CScoreRecord	*pcScoreRecord,
    									    CString			&cCodes,
    										CString			&cPenality,
    										int				&nCodes,
    										CStringArray	&cCodeArray,
    										CStringArray	&cReasonArray);
    
    	void LoadContestantComboBox(CComboBox& cCombo, BOOL bBlankContestant=FALSE);
    	CString GetUnitsText();
		CString GetUnitsSpeedText();
    	void FreeDateComboBox(CComboBox&);
    	void LoadDateComboBox(CComboBox&,bool bIncludePreContest=false);
    	void InitializeDefaultContest();
    	BOOL PutUpContestInfoDlg(BOOL bInitialize);
    	void SetAvailableClasses(CComboBox*);
    	void SetAvailableClasses(CClassTabCtrl&);
		CTime	GetPreContestDate();
		int		GetNumPreContestDays();

  
    	void CalculateTimeDistance(	CScoreRecordList& cScoreRecordList, 
    								CTask* pcTask, 
    								CTime cDate, 
    								EClass eClass, 
    								CWinscoreListCtrl& cStatus);
    
    void CalculateHandicapData(	CScoreRecordList& cScoreRecordList, 
    							 CTask* pcTask, 
    							 CTime cDate, 
    							 EClass eClass, 
    							 CWinscoreListCtrl& cStatus );
    
    
    	double	GetHandicapDistance(double dDistance, CContestant* pcContestant);
    
    	void CWinscoreDoc::DrawTaskInfoSheet(CDC *pDC, CTask *pcTask, CString& cContestName, CString& cContestLocation );
   
    
    //  Top level contest data
  
    	CString			m_strContestName;
    	CString			m_strLocation;
    	EContest		m_eContest;
    	EUnits			m_eUnits;
    	CTime			m_CContestStartDate;
		CTime			m_cPreContestDate;
        ECoordFormat    m_eCoordinateFormat;

		int				m_iNumContestDays;
		int				m_iNumPracticeDays;
    	CTime			m_caPracticeDays[30];
    	CTime			m_caContestDays[301];
    	BOOL			m_bGridPositionCalculated[NUMCLASSES];
    
    	//  Data Arrays/Lists
    	CTurnpointArray  m_turnpointArray;
    	CContestantList	 m_contestantList;
        CEventList       m_eventList;
    	CTaskList		 m_taskList;
    	CScoreRecordList m_ScoreRecordList;

    	CPenalityList	 m_PenalityList;
    	CStringArray	 m_caGridPositions[NUMCLASSES];

	  	CFlightList		 m_FlightList;
    	CStringArray	 m_straPassedLogs;
  
    	CString			 m_strImportFilePath;

		CSemaphore		m_cSem_ExportDayXML, m_cSem_DoSave;

		CGliderInfoList  m_cGIList;
    
    	virtual ~CWinscoreDoc();
    #ifdef _DEBUG
    	virtual void AssertValid() const;
    	virtual void Dump(CDumpContext& dc) const;
    #endif
    
    public:
	    void AutoExportXML(CTime cDate);
	    int m_iSSA_ID;
	    bool ExportDayXML(CString &strFile, int nClasses, EClass aeClasses[], CTime cDate, CString& strFail, bool bCompute=true);
	    CString GetFTPContestName();
	    int IsPracticeDay(CTime cDate);
		void LogPassed(CString strIGCFileName);
		void LogFailed(CString strIGCFileName);

	    bool PassedSecurity(CString strIGCFileName);
	    bool FailedSecurity(CString strIGCFileName);
	    CTurnpoint* GetTurnpointByID( int iID);
	    void CalculateGridPositions(EClass eClass, bool bForceRecalculation);

    	CString m_cTaskInfoString;
    	bool IsTurnpointAirfield(int iCSID);
    	bool IsCPIDValid(int cid);
    	bool IsContestNoValid(CString cContestNo);
    	bool IsDateValid(CTime cDate);
    	CString m_cContestDescription;
    	CString m_cContestSerialNo;
		EDatum m_eDatum;

		int GetMultipleStartPoints(	CFlight *pcFlight, 
									CTask *pcTask,
									int &iBestScorePtr,
									bool &bAlreadyBest,
									int iMaxReturn, 
									CTime caStartTimes[], 
									CTimeSpan caTOCs[], 
									double daSpeeds[], 
									double daDistances[], 
									int iaPointDiff[], 
									int iaPenalties[], 
									int iaPoints[] );

		double GetLogTurninRatio(CTime cDate, EClass eClass);

	
		void DrawTask(CDC *pDC, CTask &cTask, int iYStart, int *piYFinish);
		void CWinscoreDoc::DrawGrid(	CDC *pDC,
    										CTask&	cTask,
    										int iYStart, 
    										int nContestants,
    										int *piYFirstLine);
    
    void CWinscoreDoc::DrawLine(CDC		*pDC,
    							int		*piYLine, 
    							CTask	*pcTask, 
    							CContestant  *pcContestant,
    							CScoreRecord	*pcScoreRecord,
    							int				&nCodes,
    							int				&iStanding,
    							double			&dPrevScore,
    							int				&iPrevStanding,
    							CStringArray	&cCodeArray,
    							CStringArray	&cReasonArray);
    
    BOOL CWinscoreDoc::DrawHeader(	 CDC *pDC, 
    								 CTime &cDate,
    								 EClass	eClass,
    								 int *piYpos);
    
    BOOL CWinscoreDoc::DrawTitle(	CDC *pDC, 							
    								CTime &cDate,
    								EClass	eClass,
    								int &iYpos);
    
    BOOL CWinscoreDoc::ExportCumulativeText(	CStdioFile&	cStdioFile,
    											EClass		eClass, 
    											CString		&cNewLine);

	bool SaveReminder();
    
    protected:
    
    // Generated message map functions
    protected:
    	//{{AFX_MSG(CWinscoreDoc)
    	afx_msg void OnFileNew();
    	//}}AFX_MSG
    	DECLARE_MESSAGE_MAP()
	public:
		CString GetWSCPath(void);
};
    
    void FreeDateComboBox(CComboBox&);
    
    /////////////////////////////////////////////////////////////////////////////
    
    //{{AFX_INSERT_LOCATION}}
    // Microsoft Developer Studio will insert additional declarations immediately before the previous line.
    
    #endif // !defined(AFX_WINSCOREDOC_H__BAD124E0_4095_11D1_ACCF_000000000000__INCLUDED_)

