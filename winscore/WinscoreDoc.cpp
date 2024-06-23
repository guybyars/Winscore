//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
    // WinscoreDoc.cpp : implementation of the CWinscoreDoc class
    //
	#include<time.h>
	#include <stdlib.h>

    #include "stdafx.h"
    #include "Winscore.h"
    #include "wsversion.h"

    #include "WinscoreDoc.h"
    #include "ListVwEx.h"
    #include "WinscoreView.h"
    #include "ContestInfoDlg.h"
    #include "Global_Prototypes.h"
    #include "WinscoreFonts.h"
	#include "classlistctrl.h"
    #include <math.h>
	#include <atlbase.h>
    #include "suautil.h"

    static CTime cLastSavedTime;

   #ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
    #endif
    
    
    #define PAGE_WIDTH    800
    #define LINE_HEIGHT   20
    #define PENALITY_CODE_Y   -950
    #define PENALITY_CODE_LEN  75
    
    
    #define LEFTMARGIN  0
    #define CUMPOINTS	1
    #define CONID		2
    #define	CONNAME		3
    #define	CONGLIDER	4
    #define DAYRANK		5
    #define	DAYPOINTS	6
    #define	DAYSPEED	7
    #define	DAYDIST		8
    #define REMCODE		9
    #define	REMPOINTS	10
    #define RIGHTMARGIN 11
    //									 p   id   name glid	pos  pts  spd  dis  cod  pts  marg	
    static const int iaSpeedTab[12]= {0, 50, 110, 150, 300, 400, 430, 480, 580, 680, 750, 800};
    static const int aiHdapTab[2]={									  530, 630};
    

#define OUTPUT_TPT_HTML( pcTurnpoint, dDist, dRadius, cLine, cStdioFile ) {\
				\
				if( dRadius>0 )\
				{\
    			cLine.Format(	_T("<tr>  <td>%d</td> <td>%s</td> <td>%6.2lf</td> <td>%6.2lf</td></tr>\n"), \
    							pcTurnpoint->m_iID,		\
    							pcTurnpoint->m_strName, \
    							dDist,					\
								dRadius);				\
				}\
				else\
				{\
   				cLine.Format(	_T("<tr>  <td>%d</td> <td>%s</td> <td>%6.2lf</td> <td> - </td> </tr>\n"), \
    								pcTurnpoint->m_iID,		\
    								pcTurnpoint->m_strName, \
    								dDist );				\
				}\
				cStdioFile.WriteString( cLine ); }					\



#define OUTPUT_TPT_OUT( pcTurnpoint, dDist, dRadius, pDC, iXTask, iYpos, aiAssignedTabs ) {\
				\
				CString cText;\
    			cText.Format(_T("%d"),pcTurnpoint->m_iID );\
    			pDC->TextOut( iXTask+aiAssignedTabs[0], iYpos, cText, cText.GetLength() );\
				\
    			cText=pcTurnpoint->m_strName;\
    			pDC->TextOut( iXTask+aiAssignedTabs[1], iYpos, cText, cText.GetLength() );\
    			\
    			cText.Format( _T("%6.2lf"), dDist );\
    			pDC->TextOut( iXTask+aiAssignedTabs[2], iYpos, cText, cText.GetLength() );\
    			cTextSize=pDC->GetTextExtent(cText);\
				\
				if(dRadius>0.001) {\
    			cText.Format( _T("%6.2lf"), dRadius );\
    			pDC->TextOut( iXTask+aiAssignedTabs[3], iYpos, cText, cText.GetLength() );\
				}\
    			iYpos-=cTextSize.cy;\
				}\


    /////////////////////////////////////////////////////////////////////////////
    // CWinscoreDoc
    IMPLEMENT_DYNCREATE(CWinscoreDoc, CDocument)
    
    BEGIN_MESSAGE_MAP(CWinscoreDoc, CDocument)
    	//{{AFX_MSG_MAP(CWinscoreDoc)
    	ON_COMMAND(ID_FILE_NEW, OnFileNew)
    	//}}AFX_MSG_MAP
    END_MESSAGE_MAP()
    
    /////////////////////////////////////////////////////////////////////////////
    // CWinscoreDoc construction/destruction

    CWinscoreDoc::CWinscoreDoc()
    {
    	// Initialize defaults here
    	InitializeDefaultContest();
    
    }
    
    CWinscoreDoc::~CWinscoreDoc()
		{

		}

    void CWinscoreDoc::ImportXMLClasses(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pWinscore )
			{
			MSXML2::IXMLDOMNodePtr pClasses;
			TESTHR(cMgr.SelectChild(_T("Classes"),pWinscore,pClasses));
			if( pClasses==NULL ) return;

			MSXML2::IXMLDOMNodeListPtr pClassList=cMgr.GetChildList(pClasses );
			if( pClassList==NULL ) return;

			long length=0;
			TESTHR(pClassList->get_length(&length));
			if( length==0 ) return;

			for( int i=0; i<length; i++ )
				{
				MSXML2::IXMLDOMNodePtr pClass;
				TESTHR(pClassList->get_item(i,&pClass));

				CWSClass cClass;
				cClass.ImportXML(cMgr, pClass);
				cClass.SetActive(true);
				CWSClass &cClass2=GetClass(cClass.m_eContest, cClass.m_eClass);
				cClass2=cClass;
				}
			}

    void CWinscoreDoc::ImportXMLGridPositions(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pWinscore )
			{
			TCHAR *token;
			TCHAR seps[]   = _T(" ,\t\n");

			MSXML2::IXMLDOMNodePtr pGrids;
			TESTHR(cMgr.SelectChild(_T("GridPositions"),pWinscore,pGrids));
			if( pGrids==NULL ) return;

			MSXML2::IXMLDOMNodeListPtr pGridList=cMgr.SelectNodeList(_T("GridPosition"),pGrids );
			if( pGridList==NULL ) return;

			long length=0;
			TESTHR(pGridList->get_length(&length));
			if( length==0 ) return;

			for( int i=0; i<length; i++ )
				{
				MSXML2::IXMLDOMNodePtr pGridPosition;
				TESTHR(pGridList->get_item(i,&pGridPosition));

				int iPos;
				CString strTemp;
				GET_XML_INT( cMgr, pGridPosition, _T("ClassID"),	  int, iPos, 0 );
				GET_XML_STR( cMgr, pGridPosition, _T("Positions"),  strTemp );
				if( strTemp.GetLength()==0 ) continue;

				m_bGridPositionCalculated[iPos]=true;
				TCHAR *pstr=new TCHAR[strTemp.GetLength()+1];
				strcpy_s(pstr, strTemp.GetLength()+1, strTemp);

				TCHAR *next;
			    token = strtok_s( pstr, seps, &next );
			    while( token != NULL )
				   {
				   m_caGridPositions[iPos].Add(token);
				   token = strtok_s( NULL, seps, &next  );
				   }
			 	 delete pstr;
				}
			}


    void CWinscoreDoc::ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMWinscoreNode )
		{
		int nSkipped=0;
		GET_XML_STR( cMgr, pIDOMWinscoreNode, _T("Name"),		(m_strContestName) );
		GET_XML_STR( cMgr, pIDOMWinscoreNode, _T("Location"),	(m_strLocation) );
		GET_XML_INT( cMgr, pIDOMWinscoreNode, _T("Contesttype"),	EContest,	m_eContest, eRegional );
		GET_XML_INT( cMgr, pIDOMWinscoreNode, _T("Units"),		EUnits,		m_eUnits,	eStatute );
		GET_XML_INT( cMgr, pIDOMWinscoreNode, _T("NumberOfDays"),	int,		m_iNumContestDays, 6 );
		GET_XML_INT( cMgr, pIDOMWinscoreNode, _T("NumberOfPracticeDays"),	int,		m_iNumPracticeDays, 2 );
		GET_XML_INT( cMgr, pIDOMWinscoreNode, _T("SSAID"),	int,	m_iSSA_ID, 0 );

		GET_XML_INT( cMgr, pIDOMWinscoreNode, _T("CoordinateFormat"),	ECoordFormat, m_eCoordinateFormat, eHHMMMM );
		GET_XML_STR( cMgr, pIDOMWinscoreNode, _T("Importflightpath"),	(m_strImportFilePath) );
		GET_XML_INT( cMgr, pIDOMWinscoreNode, _T("Datum"),	EDatum, m_eDatum, DEFAULTDATUM );
		CLocation::SetDatum(m_eDatum);

//The cr/lf's in the task info string cause sever problems
//		GET_XML_STR( cMgr, pIDOMWinscoreNode, _T("Taskinfostring"),	(m_cTaskInfoString) );

		ImportXMLDate(cMgr, pIDOMWinscoreNode, _T("startdate"), m_CContestStartDate);

    	for( int i=0; i<m_iNumContestDays; i++) 
				m_caContestDays[i]=m_CContestStartDate+CTimeSpan(i,0,0,0);

    	for( int i=0; i<m_iNumPracticeDays; i++) 
				m_caPracticeDays[i]=m_CContestStartDate-CTimeSpan(i+1,0,0,0);

   		m_turnpointArray.ImportXML	(cMgr, pIDOMWinscoreNode);
	  	m_contestantList.ImportXML	(cMgr, pIDOMWinscoreNode,nSkipped);
		ImportXMLClasses			(cMgr, pIDOMWinscoreNode);
		ImportXMLGridPositions		(cMgr, pIDOMWinscoreNode);
    	m_taskList.ImportXML2		(cMgr, pIDOMWinscoreNode);
    	m_ScoreRecordList.ImportXML	(cMgr, pIDOMWinscoreNode);
    	m_PenalityList.ImportXML	(cMgr, pIDOMWinscoreNode);

		ImportRegistryXML(cMgr, pIDOMWinscoreNode );

		m_FlightList.ImportXML(cMgr, pIDOMWinscoreNode, m_taskList);

		MSXML2::IXMLDOMNodePtr pPassedLogs;
		TESTHR(cMgr.SelectChild(_T("PassedLogs"), pIDOMWinscoreNode, pPassedLogs));
		if( pPassedLogs ) 
			{

			MSXML2::IXMLDOMNodeListPtr pPassedLogsList=cMgr.GetChildList(pPassedLogs);
			if( pPassedLogsList ) 
				{
				long length=0;
				TESTHR(pPassedLogsList->get_length(&length));
				for( int i=0; i<length; i++ )
					{
					MSXML2::IXMLDOMNodePtr pPassedLog;
					TESTHR(pPassedLogsList->get_item(i,&pPassedLog));
					BSTR bstr;
					TESTHR(pPassedLog->get_text(&bstr));
					CString str=bstr;
					m_straPassedLogs.Add(str);
					}
				}
			}
		}

	void CWinscoreDoc::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pIDOMWinscoreNode )
		{
		cMgr.CreateElement( pIDOMWinscoreNode, _T("Version"),	WINSCOREVERSION );
		cMgr.CreateElement( pIDOMWinscoreNode, _T("Name"),	LPCSTR(m_strContestName) );
		cMgr.CreateElement( pIDOMWinscoreNode, _T("Location"),	LPCSTR(m_strLocation) );
		cMgr.CreateElementInt( pIDOMWinscoreNode, _T("Contesttype"),	(WORD)m_eContest );
		cMgr.CreateElementInt( pIDOMWinscoreNode, _T("Units"),	(WORD)m_eUnits );
		cMgr.CreateElementInt( pIDOMWinscoreNode, _T("NumberOfDays"),	m_iNumContestDays );
		cMgr.CreateElementInt( pIDOMWinscoreNode, _T("NumberOfPracticeDays"),	m_iNumPracticeDays );
		cMgr.CreateElementInt( pIDOMWinscoreNode, _T("SSAID"),	m_iSSA_ID );
		cMgr.CreateElementInt( pIDOMWinscoreNode, _T("CoordinateFormat"),	(WORD)m_eCoordinateFormat );
		cMgr.CreateElementInt( pIDOMWinscoreNode, _T("Datum"),	(WORD)m_eDatum );
		cMgr.CreateElement( pIDOMWinscoreNode, _T("Importflightpath"),	LPCSTR(m_strImportFilePath) );

   		GetXMLDate(cMgr, pIDOMWinscoreNode, _T("startdate"), m_CContestStartDate);
		
		MSXML2::IXMLDOMNodePtr pClasses=NULL;
		cMgr.CreateChild( pIDOMWinscoreNode, pClasses, _T("Classes"));

		bool bAnyGrids=false;
   		for( int j=0; j<NUMCLASSES; j++ )
    		{
			if( !GetClass(j).IsActive() ) continue;
			GetClass(j).GetXML(cMgr, pClasses);
    		if( m_bGridPositionCalculated[j] ) bAnyGrids=true;
    		}

		if( bAnyGrids )
			{
			MSXML2::IXMLDOMNodePtr pGridPositions=NULL;
			cMgr.CreateChild( pIDOMWinscoreNode, pGridPositions, _T("GridPositions"));

    		for( int j=0; j<NUMCLASSES; j++ )
	  			{
    			if( !m_bGridPositionCalculated[j] ) continue;

				MSXML2::IXMLDOMNodePtr pGridPosition=NULL;
				cMgr.CreateChild( pGridPositions, pGridPosition, _T("GridPosition"));
				CString strTemp;	
				for( int ig=0; ig<m_caGridPositions[j].GetSize(); ig++ )
					{
					strTemp+=m_caGridPositions[j].ElementAt(ig);
					strTemp+=_T(", ");
					}
				cMgr.CreateElementInt( pGridPosition, _T("ClassID"),	 j  );
				cMgr.CreateElement( pGridPosition, _T("Positions"),  LPCSTR(strTemp) );
    			}
			}

    	m_turnpointArray.GetXML(cMgr, pIDOMWinscoreNode);
	  	m_contestantList.GetXML(cMgr, pIDOMWinscoreNode);
    	m_taskList.GetXML(cMgr, pIDOMWinscoreNode);
    	m_ScoreRecordList.GetXML(cMgr, pIDOMWinscoreNode);
    	m_PenalityList.GetXML(cMgr, pIDOMWinscoreNode);
		m_FlightList.GetXML(cMgr, pIDOMWinscoreNode);
		GetRegistryXML(cMgr, pIDOMWinscoreNode );

		if( m_straPassedLogs.GetSize() >0 )
			{
			MSXML2::IXMLDOMNodePtr pPassedLogs=NULL;
			cMgr.CreateChild( pIDOMWinscoreNode, pPassedLogs, _T("PassedLogs"));
			for( int i=0; i<m_straPassedLogs.GetSize(); i++ )
				cMgr.CreateElement( pPassedLogs, _T("PassedLog"),  LPCSTR(m_straPassedLogs.GetAt(i) ) );

			}

//		m_straPassedLogs.GetXML(pIDOMWinscoreNode);
		}

    
    /////////////////////////////////////////////////////////////////////////////
    // CWinscoreDoc diagnostics
    
    #ifdef _DEBUG
    void CWinscoreDoc::AssertValid() const
    {
    	CDocument::AssertValid();
    }
    
    void CWinscoreDoc::Dump(CDumpContext& dc) const
    {
    	CDocument::Dump(dc);
    }
    #endif //_DEBUG
       

	
void CWinscoreDoc::SetAvailableClasses(CComboBox *pcComboBox )
    {
       	int iCurSel=pcComboBox->GetCurSel();
    	int iDefault=0;
    	iCurSel=max( iCurSel, 0);
    	pcComboBox->ResetContent( );
    	int nList=0;
    	EClass eDefaultClass=GetDefaultClass();
 
		for( int iClass=0; iClass<NUMCLASSES; iClass++ )
			{
			if( GetClass(iClass).IsActive() )
    			{
    			int iSel=pcComboBox->AddString( GetClass(iClass).GetName());
    			pcComboBox->SetItemData( iSel, GetClass(iClass).GetType());
    			if( eDefaultClass== GetClass(iClass).GetType() ) iDefault=iSel;
    			nList++;
    			}
			}
    	pcComboBox->SetCurSel(iDefault);
    	pcComboBox->SetTopIndex(iDefault);
    
    // Better not grey out if only one in the list... don't want
    // the dreaded <GASP> "SPORTS CLASS GREYOUT!!!!
    //	if( nList==1 )  pcComboBox->EnableWindow(FALSE);
    
    }
    void CWinscoreDoc::SetAvailableClasses(CClassTabCtrl& cClassTabCtrl)
    {
    	TC_ITEM  TabCtrlItem;
        TabCtrlItem.mask = TCIF_TEXT ; 
    	int nList=0;
    	EClass eDefaultClass=GetDefaultClass();
    	int iDefault=0;
    	int iSel=0;
    
    	cClassTabCtrl.DeleteAllItems();

/*
		for( int iClass=0; iClass<NUMCLASSES; iClass++ )
			{			
			bool bAlreadyThere=false;
			for( int j=0; j<iClass; j++ )
				if( GetClass(j).GetType()==GetClass(iClass).GetType() ) bAlreadyThere=true;
			if( !bAlreadyThere &&  GetClass(iClass).IsActive() )
    			{
    			TabCtrlItem.pszText= (char *)LPCTSTR(GetClass(iClass).GetName());    
    			cClassTabCtrl.m_aeClasses[nList]= GetClass(iClass).GetType();
    			cClassTabCtrl.InsertItem(nList++, &TabCtrlItem);
    			if( eDefaultClass== GetClass(iClass).GetType() ) iDefault=iSel;
    			iSel++;
    			}
			}
*/
		for( int iClass=0; iClass<NUMCLASSES; iClass++ )
			{			
			if( GetClass(iClass).IsActive() )
    			{
				CString strClassName=GetClass(iClass).GetName();
    			TabCtrlItem.pszText= (TCHAR*)LPCSTR(strClassName);    
    			cClassTabCtrl.m_aeClasses[nList]= GetClass(iClass).GetType();
    			cClassTabCtrl.InsertItem(nList++, &TabCtrlItem);
    			if( eDefaultClass== GetClass(iClass).GetType() ) iDefault=iSel;
    			iSel++;
    			}
			}

    	cClassTabCtrl.SetCurSel(iDefault);
    }
    
    void CWinscoreDoc::OnFileNew() 
    	{
    	CTime cToday = CTime::GetCurrentTime();
    	int iYear=cToday.GetYear();
    
    
    	//  Make sure the program can be used in the current year.
    
    	if(	iYear>CURRENTYEAR )
    		{
    		CString cError;
    		cError.Format(
    			_T("The program is only valid for %d contests, but it is now %d.\nPlease use the  %d version of Winscore or a different scoring program."),
    			CURRENTYEAR, iYear, iYear);
    		AfxMessageBox( cError, MB_OK, 0 );
    		return;
    		};
    
    	if( SaveModified()==0 ) return;
    	if( PutUpContestInfoDlg(FALSE) )
    		{
    		POSITION pos=GetFirstViewPosition();
    		CWinscoreView* pView = (CWinscoreView*)GetNextView(pos);
			pView->InitialUpdate();
    		pView->ViewContestantList();   
    		}
		CString strNull;
		SetWSCPath(strNull);
    	}
    
    BOOL CWinscoreDoc::PutUpContestInfoDlg(BOOL bExists)
    	{
    	CContestInfoDlg	dlg(this);
    
    	if( bExists )
    		{
    		dlg.m_strContestName	= m_strContestName;
    		dlg.m_strLocation		= m_strLocation;
    		dlg.m_eContest			= m_eContest;
    		dlg.m_eUnits			= m_eUnits;
			dlg.m_eDatum			= m_eDatum;
    		dlg.m_iNumDays			= m_iNumContestDays;
    		dlg.m_iNumPracticeDays	= m_iNumPracticeDays;
    		dlg.m_cContestStartDate	= m_CContestStartDate;
    		dlg.m_eCoordinateFormat = m_eCoordinateFormat;
			dlg.m_iSSA				= m_iSSA_ID;
			for( int iClass=0; iClass<NUMCLASSES; iClass++ )
				{
				dlg.m_acClass[iClass]=GetClass(iClass);
				}
    		}
    	else
    		{
    		dlg.m_strContestName	= _T("");
    		dlg.m_strLocation		= _T("");
    		dlg.m_eContest			= eRegional;
    		dlg.m_eUnits			= eStatute;
    		dlg.m_iNumDays			= 6;
			dlg.m_iSSA				= 0;
			dlg.m_eDatum			= DEFAULTDATUM;
		
			CTime tnow = CTime::GetCurrentTime();
    		dlg.m_cContestStartDate	= tnow;
    		dlg.m_eCoordinateFormat = eHHMMMM;
			int iC=0;
			for( int i=0; i<(NUMCLASSES/2); i++)
				{
				dlg.m_acClass[iC++].Initialize(eRegional, (EClass)i);
				dlg.m_acClass[iC++].Initialize(eNational, (EClass)i);
				}
    		}
    	
    	if (dlg.DoModal() == IDOK)
    		{
    		if( !bExists ) 
    			{
    			DeleteContents();
    			CDocument::OnNewDocument();
    			}

    		m_strContestName	= dlg.m_strContestName;
    		m_strLocation		= dlg.m_strLocation;
    		m_eContest			= dlg.m_eContest;
    		m_eUnits			= dlg.m_eUnits;
    		m_iNumContestDays	= dlg.m_iNumDays;
    		m_iNumPracticeDays	= dlg.m_iNumPracticeDays;
			m_iSSA_ID			= dlg.m_iSSA;
            m_eDatum            = dlg.m_eDatum;
            CLocation::SetDatum(m_eDatum);

			if( bExists &&
				m_CContestStartDate.GetYear() != dlg.m_cContestStartDate.GetYear() )
				{
				CString strError;
				strError.Format(_T("WARNING: Changing the year of an old contest file can produce incorrect results.  Always start a contest with a new contest file."));
				AfxMessageBox( strError );
				}
    		m_CContestStartDate	= dlg.m_cContestStartDate;
    		m_eCoordinateFormat = dlg.m_eCoordinateFormat;
    
    		CTimeSpan cOneDay(1, 0, 0, 0 );
    
    		m_caPracticeDays[0]=m_CContestStartDate-cOneDay;
    		for( int i=1; i<m_iNumPracticeDays; i++)
    			{
    			m_caPracticeDays[i]=m_caPracticeDays[i-1]-cOneDay; 
    			}

    		m_caContestDays[0]=m_CContestStartDate;
    		for( int i=1; i<m_iNumContestDays; i++)
    			{
    			m_caContestDays[i]=m_caContestDays[i-1]+cOneDay ; 
    			}


			for( int iClass=0; iClass<NUMCLASSES; iClass++ )
				{
				GetClass(iClass)=dlg.m_acClass[iClass];
				}
    
    		SetTitle(m_strContestName);
    		SetModifiedFlag(TRUE);
    
    		return TRUE;
    		}
    	return FALSE;
    	}
    
    void CWinscoreDoc::InitializeDefaultContest()
		{
    	m_strContestName	=_T("");
    	m_eContest			=eRegional;
    	m_eUnits			=eStatute;
		m_eDatum            =DEFAULTDATUM;
        CLocation::SetDatum(m_eDatum);
    	m_iNumContestDays	=6;
    	m_iNumPracticeDays	=2;
    	CTime tnow = CTime::GetCurrentTime();
    	m_CContestStartDate	=tnow;
		m_iSSA_ID			=0;

		for(int i=0; i<m_iNumContestDays; i++)
			m_caContestDays[i]=tnow+CTimeSpan(1+i,0,0,0);

		for(int i=0; i<m_iNumPracticeDays; i++)
			m_caPracticeDays[i]=tnow-CTimeSpan(1+i,0,0,0);

        m_eCoordinateFormat	=eHHMMMM;
    	for( int i=0; i<NUMCLASSES; i++ )
			{
			m_bGridPositionCalculated[i]=FALSE;
			m_caGridPositions[i].RemoveAll();
			}
    	m_strImportFilePath		=_T("");
    	m_cTaskInfoString		=_T("");

		int iC=0;
		for( int i=0; i<(NUMCLASSES/2); i++)
			{
			GetClass(iC++).Initialize(eRegional, (EClass)i);
			GetClass(iC++).Initialize(eNational, (EClass)i);
			}
   
    }
    
    void CWinscoreDoc::DeleteContents() 
    {
    	// TODO: Add your specialized code here and/or call the base class
    
    	InitializeDefaultContest();
    
    	m_turnpointArray.Purge();
    	m_contestantList.Purge();
    	m_taskList.Purge();
    	m_ScoreRecordList.Purge();
    	m_PenalityList.Purge();
      	m_FlightList.Purge();
		m_straPassedLogs.RemoveAll();

		GetSUAArray()->Purge();

		for( int iClass=0; iClass<NUMCLASSES; iClass++ )
			m_caGridPositions[iClass].RemoveAll( );

		PurgeWinscoreRegistry();

    	CDocument::DeleteContents();
    }
    
    void CWinscoreDoc::LoadDateComboBox(CComboBox &cComboBox)
    {
    
    	CTime cDefaultDay = GetDefaultDay();
    
    	int iSel= -1;
    	int iPos=0;
    
    	for( int i=m_iNumPracticeDays-1; i>=0; i-- )
    		{
    		CString s = m_caPracticeDays[i].Format( _T("Practice Day %A, %B %d, %Y") );
    		iPos=cComboBox.AddString(s);
    		ASSERT(iPos>=0);
    		if( m_caPracticeDays[i].GetDay()  ==cDefaultDay.GetDay()	&&
    			m_caPracticeDays[i].GetMonth()==cDefaultDay.GetMonth() &&
    			m_caPracticeDays[i].GetYear() ==cDefaultDay.GetYear()		) iSel=iPos;
    		cComboBox.SetItemDataPtr( iPos, &m_caPracticeDays[i] );
    		}

    	for( int i=0;i<m_iNumContestDays;i++ )
    		{
    		CString s = m_caContestDays[i].Format( _T("%A, %B %d, %Y") );
    		iPos=cComboBox.AddString(s);
    		ASSERT(iPos>=0);
    		if( m_caContestDays[i].GetDay()	 ==cDefaultDay.GetDay()	&&
    			m_caContestDays[i].GetMonth()==cDefaultDay.GetMonth() &&
    			m_caContestDays[i].GetYear() ==cDefaultDay.GetYear()		) iSel=iPos;
    		cComboBox.SetItemDataPtr( iPos, &m_caContestDays[i] );
    		}
    	if( iSel<0 ) iSel=0;
    	cComboBox.SetCurSel(iSel);
    }
    
    void FreeDateComboBox(CComboBox& cComboBox)
    {
    	int nItems=cComboBox.GetCount();
    	if( nItems<=0 ) return;
    	cComboBox.ResetContent();
    }
    
    CString CWinscoreDoc::GetUnitsText()
    {
    	return UnitsText(m_eUnits);
    }

    CString CWinscoreDoc::GetUnitsSpeedText()
    {
    	return UnitsSpeedText(m_eUnits);
    }


    void CWinscoreDoc::LoadContestantComboBox(CComboBox & cComboBox, BOOL bBlankContestant )
    {
    	int iPos;
    	cComboBox.ResetContent();
    	if( m_contestantList.IsEmpty() !=0  )  return;
    
    	if( bBlankContestant )
    		{
    		iPos=cComboBox.AddString(_T("   "));
    		cComboBox.SetItemDataPtr( iPos, NULL );
    		}
    		
    	POSITION pos = m_contestantList.GetHeadPosition();
    	while(pos!=NULL)
    		{
    		CContestant* pcContestant=m_contestantList.GetNext(pos);
    		if( pcContestant ) 	
    			{
    			iPos=cComboBox.AddString(pcContestant->m_strContestNo);
    			cComboBox.SetItemDataPtr( iPos, pcContestant );
    			}
    		}
    	cComboBox.SetCurSel(0);
    }
    
    
    
    void CWinscoreDoc::LoadDateTabs(CTabCtrl & cTabCtrl)
    {
    	CTime cToday = CTime::GetCurrentTime();
    	TC_ITEM  TabCtrlItem;
        TabCtrlItem.mask = TCIF_TEXT | TCIF_PARAM ; 
    
    	int iDay=m_CContestStartDate.GetDay();
    	int iMonth=m_CContestStartDate.GetMonth();
    	int iYear=m_CContestStartDate.GetYear();
    	int iSel=0;
    
    
    	CTime *pcTime;
    
    	cTabCtrl.DeleteAllItems();
    
    	for( int i=0;i<m_iNumContestDays;i++ )
    		{
    		pcTime= new CTime( iYear, iMonth, iDay+i, 0, 0, 0 );
    		CString s = pcTime->Format( _T("%a, %m %d") );
    
    		if( pcTime->GetDay()	==cToday.GetDay()	&&
    			pcTime->GetMonth()	==cToday.GetMonth() &&
    			pcTime->GetYear()	==cToday.GetYear()		) iSel=i;
    
    		TCHAR cBuff[100];
    		strcpy_s (cBuff,s);
    		TabCtrlItem.pszText=cBuff;    
    		TabCtrlItem.lParam=(LPARAM)pcTime;
    		cTabCtrl.InsertItem(i, &TabCtrlItem);
    		}
    	cTabCtrl.SetCurSel(iSel);
    }
    
    
    void CWinscoreDoc::CalculateScores(CTime cDate, EClass eClass, CWinscoreListCtrl& cStatus,  CSummary& cSummary, CXMLMgr *pMgr, MSXML2::IXMLDOMNodePtr pClassNode)
    	{
    	
    	BOOL fReport=cStatus.m_fReport;
		for( int iDay=0; iDay<m_iNumPracticeDays; iDay++ )
    		{
    		if( DatesEqual(cDate,m_caPracticeDays[iDay])  )
    			{
    			CalculateDayScores(cDate, eClass, cStatus, cSummary, pMgr, pClassNode);
    			return;
    			}
			}
    
    	cStatus.m_fReport=FALSE;
    	for( int iDay=0; iDay<m_iNumContestDays; iDay++ )
    		{
    		if( m_caContestDays[iDay]<cDate )
				{
				cSummary.Initialize();
    			CalculateDayScores(m_caContestDays[iDay], eClass, cStatus, cSummary);
				}
    
    		if( DatesEqual(m_caContestDays[iDay], cDate) )
    			{
				cSummary.Initialize();
    			cStatus.m_fReport=fReport;
    			CalculateDayScores(cDate, eClass, cStatus, cSummary, pMgr, pClassNode);
    			break;
    			}
    		}
    
    	UpdateCumScores(eClass);
    
    	}
    
    
    void CWinscoreDoc::CalculateDayScores(CTime cDate, EClass eClass, CWinscoreListCtrl& cStatus, CSummary& cSummary, CXMLMgr *pMgr, MSXML2::IXMLDOMNodePtr pClassNode)
    	{
    
    	CScoreRecordList  cScoreRecordList;
		CWSClass &cClass=GetClass(eClass);

    	double dMinDistance=cClass.GetMinTaskDistance(SYSTEMUNITS);
 
    	// Get Today's Task
    	CTask* pcTask=m_taskList.GetByDateClass( cDate, eClass);
    	if( pcTask==NULL) 
    		{
    		cStatus.AddTwoStringItem(_T("Status"),_T("No task defined for this class on this day."));
    		return;
    		}
    
		//This day is official, and we aren't generating report messages, don't recalc
    	if( pcTask->m_eStatus==eOfficial && !cStatus.m_fReport ) return;

    	//  List Contest day
    	CString		cTemp;
    	if( IsPracticeDay(cDate) >0 )
    		cTemp=_T("Practice Day");
    	else
    		cTemp.Format(_T("%d"), GetContestDay(cDate, eClass) );

    	cStatus.AddTwoStringItem(_T("Contest Day"),cTemp);
    
    	cStatus.AddTwoStringItem(_T("Status"),pcTask->GetStatusText());
    
    
    //  First calculate start/finish/sti and raw distances 
    	CalculateTimeDistance( cScoreRecordList, pcTask, cDate, eClass, cStatus); 

    //  Compute Handicap Distances
        CalculateHandicapData( cScoreRecordList, pcTask, cDate, eClass, cStatus);
    
		if( !pcTask->IsFAITask() ) 
			{
			cScoreRecordList.CheckMinimumTaskDistance(cClass, cDate);
			}

	// Get max num acheived assigned turnpoints
		int	nMaxAssignedTPAcheived=cScoreRecordList.CalculateNumAssignedTurnpointsAcheived( 
												pcTask,
   	  											cDate, 
    											cClass );

    //  Compute Speeds
        cScoreRecordList.CalculateSpeeds( pcTask, cDate, cClass, nMaxAssignedTPAcheived, cStatus, m_turnpointArray );
    
    //  Get summary for score calculation
    	cScoreRecordList.GetSummary(	cDate,  
										eClass,
    									pcTask,
    									dMinDistance,
										cSummary );

		cSummary.SetTaskDistance(pcTask->GetDistance(m_turnpointArray, SYSTEMUNITS));

    
    	cScoreRecordList.CalculatePoints(	cDate,  
    										cClass,
    										pcTask,
											cSummary );


    	double dMaxDayPoints=cScoreRecordList.GetMaxDayPoints(	cDate, eClass );
    
    // Apply Penaltities
    	cScoreRecordList.ApplyPenalties(	cDate, 
											eClass, 
											m_PenalityList,
											dMaxDayPoints);
    
    //  OUTPUT SUMMARY  ................
    
    	CString strSummary;
    
    	CString cUnitsStr=UnitsText(m_eUnits);
    
    	cStatus.AddTwoStringItem(_T("Summary"),_T("-------------------------"));
    
		if( !pcTask->IsFAITask() ) 
			{
    		strSummary.Format(_T("%6.2lf "), cClass.GetMinTaskDistance( m_eUnits ) );
    		strSummary+=cUnitsStr;
    		cStatus.AddTwoStringItem(_T("SMTD - Standard Minimum Task Distance"),strSummary);
			if( pMgr && pClassNode )
				pMgr->CreateElement( pClassNode, _T("SMTD"),	strSummary);

    		strSummary=cClass.GetMinTaskTime().Format(_T(" %H:%M:%S ") );
    		cStatus.AddTwoStringItem(_T("SMTT - Standard Minimum Task Time"),strSummary);
			if( pMgr && pClassNode )
				pMgr->CreateElement( pClassNode, _T("SMTT"),	strSummary);
			}

        if( pcTask->m_eType==eAssigned || pcTask->m_eType==eTurnArea )
    		{
    		strSummary.Format(_T("%6.3lf "), pcTask->GetDistance(m_turnpointArray, m_eUnits) );
    		strSummary+=cUnitsStr;
    		cStatus.AddTwoStringItem(_T("Task Distance"),strSummary);
    		}
   			
		cSummary.Output( pcTask, m_eUnits, cClass.IsHandicapped(), cStatus );

		if( !pcTask->IsFAITask() ) 
			{
	   		strSummary.Format(_T("%lf"),	cClass.GetCompletionRatio(pcTask->m_eType, cSummary) );
    		cStatus.AddTwoStringItem(_T("Completion Ratio"),strSummary); 
			if( pMgr && pClassNode )
				pMgr->CreateElement( pClassNode, _T("SCR"),	strSummary);

			double dSTF=cClass.GetShortTaskFactor( cSummary);
			double dCF =cClass.GetContestantFactor(cSummary);


    		if( cSummary.m_nFinishers>0 )
    			{
    			strSummary.Format(_T("%4.0lf"), dSTF*dCF*cClass.GetMaximumSpeedPoints(pcTask->m_eType, cSummary) );
    			cStatus.AddTwoStringItem(_T("Max. Speed Points"),strSummary);

				if( pMgr && pClassNode )
					pMgr->CreateElement( pClassNode, _T("MSP"),	strSummary);
    			}

    		strSummary.Format(_T("%4.0lf"), dSTF*dCF*cClass.GetMaximumDistancePoints(pcTask->m_eType, cSummary) );
    		cStatus.AddTwoStringItem(_T("Max. Dist. Points"),strSummary);
    
			if( pMgr && pClassNode )
				pMgr->CreateElement( pClassNode, _T("MDP"),	strSummary);

    		strSummary.Format(_T("%lf"), dSTF );
    		cStatus.AddTwoStringItem(_T("Short Task Factor"),strSummary);

    		strSummary.Format(_T("%lf"), dCF );
    		cStatus.AddTwoStringItem(_T("Contestant Factor"),strSummary);

			if( pMgr && pClassNode )
				pMgr->CreateElement( pClassNode, _T("STF"),	strSummary);		
			}



    	//  If the Day was official, we don't want to keep these scores,
    	//  Otherwise, move them over to the main list
    
    	if( pcTask->m_eStatus!=eOfficial )
    		{
    		m_ScoreRecordList.Purge(eClass,cDate);
    		m_ScoreRecordList << cScoreRecordList;
    		}
    
    	}
    
     double	CWinscoreDoc::GetHandicapDistance(double dDistance, 
    										  CContestant* pcContestant)
    	{
    	if( !GetClass(pcContestant->m_eClass).IsHandicapped() ) return dDistance;
    
    	return dDistance*pcContestant->m_fHandicap;
    
    	}
    
    CTime CWinscoreDoc::FirstContestDay()
		{
    	return m_caContestDays[0];
		}
    
BOOL CWinscoreDoc::NoContestDay(CTime cDate, EClass eClass)
    {
     CTask* pcTask=m_taskList.GetByDateClass(cDate, eClass);
     if ( pcTask==NULL || pcTask->m_eStatus==eNoContest )
    	return TRUE;
    else
    	return FALSE;
    }
    
    
    int CWinscoreDoc::GetContestDay(CTime cDate, EClass eClass)
    {
    	int nDays=0;
    	if( IsPracticeDay(cDate)>0 ) return 0;
    	for( int i=0; i<m_iNumContestDays; i++ )
    		{
    		if( NoContestDay( m_caContestDays[i], eClass )==FALSE ) nDays++;
    		if( DatesEqual(m_caContestDays[i],cDate) ) break;
    		}
    	return nDays;
    }
    
    BOOL CWinscoreDoc::Valid()
		{
		for( int i=0; i<NUMCLASSES; i++ )
			if( GetClass(i).IsActive() ) return true;
		return false;
		}
    
    int CWinscoreDoc::GetNumClasses()
    {
    	int iNumClasses=0;
 		for( int i=0; i<NUMCLASSES; i++ )
			if( GetClass(i).IsActive() ) iNumClasses++;
    	return iNumClasses;
    }
    void CWinscoreDoc::UpdateCumScores(EClass eClass)
    {
		CWSClass	&cClass=GetClass(eClass);

		bool bWDSA=cClass.IsUsingWDSA();
    	double dCumPts=0.0;
    	CString  cTemp;
    	CPenality* pcPenalty;
    	CScoreRecord* pcScoreRecord;
		CScoreRecord* pcLastScoreRecord=NULL;
    	CContestant* pcContestant;
    
    	if( m_ScoreRecordList.IsEmpty() ) return ;
    
    	POSITION contestantpos = m_contestantList.GetHeadPosition();
    	while( contestantpos!=NULL )
    		{
    		pcContestant=m_contestantList.GetNext(contestantpos);
    		if( pcContestant->m_eClass !=eClass ) continue;
    
			double dWDSD=0;
			double dUnassignedContestPenalty=0.0;
    		dCumPts=0.0;
			int numOfficialDays=0;

//			Process contest penalties assigned on practice days
			for( int i=0; i< m_iNumPracticeDays; i++)
				{
				pcPenalty=m_PenalityList.Get( pcContestant->m_strContestNo, m_caPracticeDays[i],  eContestPenalty);
				if( pcPenalty )
						{
						dCumPts-=pcPenalty->m_dPenality;
						}
				}



    		for( int iDay=0; iDay<m_iNumContestDays; iDay++ )
    			{
				// Check for a contest penalty on this contest day
    			pcPenalty=m_PenalityList.Get( pcContestant->m_strContestNo, m_caContestDays[iDay],  eContestPenalty);

    			POSITION pos = m_ScoreRecordList.GetHeadPosition();
    			while( pos!=NULL)
    				{
    				pcScoreRecord=m_ScoreRecordList.GetNext(	pos, 
    															m_caContestDays[iDay],
    															pcContestant->m_strContestNo);
    			    if( pcScoreRecord==NULL ) 
						{
						// Check if there is a contest penalty which was applied on a date were no scores were generated.
						if( pcPenalty )
							{
							dUnassignedContestPenalty+=pcPenalty->m_dPenality;
							pcPenalty=NULL;
							}
						break;
						}
    	
    				if( pcPenalty )
    					{
    					// Poor bastard has a contest penality on this date., apply it!
    					dCumPts-=pcPenalty->m_dPenality;
						pcPenalty=NULL;
    					}
    	
    				if( m_taskList.NoContestDay(pcScoreRecord->m_cDate, eClass) ) continue;
					
					numOfficialDays++;

    				dCumPts+=pcScoreRecord->m_dPoints;

					//Remember the last record processed for this contestant
					pcLastScoreRecord=pcScoreRecord;
    
					if( bWDSA )
						{
						double dMaxScore = m_ScoreRecordList.GetMaxDayPoints(	m_caContestDays[iDay],	eClass );
						double dDifference = dMaxScore - pcScoreRecord->m_dPoints;
						dWDSD=max(dWDSD,dDifference);

						double dWDSA=0;
						if( numOfficialDays==2 )
							dWDSA=dWDSD*.25;
						else if( numOfficialDays==3 )
							dWDSA=dWDSD*.5;
						else if( numOfficialDays==4 )
							dWDSA=dWDSD*.75;
						else if( numOfficialDays>=5 )
							dWDSA=dWDSD;

						pcScoreRecord->m_dWDSA=dWDSA;
	    				pcScoreRecord->m_dCumPoints=dCumPts+dWDSA;
						}
					else
	    				pcScoreRecord->m_dCumPoints=dCumPts;

    				}// end for each score record 
    
    			}//end for each contest day
    
/*
               After one official day: WDSA = zero  
               After 2 official days: WDSA = 0.25 * WDSD  
               After 3 official days: WDSA = 0.5 * WDSD  
               After 4 official days: WDSA = 0.75 * WDSD  
               After 5 or more official days: WDSA = WDSD  
*/

			if( pcLastScoreRecord!=NULL && dUnassignedContestPenalty>0.0 )
				{
                // This contest penalty tried to slip through the cracks, apply it now to the last recored we had on this loser.
				pcLastScoreRecord->m_dCumPoints-=dUnassignedContestPenalty;
				dUnassignedContestPenalty=0.0;
				}

    		}//end for each contestant
    	}
    
    ////////////////////////////////////////////////////////////////////////
    ///////   PRINT/DRAW FUNCTIONS  ////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    
    
    
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
    								CStringArray     &cReasonArray
    								)
    	{
    
    	CString	cLine,  cPenality, cCodes;
    	CSize	cTextSize;
    	int		iRec=0;
    	int  iYLine=iYptr-(int)(2.5*LINE_HEIGHT);
    
    	CTask* pcTask=m_taskList.GetByDateClass( cDate, eClass);
    
    
    	while( posScoreLine!=NULL )
    		{
    
    		CScoreRecord* pcScoreRecord=(CScoreRecord*)pcScoreRecordList->CObList::GetNext(posScoreLine);
    		ASSERT(pcScoreRecord);
    		CContestant* pcContestant=m_contestantList.GetByContestNo( pcScoreRecord->m_cContestNo);
    		ASSERT (pcContestant);
			
			if( !pcContestant || !pcScoreRecord ) continue;
    
    		DrawLine(	pDC,
    					&iYLine, 
    					pcTask, 
    					pcContestant,
    					pcScoreRecord,
    					nCodes,
    					iStanding,
    					dPrevScore,
    					iPrevStanding,
    					cCodeArray,
    					cReasonArray);
    		iRec++;
    		nRecordsPrinted++;
    		if( iRec==NUM_SCORES_PER_PAGE ) break;
    		}
    
    	int iYLastEntry=iYLine;
    	DrawGrid( pDC, *pcTask, iYptr, iRec, &iYLine );
    
    	if( nCodes>0 && nRecordsPrinted==pcScoreRecordList->GetCount(eClass,cDate)  )
    		{					
    		int iSizeCodesOutput=0, i=0;
    		int iYCode=min( iYLastEntry, PENALITY_CODE_Y+20);
    
    		// Output the title
    		SelectFont( pDC, 17, FW_BOLD, TRUE );
    		TEXT_OUT( _T("Codes:"), 0, iYCode );
    
    		//  Find the number of unique codes and set the font accordingly
    		int nCodesOutput=0;
    		for( i=0; i<nCodes; i++)
    			{
    			BOOL bDupulicate=FALSE;
    			if( i>0 )
    				{
    				for( int j=0; j<i; j++ ) 
    					if( cCodeArray[j]==cCodeArray[i] ) bDupulicate=TRUE;
    				}
    			if( bDupulicate ) continue;
    			nCodesOutput++;
    			}
    		if( nCodesOutput<=10 ) 
    			SelectFont( pDC, 14, FW_NORMAL, FALSE );
    		else
    			SelectFont( pDC, 10, FW_NORMAL, FALSE );
    
    		// Now output the codes
    		int iOffset=80;
    		for( i=0; i<nCodes; i++)
    			{
    			BOOL bDupulicate=FALSE;
    			if( i>0 )
    				{
    				for( int j=0; j<i; j++ ) 
    					if( cCodeArray[j]==cCodeArray[i] ) bDupulicate=TRUE;
    				}
    			if( bDupulicate ) continue;
    
    			CSize cTextSize=pDC->GetTextExtent(cCodeArray[i]);
    			iSizeCodesOutput+=cTextSize.cy;
    			if( iOffset==0 && iSizeCodesOutput>PENALITY_CODE_LEN )
    				{
    				iOffset=GetPageWidth(pDC)/2;
    				iYCode=PENALITY_CODE_Y;
    				}
    			TEXT_OUT( cCodeArray[i]+_T(" -"),	0+iOffset, iYCode );
    			TEXT_OUT( cReasonArray[i],		60+iOffset, iYCode );
    			iYCode-=cTextSize.cy;
    			}
    		}
    
    
    	DrawScoresheetTrailer(pDC);
    	}
    
    ////////////////////////////////////////////////////////////////////////
    ///////   EXPORT FUNCTIONS  ////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    
    #define   NEWLINE   _T("\n")
    
    
    BOOL CWinscoreDoc::ExportDay(	CStdioFile& cStdioFile,
    								EClass			eClass, 
    								CTime&			cDate, 
    								ESortBy		eSortBy)
    	{
    
		CWSClass	&cClass=GetClass(eClass);

		CTask* pcTask=m_taskList.GetByDateClass( cDate, eClass);

    	CString cFDRText, cScoreSheetStatus,cDayDate, cLine, cLine1, cLine2, cLine3,
    			cScore, cPenality, cCodes, cTaskLine;
    	int nCodes=0;
    	CStringArray  cCodeArray, cReasonArray; 
    	CScoreRecordList cScoreRecordList;
    	double dPrevScore=0.0;
    	int iPrevStanding=(-1);
    
    	int iDay	=  GetContestDay( cDate,  eClass);
    
		if( pcTask ) 
			cScoreSheetStatus=pcTask->GetStatusText();    
    
    	if( pcTask==NULL || pcTask->m_eStatus==eNoContest )
    		{
    		cStdioFile.WriteString(_T("<table  border=2, cellspacing=1 cellpadding=5 >\n") );
    		cLine.Format(	_T("<tr>  <th> %s Class: No Contest Day </th> </tr>\n"),
    						cClass.GetName() );
    		cStdioFile.WriteString( cLine );
    		cStdioFile.WriteString( _T("</table>\n<p>\n"));
    		return TRUE;
    		}
    
    	int nRecords=m_ScoreRecordList.CopySort(cDate, 
    											eClass, 
    											cScoreRecordList,
    											eSortBy);
    
    
    //////// Write Task information
    
    	if( pcTask->IsAreaTask() || pcTask->m_eType== eAssigned || pcTask->m_eType==eFAIRacing || pcTask->m_eType== eModifiedAssigned )
    		{
    		cStdioFile.WriteString(_T("<table  border=2, cellspacing=1 cellpadding=5 >"));
    		cStdioFile.WriteString( NEWLINE);
			if (pcTask->IsAreaTask()  )
				{
    			cLine.Format(_T("<tr>  <th colspan=4>%s %s Day %d Task: Turn Area Task </th> </tr>"), cScoreSheetStatus, cClass.GetName(), iDay );
    			cStdioFile.WriteString( cLine+NEWLINE);
		   		cLine=pcTask->m_cPostTime.Format(_T("<tr>  <td>Minimum Time </td> <td colspan=4> %H:%M </td> </tr>\n") );
    			cStdioFile.WriteString( cLine);
				}
			else if ( pcTask->m_eType== eModifiedAssigned )
				{
    			cLine.Format(_T("<tr>  <th colspan=3>%s %s Day %d Task: Modified Assigned Task</th> </tr>"), cScoreSheetStatus, cClass.GetName(), iDay );
    			cStdioFile.WriteString( cLine+NEWLINE);
		   		cLine=pcTask->m_cPostTime.Format(_T("<tr>  <td>Minimum Time </td> <td colspan=4> %H:%M </td> </tr>\n") );
    			cStdioFile.WriteString( cLine);
				}
			else
				{
    			cLine.Format(_T("<tr>  <th colspan=4>%s %s Day %d Task: Assigned </th> </tr>"), cScoreSheetStatus, cClass.GetName(), iDay );
    			cStdioFile.WriteString( cLine+NEWLINE);
				}

    		cLine.Format( _T("<tr>  <th>ID</th> <th>Name</th>			<th>Distance (%s)</th><th>Radius</th></tr>\n"), UnitsText(m_eUnits));
    		cStdioFile.WriteString( cLine+NEWLINE);
    		double dDist=0.0;
    		CTurnpoint *pcTurnpoint, *pcPrevTurnpoint;

    		pcTurnpoint=m_turnpointArray[pcTask->m_cStartGate.GetPointID()-1];
			pcPrevTurnpoint=pcTurnpoint;

  			OUTPUT_TPT_HTML( pcTurnpoint, dDist, pcTask->m_cStartGate.IsGPSCylinder()?(ConvertDistance(pcTask->m_cStartGate.GetRadius(),SYSTEMUNITS, m_eUnits)):(0.0), cLine, cStdioFile);
    		dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, m_eUnits);

			if( pcTask->m_cStartGate.IsGPSCylinder() )
				dDist-=ConvertDistance(pcTask->m_cStartGate.GetRadius(), SYSTEMUNITS, m_eUnits);


    		for( int iTpt=0; iTpt<pcTask->GetNumTurnpoints(); iTpt++ )
    			{
    			pcTurnpoint=m_turnpointArray[pcTask->GetTurnpointID(iTpt)-1];
        		dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, m_eUnits);

				OUTPUT_TPT_HTML( pcTurnpoint, dDist, (pcTask->IsAreaTask())?(ConvertDistance(pcTask->GetTurnpointRadius(iTpt), SYSTEMUNITS, m_eUnits)):(0), cLine, cStdioFile);

    			pcPrevTurnpoint=pcTurnpoint;
    			}

   			pcTurnpoint=m_turnpointArray[pcTask->m_cFinishGate.GetPointID()-1];
       		dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, m_eUnits);

			if( pcTask->m_cFinishGate.IsGPSCylinder() )
				dDist-=ConvertDistance(pcTask->m_cFinishGate.GetRadius(), SYSTEMUNITS, m_eUnits);

  			OUTPUT_TPT_HTML( pcTurnpoint, dDist, pcTask->m_cFinishGate.IsGPSCylinder()?(ConvertDistance(pcTask->m_cFinishGate.GetRadius(),SYSTEMUNITS, m_eUnits)):(0.0), cLine, cStdioFile);

    		cStdioFile.WriteString( _T("</table>\n<p>\n"));
    		}
    	else
    		{
    		cStdioFile.WriteString(_T("<table  border=2, cellspacing=1 cellpadding=5 >"));
    		cStdioFile.WriteString( NEWLINE); 
//    		if( pcTask->m_eType==eModifiedAssigned )
    		  cLine.Format(_T("<tr>  <th colspan=2> %s %s Day %d Task: Pilot Selected </th> </tr>\n"),cScoreSheetStatus, cClass.GetName(), iDay );
//    		else
//    		  cLine.Format(_T("<tr>  <th colspan=2> %s %s Day %d Task: Modified Assigned</th> </tr>\n"), cScoreSheetStatus, cClass.GetName(), iDay );
    
    		cStdioFile.WriteString( cLine);
    
    		cLine=pcTask->m_cPostTime.Format(_T("<tr>  <td>Minimum Time </td> <td> %H:%M </td> </tr>\n") );
    		cStdioFile.WriteString( cLine);
    
    		cStdioFile.WriteString( _T("</table>\n<p>\n"));
    		}
    
    
    	///////   Write Scoresheet   /////////////////
    
    	cStdioFile.WriteString(_T("<table  border=2, cellspacing=1 cellpadding=10 >\n"));
    
    	if( cClass.IsHandicapped() )
    		cLine.Format(	_T("<tr> <th colspan=14 align=center> %s Day %d  ** %s ** </th>\n"),
    						 cClass.GetName(),
    						  iDay,
    						  cScoreSheetStatus );
    	else
    		cLine.Format(	_T("<tr> <th colspan=12 align=center> %s Day %d  ** %s ** </th>\n"),
    						 cClass.GetName(),
    						  iDay,
    						  cScoreSheetStatus );
    	cStdioFile.WriteString( cLine );
    
    	if( cClass.IsHandicapped() )
    		cLine.Format(_T("<tr> <th colspan=2>Cumulative</th> <th colspan=3>Contestant </th>  <th colspan=6>Day %d Results </th> <th colspan=2>Remarks </th></tr>\n"),
    					 iDay);
    	else
    		cLine.Format(_T("<tr> <th colspan=2>Cumulative</th> <th colspan=3>Contestant </th>  <th colspan=4>Day %d Results </th> <th colspan=2>Remarks </th></tr>\n"),
    					 iDay);
    	cStdioFile.WriteString( cLine );
    
    	if( cClass.IsHandicapped() )
    		cLine.Format( _T("<tr> <th>Rank </th> <th>Points </th>  <th>ID </th> <th>Name </th> <th>Glider </th> <th>Rank </th> <th>Points </th> <th>Handicapped Speed (%s/Hr)</th> <th>Speed (%s/Hr)</th> <th>Handicapped Distance (%s)</th>  <th>Distance (%s)</th><th>Code</th> <th> Penalty Points</th> </tr>\n"),
    					  UnitsText(m_eUnits),
    					  UnitsText(m_eUnits),
    					  UnitsText(m_eUnits),
    					  UnitsText(m_eUnits));
    	else
    		cLine.Format( _T("<tr> <th>Rank </th> <th>Points </th>  <th>ID </th> <th>Name </th> <th>Glider </th> <th>Rank </th> <th>Points </th><th>Speed (%s/Hr)</th> <th>Distance (%s)</th><th>Code</th> <th> Penalty Points</th> </tr>\n"),
    					  UnitsText(m_eUnits),
    					  UnitsText(m_eUnits));
    
    	cStdioFile.WriteString( cLine );
    
    	int  iRec=0;
    	if( nRecords>0 )
    		{
    		POSITION pos = cScoreRecordList.GetHeadPosition();
    		while( pos!=NULL )
    			{
    
    			CScoreRecord* pcScoreRecord=(CScoreRecord*)cScoreRecordList.CObList::GetNext(pos);
    			ASSERT(pcScoreRecord);
    			CContestant* pcContestant=m_contestantList.GetByContestNo( pcScoreRecord->m_cContestNo);
    			ASSERT (pcContestant);
    
    			if( !GetClass(eClass).IsHandicapped() )
    				{
    				if( pcScoreRecord->m_fContestant )
    					{
    					if( pcScoreRecord->m_fFinisher )
    						cScore.Format(_T("<td>%6.2lf</td>  <td>%6.2lf</td>"),
    										ConvertDistance(pcScoreRecord->m_dSpeed, SYSTEMUNITS, m_eUnits),
    										ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    					else
    						cScore.Format(_T("<td>-</td>  <td>%6.2lf</td>"),
    										ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    					}
    				else
    					cScore=_T("<td colspan=2> ")+pcScoreRecord->DNCText(pcTask->m_eStatus)+_T(" </td>");
    				}
    			else
    				{
    				if( pcScoreRecord->m_fContestant )
    					{
    					if( pcScoreRecord->m_fFinisher )
    						cScore.Format(_T("<td>%6.2lf</td> <td>%6.2lf</td> <td>%6.2lf</td> <td>%6.2lf</td>"),
    										ConvertDistance(pcScoreRecord->m_dHandicappedSpeed, SYSTEMUNITS, m_eUnits),
    										ConvertDistance(pcScoreRecord->m_dSpeed, SYSTEMUNITS, m_eUnits),
    										ConvertDistance(pcScoreRecord->m_dHandicappedDistance, SYSTEMUNITS, m_eUnits),
    										ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    					else
    						cScore.Format(_T("<td>-</td> <td>-</td> <td>%6.2lf</td> <td>%6.2lf</td>"),
    										ConvertDistance(pcScoreRecord->m_dHandicappedDistance, SYSTEMUNITS, m_eUnits),
    										ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    					}
    				else
    					cScore=_T("<td colspan=4> ")+pcScoreRecord->DNCText(pcTask->m_eStatus)+_T(" </td>");
    				}
    
    			FormatPenalityLine(	eClass,
									pcScoreRecord,
    								cCodes,
    								cPenality,
    								nCodes,
    								cCodeArray,
    								cReasonArray);
    	
    //								 rank                      cumpts           ID                    Name          Glider       dayrnk                 DayPts       score       codes     penality
    			cLine.Format(_T("<tr> <td>%s </td><td align=right>%6.0lf </td>  <td>%s </td>  <td nowrap>%s </td>  <td>%s</td>  <td>%d</td> <td align=right>%6.0lf</td>  %s      <td>%s</td> <td>%s</td></tr>\n"),
    							pcScoreRecord->m_cCumRankText,
    							max( pcScoreRecord->m_dCumPoints, 0),
    							pcScoreRecord->m_cContestNo,
    							pcContestant->m_strLastName+_T(", ")+pcContestant->m_strFirstName,
    							pcContestant->m_strGlider,
    							pcScoreRecord->m_iDayRank,
    							pcScoreRecord->m_dPoints,
    							cScore,
    							cCodes,
    							cPenality
    							);
    			cStdioFile.WriteString( cLine );
    			iRec++;
    			}
    		}
    
    
    	if( nCodes>0 )
    		{
    		if( cClass.IsHandicapped() )
    			cStdioFile.WriteString( _T("<tr> <th colspan=14 align=left> Codes: </th>\n"));
    		else
    			cStdioFile.WriteString( _T("<tr> <th colspan=12 align=left> Codes: </th>\n"));
    
    		for( int i=0; i<nCodes; i++)
    			{
    
    			BOOL bDupulicate=FALSE;
    			if( i>0 )
    				{
    				for( int j=0; j<i; j++ ) 
    					if( cCodeArray[j]==cCodeArray[i] ) bDupulicate=TRUE;
    				}
    			if( bDupulicate ) continue;
    
    			cLine.Format( _T("<tr> <td>%s</td><td colspan=11 align=left>%s </td>\n"),
    									cCodeArray[i],
    									cReasonArray[i]);
    			cStdioFile.WriteString( cLine );
    			}
    		}
    	cStdioFile.WriteString( _T("</table>\n"));
    	cStdioFile.WriteString( _T("<p>\n"));
    
    	return TRUE;
    }
    
        
    
    
    BOOL CWinscoreDoc::ExportCumulative(	CStdioFile& cStdioFile,
    										EClass		eClass )
    	{
    
    	CString cDayDate, cLine, cScore;
    	int nColumns, iDay;
    	CScoreRecordList cScoreRecordList;
    	CTime cLastContestDay;
    	double	dPrevScore=0.0;
    	int		iPrevStanding=(-1);
    
		CWSClass &cClass=GetClass(eClass);

   
    	for( int i=0; i<m_iNumContestDays; i++ )
    		{
    		if( !NoContestDay(m_caContestDays[i], eClass) ) cLastContestDay=m_caContestDays[i];
    		}
       
    
    	//  Get the records for the last contest day sorted by cum winners,
    	//  This is the order we want
    
    	int nRecords=m_ScoreRecordList.CopySort(cLastContestDay, 
    											eClass, 
    											cScoreRecordList,
    											eCumulative);
    
    
    	///////   Write Scoresheet   /////////////////
    
    	nColumns = 5+ 2*m_iNumContestDays;
    
    
    	/////////  1st Write Header Line
    
    	cStdioFile.WriteString(_T("<table  border=2, cellspacing=1 cellpadding=10 >\n"));
    
    	cLine.Format(	_T("<tr> <th colspan=%d align=center> %s Class </th>\n"),
    					nColumns,
    					GetClass(eClass).GetName() );
    	cStdioFile.WriteString( cLine );
    
    	/////////  Write 2nd Header Line
    
    	cStdioFile.WriteString(_T("<tr> <th colspan=2 rowspan=2>Cumulative Score</th> <th colspan=3 rowspan=2>Contestant </th>"));
    	for( iDay=m_iNumContestDays-1; iDay>=0; iDay-- )
    		{
    		CString cDateString = m_caContestDays[iDay].Format(_T("%b %d, %y") );
    		cLine.Format(	_T("<th colspan=2 align=center> %s </th>"),
    						cDateString);
    		cStdioFile.WriteString( cLine );
    		}
    	cStdioFile.WriteString( _T(" </tr>\n") );
    	
    
    	cStdioFile.WriteString(_T("<tr>"));
    	for( iDay=m_iNumContestDays-1; iDay>=0; iDay-- )
    		{
    		if( NoContestDay( m_caContestDays[iDay], eClass) )
    			{
    			cStdioFile.WriteString( _T("<th colspan=2 align=center> No Contest</th>") );
    			}
    		else
    			{
    			CTask* pcTask=m_taskList.GetByDateClass(m_caContestDays[iDay], eClass);
    			if( pcTask->m_eType==eAssigned || pcTask->m_eType==eFAIRacing)
					{
    				cLine.Format(	_T("<th colspan=2 align=center> Distance %6.2lf %s </th>"),
    						pcTask->GetDistance(m_turnpointArray,m_eUnits),
    						UnitsText(m_eUnits) );
					}
	  			else if( pcTask->IsAreaTask() )
    				{
    				cLine.Format(_T("<th colspan=2 align=center> Turn Area %s </th>"),
    							   pcTask->m_cPostTime.Format(_T("%H:%M")) );
    				}
    			else if( pcTask->m_eType==eModifiedAssigned )
    				{
    				cLine.Format(_T("<th colspan=2 align=center> Modified Assigned %s </th>"),
    							   pcTask->m_cPostTime.Format(_T("%H:%M")) );
    				}
    			else
    				{
    				cLine.Format(_T("<th colspan=2 align=center> Pilot Selected %s </th>"),
    							   pcTask->m_cPostTime.Format(_T("%H:%M")) );
    				}
    			cStdioFile.WriteString( cLine );
    			}
    		}
    	cStdioFile.WriteString( _T(" </tr>\n") );
    
    	/////////  Write 3rd Header Line
    
    	cStdioFile.WriteString(_T("<tr> <th>Rank</th> <th>Points </th> <th>ID </th> <th>Name </th> <th>Glider</th> "));
    	for( iDay=m_iNumContestDays-1; iDay>=0; iDay-- )
    		{
    		cStdioFile.WriteString( _T("<th>Rank</th> <th>Points </th>")  );
    		}
    	cStdioFile.WriteString( _T(" </tr>\n") );
    
    	int  iRec=0;
    	if( nRecords>0 )
    		{
    		POSITION pos = cScoreRecordList.GetHeadPosition();
    		while( pos!=NULL )
    			{
    
    			CScoreRecord* pcScoreRecord=(CScoreRecord*)cScoreRecordList.CObList::GetNext(pos);
    			ASSERT(pcScoreRecord);
    			CContestant* pcContestant=m_contestantList.GetByContestNo( pcScoreRecord->m_cContestNo);
    			ASSERT (pcContestant);
    
    //								 rank                      cumpts           ID                    Name          Glider       
    			cLine.Format(_T("<tr> <td>%s </td><td align=right>%6.0lf </td>  <td>%s </td>  <td nowrap>%s </td>  <td>%s</td> "),
    							pcScoreRecord->m_cCumRankText,
    							max(pcScoreRecord->m_dCumPoints,0),
    							pcScoreRecord->m_cContestNo,
    							pcContestant->m_strLastName+_T(", ")+pcContestant->m_strFirstName,
    							pcContestant->m_strGlider );
    			cStdioFile.WriteString( cLine );
    
    			for( iDay=m_iNumContestDays-1; iDay>=0; iDay-- )
    				{
    				CScoreRecordList cDayRecordList;
    
    				m_ScoreRecordList.CopySort( m_caContestDays[iDay], 
    											eClass, 
    											cDayRecordList,
    											eDaily);
    
    				if( NoContestDay( m_caContestDays[iDay], eClass) )
    					{
    					cStdioFile.WriteString( _T("<td>-</td> <td>0</td>") );
    					}
    				else
    					{
    					CScoreRecordList cDayRecordList;
    					m_ScoreRecordList.CopySort( m_caContestDays[iDay], 
    												eClass, 
    												cDayRecordList,
    												eDaily);
    
    					CScoreRecord* pcRec=cDayRecordList.Get( pcScoreRecord->m_cContestNo, m_caContestDays[iDay] );
    					if( pcRec )
    						cLine.Format( _T("<td>%d</th> <td>%6.0lf</td>"),
    									   pcRec->m_iDayRank,
    									   pcRec->m_dPoints);
    					else
    						cLine=_T("<td>-</td> <td>0</td> ");
    
    					cStdioFile.WriteString( cLine );
    					}
    				}
    			cStdioFile.WriteString( _T(" </tr>\n") );
    			}
    		}
    
    	cStdioFile.WriteString( _T("</table>\n") );
    	cStdioFile.WriteString( _T("<p>\n") );
    
    	return TRUE;
    }
      
    
    
void CWinscoreDoc::CalculateHandicapData(	CScoreRecordList& cScoreRecordList, 
    										CTask* pcTask, 
    										CTime cDate, 
    										EClass eClass, 
    										CWinscoreListCtrl& cStatus )
    	{
    
		CWSClass &cClass=GetClass(eClass);

    	if( !cClass.IsHandicapped() ) return;

		//double dLowestHandicap=1.0;

		// Adjustment for lowest handicap removed for 2024
		//if( pcTask->IsFAITask() )
		//	dLowestHandicap=m_contestantList.GetLowestPerformanceHandicap(eClass);

    	POSITION pos = cScoreRecordList.GetHeadPosition();
    	while(pos!=NULL)
    		{
    		CScoreRecord* pcScoreRecord=cScoreRecordList.GetNext(pos, cDate, eClass);
    
			pcScoreRecord->m_dHandicappedDistance=0.0;
			if( pcScoreRecord->m_dDistance==0.0 ) continue;

			// FAI Adjustment removed for 2024
			//if( pcTask->IsFAITask() )
				//{
				//double dFactor=pcScoreRecord->m_dHandicap/dLowestHandicap;
				//pcScoreRecord->m_dHandicappedDistance=dFactor*pcScoreRecord->m_dDistance;
				//}
			//else
				pcScoreRecord->m_dHandicappedDistance=pcScoreRecord->m_dHandicap*pcScoreRecord->m_dDistance;
 

    		}// End for each score record
    
		}
    
    
    void CWinscoreDoc::CalculateTimeDistance(CScoreRecordList& cScoreRecordList, 
    										 CTask*		pcTask, 
    										 CTime		cDate, 
    										 EClass		eClass, 
    										 CWinscoreListCtrl& cStatus)
    	{
    
		CWSClass &cClass=GetClass(eClass);
    	CTime  cRollTime, cStartTime, cFinishTime;
    	CTimeSpan cSTI, cFlightTime, cSTICalculated;
    	CString cError, cSTIstr, cSTICalstr;
    	int uPenalityMask;
 		CEvent cEvent;
    
    	BOOL	fContestant, fFinisher;
    	double	dDistance=0., dSpeed=0., dHours=0., dHdcapDist=0., dHdcapSpeed=0., dScoredDistance=0.;
    	BOOL	fPointsOverride=FALSE,	fDistanceOverride=FALSE;

		    
    // Loop through all contestants, and compute  Start Time, 
    // STI, Finish Time, speed and distance; 
    
    	POSITION posContestant = m_contestantList.GetHeadPosition();
    	while(posContestant!=NULL)
    		{
    		CContestant* pcContestant=m_contestantList.GetNext(posContestant);
    		if (pcContestant->m_eClass!=eClass) continue;
    
  			CFlight*		pcFlight		=m_FlightList.Get( cDate, pcContestant->m_strContestNo);

      		fFinisher=FALSE;
    		fContestant=FALSE;
    		uPenalityMask=0;

   			if( pcContestant->HasWithdrawn() ) 
				uPenalityMask|=WSP_WITHDREW;

   			if( pcContestant->IsForeign() ) 
				uPenalityMask|=WSP_FOREIGN;

	 		if( pcContestant->m_iSSANumber==0 )
    			uPenalityMask |=WSP_NOSSANUMBER;
   

			if( pcTask->m_b2ndGateActive )
				{
				if( pcContestant->IsInBGroup() )
					uPenalityMask|=WSP_B_GATE;
				else
					uPenalityMask|=WSP_A_GATE;
				}
				
     
			if( !pcFlight )
				{
    			uPenalityMask |=WSP_NOFLIGHTLOG;
    			cStatus.AddTwoStringItem(pcContestant->m_strContestNo,
    									_T("NO FLIGHT LOG - A flight log is required to calculate a score.") );
    			cScoreRecordList.DNC(	pcContestant->m_strContestNo, 
    									eClass, 
    									cDate,
    									pcContestant->IsGuest(),
    									uPenalityMask);						
    			continue;
				}
  			else
  				{
    			cRollTime=pcFlight->GetRollTime();
    			cStartTime=pcFlight->GetStartTime();
    			cFinishTime=pcFlight->GetFinishTime();
  				}

     
    
        
    //		START TIME PROCESSING --------------

    	if( !pcFlight->IsStartTimeValid() )
    		{
//			No Start time for this pilot
    		cStatus.AddTwoStringItem(pcContestant->m_strContestNo,
    								 _T("NO VALID START - No start times were entered for this pilot on this day."));
			uPenalityMask|=WSP_NOSTART;
    		}
   		else if( pcTask->m_cGateOpenTime > cStartTime )
    		{
//			No start after Task opened, do not score
    		cStatus.AddTwoStringItem(pcContestant->m_strContestNo,
    								 _T("NO VALID START - No start time after the task open time, Pilot Not scored"));
			uPenalityMask|=WSP_NOSTART;
			cScoreRecordList.DNC(	pcContestant->m_strContestNo, eClass, cDate,
    								pcContestant->IsGuest(),
    								uPenalityMask	);
    		continue;
    		}



/////////////////////////////  DISTANCE CALCULATIONS  //////////////////////////
		

	 
			dDistance=pcFlight->GetDistance();

    		fContestant=dDistance>0.0001;
    
            if( eClass==eSports )
				fFinisher=( pcFlight->IsFinishTimeValid()  && dDistance>(cClass.GetMinTaskDistance(SYSTEMUNITS)/((cClass.IsHandicapped())?(pcContestant->m_fHandicap):(1.0))) );
			else
				fFinisher=( pcFlight->IsFinishTimeValid()  && dDistance>cClass.GetMinTaskDistance(SYSTEMUNITS) );
    		
			// Mark designated airfield.
    		if( !fFinisher && 
				pcFlight->IsAirfieldBonus() &&
				dDistance>0.0 )
				{
    			uPenalityMask|=WSP_AIRFIELDBONUS;
				}

    		// Create the score Record
    		cScoreRecordList.StoreData(	pcContestant->m_strContestNo,
    										cDate,
    										eClass,
    										pcContestant->IsGuest(),
    										pcContestant->m_fHandicap,
    										fContestant,
    										fFinisher,
    										cStartTime,
    										cFinishTime,
    										cSTI,
    										dDistance,
    										0.0,
    										0.0,
    										0.0,
    										0.0,
    										0.0, 
											0,
    										uPenalityMask );
    
    		}// End for each Contestant
    
   
    	}
    
    
    
    void CWinscoreDoc::DrawGrid(	CDC *pDC,
    								CTask&	cTask,
    								int iYStart, 
    								int nContestants,
    								int *piYFirstLine)
    {
    
		CWSClass	&cClass=GetClass(cTask.m_eClass);

    	int iLineHeight=LINE_HEIGHT;
    
    	iYStart-=(iLineHeight/2);
    	int iYFirstHeader=iYStart-iLineHeight;
    	int iY2ndHeader=iYStart-iLineHeight*2;
    	CString cText;
    
    	*piYFirstLine=iY2ndHeader;
    
    	int iYBottom=iYStart-((nContestants+2)*iLineHeight);
    
        pDC->SetTextAlign( TA_TOP| TA_LEFT );
    
    	//  First draw the main vertical lines.
    	DRAW_VERTICAL( iaSpeedTab[LEFTMARGIN],	iYStart, iYBottom);
    	DRAW_VERTICAL( iaSpeedTab[CONID],		iYStart, iYBottom);
    	DRAW_VERTICAL( iaSpeedTab[DAYRANK],		iYStart, iYBottom);
    	DRAW_VERTICAL( iaSpeedTab[REMCODE],		iYStart, iYBottom);
//    	DRAW_VERTICAL( iaSpeedTab[RIGHTMARGIN], iYStart, iYBottom);
     	DRAW_VERTICAL( GetPageWidth(pDC), iYStart, iYBottom);
   
    	// Now the short vertical lines in the header
    	DRAW_VERTICAL( iaSpeedTab[CUMPOINTS],	iYFirstHeader, iY2ndHeader);
    	DRAW_VERTICAL( iaSpeedTab[CONNAME],		iYFirstHeader, iY2ndHeader);
    	DRAW_VERTICAL( iaSpeedTab[CONGLIDER],	iYFirstHeader, iY2ndHeader);
    	DRAW_VERTICAL( iaSpeedTab[DAYPOINTS],	iYFirstHeader, iY2ndHeader);
    	DRAW_VERTICAL( iaSpeedTab[REMPOINTS],	iYFirstHeader, iY2ndHeader);
    
    	//  The ones around the speed can be short or long
    	if( cClass.IsHandicapped() )
    		{	
    		DRAW_VERTICAL( iaSpeedTab[DAYSPEED],	iYStart, iYBottom);
    		DRAW_VERTICAL( iaSpeedTab[DAYDIST],		iYStart, iYBottom);
    		}
    	else
    		{
    		DRAW_VERTICAL( iaSpeedTab[DAYSPEED],	iYFirstHeader, iY2ndHeader);
    		DRAW_VERTICAL( iaSpeedTab[DAYDIST],		iYFirstHeader, iY2ndHeader);
    		}
    
    	// Draw the Horizontal lines for the header and the bottom
    	DRAW_HORIZONTAL(iYStart);
    	DRAW_HORIZONTAL(iYFirstHeader);
    	int iYBottomHeader=iYStart-2*iLineHeight;
    	DRAW_HORIZONTAL(iYBottomHeader);
    	for (int i=0; i<(nContestants/5); i++)
    		{
    		DRAW_HORIZONTAL(iYBottomHeader-((i+1)*100));
    		}
    	DRAW_HORIZONTAL(iYBottom);
    
    	SelectFont( pDC, 16, FW_BOLD, FALSE);
    
    	//  Now create the font and draw the headers.
    	TEXT_OUT( _T("Cumulative"),		iaSpeedTab[LEFTMARGIN], iYStart );
    	TEXT_OUT( _T("Contestant"),		iaSpeedTab[CONNAME], iYStart );
    
    	if( cClass.IsHandicapped() )
    		{
    		TEXT_OUT( _T("Day"),		iaSpeedTab[DAYRANK],	iYStart );
    		TEXT_OUT( _T("Handicapped"),iaSpeedTab[DAYSPEED],	iYStart );
    		TEXT_OUT( _T("Actual"),		iaSpeedTab[DAYDIST],	iYStart );	
    		}
    	else
    		TEXT_OUT( _T("Day Results"),	iaSpeedTab[DAYPOINTS], iYStart );
    	
    	TEXT_OUT( _T("Remarks"),		iaSpeedTab[REMCODE], iYStart );
    
    	TEXT_OUT( _T("Rank"),	iaSpeedTab[LEFTMARGIN],	iYFirstHeader );
    	TEXT_OUT( _T("Points"),	iaSpeedTab[CUMPOINTS],	iYFirstHeader );
    	TEXT_OUT( _T("ID"),		iaSpeedTab[CONID],		iYFirstHeader );
    	TEXT_OUT( _T("Name"),	iaSpeedTab[CONNAME],	iYFirstHeader );
    	TEXT_OUT( _T("Glider"),	iaSpeedTab[CONGLIDER],	iYFirstHeader );
    	TEXT_OUT( _T(" #"),	iaSpeedTab[DAYRANK],	iYFirstHeader );
    	TEXT_OUT( _T("Points"),	iaSpeedTab[DAYPOINTS],	iYFirstHeader );
    
    	if( cClass.IsHandicapped() )
    		{
    		cText=_T("Speed | Dist");
    		TEXT_OUT( cText,	iaSpeedTab[DAYSPEED],	iYFirstHeader );
    		cText=_T("Speed | Dist");
    		TEXT_OUT( cText,	iaSpeedTab[DAYDIST],	iYFirstHeader );
    		}
    	else
    		{
    		cText.Format(_T("Speed %s"), UnitsSpeedText(m_eUnits));
    		TEXT_OUT( cText,	iaSpeedTab[DAYSPEED],	iYFirstHeader );
    		cText.Format(_T("Dist (%s)"), UnitsTextShort(m_eUnits));
    		TEXT_OUT( cText,	iaSpeedTab[DAYDIST],	iYFirstHeader );
    		}
    
    	if( cTask.m_eType== eAssigned || cTask.m_eType==eFAIRacing )
    		{
    		TEXT_OUT( _T("Code"),	iaSpeedTab[REMCODE],	iYFirstHeader );
    		TEXT_OUT( _T("Pnts"),	iaSpeedTab[REMPOINTS],	iYFirstHeader );
    		}
    	else
    		{
    		SelectFont( pDC, 8, FW_SEMIBOLD, FALSE);
    		TEXT_OUT( _T("Code"),	iaSpeedTab[REMCODE],	iYFirstHeader );
    		TEXT_OUT( _T("Pnts"),	iaSpeedTab[REMPOINTS],	iYFirstHeader );
    		TEXT_OUT( _T("Turnpoints"),	iaSpeedTab[REMCODE],	iYFirstHeader-(iLineHeight/2)+1 );
    		}
    
    
    }
    
    void CWinscoreDoc::DrawTask(CDC * pDC, 
    							CTask & cTask, 
    							int iYStart, 
    							int * piYFinish)
    {
    	int iXTask=400;
		int	aiAssignedTabs[4]={ 0, 30, 150, 260 };

    	CString cText, cLine, cFDRText;
    	CSize	cTextSize;
    
    	SelectFont( pDC, 16, FW_BOLD, FALSE );
    
    	int iYpos=iYStart;
    	if( cTask.IsAreaTask() || cTask.m_eType== eAssigned || cTask.m_eType==eFAIRacing || cTask.m_eType== eModifiedAssigned )
    		{
			cText = cTask.TypeText()+_T(" Task");
    
    		pDC->TextOut( iXTask , iYpos, cText, cText.GetLength() );
    		cTextSize=pDC->GetTextExtent(cText);
    		iYpos-=cTextSize.cy;
    
    		SelectFont( pDC, 14, FW_SEMIBOLD, TRUE );
    
    		cText=_T("ID");
    		pDC->TextOut( iXTask+aiAssignedTabs[0], iYpos, cText, cText.GetLength() );
    		cText=_T("Name");
    		pDC->TextOut( iXTask+aiAssignedTabs[1], iYpos, cText, cText.GetLength() );
    		cText=_T("Distance (");
    		cText+=UnitsText(m_eUnits);
    		cText+=_T(")");
    		pDC->TextOut( iXTask+aiAssignedTabs[2], iYpos, cText, cText.GetLength() );
    		cTextSize=pDC->GetTextExtent(cText);
			if( cTask.IsAreaTask() || 
				cTask.m_cStartGate.IsGPSCylinder() ||
				cTask.m_cFinishGate.IsGPSCylinder()   )
				{
				cText=_T("Radius");
    			pDC->TextOut( iXTask+aiAssignedTabs[3], iYpos, cText, cText.GetLength() );
    			cTextSize=pDC->GetTextExtent(cText);
				}
    		iYpos-=(cTextSize.cy+2);
    
    		if( cTask.GetNumTurnpoints()<6 )
    			SelectFont( pDC, 12, FW_NORMAL, FALSE );
    		else
    			SelectFont( pDC, 10, FW_NORMAL, FALSE );
    
    		double dDist=0.0;
    		CTurnpoint *pcTurnpoint=NULL, *pcPrevTurnpoint=NULL;    

   			pcTurnpoint=m_turnpointArray[cTask.m_cStartGate.GetPointID()-1];
			OUTPUT_TPT_OUT( pcTurnpoint, dDist, cTask.m_cStartGate.IsGPSCylinder()?(ConvertDistance(cTask.m_cStartGate.GetRadius(),SYSTEMUNITS, m_eUnits)):(0.0), pDC, iXTask, iYpos, aiAssignedTabs );
    		pcPrevTurnpoint=pcTurnpoint;

			if( cTask.m_cStartGate.IsGPSCylinder() )
				dDist-=ConvertDistance(cTask.m_cStartGate.GetRadius(), SYSTEMUNITS, m_eUnits);

    		for( int iTpt=0; iTpt<cTask.GetNumTurnpoints(); iTpt++ )
    			{
     			pcPrevTurnpoint=pcTurnpoint;
   				pcTurnpoint=m_turnpointArray[cTask.GetTurnpointID(iTpt)-1];
        		dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, m_eUnits);

				OUTPUT_TPT_OUT( pcTurnpoint, dDist, (cTask.IsAreaTask())?(ConvertDistance(cTask.GetTurnpointRadius(iTpt), SYSTEMUNITS, m_eUnits)):(0), pDC, iXTask, iYpos, aiAssignedTabs );
     			pcPrevTurnpoint=pcTurnpoint;
    			}

   			pcTurnpoint=m_turnpointArray[cTask.m_cFinishGate.GetPointID()-1];
       		dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, m_eUnits);

			if( cTask.m_cFinishGate.IsGPSCylinder() )
				dDist-=ConvertDistance(cTask.m_cFinishGate.GetRadius(), SYSTEMUNITS, m_eUnits);

			OUTPUT_TPT_OUT( pcTurnpoint, dDist, cTask.m_cFinishGate.IsGPSCylinder()?(ConvertDistance(cTask.m_cFinishGate.GetRadius(),SYSTEMUNITS, m_eUnits)):(0.0), pDC, iXTask, iYpos, aiAssignedTabs );
			}	
    
    	if(  cTask.IsTimedTask() )
    		{
       
    		SelectFont( pDC, 14, FW_SEMIBOLD, FALSE );
    
   			cText=cTask.m_cPostTime.Format(_T("Minimum Time: %H:%M") );
    		pDC->TextOut( iXTask , iYpos, cText, cText.GetLength() );
    		cTextSize=pDC->GetTextExtent(cText);
   			iYpos-=cTextSize.cy;

    		}
    
    
    	*piYFinish=iYpos;
    
    }
    
    
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
    							CStringArray	&cReasonArray)
    {

		CWSClass	&cClass=GetClass(pcContestant->m_eClass);

    	CString cSpeed, cHSpeed, cCodes=_T(""), cPenality, cDistance, cHDistance, cText;
    	CSize   cTextSize;
    	int     iInitialFontSize=17;
    
        pDC->SetTextAlign( TA_TOP| TA_LEFT );
    
    	FormatPenalityLine(	pcContestant->m_eClass,
							pcScoreRecord,
    						cCodes,
    						cPenality,
    						nCodes,
    						cCodeArray,
    						cReasonArray);
    
    	SelectFont( pDC, iInitialFontSize, FW_NORMAL, FALSE); //Text Font
    
    	if( !GetClass(pcTask->m_eClass).IsHandicapped()  )
    		{
    		if( pcScoreRecord->m_fContestant )
    			{
    			if( pcScoreRecord->m_fFinisher )
    				{
    				cSpeed.Format(_T("%6.2lf"),
    								ConvertDistance(pcScoreRecord->m_dSpeed, SYSTEMUNITS, m_eUnits) );
    				cDistance.Format(_T("%6.2lf"),
    								ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    				}
    			else
    				{
    				cSpeed=_T(" ");
    				cDistance.Format(_T("%6.2lf"),
    								ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    				}
    			}
    		else
    			{
    			cSpeed=_T(" ");
    			cDistance=pcScoreRecord->DNCText(pcTask->m_eStatus);
    			}
    		}
    	else
    		{
    		if( pcScoreRecord->m_fContestant )
    			{
    			if( pcScoreRecord->m_fFinisher )
    				{
    				cHSpeed.Format(_T("%6.2lf"),
    								ConvertDistance(pcScoreRecord->m_dHandicappedSpeed, SYSTEMUNITS, m_eUnits));
    				cSpeed.Format(_T("%6.2lf"),
    								ConvertDistance(pcScoreRecord->m_dSpeed, SYSTEMUNITS, m_eUnits));
    				cHDistance.Format(_T("%6.2lf"),
    								ConvertDistance(pcScoreRecord->m_dHandicappedDistance, SYSTEMUNITS, m_eUnits));
    				cDistance.Format(_T("%6.2lf"),
    								ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    				}
    			else
    				{
    				cSpeed=_T(" ");
    				cHSpeed=_T(" ");
    				cHDistance.Format(_T("%6.2lf"),
    								ConvertDistance(pcScoreRecord->m_dHandicappedDistance, SYSTEMUNITS, m_eUnits) );
    				cDistance.Format(_T("%6.2lf"),
    								ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    				}
    			}
    		else
    			{
    			cHSpeed=_T(" ");
    			cSpeed=_T(" ");
    			cHDistance=_T(" ");
    			cDistance=pcScoreRecord->DNCText(pcTask->m_eStatus);
    			}
    		}
    
    	TEXT_OUT( pcScoreRecord->m_cCumRankText,	iaSpeedTab[LEFTMARGIN], *piYLine );
    
        pDC->SetTextAlign( TA_RIGHT );
    	cText.Format(_T("%5.0lf"), max(pcScoreRecord->m_dCumPoints,0) );
    	TEXT_OUT( cText,	iaSpeedTab[CUMPOINTS]+40,	*piYLine );
        pDC->SetTextAlign( TA_LEFT );
    
    	TEXT_OUT( pcScoreRecord->m_cContestNo,	iaSpeedTab[CONID],		*piYLine );
    
    	cText.Format(_T("%d"), pcScoreRecord->m_iDayRank);
    	TEXT_OUT( cText,	iaSpeedTab[DAYRANK],	*piYLine );
    
        pDC->SetTextAlign( TA_RIGHT );
    	cText.Format(_T("%4.0lf"), pcScoreRecord->m_dPoints);
    	TEXT_OUT( cText,	iaSpeedTab[DAYPOINTS]+35,	*piYLine );
        pDC->SetTextAlign( TA_LEFT );
    
    	TEXT_OUT( pcContestant->m_strGlider,	iaSpeedTab[CONGLIDER],	*piYLine );
    
        pDC->SetTextAlign( TA_RIGHT );
    	if( cClass.IsHandicapped() )
    		{
    		SelectFont( pDC, 14, FW_NORMAL, FALSE); 
    		TEXT_OUT( cHSpeed,		iaSpeedTab[DAYSPEED]+40,	*piYLine );
    		TEXT_OUT( cHDistance,	aiHdapTab[0]+40,			*piYLine );
    		TEXT_OUT( cSpeed,		iaSpeedTab[DAYDIST]+40,		*piYLine );
    		TEXT_OUT( cDistance,	aiHdapTab[1]+40,			*piYLine );
    		}
    	else
    		{
    		TEXT_OUT( cSpeed,		iaSpeedTab[DAYSPEED]+70,	*piYLine );
    		TEXT_OUT( cDistance,	iaSpeedTab[DAYDIST]+60,	*piYLine );
    		}
        pDC->SetTextAlign( TA_LEFT );
    
    
    	// Output the name and glider type in smaller font if there is
    	// not enough room.  Find the correct size, but don't go below 9
    
    	cText=pcContestant->m_strLastName+_T(", ")+pcContestant->m_strFirstName+_T(" ")+pcContestant->m_strMiddleName;
    	cText=cText.Left(30); // Only worry about the first 30 chars
    	SelectFont( pDC, iInitialFontSize, FW_NORMAL, FALSE); 
    
    	for( int iSize=iInitialFontSize-1; iSize>9; iSize-- )
    		{
    		cTextSize=pDC->GetTextExtent(cText);
    		if( cTextSize.cx <(iaSpeedTab[CONGLIDER]-iaSpeedTab[CONNAME]) ) break;
    		SelectFont( pDC, iSize, FW_NORMAL, FALSE); 
    		}
    	TEXT_OUT( cText, iaSpeedTab[CONNAME], *piYLine );
    
    	// Output the code field and points in a smaller font
    	SelectFont( pDC, 9, FW_NORMAL, FALSE); //Text Font
    	TEXT_OUT( cCodes,		iaSpeedTab[REMCODE],	*piYLine );
    	TEXT_OUT( cPenality,	iaSpeedTab[REMPOINTS],	*piYLine );
    
    	if( /*pcTask->m_eType==ePost ||*/ pcTask->m_eType==eModifiedAssigned )
    		{
    		CFlight *pcFlight=m_FlightList.Get(pcScoreRecord->m_cDate, pcScoreRecord->m_cContestNo );
    		if( pcFlight!=NULL )
    			{
    			SelectFont( pDC, 9, FW_NORMAL, FALSE); //Text Font
    			cText=pcFlight->AcheivedTurnpointText();
//    			if( !pcFlight->IsTaskComplete() && pcFlight->GetSoughtTurnpoint()>0 )
  //  				{
    //				CString cTempSought;
    //				cTempSought.Format(_T(">%d"),pcFlight->GetSoughtTurnpoint() );
    //				cText+=cTempSought;
    //				}
    			TEXT_OUT( cText,		iaSpeedTab[REMCODE],	(*piYLine)-9 );
    			}
    		}
    
    	(*piYLine)-=LINE_HEIGHT;
    
    }
    
    
    BOOL CWinscoreDoc::DrawHeader(	 CDC *pDC, 
    								 CTime &cDate,
    								 EClass	eClass,
    								 int *piYpos)
    	{

    	CSize	 cTextSize;
    	int iYptr= *piYpos, iYTitleBottom= *piYpos;
    
    	CTask* pcTask=m_taskList.GetByDateClass( cDate, eClass);
    	if( pcTask==NULL) 
    		{
    		return FALSE;
    		}
    
    	//  Draw the contest date, name & location 
    
        pDC->SetTextAlign( TA_TOP| TA_LEFT );
    	SelectFont( pDC, 20, FW_SEMIBOLD, FALSE); 
    
    	CString strDate = cDate.Format( _T("%A, %B %d, %Y") );
    	pDC->TextOut( 0, iYptr, strDate, strDate.GetLength() );
    	cTextSize=pDC->GetTextExtent(strDate);
    	iYptr-=cTextSize.cy;
    
    	pDC->TextOut( 0, iYptr, m_strContestName, m_strContestName.GetLength() );
    	cTextSize=pDC->GetTextExtent(m_strContestName);
    	iYptr-=cTextSize.cy;
    
    	pDC->TextOut( 0, iYptr, m_strLocation, m_strLocation.GetLength() );
    	cTextSize=pDC->GetTextExtent(m_strLocation);
    	iYptr-=cTextSize.cy;
    
    	int iTitleFinish=0;
    	DrawTask(pDC, *pcTask, iYTitleBottom, &iTitleFinish);
    	if( iTitleFinish< iYptr ) iYptr = iTitleFinish;
    
    	*piYpos=iYptr;
    
    	return TRUE;
    	}
    
    
    void CWinscoreDoc::DrawScoresheetTrailer(CDC * pDC)
    {
    
    CTime cToday = CTime::GetCurrentTime();
    CString strDate=cToday.Format(_T("Created on %A %B %d, %Y - %H:%M by Winscore") );
    pDC->SetTextAlign( TA_TOP| TA_CENTER );
    SelectFont( pDC, 13, FW_NORMAL, FALSE); 
    
    DRAW_HORIZONTAL( -1020 );
    
    TEXT_OUT( strDate,	GetPageWidth(pDC)/2, -1025 );
    
    }
    
    
    BOOL CWinscoreDoc::DrawTitle(	CDC *pDC, 							
    								CTime &cDate,
    								EClass	eClass,
    								int &iYpos)
    	{
    	CString  cScoreSheetStatus;
    	CSize	 cTextSize;
    
    	CTask* pcTask=m_taskList.GetByDateClass( cDate, eClass);
    	if( pcTask==NULL) return FALSE;
    
    	BOOL fNoContestDay=( pcTask==NULL || pcTask->m_eStatus==eNoContest );
    	int iDay	=  GetContestDay( cDate,  eClass);
    	CString  cDay;
    	if( iDay>0 ) 
    		cDay.Format(_T("Day %d"),iDay);
    	else
    		cDay=_T("Practice");
    
    	if( GetNumClasses()>1 ) 
    		cScoreSheetStatus	= GetClass(eClass).GetName();
    
		cScoreSheetStatus+= _T("  ** ");
		cScoreSheetStatus+=pcTask->GetStatusText();
    	cScoreSheetStatus+= _T(" **");  

    	SelectFont( pDC, 30, FW_BOLD, FALSE); //Big Bold Font
    
    	if( fNoContestDay )
    		{
    		cScoreSheetStatus = _T("No Contest Day");
    		pDC->SetTextAlign( TA_TOP| TA_CENTER );
    		pDC->TextOut( GetPageWidth(pDC)/2, 0, cScoreSheetStatus, cScoreSheetStatus.GetLength() );
    		return FALSE;
    		}
      
        pDC->SetTextAlign( TA_TOP| TA_CENTER );
        pDC->TextOut( GetPageWidth(pDC)/2, 0, cScoreSheetStatus, cScoreSheetStatus.GetLength() );
    	cTextSize=pDC->GetTextExtent(cScoreSheetStatus);
    
        pDC->SetTextAlign( TA_TOP| TA_LEFT );
        pDC->TextOut( 0, 0, cDay, cDay.GetLength() );
    
        pDC->SetTextAlign( TA_TOP| TA_RIGHT );
        pDC->TextOut( GetPageWidth(pDC), 0, cDay, cDay.GetLength() );
    
    	iYpos-=cTextSize.cy;
    	iYpos-=20;
    
    	return TRUE;
    	}
    
    
    BOOL CWinscoreDoc::ExportDayText(	CStdioFile& cStdioFile,
    									EClass			eClass, 
    									CTime&			cDate, 
    									ESortBy		eSortBy,
    									CString		&cNewLineChar )
    	{
    
    
  	CString str=GetDayText( eClass, 
  							cDate, 
  							eSortBy,
  							cNewLineChar,
  							false );
  
  	cStdioFile.WriteString( str );
  
  	return TRUE;
  
  	}
  
  
  
  CString CWinscoreDoc::GetDayText(	EClass			eClass, 
  									CTime&			cDate, 
  									ESortBy			eSortBy,
  									CString			&cNewLineChar,
  									bool			bTabs			 )
  	{
  	CString strOut;
  
	CWSClass	&cClass=GetClass(eClass);

    CString cFDRText, cScoreSheetStatus,cDayDate, cLine, cLine1, cLine2, cLine3, cFormat, cName,
    		cScore, cPenality, cCodes, cTaskLine;
    int nCodes=0;
    CStringArray  cCodeArray, cReasonArray; 
    CScoreRecordList cScoreRecordList;

    int iDay	=  GetContestDay( cDate,  eClass);

    CTask* pcTask=m_taskList.GetByDateClass( cDate, eClass);
    if( pcTask==NULL || pcTask->m_eStatus==eNoContest )
    	{
    	cLine.Format(_T("%s: No Contest Day"), cClass.GetName() );
    	cLine+=cNewLineChar;
  		strOut+= cLine;
  		return strOut;
    	}

	cScoreSheetStatus=pcTask->GetStatusText();
   
  	strOut+=cNewLineChar;
  
  	strOut+=cScoreSheetStatus+cNewLineChar;
    
   
    int nRecords=m_ScoreRecordList.CopySort(cDate, 
    										eClass, 
    										cScoreRecordList,
    										eSortBy);

    cLine.Format(_T("%s Day %d"),cClass.GetName(), iDay );
    cLine+=cNewLineChar;
    cLine+=cNewLineChar;
  	strOut+=cLine;

    if( pcTask->m_eType==eModifiedAssigned )
    	cLine=_T("Task: Modified Assigned  ");
    else if( pcTask->m_eType==eAssigned)
    	cLine=_T("Task: Assigned		");
    else if( pcTask->m_eType==eFAIRacing)
    	cLine=_T("Task: FAI Racing		");
    else if( pcTask->m_eType==eFAIAssignedArea)
    	cLine=_T("Task: FAI Assigned Area		");
    else if( pcTask->m_eType==eTurnArea)
    	cLine=_T("Task: Turn Area     		");
    cLine+=cNewLineChar;
  	strOut+=cLine;


    //////// Write Task information
    
    	if( pcTask->IsAreaTask() || pcTask->m_eType==eAssigned || pcTask->m_eType==eFAIRacing || pcTask->m_eType==eModifiedAssigned)
    		{

			CString strRadiusText;
			if( pcTask->IsAreaTask() || 
				pcTask->m_cStartGate.IsGPSCylinder() ||
				pcTask->m_cFinishGate.IsGPSCylinder()   )
				{
				strRadiusText=_T("Radius");
				}

  			if( bTabs )
  				cLine.Format(_T("    ID\tName\tDistance(%s)\t%s"), UnitsText(m_eUnits), strRadiusText);
  			else
    			cLine.Format(_T("    ID   Name                 Distance (%s)   %s"), UnitsText(m_eUnits), strRadiusText);
    		cLine+=cNewLineChar;
  			strOut+=cLine;

    		double dDist=0.0;
    		CTurnpoint *pcTurnpoint, *pcPrevTurnpoint;

   			pcTurnpoint=m_turnpointArray[pcTask->m_cStartGate.GetPointID()-1];
  			OUTPUT_TPT_TEXT( pcTurnpoint, dDist,pcTask->m_cStartGate.IsGPSCylinder()?(ConvertDistance(pcTask->m_cStartGate.GetRadius(),SYSTEMUNITS, m_eUnits)):(0.0), cLine, bTabs, cNewLineChar, strOut );
			pcPrevTurnpoint=pcTurnpoint;

			if( pcTask->m_cStartGate.IsGPSCylinder() )
				dDist-=ConvertDistance(pcTask->m_cStartGate.GetRadius(), SYSTEMUNITS, m_eUnits);

    		for( int iTpt=0; iTpt<pcTask->GetNumTurnpoints(); iTpt++ )
    			{
    			pcTurnpoint=m_turnpointArray[pcTask->GetTurnpointID(iTpt)-1];
        		dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, m_eUnits);

				OUTPUT_TPT_TEXT( pcTurnpoint, dDist, (pcTask->IsAreaTask())?(ConvertDistance(pcTask->GetTurnpointRadius(iTpt),SYSTEMUNITS, m_eUnits)):(0), cLine, bTabs, cNewLineChar, strOut );

    			pcPrevTurnpoint=pcTurnpoint;
    			}

   			pcTurnpoint=m_turnpointArray[pcTask->m_cFinishGate.GetPointID()-1];
        	dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, m_eUnits);

			if( pcTask->m_cFinishGate.IsGPSCylinder() )
				dDist-=ConvertDistance(pcTask->m_cFinishGate.GetRadius(), SYSTEMUNITS, m_eUnits);
			
			OUTPUT_TPT_TEXT( pcTurnpoint, dDist, pcTask->m_cFinishGate.IsGPSCylinder()?(ConvertDistance(pcTask->m_cFinishGate.GetRadius(),SYSTEMUNITS, m_eUnits)):(0.0),cLine, bTabs, cNewLineChar, strOut );
			}
  



    	if( pcTask->IsTimedTask()  )
    		{
  			strOut+=cNewLineChar;

    		cLine=pcTask->m_cPostTime.Format(_T(" Minimum Time\t%H:%M") );
    		cLine+=cNewLineChar;
  			strOut+=cLine;
    
  			}
  
 		strOut+=cNewLineChar;
    
    	///////   Write Scoresheet   /////////////////
    
    
    
    	if( cClass.IsHandicapped() )
  		{
  		if( bTabs )
  			{
  			cLine1.Format(_T(" Cumulative\t\t   \t                \t        \t     \tDay   \tSpeed\t\tDistance\t\tRemarks"));
  			cLine2.Format(_T(" Rank\tPoints\tID \tName            \tGlider  \tRank \tPoints\tHndcp\tActual\tHndcp\tActual\tCode\tPenality Points"));
  			cLine3.Format(_T(" ----\t----- \t-- \t----            \t------  \t---- \t------\t-----\t------\t-----\t------\t----\t-------- ------"));
  			}
  		else
    		{
    		cLine1.Format(_T(" Cumulative                                                Day             Speed             Distance         Remarks"));
    		cLine2.Format(_T(" Rank  Points  ID   Name                   Glider      Rank    Points  Hndcp   Actual    Hndcp   Actual    Code    Penalty Points"));
    		cLine3.Format(_T(" ----  -----   --   ----                   ------      ----    ------  -----   ------    -----   ------    ----    ------- ------"));
    		}
  		}
  	else
  		{
  		if( bTabs )
  			{
  			cLine1.Format(_T(" Cumulative\t    \t    \t               \t        \tDay   \t      \t     \t        \tRemarks"));
  			cLine2.Format(_T(" Rank  \tPoints  \tID  \tName           \tGlider  \tRank  \tPoints\tSpeed\tDistance\tCode\tPenality Points"));
  			cLine3.Format(_T(" ----  \t-----   \t--  \t----           \t------  \t----  \t------\t-----\t--------\t----\t-------- ------"));
  			}
    	else
    		{
    		cLine1.Format(_T(" Cumulative                                                Day                              Remarks"));
    		cLine2.Format(_T(" Rank  Points  ID   Name                   Glider      Rank    Points  Speed   Distance  Code    Penalty  Points"));
    		cLine3.Format(_T(" ----  -----   --   ----                   ------      ----    ------  -----   --------  ----    -------- ------"));
    		}
  		}
  	strOut+=cLine1+cNewLineChar;
  	strOut+=cLine2+cNewLineChar;
  	strOut+=cLine3+cNewLineChar;
    
    	int  iRec=0;
    	if( nRecords>0 )
    		{
    		POSITION pos = cScoreRecordList.GetHeadPosition();
    		while( pos!=NULL )
    			{
    
    			CScoreRecord* pcScoreRecord=(CScoreRecord*)cScoreRecordList.CObList::GetNext(pos);
    			ASSERT(pcScoreRecord);
    			CContestant* pcContestant=m_contestantList.GetByContestNo( pcScoreRecord->m_cContestNo);
    			ASSERT (pcContestant);
				if( !pcContestant ) continue;
    
    			if( !GetClass(eClass).IsHandicapped() )
    				{
    				if( pcScoreRecord->m_fContestant )
    					{
    					if( pcScoreRecord->m_fFinisher )
  						cScore.Format(bTabs?(_T("%6.2lf\t %6.2lf")):(_T("%6.2lf  %6.2lf")),
    										ConvertDistance(pcScoreRecord->m_dSpeed, SYSTEMUNITS, m_eUnits),
    										ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    					else
  						cScore.Format(bTabs?(_T(" \t%6.2lf")):(_T("        %6.2lf")),
    										ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    					}
    				else
    					cScore=_T("           ")+pcScoreRecord->DNCText(pcTask->m_eStatus);
    				}
    			else
    				{
    				if( pcScoreRecord->m_fContestant )
    					{
    					if( pcScoreRecord->m_fFinisher )
  						cScore.Format(bTabs?(_T("%6.2lf\t%6.2lf\t%6.2lf\t%6.2lf")):(_T("%6.2lf  %6.2lf    %6.2lf  %6.2lf")),
     										ConvertDistance(pcScoreRecord->m_dHandicappedSpeed, SYSTEMUNITS, m_eUnits),
    										ConvertDistance(pcScoreRecord->m_dSpeed, SYSTEMUNITS, m_eUnits),
    										ConvertDistance(pcScoreRecord->m_dHandicappedDistance, SYSTEMUNITS, m_eUnits),
    										ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    					else
  						cScore.Format(bTabs?(_T("\t\t%6.2lf\t%6.2lf")):(_T("                  %6.2lf  %6.2lf")),
     										ConvertDistance(pcScoreRecord->m_dHandicappedDistance, SYSTEMUNITS, m_eUnits),
    										ConvertDistance(pcScoreRecord->m_dDistance, SYSTEMUNITS, m_eUnits) );
    					}
    				else
  					cScore=			  bTabs?(_T("\t\t\t\t")):(_T("                             "))+pcScoreRecord->DNCText(pcTask->m_eStatus);
     				}
    
    
    			FormatPenalityLine(	eClass,
									pcScoreRecord,
    								cCodes,
    								cPenality,
    								nCodes,
    								cCodeArray,
    								cReasonArray);
    
    			if( cClass.IsHandicapped() )
  				{
  				if( bTabs )
  		 			cFormat=_T("%5s\t%6.0lf\t%3s\t%-20s\t%-10s\t%5d\t%6.0lf\t%-35s\t%-s\t%-s");
  				else
    		 		cFormat=_T("%5s %6.0lf  %3s   %-20s   %-10s %5d   %6.0lf   %-35s %-s    %-s  ");
  				}
  			else
  				{
  				if( bTabs )
  		 			cFormat=_T("%5s\t%6.0lf\t%3s\t%-20s\t%-10s\t%5d\t%6.0lf\t%-17s\t%-s\t%-s");
    			else
    		 		cFormat=_T("%5s %6.0lf  %3s   %-20s   %-10s %5d   %6.0lf   %-17s %-s    %-s  ");
  				}
  
    
    			cName=pcContestant->m_strLastName+_T(", ")+pcContestant->m_strFirstName+_T(" ")+pcContestant->m_strMiddleName;
    			cLine.Format(	cFormat,
    							pcScoreRecord->m_cCumRankText,
    							max(pcScoreRecord->m_dCumPoints,0),
    							pcScoreRecord->m_cContestNo,
    							cName.Left(20),
    							pcContestant->m_strGlider.Left(10),
    							pcScoreRecord->m_iDayRank,
    							pcScoreRecord->m_dPoints,
    							cScore,
    							cCodes,
    							cPenality
    							);
  				strOut+=cLine +cNewLineChar;
    			iRec++;
				if( iRec%10==0 ) strOut+=cNewLineChar;
    			}
    		}
    
    
    	if( nCodes>0 )
    		{
  		strOut+=cNewLineChar;
  		strOut+=_T("Codes:");
  		strOut+=cNewLineChar;
    
    		for( int i=0; i<nCodes; i++)
    			{
    
    			BOOL bDupulicate=FALSE;
    			if( i>0 )
    				{
    				for( int j=0; j<i; j++ ) 
    					if( cCodeArray[j]==cCodeArray[i] ) bDupulicate=TRUE;
    				}
    			if( bDupulicate ) continue;
    
			CString cOCode=cCodeArray[i];
			CString cOReason=cReasonArray[i];
  			cLine.Format( bTabs?(_T("%5s\t%30s")):(_T("   %-5s  %-30s")), cOCode, cOReason);
    			cLine+=cNewLineChar;
  			strOut+=cLine;
     			}
    		}
    
  	return strOut;
     }
    
    void CWinscoreDoc::FormatPenalityLine(	EClass			eClass,
											CScoreRecord	*pcScoreRecord,
    									    CString			&cCodes,
    										CString			&cPenality,
    										int				&nCodes,
    										CStringArray	&cCodeArray,
    										CStringArray	&cReasonArray)
    
        {
		CWSClass &cClass=GetClass(eClass);
    	cCodes=_T("");
    	if( pcScoreRecord->m_uPenalityMask & WSP_A_GATE )
    		{
    		cCodes+=_T("A");
    		cCodeArray.Add(_T("A "));
    		cReasonArray.Add(_T("Assigned to A Start Gate "));
    		nCodes++;
    		}
    	if( pcScoreRecord->m_uPenalityMask & WSP_B_GATE )
    		{
    		cCodes+=_T("B");
    		cCodeArray.Add(_T("B "));
    		cReasonArray.Add(_T("Assigned to B Start Gate "));
    		nCodes++;
    		}

    	if( pcScoreRecord->m_uPenalityMask & WSP_MINDIST)
    		{
 			if( cCodes.GetLength()>0 ) cCodes+=_T(", ");
	  		cCodes+=_T("MD");
    		cCodeArray.Add(_T("MD "));
			CString cStr;
			cStr.Format(_T("Flight less than minimum distance of%3.0lf %s"),   cClass.GetMinTaskDistance( m_eUnits ), GetUnitsText() );
    		cReasonArray.Add(cStr);
    		nCodes++;
    		}
    	if( pcScoreRecord->m_uPenalityMask & WSP_MINTIME15)
    		{
 			if( cCodes.GetLength()>0 ) cCodes+=_T(", ");
    		cCodes+=_T("MT15");
    		cCodeArray.Add(_T("MT15 ") );
    		cReasonArray.Add(_T("Time > 15 Min. under minimum. task time."));
    		nCodes++;
    		}
		else if( pcScoreRecord->m_uPenalityMask & WSP_MINTIME)
    		{
 			if( cCodes.GetLength()>0 ) cCodes+=_T(", ");
    		cCodes+=_T("MT ");
    		cCodeArray.Add(_T("MT "));
    		cReasonArray.Add(_T("Flight time less than minimum time."));
    		nCodes++;
    		}  	
		if( pcScoreRecord->m_uPenalityMask & WSP_NOSSANUMBER)
    		{
 			if( cCodes.GetLength()>0 ) cCodes+=_T(", ");
    		cCodes+=_T("SSA");
    		cCodeArray.Add(_T("SSA "));
    		cReasonArray.Add(_T("No SSA number on file for this pilot."));
    		nCodes++;
    		}
    	if( pcScoreRecord->m_uPenalityMask & WSP_NOSTART)
    		{
 			if( cCodes.GetLength()>0 ) cCodes+=_T(", ");
    		cCodes+=_T("NVS");
    		cCodeArray.Add(_T("NVS "));
    		cReasonArray.Add(_T("Did not make a valid start."));
    		nCodes++;
    		}
    	if( pcScoreRecord->m_uPenalityMask & WSP_AIRFIELDBONUS)
    		{
 			if( cCodes.GetLength()>0 ) cCodes+=_T(", ");
    		cCodes+=_T("AB");
    		cCodeArray.Add(_T("AB "));
    		cReasonArray.Add(_T("25pt Airfield Bonus."));
    		nCodes++;
    		}
  		if( pcScoreRecord->m_uPenalityMask & WSP_FOREIGN)
    		{
 			if( cCodes.GetLength()>0 ) cCodes+=_T(", ");
    		cCodes+=_T("N");
    		cCodeArray.Add(_T("N "));
    		cReasonArray.Add(_T("Foreign Pilot."));
    		nCodes++;
    		}   
  		if( pcScoreRecord->m_uPenalityMask & WSP_WITHDREW)
    		{
 			if( cCodes.GetLength()>0 ) cCodes+=_T(", ");
    		cCodes+=_T("W");
    		cCodeArray.Add(_T("W "));
    		cReasonArray.Add(_T("Withdrew from contest."));
    		nCodes++;
    		}    
 		if( pcScoreRecord->m_uPenalityMask & WSP_NOFLIGHTLOG)
    		{
 			if( cCodes.GetLength()>0 ) cCodes+=_T(", ");
    		cCodes+=_T("F");
    		cCodeArray.Add(_T("F "));
    		cReasonArray.Add(_T("No flight log."));
    		nCodes++;
    		}     
	    int nLocCodes=0;
    	CStringArray cLocCodeArray, cLocReasonArray;
    	m_PenalityList.AddCodes(pcScoreRecord->m_cContestNo, 
    							pcScoreRecord->m_cDate, 
    							&nLocCodes, 
    							cLocCodeArray, 
    							cLocReasonArray);
    	if( nLocCodes>0 )
    		{
    		for( int iPen=0; iPen<nLocCodes; iPen++ )
    			{
 			    if( cCodes.GetLength()>0 ) cCodes+=_T(", ");
    			cCodes+=cLocCodeArray[iPen];
				if( iPen<nLocCodes-1) cCodes+=_T(", ");
    			cCodeArray.Add(cLocCodeArray[iPen]);
    			cReasonArray.Add(cLocReasonArray[iPen]);
    			nCodes++;
    			}
		

    	}
    
    
    	double dPenality=pcScoreRecord->m_dAppliedPenalityPoints;
    	CPenality *pcContestPenality=m_PenalityList.Get( pcScoreRecord->m_cContestNo, 
    											  pcScoreRecord->m_cDate,
    											  eContestPenalty);
    	if( pcContestPenality )
    		{
    		// Poor bastard has a contest penality, apply it!
    		dPenality+=pcContestPenality->m_dPenality;
    		}
    
    	if( dPenality!=0.0 )
    		cPenality.Format(_T("%4.0lf"),dPenality);
    	else
    		cPenality=_T(" ");
    	}
    
    
   
    void CWinscoreDoc::ExportTextHeader(CStdioFile& cStdioFile, 
    									CTime& cDate,
    								    EScoresheetType eType,
    									CString   &cNewLineChar )
    {
  	cStdioFile.WriteString( GetTextHeader(	cDate,
  											eType,
  											cNewLineChar ));
  	}
  
  
  CString CWinscoreDoc::GetTextHeader( CTime& cDate,
  								     EScoresheetType eType,
  									 CString   &cNewLineChar )
  	{
  	CString strOut;
    CString cLine1, cLine;
    
  	strOut+=cNewLineChar;
    
  	strOut+=m_strContestName+cNewLineChar+m_strLocation+cNewLineChar;

    
    if( eType==eDay )
    	{
    	CString strDate = cDate.Format( _T("%A, %B %d, %Y") );
  		strOut+=strDate+cNewLineChar;
    	}
    else
    	{
		strOut+=_T("Cumulative Summary")+cNewLineChar;
    	}
    
    return strOut;
    }
       
    
    BOOL CWinscoreDoc::ExportCumulativeText(	CStdioFile&	cStdioFile,
    											EClass		eClass, 
    											CString		&cNewLine)
    	{    
  		cStdioFile.WriteString( GetCumulativeText(	eClass, 
  													cNewLine,
  													false)      );
  		return TRUE;
  		}
  
  CString CWinscoreDoc::GetCumulativeText(	EClass	eClass, 
  											CString	&cNewLine,
  											bool	bTabs )
  	{

	  CWSClass &cClass=GetClass(eClass);


  	CString strOut;
    	CString cDayDate, cLine, cScore, cString;
    	int nColumns, iDay;
    	CScoreRecordList cScoreRecordList;
    	CTime cLastContestDay;
    	double	dPrevScore=0.0;
    	int		iPrevStanding=(-1);
       
    	for( int i=0; i<m_iNumContestDays; i++ )
    		{
    		if( !NoContestDay(m_caContestDays[i], eClass) ) cLastContestDay=m_caContestDays[i];
    		}
    
    
    
    	//  Get the records for the last contest day sorted by cum winners,
    	//  This is the order we want
    
    	int nRecords=m_ScoreRecordList.CopySort(cLastContestDay, 
    											eClass, 
    											cScoreRecordList,
    											eCumulative);
    
    
    	///////   Write Scoresheet   /////////////////
    
    	nColumns = 5+ 2*m_iNumContestDays;
    
    
    	/////////  1st Write Header Line
    
    
    	cLine.Format(	_T("%s Class %s%s"),
    					cClass.GetName(),
    					cNewLine,cNewLine
    					 );
 	strOut+= cLine +cNewLine;

    
    	/////////  Write 2nd Header Line
    
  	if( bTabs )
  		cLine=_T("    \t        \t    \t    \t           \t    ");
  	else
    	cLine=_T("                                                                      ");
    
    	for( iDay=m_iNumContestDays-1; iDay>=0; iDay-- )
    		{
    		CString cDateString = m_caContestDays[iDay].Format(_T("%b %d, %y") );
  		cString.Format(	bTabs?(_T("\t%-s\t")):(_T("%-18s")), cDateString);
     		cLine+=cString;
    		}
  	strOut+= cLine+cNewLine;
     	
  	if( bTabs )
  		cLine=_T("Cumulative Score\t    \t    \t      \t      \t    ");
  	else
    	cLine=_T("Cumulative Score                                                      ");
    
    	for( iDay=m_iNumContestDays-1; iDay>=0; iDay-- )
    		{
    		if( NoContestDay( m_caContestDays[iDay], eClass) )
    			{
  			if( bTabs ) 
  				cLine+=_T("\tNo Contest\t");
  			else
    			cLine+=_T("No Contest        ");
    			}
    		else
    			{
    			CTask* pcTask=m_taskList.GetByDateClass(m_caContestDays[iDay], eClass);
    			if( pcTask->m_eType==eAssigned || pcTask->m_eType==eFAIRacing)
  				cString.Format(	bTabs?(_T("\tDist: %6.1lf %s\t")):(_T("Dist: %6.1lf %s   ")),
     								pcTask->GetDistance(m_turnpointArray,m_eUnits),
    								UnitsTextShort(m_eUnits) );
    			else if( pcTask->m_eType==eModifiedAssigned  )
    				{
  					cString.Format(bTabs?(_T("\tMod Assgn: %5s\t")):(_T("Mod Assgn: %5s  ")),
     								   pcTask->m_cPostTime.Format(_T("%H:%M")) );
   					}
   			    else if( pcTask->IsAreaTask()  )
    				{
  					cString.Format(bTabs?(_T("\tTurn Area: %5s\t")):(_T("Turn Area: %5s  ")),
     								   pcTask->m_cPostTime.Format(_T("%H:%M")) );
   					}
    			else
    				{
  				cString.Format(bTabs?(_T("\tPilot Sel: %5s\t")):(_T("Pilot Sel: %5s  ")),
     							   pcTask->m_cPostTime.Format(_T("%H:%M")) );
    				}
    			cLine+=cString;
    			}
    		}
  	strOut+= cLine+cNewLine;
    
    	/////////  Write 3rd Header Line
    
  	if( bTabs )
  		cLine=_T(" Rank\tPoints\tCPen\tID\tSSA#\tName            \tGlider   ");
  	else
    	cLine=_T(" Rank  Points  CPen  ID   SSA#   Name                  Glider        ");
    	for( iDay=m_iNumContestDays-1; iDay>=0; iDay-- )
    		{
  		cLine+=bTabs?(_T("\tRank\tPoints ")):(_T("Rank  Points      "))  ;
     		}
  	strOut+= cLine+cNewLine;
     
  	if( bTabs )
  		cLine=_T("---- \t------\t---- \t-- \t---- \t----            \t------     \t");
  	else
    	cLine=_T(" ----  ------  ----  --  ------  ----                  ------        ");

    for( iDay=m_iNumContestDays-1; iDay>=0; iDay-- )
    	{
  	if( bTabs )
  		cLine+=_T("---- \t------\t ")  ;
  	else
    	cLine+=_T("----  ------      ")  ;
    	}

  	strOut+= cLine+cNewLine;
     
    	int  iRec=0;
    	if( nRecords>0 )
    		{
    		POSITION pos = cScoreRecordList.GetHeadPosition();
    		while( pos!=NULL )
    			{
    
    			CScoreRecord* pcScoreRecord=(CScoreRecord*)cScoreRecordList.CObList::GetNext(pos);
    			ASSERT(pcScoreRecord);
    			CContestant* pcContestant=m_contestantList.GetByContestNo( pcScoreRecord->m_cContestNo);
    			ASSERT (pcContestant);
				int iCPen=m_PenalityList.GetContestTotal(pcScoreRecord->m_cContestNo);
				iCPen=iCPen*(-1);
    			
    			cString=pcContestant->m_strLastName+_T(", ")+pcContestant->m_strFirstName+_T(" ")+pcContestant->m_strMiddleName;
  			    cLine.Format(bTabs?(_T("%5s\t%6.0lf\t%4d\t%3s\t%8d\t%-20s\t%-10s")):(_T("%5s  %6.0lf  %4d  %3s  %6d  %-20s  %-10s ")),
     							pcScoreRecord->m_cCumRankText,
    							max(pcScoreRecord->m_dCumPoints,0),
								iCPen,
    							pcScoreRecord->m_cContestNo,
								pcContestant->m_iSSANumber,
    							cString.Left(20),
    							pcContestant->m_strGlider.Left(10) );
    
    			for( iDay=m_iNumContestDays-1; iDay>=0; iDay-- )
    				{
    				CScoreRecordList cDayRecordList;
    
    				m_ScoreRecordList.CopySort( m_caContestDays[iDay], 
    											eClass, 
    											cDayRecordList,
    											eDaily);
    
    				if( NoContestDay( m_caContestDays[iDay], eClass) )
    					{
  					cLine+=bTabs?(_T("\t\t0")):(_T("           0      "));
     					}
    				else
    					{
    					CScoreRecordList cDayRecordList;
    					m_ScoreRecordList.CopySort( m_caContestDays[iDay], 
    												eClass, 
    												cDayRecordList,
    												eDaily);
    
    					CScoreRecord* pcRec=cDayRecordList.Get( pcScoreRecord->m_cContestNo, m_caContestDays[iDay] );
    					if( pcRec )
    						{
  						cString.Format(bTabs?(_T("\t%2d\t%5.0lf")):(_T("  %2d   %5.0lf      ")),
     												pcRec->m_iDayRank,
    												pcRec->m_dPoints	);
    					    cLine+=cString;
    						}
    					else
  						cLine+=bTabs?(_T("/t/t0")):(_T("           0      "));
  
  					}
  				}
  			strOut+= cLine+cNewLine;
  			}
  		}
  
  
  	strOut+= cNewLine;
  	strOut+= cNewLine;
  
  	return strOut;
  }
  
  void CWinscoreDoc::ExportTextTrailer(	CStdioFile& cStdioFile, 
  										CString &cNewLineChar, 
  										CString cComment, 
  										EExportType	eExportType )
  	{
  	cStdioFile.WriteString( GetTextTrailer(	cNewLineChar, 
  										    cComment, 
  										    eExportType ) );
  	}
  
  
  
  CString CWinscoreDoc::GetTextTrailer(	CString &cNewLineChar, 
  										CString cComment, 
  										EExportType	eExportType )
  	{
  	CString strOut;
 
    	// Write the comment
    	if( cComment.GetLength() >0 ) 
    		{
  		strOut+= cNewLineChar;
    		CString cLine=cComment[0];
    		for (int i=1; i<cComment.GetLength(); i++)
    			{
    			if( cComment[i]=='\r') continue;
    			if( cComment[i]=='\n')
    				{
  				strOut+=cLine+cNewLineChar;
     				cLine=_T("");
    				}
    			else
    				{
    				cLine+=cComment[i];
    				}
    			}
  		if( cLine.GetLength()>0 ) strOut+=cLine+cNewLineChar;
     		}
    
    	CTime cToday = CTime::GetCurrentTime();
  	strOut+= cNewLineChar;
  	strOut+= cNewLineChar;
 	CString cLine=cToday.Format(_T("Created on %A %B %d, %Y - %H:%M by Winscore  "));
	cLine+=SHORTVERSION;
	cLine+=_T(",  http://");
	cLine+=WINSCOREWEB;
	strOut+= cLine+cNewLineChar+cNewLineChar;
  	return strOut;
    }
    
    bool CWinscoreDoc::IsDateValid(CTime cDate)
    	{
    		if( IsPracticeDay(cDate)>0 ) return true;
    
    		for( int i=0; i<m_iNumContestDays; i++) 
    			{
    			if( DatesEqual(cDate,m_caContestDays[i]) ) return true;
    			}
    
    	return false;
    	}
    
    bool CWinscoreDoc::IsContestNoValid(CString cContestNo)
    {
    CContestant* ptr=m_contestantList.GetByContestNo(cContestNo);
    if( ptr==NULL ) return false;
    
    return true;
    }
    
    bool CWinscoreDoc::IsCPIDValid(int cid)
    {
    	if( cid<1 || cid>m_turnpointArray.GetSize()) return false;
    
    	if( m_turnpointArray[cid]==NULL ) return false;
    
    	return true;
    }
    
    bool CWinscoreDoc::IsTurnpointAirfield(int iCSID)
    {
    	if( iCSID<1 || iCSID>m_turnpointArray.GetSize()) return false;
    
    	CTurnpoint *ptp=m_turnpointArray[iCSID];
    	if( ptp==NULL ) return false;
    
    	return ptp->IsAirfield();
    }
    
    BOOL CWinscoreDoc::OnNewDocument() 
    	{
			
    	CString strNull;
		SetWSCPath(strNull);

    	return CDocument::OnNewDocument();
    
    	}
    
    void CWinscoreDoc::OnCloseDocument() 
    {
    	// Save this name in the registry for next start
    	CString strName=GetPathName( );
    	CWinApp* pApp = AfxGetApp();
    	pApp->WriteProfileString( REGSECTION, REGCONTEST, strName );
       
    	CString strNull;
		SetWSCPath(strNull);

    	CDocument::OnCloseDocument();
    }
    
    
    void CWinscoreDoc::DrawTaskInfoSheet(CDC *pDC, CTask *pcTask, CString& cContestName, CString& cContestLocation  )
    {
    	CSize	 cTextSize;
    	int iYpos=0;
    	CString  cTitle=_T("Task Sheet"), cText, cPage;
		int	aiAssignedTabs[4]={ 0, 30, 150, 300 };
    
    	SelectFont( pDC, 30, FW_BOLD, FALSE);
    
        pDC->SetTextAlign( TA_TOP| TA_CENTER );
        pDC->TextOut( GetPageWidth(pDC)/2, 0, cTitle, cTitle.GetLength() );
    	cTextSize=pDC->GetTextExtent(cTitle);
    
        pDC->SetTextAlign( TA_TOP| TA_LEFT );
    	CString cClassText = GetClass(pcTask->m_eClass).GetName();
    	pDC->TextOut( 0, iYpos, cClassText, cClassText.GetLength() );
    	cTextSize=pDC->GetTextExtent(cClassText);
    
    	CString strDate = pcTask->m_cDate.Format( _T("%a. %b %d, %y") );
    	cTextSize=pDC->GetTextExtent(strDate);
    	pDC->TextOut( GetPageWidth(pDC)-cTextSize.cx, iYpos, strDate, strDate.GetLength() );
    
    	iYpos-=cTextSize.cy;
    	iYpos-=5;
    
    	SelectFont( pDC, 20, FW_BOLD, FALSE);
    
    	pDC->TextOut( 0, iYpos, cContestName, cContestName.GetLength() );
    	cTextSize=pDC->GetTextExtent(cContestName);
    	iYpos-=cTextSize.cy;
    
    	pDC->TextOut( 0, iYpos, cContestLocation, cContestLocation.GetLength() );
    	cTextSize=pDC->GetTextExtent(cContestLocation);
    	iYpos-=cTextSize.cy;
    	iYpos-=4;
    
    	DRAW_HORIZONTAL( iYpos );
    
    	iYpos-=9;
    
    	int iXTask=0;
    
    	CString  cLine, cFDRText;
    
    	if( pcTask->IsAreaTask() || pcTask->m_eType== eAssigned || pcTask->m_eType== eModifiedAssigned )
    		{
    		if( pcTask->m_eType==eAssigned )
    			cText=_T("Assigned Task");
    		else if(pcTask->m_eType== eModifiedAssigned)
    			cText=_T("Modified Assigned Task");
    		else if(pcTask->m_eType== eTurnArea)
    			cText=_T("Turn Area Task");
    		else if(pcTask->m_eType== eFAIRacing)
    			cText=_T("FAI Racing");
    		else if(pcTask->m_eType== eFAIAssignedArea)
    			cText=_T("FAI Assigned Area");    
    
    		pDC->TextOut( iXTask , iYpos, cText, cText.GetLength() );
    		cTextSize=pDC->GetTextExtent(cText);
    		iYpos-=cTextSize.cy;
    
    		SelectFont( pDC, 20, FW_NORMAL, true );
    
    		cText=_T("ID");
    		pDC->TextOut( iXTask+aiAssignedTabs[0], iYpos, cText, cText.GetLength() );
    		cText=_T("Name");
    		pDC->TextOut( iXTask+aiAssignedTabs[1], iYpos, cText, cText.GetLength() );
    		cText=_T("Distance (");
    		cText+=UnitsText(m_eUnits);
    		cText+=_T(")");
    		pDC->TextOut( iXTask+aiAssignedTabs[2], iYpos, cText, cText.GetLength() );

			if( pcTask->IsAreaTask() || 
				pcTask->m_cStartGate.IsGPSCylinder() ||
				pcTask->m_cFinishGate.IsGPSCylinder()   )
				{
				cText=_T("Radius");
    			pDC->TextOut( iXTask+aiAssignedTabs[3], iYpos, cText, cText.GetLength() );
    			cTextSize=pDC->GetTextExtent(cText);
				}
			
			cTextSize=pDC->GetTextExtent(cText);
    		iYpos-=cTextSize.cy;
    
    		if( pcTask->GetNumTurnpoints()<6 )
    			SelectFont( pDC, 17, FW_NORMAL, FALSE );
    		else
    			SelectFont( pDC, 15, FW_NORMAL, FALSE );
    
    		double dDist=0.0;
    		CTurnpoint *pcTurnpoint=NULL, *pcPrevTurnpoint=NULL;
    
   			pcTurnpoint=m_turnpointArray[pcTask->m_cStartGate.GetPointID()-1];
			OUTPUT_TPT_OUT( pcTurnpoint, dDist,  pcTask->m_cStartGate.IsGPSCylinder()?(ConvertDistance(pcTask->m_cStartGate.GetRadius(),SYSTEMUNITS, m_eUnits)):(0.0), pDC, iXTask, iYpos, aiAssignedTabs );
    		pcPrevTurnpoint=pcTurnpoint;

			if( pcTask->m_cStartGate.IsGPSCylinder() )
				dDist-=ConvertDistance(pcTask->m_cStartGate.GetRadius(), SYSTEMUNITS, m_eUnits);

    		for( int iTpt=0; iTpt<pcTask->GetNumTurnpoints(); iTpt++ )
    			{
     			pcPrevTurnpoint=pcTurnpoint;
   				pcTurnpoint=m_turnpointArray[pcTask->GetTurnpointID(iTpt)-1];
        		dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, m_eUnits);

				OUTPUT_TPT_OUT( pcTurnpoint, dDist, (pcTask->IsAreaTask())?(ConvertDistance(pcTask->GetTurnpointRadius(iTpt), SYSTEMUNITS, m_eUnits)):(0),pDC, iXTask, iYpos, aiAssignedTabs );
     			pcPrevTurnpoint=pcTurnpoint;
    			}

   			pcTurnpoint=m_turnpointArray[pcTask->m_cFinishGate.GetPointID()-1];
       		dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, m_eUnits);

			if( pcTask->m_cFinishGate.IsGPSCylinder() )
				dDist-=ConvertDistance(pcTask->m_cFinishGate.GetRadius(), SYSTEMUNITS, m_eUnits);

   			pcTurnpoint=m_turnpointArray[pcTask->m_cFinishGate.GetPointID()-1];
 			OUTPUT_TPT_OUT( pcTurnpoint,  dDist,  pcTask->m_cFinishGate.IsGPSCylinder()?(ConvertDistance(pcTask->m_cFinishGate.GetRadius(),SYSTEMUNITS, m_eUnits)):(0.0), pDC, iXTask, iYpos, aiAssignedTabs );

    		
    		if( pcTask->m_eType== eAssigned || pcTask->m_eType== eFAIRacing  )
    			{
    			iYpos-=5;
    			SelectFont( pDC, 20, FW_BOLD, FALSE);
    			cText.Format( _T("Total Distance: %6.2lf %s"), dDist, GetUnitsText()  );
    			pDC->TextOut( iXTask, iYpos, cText, cText.GetLength() );
    			cTextSize=pDC->GetTextExtent(cText);
    			iYpos-=cTextSize.cy;
    			}
   			else if(  pcTask->IsAreaTask() )
    			{
				double dMin, dDist, dMax;
				pcTask->GetTurnAreaDistances( m_turnpointArray,  dMin, dDist,  dMax );

    			iYpos-=5;
    			SelectFont( pDC, 20, FW_BOLD, FALSE);
    			cText.Format( _T("Distance (%s)- %6.2lf Min: %6.2lf, Max: %6.2lf"), GetUnitsText(),	ConvertDistance(dDist, SYSTEMUNITS, m_eUnits),
																									ConvertDistance(dMin, SYSTEMUNITS, m_eUnits),
																									ConvertDistance(dMax, SYSTEMUNITS, m_eUnits)  );
    			pDC->TextOut( iXTask, iYpos, cText, cText.GetLength() );
    			cTextSize=pDC->GetTextExtent(cText);
    			iYpos-=cTextSize.cy;
    			}

    
    		}	
    
    	if( pcTask->IsTimedTask() )
    		{
    		SelectFont( pDC, 17, FW_BOLD, FALSE);
    
       		iYpos-=15;
    		cText=pcTask->m_cPostTime.Format(_T("Minimum Time: %H:%M") );
    		pDC->TextOut( iXTask , iYpos, cText, cText.GetLength() );
    		cTextSize=pDC->GetTextExtent(cText);
    		iYpos-=cTextSize.cy;
    
    		}
    
    		iYpos-=25;
    
    		DRAW_HORIZONTAL( iYpos );
    		iYpos-=10;
    	
    
    		SelectFont( pDC, 18, FW_NORMAL, FALSE );
    
    		// Write the user modified text
    		cText=m_cTaskInfoString;
    		while( 1 )
    			{
    			int iPos=cText.Find('\r');
    			if( iPos<0 ) break;
    			if( iPos==0 )
    				{
    				iYpos-=12;
    				}
    			else
    				{
    				cLine=cText.Left(iPos);
    				pDC->TextOut( iXTask , iYpos, cLine, cLine.GetLength() );
    				cTextSize=pDC->GetTextExtent(cLine);
    				iYpos-=cTextSize.cy;
    				}
    			cText=cText.Right(cText.GetLength()-iPos-2);
    			if(cText.GetLength()==0) break;
    
    			// Overrun safety check;
    			if( iYpos< -1000 ) break;
    			}
    
    	DrawScoresheetTrailer(pDC);
    
    }
    
 /*   
    BOOL CWinscoreDoc::OnOpenDocument(LPCTSTR lpszPathName) 
    {
    	if (!CDocument::OnOpenDocument(lpszPathName))
    		return FALSE;
    	    	
    	return TRUE;
    }


*/

void CWinscoreDoc::CalculateGridPositions(EClass eClass, bool bForceRecalculation)
{

	int iClass=(int)eClass;

	if( !bForceRecalculation && m_bGridPositionCalculated[iClass]==TRUE ) return;

	double dRand;
	int i, j, iSwapPos;
	CString  cTemp;

	m_caGridPositions[iClass].RemoveAll( );
	m_contestantList.GetContestants( eClass, m_caGridPositions[iClass],true);

	int nEntries=m_caGridPositions[iClass].GetSize();

  /* Seed the random-number generator with current time so that
    * the numbers will be different every time we run.
    */

   srand( (unsigned)time( NULL ) );

	for( i=0; i<nEntries; i++ )
		{
		for( j=0; j<100; j++ )
			{
			dRand=(double)rand()/(double)RAND_MAX;
			iSwapPos=int(((double)nEntries)*dRand);
			iSwapPos=min( iSwapPos, nEntries-1);
			iSwapPos=max( iSwapPos, 0);
			cTemp=m_caGridPositions[iClass].GetAt(iSwapPos);
			m_caGridPositions[iClass].SetAt(iSwapPos, m_caGridPositions[iClass].GetAt(i));
			m_caGridPositions[iClass].SetAt(i,cTemp);
			}
		}
	m_bGridPositionCalculated[iClass]=TRUE;
}


CTurnpoint* CWinscoreDoc::GetTurnpointByID(int iID)
{
	return (CTurnpoint*)m_turnpointArray.GetAt(iID-1);
}


void CWinscoreDoc::LogPassed(CString strIGCFileName)
	{
	if( !PassedSecurity(strIGCFileName) )
		m_straPassedLogs.Add(strIGCFileName);
	}

void CWinscoreDoc::LogFailed(CString strIGCFileName)
	{
	strIGCFileName.MakeLower();

	for(int i=0; i<m_straPassedLogs.GetSize(); i++ )
		{
		CString strTemp=m_straPassedLogs[i];
		strTemp.MakeLower();
		if( strTemp==strIGCFileName ) 
			{
			m_straPassedLogs.RemoveAt(i);
			break;
			}
		}

	strIGCFileName.Replace(_T(".igc"),_T(".failed"));
	m_straPassedLogs.Add(strIGCFileName);
	}

bool CWinscoreDoc::PassedSecurity(CString strIGCFileName)
{
	for(int i=0; i<m_straPassedLogs.GetSize(); i++ )
		{
		strIGCFileName.MakeLower();
		CString strTemp=m_straPassedLogs[i];
		strTemp.MakeLower();
		if( strTemp==strIGCFileName ) return true;
		}
	return false;
}

bool CWinscoreDoc::FailedSecurity(CString strIGCFileName)
{
	strIGCFileName.MakeLower();
	strIGCFileName.Replace(_T(".igc"),_T(".failed"));
		
	for(int i=0; i<m_straPassedLogs.GetSize(); i++ )
		{
		CString strTemp=m_straPassedLogs[i];
		strTemp.MakeLower();


		if( strTemp==strIGCFileName ) return true;
		}
	return false;
}

int CWinscoreDoc::IsPracticeDay(CTime cDate)
{
	for( int i=0; i< m_iNumPracticeDays; i++)
		if( DatesEqual(cDate,m_caPracticeDays[i]) ) return	i+1;
   
	return 0;
}

CString CWinscoreDoc::GetFTPContestName()
	{
	CString strTemp;
	strTemp=m_strContestName;

	strTemp.Replace(_T("/"),_T("_"));
	strTemp.Replace(_T("\\"),_T("_"));
	strTemp.Replace(_T(" "),_T("_"));
	strTemp.TrimLeft();
	strTemp.TrimRight();
	return strTemp;
	}

BOOL CWinscoreDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	CFileException fe;

	CString strTest=lpszPathName;
	strTest.MakeLower();
	if( strTest.Right(5)==_T(".wscx") || strTest.Right(4)==_T(".xml"))
		{

		try
			{
			CXMLMgr cMgr;
			cMgr.Initlize();

			TESTHR(cMgr.Load(lpszPathName));

			CString strRoot=cMgr.GetNodeName(cMgr.GetRoot());
			if(strRoot!=_T("Winscore") )
				{
				AfxMessageBox( _T("Not a valid Winscore XML file."),  MB_ICONSTOP   );
				return FALSE;
				}

			CWaitCursor wait;
			DeleteContents();
			SetModifiedFlag();

			ImportXML(cMgr, cMgr.GetRoot());
			}
		catch (...)
			{
			ReportSaveLoadException(lpszPathName, &fe,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
			return FALSE;
			}

		}
	else
		{
		AfxMessageBox( _T("Not a valid Winscore file."),  MB_ICONSTOP   );
		return FALSE;
	}

	SetModifiedFlag(FALSE);     // start off with unmodified

	//View the contestants. 
    POSITION pos=GetFirstViewPosition();
    CWinscoreView* pView = (CWinscoreView*)GetNextView(pos);
	if( pView )
		{
		pView->InitialUpdate();
		pView->ViewContestantList(); 
		}

	CString strPathName(lpszPathName);
	int iPos=strPathName.ReverseFind('\\');
	SetWSCPath(strPathName.Left(iPos));

    cLastSavedTime=CTime::GetCurrentTime();

	return TRUE;
}


BOOL CWinscoreDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CFileException fe;
	CFile* pFile = NULL;
	
	CString strPath=lpszPathName;
	if(strPath.GetLength()==0) strPath=m_strPathName;

	pFile = GetFile(strPath, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe);

	if (pFile == NULL)
	{
		ReportSaveLoadException(strPath, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	CString strTest=strPath;
	strTest.MakeLower();
	if( strTest.Right(4)==_T(".wsc") )
		{// Save in .wsc format
		AfxMessageBox("Saving in .wsc format is no longer supported.  You must Save_As... in .wscx format.", MB_ICONSTOP);
		return FALSE;
		}
	else
		{
		ReleaseFile(pFile, FALSE);
		TRY
			{
			CXMLMgr cMgr; 
			cMgr.Initlize();
			cMgr.CreateXMLFileHeader();
			cMgr.CreateRoot(_T("Winscore"));

			CWaitCursor cWait;
			MSXML2::IXMLDOMNodePtr pRoot=cMgr.GetRoot();
			GetXML(cMgr, pRoot);

			TESTHR(cMgr.Save(strPath) );

			}
		CATCH_ALL(e)
			{
				ReportSaveLoadException(strPath, e,
					TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
			}
		END_CATCH_ALL

		}

	SetModifiedFlag(FALSE);     // back to unmodified

	CString strPathName(strPath);
	int iPos=strPathName.ReverseFind('\\');
	SetWSCPath(strPathName.Left(iPos));

    cLastSavedTime=CTime::GetCurrentTime();

	return TRUE;        // success
}




bool CWinscoreDoc::SaveReminder()
	{
	if( !IsModified() ) return false;
	if( cLastSavedTime.GetTime()==0 ) return false;

	EAutoSaveOption eOption	=(EAutoSaveOption)AfxGetApp()->GetProfileInt(REGSECTION, AUTOSAVEOPTION,	eRemind );
	if( eOption==eNone ) 
		{
		return false;
		}
	else if( eOption==eRemind ) 
		{

		int iRemindHours		=AfxGetApp()->GetProfileInt(REGSECTION, AUTOSAVEREMINDHOURS,  SAVE_REMIND_HOURS );
	
		CTime cNow=CTime::GetCurrentTime();

		CTimeSpan cTimeSinceLastSave=cNow-cLastSavedTime;
		int iHours=cTimeSinceLastSave.GetHours();
		if( iHours>=iRemindHours ) 
			{
			CString strSave;
			strSave.Format("It has been over %d hours since you saved your work.  Save now?", iHours);
			int iAnswer=AfxMessageBox( strSave, MB_YESNO  );
			if( iAnswer==IDYES )
				{
				DoFileSave();
				return true;
				}
			else
				{
				// Reset so no reminders
				cLastSavedTime=CTime::GetCurrentTime();
				return true;
				}
			}
		else
			return false;
		}
	else if( eOption==eAutoSave && m_strPathName.GetLength()>0)
		{
		CWaitCursor cWait;
		int iAutoSaveHours		=AfxGetApp()->GetProfileInt(REGSECTION, AUTOSAVESAVEHOURS,  SAVE_AUTOSAVE_HOURS );
		CTime cNow=CTime::GetCurrentTime();

		CTimeSpan cTimeSinceLastSave=cNow-cLastSavedTime;
		int iHours=cTimeSinceLastSave.GetHours();
		if( iHours>=iAutoSaveHours )
			{
			DoFileSave();
			return false;
			}

		}
	return false;
	}

BOOL CWinscoreDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
	// Save the document data to a file
	// lpszPathName = path name where to save document file
	// if lpszPathName is NULL then the user will be prompted (SaveAs)
	// note: lpszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CSingleLock cLock(&m_cSem_DoSave);
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T("#%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			  !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');

				int nSemi;                       //added
				if(nSemi = strExt.Find(';'))    //added
			      strExt = strExt.Left(nSemi);     //added

				newName += strExt;
			}
		}

		if (!AfxGetApp()->DoPromptFileName(newName,
		  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				e->Delete(); 
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE;        // success
}


bool CWinscoreDoc::ExportDayXML( CString &strFile, int nClasses, EClass aeClasses[], CTime cDate, CString &strFail, bool bCompute)
	{
	HRESULT hr;

	CoInitialize(NULL);// Extra call possibly, but lets do it to be sure it was done in this thread....

	CSingleLock cLock(&m_cSem_ExportDayXML);
	try
		{
		strFail="Exception while exporting scoresheet header";
		CXMLMgr cMgr;
		hr = cMgr.Initlize();
		if( FAILED(hr)  ) 
			{
		    strFail="cMgr.Initlize() returned failure code;";
			return false;
			}

		strFail="Exception while calling: CreateXMLFileHeader";
		cMgr.CreateXMLFileHeader();

		strFail="Exception while calling: CreateProcInstruction";
		cMgr.CreateProcInstruction(_T("xml-stylesheet"),_T("type=\"text/xsl\" href=\"ws_scoresheet.xsl\""));

		strFail="Exception while calling: CreateRoot";
		cMgr.CreateRoot(_T("SSA_Scoresheet"));

		CTime cToday = CTime::GetCurrentTime();
		CString strDate=cToday.Format(_T("%A %B %d, %Y - %H:%M"));
		strFail="Exception at call: CreateElement( cMgr.GetRoot(), _T(CreationTime),  LPCSTR(strDate) );";
		cMgr.CreateElement( cMgr.GetRoot(), _T("CreationTime"),  LPCSTR(strDate) );

		strFail="Exception at call: CreateElement( cMgr.GetRoot(), _T(ProgramVersion), SHORTVERSION )";
		cMgr.CreateElement( cMgr.GetRoot(), _T("ProgramVersion"), SHORTVERSION );
		
		strFail="Exception at call: cMgr.GetRoot(), _T(Title),		LPCSTR(m_strContestName) ";
		cMgr.CreateElement( cMgr.GetRoot(), _T("Title"),		LPCSTR(m_strContestName) );

		strFail="Exception at call: CreateElement( cMgr.GetRoot(), _T(Location),	LPCSTR(m_strLocation) )";
		cMgr.CreateElement( cMgr.GetRoot(), _T("Location"),	LPCSTR(m_strLocation) );

		strFail="Exception at call: GetXMLDateFull(cMgr, cMgr.GetRoot(), _T(Date), cDate);";
		GetXMLDateFull(cMgr, cMgr.GetRoot(), _T("Date"), cDate);

		strFail="Exception at call: CreateChild(cMgr.GetRoot(),pUnitsNode,_T(Units)";
		MSXML2::IXMLDOMNodePtr pUnitsNode;
		cMgr.CreateChild(cMgr.GetRoot(),pUnitsNode,_T("Units"));

		strFail="Exception at call: CreateElement( pUnitsNode, _T(Distance),  UnitsText(m_eUnits) ";
		cMgr.CreateElement( pUnitsNode, _T("Distance"),  UnitsText(m_eUnits) );

		strFail="Exception at call: CreateElement( pUnitsNode, _T(Altitude),  _T(feet)";
		cMgr.CreateElement( pUnitsNode, _T("Altitude"),  _T("feet") );

		strFail="Exception at call: CreateElementInt( cMgr.GetRoot(), _T(SSAID),	m_iSSA_ID )";
	    cMgr.CreateElementInt( cMgr.GetRoot(), _T("SSAID"),	m_iSSA_ID );

		strFail="Exception while exporting turnpoints.";
		m_turnpointArray.GetXML(cMgr, cMgr.GetRoot() );

		for( int iClass=0; iClass<nClasses; iClass++)
			{
			strFail="Exception while exporting class.";
    		CString cScore, cPenality, cCodes;
    		int nCodes=0;
    		CStringArray  cCodeArray, cReasonArray; 


			MSXML2::IXMLDOMNodePtr pClassNode;
			cMgr.CreateChild(cMgr.GetRoot(), pClassNode, _T("Class"));

			CWSClass	&cClass=GetClass(aeClasses[iClass]);
			cMgr.CreateElement( pClassNode, _T("Name"),	LPCSTR(cClass.GetName(true)) );

			CTask* pcTask=m_taskList.GetByDateClass(cDate, aeClasses[iClass]);
    		if( pcTask==NULL || pcTask->m_eStatus==eNoContest )
    			{
				cMgr.CreateElement( pClassNode, _T("Status"),	_T("No Contest Day") );
				MSXML2::IXMLDOMNodePtr pIDOMChildNode;
				cMgr.CreateChild(pClassNode, pIDOMChildNode, _T("Task") );
				GetXMLDateFull(cMgr, pIDOMChildNode, _T("Date"), cDate);
				cMgr.CreateElement( pIDOMChildNode, _T("Status"),   _T("No Contest Day"));
				continue;
    			}
    		CString		cStatus;
    		if( IsPracticeDay(cDate)>0  )
    			cStatus=_T("Practice Day");
    		else
    			cStatus.Format(_T("Contest Day %d"), GetContestDay(cDate, aeClasses[iClass]) );
			cMgr.CreateElement( pClassNode, _T("Status"), cStatus);
			bool bHandicapped=cClass.IsHandicapped();
			cMgr.CreateElement( pClassNode, _T("Handicapped"),	bHandicapped?_T("Yes"):_T("No") );


		    strFail="Exception while processing GetSSAXML.";
			pcTask->GetSSAXML(cMgr, pClassNode, m_eUnits, m_turnpointArray);


			// Now output the scores for each contestant
			strFail="Exception while processing ScoreRecords.";
			CScoreRecordList  cDayRecordList;
			CWinscoreListCtrl  cDum;
			CSummary cSummary;
			cDum.m_fReport=TRUE;
			if( bCompute ) CalculateScores( cDate, aeClasses[iClass],  cDum, cSummary, &cMgr, pClassNode );
    		m_ScoreRecordList.CopySort( cDate, 
    									aeClasses[iClass], 
    									cDayRecordList,
    									eCumulative);

   			if( cDayRecordList.IsEmpty() ) continue ;

			strFail="Exception while getting day summary";

	        cSummary.SetTaskDistance(pcTask->GetDistance(m_turnpointArray, SYSTEMUNITS));
			cSummary.GetXML(cMgr, pClassNode, pcTask, m_eUnits,  GetClass(aeClasses[iClass]).IsHandicapped() );

			MSXML2::IXMLDOMNodePtr pScoreRecords;
			cMgr.CreateChild(pClassNode, pScoreRecords,_T("ScoreRecords"));


			POSITION thispos, pos = cDayRecordList.GetHeadPosition();
			while( pos!=NULL)
				{
				thispos=pos;
				CScoreRecord* pcScoreRecord=cDayRecordList.GetNext(pos,cDate,aeClasses[iClass]);
				if( !pcScoreRecord ) continue;

				CContestant* pcContestant=m_contestantList.GetByContestNo(pcScoreRecord->m_cContestNo);
				if( !pcContestant ) continue;
							
				strFail.Format("Exception while processing pilot: %s.", LPCSTR(pcScoreRecord->m_cContestNo));

				MSXML2::IXMLDOMNodePtr pScoreRecord;
				cMgr.CreateChild(pScoreRecords, pScoreRecord, _T("ScoreRecord"));

				cMgr.CreateElementInt(	pScoreRecord, _T("SSANumber"),		pcContestant->m_iSSANumber);

				cMgr.CreateElement(	pScoreRecord, _T("CID"),			LPCSTR(pcScoreRecord->m_cContestNo) );
				cMgr.CreateElement(	pScoreRecord, _T("FirstName"),		LPCSTR(pcContestant->m_strFirstName) );
				cMgr.CreateElement(	pScoreRecord, _T("LastName"),		LPCSTR(pcContestant->m_strLastName) );
				cMgr.CreateElement(	pScoreRecord, _T("Glider"),			LPCSTR(pcContestant->m_strGlider) );

				cMgr.CreateElementInt(	pScoreRecord, _T("DayRank"),		pcScoreRecord->m_iDayRank);
				cMgr.CreateElement(	pScoreRecord, _T("CumRank"),			pcScoreRecord->m_cCumRankText);


				cMgr.CreateElementInt(	pScoreRecord, _T("Points"),			(int)pcScoreRecord->m_dPoints);
				cMgr.CreateElementInt(	pScoreRecord, _T("CPoints"),		(int)pcScoreRecord->m_dCumPoints);

				cMgr.CreateElement(	pScoreRecord, _T("TOC"),			pcScoreRecord->TOCText() );

				if( pcScoreRecord->m_fContestant )
					{
					cMgr.CreateElement( pScoreRecord, _T("Distance"), pcScoreRecord->DistanceText(m_eUnits) );
					if( bHandicapped ) 
							{
							cMgr.CreateElement( pScoreRecord, _T("HDistance"), pcScoreRecord->HdcapDistanceText(m_eUnits) );
							}

					if( pcScoreRecord->m_fFinisher )
						{

						cMgr.CreateElement( pScoreRecord, _T("Speed"), pcScoreRecord->SpeedText(m_eUnits) );
						if( bHandicapped )
							{
							cMgr.CreateElement( pScoreRecord, _T("HSpeed"), pcScoreRecord->HdcapSpeedText(m_eUnits) );
							}
						}
					}

			strFail="Exception while processing penalties";
	   		FormatPenalityLine(	aeClasses[iClass],
									pcScoreRecord,
    								cCodes,
    								cPenality,
    								nCodes,
    								cCodeArray,
    								cReasonArray);
			cMgr.CreateElement(	pScoreRecord, _T("Codes"),		LPCSTR(cCodes) );
			cMgr.CreateElement(	pScoreRecord, _T("Penalty"),	LPCSTR(cPenality) );
					
				
			strFail="Exception while processing flight data";
			CFlight* pcFlight=m_FlightList.Get(cDate, pcScoreRecord->m_cContestNo);
			if( pcFlight )
				{
				cMgr.CreateElement(	pScoreRecord, _T("StartTime"),	LPCSTR(pcFlight->GetStartTimeText()) );
				cMgr.CreateElement(	pScoreRecord, _T("FinishTime"),	LPCSTR(pcFlight->GetFinishTimeText()) );

				cMgr.CreateElement(	pScoreRecord, _T("AcheivedTurnpoints"),	LPCSTR(pcFlight->AcheivedTurnpointText()) );
				cMgr.CreateElement(	pScoreRecord, _T("IGCFileName"),	LPCSTR(pcFlight->m_strFileName) );
				}
			} //while( pos!=NULL)

    	if( nCodes>0 )
    		{
			strFail="Exception while processing penalty codes";
			MSXML2::IXMLDOMNodePtr pCodes;
			cMgr.CreateChild(pClassNode,pCodes,_T("Codes"));

    		for( int i=0; i<nCodes; i++)
    				{
    				BOOL bDupulicate=FALSE;
    				if( i>0 )
    					{
    					for( int j=0; j<i; j++ ) 
    						if( cCodeArray[j]==cCodeArray[i] ) bDupulicate=TRUE;
    					}
    				if( bDupulicate ) continue;
    
					MSXML2::IXMLDOMNodePtr pCode;
					cMgr.CreateChild(pCodes,pCode,_T("Code"));
					cMgr.CreateElement(	pCode, _T("ID"),	LPCSTR(cCodeArray[i]) );
					cMgr.CreateElement(	pCode, _T("Reason"),LPCSTR(cReasonArray[i]) );
    				}
    	} // endif nCodes>0
	}  // End for each class
		strFail.Format("Exception while writing to file: %s ",strFile);
		TESTHR(cMgr.Save(strFile) );
		strFail="";
		}
	catch(...)
		{
		return false;
		}
	return true;
	}


bool CWinscoreDoc::ExportCumulativeXML( CString &strFile, int nClasses, EClass aeClasses[] )  
  	{
	CXMLMgr cMgr;
	cMgr.Initlize();
	cMgr.CreateXMLFileHeader();
	cMgr.CreateProcInstruction(_T("xml-stylesheet"),_T("type=\"text/xsl\" href=\"ws_summary.xsl\""));
	cMgr.CreateRoot(_T("SSA_Summary"));

    CTime cToday = CTime::GetCurrentTime();
    CString strDate=cToday.Format(_T("%A %B %d, %Y - %H:%M"));
	cMgr.CreateElement( cMgr.GetRoot(), _T("CreationTime"),  LPCSTR(strDate) );
	cMgr.CreateElement( cMgr.GetRoot(), _T("ProgramVersion"), SHORTVERSION );

	cMgr.CreateElement( cMgr.GetRoot(), _T("Title"),		LPCSTR(m_strContestName) );
	cMgr.CreateElement( cMgr.GetRoot(), _T("Location"),	LPCSTR(m_strLocation) );

	MSXML2::IXMLDOMNodePtr pUnitsNode;
	cMgr.CreateChild(cMgr.GetRoot(),pUnitsNode,_T("Units"));

	cMgr.CreateElement( pUnitsNode, _T("Distance"),  UnitsText(m_eUnits) );
	cMgr.CreateElement( pUnitsNode, _T("Altitude"),  _T("feet") );

	MSXML2::IXMLDOMNodePtr pClassesNode;
	cMgr.CreateChild(cMgr.GetRoot(), pClassesNode, _T("Classes"));

	for( int iClass=0; iClass<nClasses; iClass++)
		{

		MSXML2::IXMLDOMNodePtr pClassNode;
		cMgr.CreateChild(pClassesNode, pClassNode, _T("Class"));

		CWSClass	&cClass=GetClass(aeClasses[iClass]);
		cMgr.CreateElement( pClassNode, _T("Name"),	LPCSTR(cClass.GetName(true)) );

    	CString cDayDate, cLine, cScore, cString;
    	int iDay;
    	CScoreRecordList cScoreRecordList;
    	CTime cLastContestDay;
    	double	dPrevScore=0.0;
    	int		iPrevStanding=(-1);
       
    	for( int i=0; i<m_iNumContestDays; i++ )
    		{
    		if( !NoContestDay(m_caContestDays[i], aeClasses[iClass]) ) cLastContestDay=m_caContestDays[i];
    		} 
 
		CTask* pcTask=m_taskList.GetByDateClass(cLastContestDay, aeClasses[iClass]);
		if( pcTask )
			if( pcTask->m_eStatus!=eOfficial) 
			{
			CString strErr;
			strErr.Format(_T("%s - Note, the last contest day must be made official before creating a contest summary."),cClass.GetName());
			AfxMessageBox(strErr);
			return false;
			}	

    	//  Get the records for the last contest day sorted by cum winners,
    	//  This is the order we want
    
    	int nRecords=m_ScoreRecordList.CopySort(cLastContestDay, 
    											aeClasses[iClass], 
    											cScoreRecordList,
    											eCumulative);      

		MSXML2::IXMLDOMNodePtr pTasksNode;
		cMgr.CreateChild(pClassNode, pTasksNode, _T("Tasks"));

    	for( iDay=0; iDay<m_iNumContestDays; iDay++ )
			{
   			CTask* pcTask=m_taskList.GetByDateClass(m_caContestDays[iDay], aeClasses[iClass]);
			if( pcTask && pcTask->m_eStatus!=eNoContest) 
				{
				pcTask->GetSSAXML(cMgr, pTasksNode, m_eUnits, m_turnpointArray, true);
				}
			}
				
		MSXML2::IXMLDOMNodePtr pScoreRecordsNode;
		cMgr.CreateChild(pClassNode, pScoreRecordsNode, _T("ScoreRecords"));
     
    	int  iRec=0;
    	if( nRecords>0 )
    		{
    		POSITION pos = cScoreRecordList.GetHeadPosition();
    		while( pos!=NULL )
    			{
    			CScoreRecord* pcScoreRecord=(CScoreRecord*)cScoreRecordList.CObList::GetNext(pos);
				if( !pcScoreRecord ) continue;
     			CContestant* pcContestant=m_contestantList.GetByContestNo( pcScoreRecord->m_cContestNo);
				if( !pcContestant ) continue;
    			
				MSXML2::IXMLDOMNodePtr pScoreRecordNode;
				cMgr.CreateChild(pScoreRecordsNode, pScoreRecordNode, _T("ScoreRecord"));

				// Write the contestant info
				pcContestant->GetSSAXML(cMgr, pScoreRecordNode);

				cMgr.CreateElement(		pScoreRecordNode,	_T("CumulativeRank"),	pcScoreRecord->m_cCumRankText );
				cMgr.CreateElementInt(	pScoreRecordNode,	_T("CumulativePoints"), (int)max(pcScoreRecord->m_dCumPoints,0) );
   
				int iContestPenalties=m_PenalityList.GetContestTotal(	pcScoreRecord->m_cContestNo );
				if( iContestPenalties>0 ) 
						cMgr.CreateElementInt(	pScoreRecordNode,	_T("Adjustment"),	-iContestPenalties);

				MSXML2::IXMLDOMNodePtr pDaysNode;
				cMgr.CreateChild(pScoreRecordNode, pDaysNode, _T("Days"));

				for( iDay=0; iDay<m_iNumContestDays; iDay++ )
    				{
					if( NoContestDay( m_caContestDays[iDay], aeClasses[iClass]) ) continue;

					MSXML2::IXMLDOMNodePtr pDayNode;
					cMgr.CreateChild(pDaysNode, pDayNode, _T("Day"));
					int iRank=0;
					int iPoints=0;

    				CScoreRecordList cDayRecordList;
    				m_ScoreRecordList.CopySort( m_caContestDays[iDay], 
    											aeClasses[iClass], 
    											cDayRecordList,
    											eDaily);
    
    				CScoreRecord* pcRec=cDayRecordList.Get( pcScoreRecord->m_cContestNo, m_caContestDays[iDay] );
    				if( pcRec )
    					{
						iRank=pcRec->m_iDayRank;
						iPoints=(int)pcRec->m_dPoints;;
    					}
					
					GetXMLDate(cMgr, pDayNode, _T("Date"), m_caContestDays[iDay]);
 					cMgr.CreateElementInt(	pDayNode,	_T("Rank"),		iRank ); 
					cMgr.CreateElementInt(	pDayNode,	_T("Points"),	iPoints);
  					}
  				}
  			}
		if( nRecords==0 ) 
			{
			CString strErr;
			strErr.Format(_T("%s - No valid cumulative scores were found for the last contest day."),cClass.GetName());
			AfxMessageBox(strErr);
			return false;
			}
  		}

  	TESTHR(cMgr.Save(strFile) );

  	return true;
  }

void CWinscoreDoc::AutoExportXML(CTime cDate)
	{

	int iFlag=AfxGetApp()->GetProfileInt(REGSECTION, AUTOEXPORTENABLEDFLAG,	0 );
	if( iFlag==0 ) return;

	CString cPathText=AfxGetApp()->GetProfileString( REGSECTION, AUTOEXPORTPATH, _T("") );


	CString cDateString= cDate.Format(_T("Scoresheet%b%d_%y"));
	CString cFileName=cPathText+_T("\\");
	cFileName+=cDateString;
	cFileName+=_T(".xml");


	int nClasses=0;
	EClass aeClasses[NUMCLASSES];

	for( int iClass=0; iClass<NUMCLASSES; iClass++)
		{
		if( GetClass(iClass).IsActive() )
			{
			aeClasses[nClasses++]=GetClass(iClass).GetType();
			}
		}
	CString strFail;
	bool bret=ExportDayXML( cFileName, nClasses, aeClasses, cDate, strFail, false );
	if(!bret )
		{
		AfxMessageBox(_T("Error writing file: ") + cFileName+_T(",\n Disabling AutoExport"), MB_ICONSTOP);
		AfxGetApp()->WriteProfileInt(REGSECTION, AUTOEXPORTENABLEDFLAG,	0 );
		return;
		}

	}


int CWinscoreDoc::GetMultipleStartPoints(	CFlight *pcFlight, 
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
											int iaPoints[]  )
	{
	CSummary cSummary;
	CWinscoreListCtrl	cDum;

	iBestScorePtr= -1;
	int iCurrentStart= 0;

	int nStarts=pcFlight->GetCandidateStarts( iMaxReturn, caStartTimes, caTOCs, iaPenalties, pcTask);
	if( nStarts==0 ) return 0;

	CWSClass &cClass	=GetClass(pcTask->m_eClass);
	cDum.m_fReport=FALSE;
	CalculateScores( caStartTimes[0], pcTask->m_eClass, cDum, cSummary );

	m_ScoreRecordList.GetSummary(	caStartTimes[0],  
									pcTask->m_eClass,
    								pcTask,
    								50,//dummy
									cSummary );
	cSummary.SetTaskDistance(pcTask->GetDistance(m_turnpointArray, SYSTEMUNITS));

	// Create a copy of the flight so we can play with the start times and compute new distances
	CFlight	cFlight(pcFlight);
	cFlight.ReadFlight();

	CScoreRecord* pcOriginalScoreRecord	=m_ScoreRecordList.Get(pcFlight->m_strCID,caStartTimes[0]);
	if( !pcOriginalScoreRecord ) return 0;
	double dIntialPoints	=pcOriginalScoreRecord->m_dPoints;
	CTime  cInitialStartTime=pcOriginalScoreRecord->m_cStartTime;

	//Create a score record list with a copy of this pilot's record in it.
	CScoreRecord*		pcScoreRecord	= new CScoreRecord(pcOriginalScoreRecord);
	CScoreRecordList	cScoreList;
	cScoreList.AddTail(pcScoreRecord);

	int iBest= -1;
	for( int iStart=0; iStart<nStarts; iStart++ )
		{
		if( cInitialStartTime==caStartTimes[iStart]) iCurrentStart=iStart;

		cFlight.SetStartTime(caStartTimes[iStart]);
		cFlight.FindStartFix();
		
		// Put the updated start times and distances on the score record, then run it through the speed and point processors.
		pcScoreRecord->m_dDistance=cFlight.ComputeDistance(pcTask, m_turnpointArray);
		if( cClass.IsHandicapped() )
			{
			pcScoreRecord->m_dHandicappedDistance=pcScoreRecord->m_dHandicap*pcScoreRecord->m_dDistance;
			daDistances[iStart]=pcScoreRecord->m_dHandicappedDistance;
			}
		else
			daDistances[iStart]=pcScoreRecord->m_dDistance;

		pcScoreRecord->m_cStartTime=caStartTimes[iStart];
		cScoreList.CalculateSpeeds(	pcTask, 
    								pcTask->m_cDate, 
    								cClass,
									0,
    								cDum,
									m_turnpointArray);
		cScoreList.CalculatePoints(pcTask->m_cDate, cClass, pcTask,cSummary);

		caTOCs[iStart]=pcScoreRecord->m_cSTOC;

		if( pcFlight->IsTaskComplete() )
			{
			if( cClass.IsHandicapped() )
				daSpeeds[iStart]=pcScoreRecord->m_dHandicappedSpeed;
			else
				daSpeeds[iStart]=pcScoreRecord->m_dSpeed;
			}
		else
			{
			if( cClass.IsHandicapped() )
				daSpeeds[iStart]=pcScoreRecord->m_dHandicappedDistance;
			else
				daSpeeds[iStart]=pcScoreRecord->m_dDistance;
			}

		iaPointDiff[iStart]=(int)(pcScoreRecord->m_dPoints-dIntialPoints);

		iaPoints[iStart]=(int)pcScoreRecord->m_dPoints-iaPenalties[iStart];
		if( iaPoints[iStart]>iBest )
			{
			iBest=iaPoints[iStart];
			iBestScorePtr=iStart;
			}
		}

	bAlreadyBest=iCurrentStart==iBestScorePtr;

	for( int iStart=0; iStart<nStarts; iStart++ )
		{
		iaPointDiff[iStart]=iaPoints[iStart]-iaPoints[iCurrentStart];
		}


	return nStarts;
	}


double CWinscoreDoc::GetLogTurninRatio(CTime cDate, EClass eClass)
	{
	int nContestantsInClass	=m_contestantList.GetCountClass(eClass); 
	int nFlightsInClassDay	=m_FlightList.GetClassDateCount(cDate, eClass);

	double dNumer=(double)nFlightsInClassDay;
	double dDenom=(double)nContestantsInClass;
	return  dNumer/dDenom;
	}


CString CWinscoreDoc::GetWSCPath(void)
	{
	int iPos=m_strPathName.ReverseFind('\\');
	if( iPos<0 ) return "";
	return m_strPathName.Left(iPos);
	}
