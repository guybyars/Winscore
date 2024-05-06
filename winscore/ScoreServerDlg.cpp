// ScoreServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscore.h"
#include "ScoreServerDlg.h"
#include "ScoreReportDlg.h"
#include "FlightLogTransferSettings.h"
#include "ZipArchive.h"
#include <Atlbase.h>
#include "Global_Prototypes.h"


#include "CkHttp.h"
#include "CkHttpRequest.h"
#include "CkHttpResponse.h"
#include "SSAContestLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		DAILY		0
#define		FLIGHTLOGS	1
//#define		REPORTONLY	2
#define		SUMMARY		2

static CString m_strComment;

static void mytest();
static void mylogin(void);

/////////////////////////////////////////////////////////////////////////////
// CScoreServerDlg dialog


CScoreServerDlg::CScoreServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScoreServerDlg::IDD, pParent)//,
//	m_Isess(_T("Winscore"))
{
	//{{AFX_DATA_INIT(CScoreServerDlg)
	m_iType = 0;
	//}}AFX_DATA_INIT

	m_iSSA_ID=0;

}


void CScoreServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScoreServerDlg)
	DDX_Control(pDX, IDC_SSA, m_cSSACombo);
	DDX_Control(pDX, IDC_DATE_COMBO, m_cDateCombo);
//	DDX_Control(pDX, IDC_COMMENT_BUTTON, m_cCommentButton);
	DDX_Control(pDX, IDC_EXCLUDE, m_cExcludeButton);
	DDX_Control(pDX, ID_SEND, m_cSendButton);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_cStatus);
	DDX_Control(pDX, IDC_CLASS_LIST, m_cClassListCtrl);
	DDX_Radio(pDX, IDC_RADIO, m_iType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScoreServerDlg, CDialog)
	//{{AFX_MSG_MAP(CScoreServerDlg)
	ON_BN_CLICKED(ID_SEND, OnSend)
	//ON_BN_CLICKED(IDC_COMMENT_BUTTON, OnCommentButton)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_BN_CLICKED(IDCANCEL, OnClose)
	ON_BN_CLICKED(IDC_EXCLUDE, OnExclude)
	ON_BN_CLICKED(IDC_RADIO, OnRadio)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
//	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////// 
// CScoreServerDlg message handlers

void CScoreServerDlg::OnSend() 
{
	m_iSSA_ID=0;
	CString strError;
	CString strFail;

	int iIndex=m_cSSACombo.GetCurSel();
	if( iIndex>=0 ) 
		{
		m_iSSA_ID=m_cSSACombo.GetItemData(iIndex);
		}
	if( m_iSSA_ID<=0 || m_iSSA_ID>10000 ) 
		{
		AfxMessageBox (_T("Invalid SSA Contest selected."));
		return;
		}

	CSSAContestLoginDlg dlg;

	dlg.m_bRemember=GetWinscoreInt( SSACONTESTREMEMBER, 0 );
	if( dlg.m_bRemember )
		{
		dlg.m_strUsername	=GetWinscoreString( SSACONTESTUSERNAME, "" );
		//dlg.m_strPassword	=GetWinscoreStringDecrypt( SSACONTESTPASSWORD, "" );
		dlg.m_strPassword	=AfxGetApp()->GetProfileString( REGSECTION, SSACONTESTPASSWORD, "" );
		}

	if( dlg.DoModal()==IDOK ) 
		{
		if( dlg.m_strPassword.GetLength()<=1 ) 
			{
			AfxMessageBox(_T("Invalid Password, login failed."));
			return;
			}
		if( dlg.m_strUsername.GetLength()<=1 ) 
			{
			AfxMessageBox(_T("Invalid Username, login failed." ));
			return;
			}
		}
	else
		{
		AfxMessageBox(_T("Transfer Cancelled."));
		return;
		}

	WriteWinscoreInt( SSACONTESTREMEMBER, (int)dlg.m_bRemember );
	if( dlg.m_bRemember )
		{
		WriteWinscoreString( SSACONTESTUSERNAME, dlg.m_strUsername );
		//WriteWinscoreStringEncrypt( SSACONTESTPASSWORD, dlg.m_strPassword );
		AfxGetApp()->WriteProfileString( REGSECTION, SSACONTESTPASSWORD, dlg.m_strPassword );
		}


	CTime cCheckDate;
	int nScoresheetsTransfered=0;
	int nLogsTransferred=0;
	CString cNEWLINE=_T("\r\n");
	bool bSuccess=false;
	CTime tnow = CTime::GetCurrentTime();

	CWaitCursor cWait;


	int iSel=m_cDateCombo.GetCurSel();
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	UpdateData();

//	if( m_iType==REPORTONLY && m_strComment.GetLength()==0 )
//		{
//		AfxMessageBox(_T("Contest reports require text input."));
//		return;
//		}


	// Change & Create the correct destination.


	if( m_iType==DAILY || m_iType==SUMMARY )
		{

		EClass		aeClasses[NUMCLASSES];
		int nClasses=m_cClassListCtrl.GetNumChecked();

		if( nClasses==0 ) 
			{
			AfxMessageBox(_T("There are no classes selected."));
			return;
			}

		for( int iClass=0; iClass<nClasses; iClass++ )
			{
			aeClasses[iClass]=m_cClassListCtrl.GetCheckedClass(iClass);
			}

		 CString cFileName;
		 CString strPathName;

		if( m_iType==SUMMARY ) 
			 cFileName.Format(_T("Summary_%d_%s.xml"), 
							  m_iSSA_ID,
			  				   tnow.Format(_T("%d-%b-%y_%H-%M")) );
		else
			 cFileName.Format(_T("Scoresheet_%d_%s.xml"), 
							  m_iSSA_ID,
			  				   tnow.Format(_T("%d-%b-%y_%H-%M")) );

		size_t bSize=256;
		TCHAR *pTemp=new TCHAR[bSize];
		_dupenv_s(&pTemp, &bSize, _T("TEMP") );
		if( pTemp==NULL ) 
			   _dupenv_s(&pTemp, &bSize, _T("TMP") );
		strPathName=pTemp;
		strPathName+=_T("\\");
		strPathName+=cFileName;
		delete pTemp;


		bool bExported;
		if( m_iType==SUMMARY ) 
			bExported=m_pDoc->ExportCumulativeXML(strPathName, nClasses, aeClasses);
		else
			bExported=m_pDoc->ExportDayXML(strPathName, nClasses, aeClasses, *pcTime, strFail);
		if( !bExported )
			{
			SetStatusText(_T("Could not create scoresheet.  Transfer Aborted."));
			return;
			}
		
		bSuccess= uploadfile(strPathName,dlg.m_strUsername,dlg.m_strPassword,false,strError);

		DeleteFile(strPathName);
		if( !bSuccess )
			{
			SetStatusText(_T("Transfer Failed."));
			return;
			}

		CString strText;
		if( m_iType==SUMMARY ) 
			strText.Format(_T("Summary Successfully Transferred"));
		else
			strText.Format(_T("Scoresheet Successfully Transferred"));
		SetStatusText(strText);

		}
	else if( m_iType==FLIGHTLOGS )
		{

		CStringArray strArray;

//		int nDays=m_cClassListCtrl.GetNumChecked();
//		if( nDays>1 ) 
//			{
//			AfxMessageBox(_T("Can't send flight logs for more than one day at a time."));
//			return;
//			}

		for( int iClass=0; iClass<NUMCLASSES; iClass++)
			{
			if( ( GetClass(iClass).IsActive()) )
				{
				m_pDoc->m_FlightList.GetFileList( GetClass(iClass).GetType(), *pcTime, strArray,m_pDoc->m_contestantList);
   				}
			}

		// Check we actually have logs to send
		int nFlights=0;
		for( int i=0; i<strArray.GetSize(); i++)
			{		
			CFileStatus status;
			if( CFile::GetStatus( GetNonRelativePath(strArray[i]), status ) ) nFlights++;
			}

		if( nFlights==0 ) 
			{
			AfxMessageBox(_T("There are no flight logs in the system for this day."));
			return;
			}
	
		 SetStatusText(_T("Sending Flight Logs."));

		 CString strZipFileName;

		 FormatFlightLogZipName( strZipFileName, m_iSSA_ID, pcTime) ;

		 //strZipFileName.Format(_T("FlightLogs_%d_%s.zip"), 
		 //				  m_iSSA_ID,
		 //	  			   pcTime->Format(_T("%d-%b-%Y")) );


		 nLogsTransferred=SendFlightLogs(strZipFileName,dlg.m_strUsername,dlg.m_strPassword);

		CString strText1;
		strText1.Format(_T("%d flight logs transferred"),nLogsTransferred);
		SetStatusText(strText1);

		}
/*	else if( m_iType==REPORTONLY )
		{

		 CString cFileName;
		 CString strPathName;

		 for( int iTry=0; iTry<20; iTry++)
			 {
			 cFileName.Format("%d_Report_%s_v%d.txt", 
							  m_iSSA_ID,
			  				   pcTime->Format("%d-%b-%Y"), iTry);
//  			 BOOL found = finder.FindFile(cFileName);
//			 if( !found ) break;
			 }

		CString strSend;
		strSend.Format("Sending Report...");
		SetStatusText(strSend);

		bool bAbort=false;


		if( bAbort )
			{
			SetStatusText("Transfer Aborted.");
			return;
			}

		CString strText;
		strText.Format("Contest Report Successfully Transferred");
		SetStatusText(strText);
		}*/
}

void CScoreServerDlg::OnCommentButton() 
{
	CScoreReportDlg dlg;
	dlg.m_strText=m_strComment;
	if( dlg.DoModal()==IDOK )
		m_strComment=dlg.m_strText;
	
}

BOOL CScoreServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cClassListCtrl.AddColumns();

	SetStatusText(_T("Ready"));
	m_cExcludeButton.EnableWindow(false);

	OnRadio();

	m_pDoc->LoadDateComboBox(m_cDateCombo);

	m_iSSA_ID=m_pDoc->m_iSSA_ID;

#if WINSCOREBETA
	int iIndex=m_cSSACombo.AddString ("Test");
	m_cSSACombo.SetItemData(iIndex,2184);
	int nLoaded=1;

	AfxMessageBox(_T("Beta Executable only supports test transfers."));

#else
	int nLoaded=LoadContestsFromSSA( &m_cSSACombo, NULL ); 
#endif

	if( nLoaded==0 )
		{
		AfxMessageBox(_T("Unable to read contest information from ssa.org.  Make sure you are connected to the Internet."));
		}
	
	for( int i=0; i<m_cSSACombo.GetCount(); i++)
		{
		if( m_iSSA_ID==(int)m_cSSACombo.GetItemData(i) )
			{
			m_cSSACombo.SetCurSel(i);
			}

		}

	m_cSendButton.EnableWindow(nLoaded>0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CScoreServerDlg::OnTest() 
{

}

void CScoreServerDlg::SetStatusText(CString str)
	{
	m_cStatus.SetWindowText(str);
	m_cStatus.UpdateWindow();
	}

void CScoreServerDlg::OnClose() 
{
	if( m_iSSA_ID>0 && m_iSSA_ID <10000)
		m_pDoc->m_iSSA_ID=m_iSSA_ID;

	CDialog::OnCancel();
}

bool  CScoreServerDlg::SendFile(   CString strContents,
								   CString strFileName)
	{
	return false;
	}



void CScoreServerDlg::OnExclude() 
{
	CFlightLogTransferSettings dlg;	
	dlg.m_pDoc=m_pDoc;
	if( dlg.DoModal()==IDOK )
		{
		dlg.m_pDoc->SetModifiedFlag();
		}	
}

int CScoreServerDlg::SendFlightLogs(CString &strFileName, CString &strUsername, CString &strPassword )
{
	CWinscoreListCtrl cStatus;
	cStatus.m_fReport=false;
	CScoreRecordList cScoreRecordList;
	EClass		aeClasses[NUMCLASSES];
	CString     strError;

	int iSel=m_cDateCombo.GetCurSel();
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);

	int nRecords=0;

	size_t bSize=256;
	TCHAR *pTemp=new TCHAR[bSize];
    _dupenv_s(&pTemp, &bSize, _T("TEMP") );
	if( pTemp==NULL ) 
		   _dupenv_s(&pTemp, &bSize, _T("TMP") );
	CString strWorkDIR=pTemp;
	CString strZipFileName=strWorkDIR+_T("\\")+strFileName;
	delete pTemp;
	DeleteFile(strZipFileName);

	BOOL fCondense=GetWinscoreInt( FTPCONDENSE, FALSE ); 
	int iTime=GetWinscoreInt( FTPCONDENSESEC, 4 ); 
	int iTopRank=100;
	if( fCondense ) iTopRank=GetWinscoreInt( FTPTRANSFERTOP, 10 ); 

	int nClasses=0;
	int nFlights=0;

	for( int iClass=0; iClass<NUMCLASSES; iClass++)
		{
		if( ( GetClass(iClass).IsActive()) )
			{
			aeClasses[nClasses++]=GetClass(iClass).GetType();
   			}
		}
	if( nClasses==0 ) return 0;

	CWaitCursor cWait;
	int nExported=0;

	SetStatusText(_T("Compressing Flight Logs."));

	try
		{
		CZipArchive cZip;
		cZip.Open(strZipFileName,CZipArchive::create);

		for( int iClass=0; iClass<nClasses; iClass++ )
			{

			m_pDoc->CalculateScores( *pcTime, aeClasses[iClass],  cStatus);
			m_pDoc->m_ScoreRecordList.CopySort(	*pcTime, 
												aeClasses[iClass],
												cScoreRecordList, 
												eCumulative); 
			CStringArray strArrayPath;
			CStringArray strArrayName;
			m_pDoc->m_FlightList.GetFileList2(aeClasses[iClass], *pcTime, strArrayPath, strArrayName, m_pDoc->m_contestantList, cScoreRecordList, iTopRank);
			for( int iFlight=0; iFlight<strArrayPath.GetSize(); iFlight++)
				{
				CFileStatus cStat;
				CString strTmp=GetNonRelativePath(strArrayPath[iFlight]);
				if(!CFile::GetStatus( strTmp, cStat) ) 
					continue;

				CString strZipName;
				if( fCondense )
					{
					strZipName=strWorkDIR;
					strZipName+=_T("\\");
					strZipName+=strArrayName[iFlight];
					CIGCFile::Smash(GetNonRelativePath(strArrayPath[iFlight]), strZipName, iTime);
					}
				else
					strZipName=GetNonRelativePath(strArrayPath[iFlight]);


				if ( cZip.AddNewFile(strZipName,GetRelativePath(strArrayPath[iFlight],false))==false)
					{
					AfxMessageBox(_T("Error writing flight log to .zip file: ")+ strArrayPath[iFlight]);
					};

				if( fCondense ) DeleteFile(strZipName);

				nExported++;
				}
			}
		//Add the current Winscore wscx file

		CXMLMgr cMgr;
		cMgr.Initlize();
		cMgr.CreateXMLFileHeader();
		cMgr.CreateRoot(_T("Winscore"));

		MSXML2::IXMLDOMNodePtr pRoot=  cMgr.GetRoot();
		m_pDoc->GetXML( cMgr, pRoot);

		CString strWSCX;
		CString strDate=pcTime->Format(_T("%d-%b-%Y"));
		strWSCX.Format(_T("%s\\Winscore_%s.wscx"), 
						strWorkDIR,
			  			strDate );
		TESTHR(cMgr.Save(strWSCX) );

		if ( cZip.AddNewFile(strWSCX)==false)
			{
			AfxMessageBox(_T("Error writing Winscore database to .zip file: ")+ strWSCX);
			};
		DeleteFile(strWSCX);

		cZip.Close();

		CFileStatus filestatus;
        CFile::GetStatus( strZipFileName, filestatus );
		int iSize=filestatus.m_size/1024;
		iSize=iSize/1000;

		if( iSize==0 )
		  SetStatusText(_T("Sending Flight Logs."));
		else
		  {
		  CString strStatus;
		  strStatus.Format("Sending Flight Logs (%d MB).",iSize);
		  SetStatusText(strStatus);
		  }

		}
	catch (...)
		{
		AfxMessageBox(_T("Error creating flight log .zip file"));
		return 0;
		}

//    Now send the.zip to the web site


	if( !uploadfile(strZipFileName,strUsername,strPassword,false,strError) )
		{
	    DeleteFile(strZipFileName);
		SetStatusText(_T("Transfer Failed."));
		return 0;
		}
	
	DeleteFile(strZipFileName);


	return nExported;

}

void CScoreServerDlg::OnRadio() 
{
	// Daily

	m_cClassListCtrl.AddColumns();
	m_cClassListCtrl.LoadAvailableClasses();
	m_cExcludeButton.EnableWindow(false);	
	//m_cCommentButton.EnableWindow(false);
	m_cClassListCtrl.EnableWindow(true);
	m_cClassListCtrl.CheckAll();
}

void CScoreServerDlg::OnRadio1() 
{
	// Flight Logs
	m_cClassListCtrl.AddColumns();
	m_cClassListCtrl.LoadAvailableClasses();
	m_cExcludeButton.EnableWindow(true);	
	//m_cCommentButton.EnableWindow(false);
	m_cClassListCtrl.EnableWindow(false);
	m_cClassListCtrl.CheckAll();
}

void CScoreServerDlg::OnRadio2() 
{
	// Contest Report only
	m_cClassListCtrl.AddColumns();
	m_cClassListCtrl.LoadAvailableClasses();
	m_cExcludeButton.EnableWindow(false);	
	//m_cCommentButton.EnableWindow(true);
	m_cClassListCtrl.EnableWindow(false);
	m_cClassListCtrl.CheckAll();
}

void CScoreServerDlg::OnRadio3() 
{
	// Cumulative Summary
	m_cClassListCtrl.AddColumns(true);
	m_cClassListCtrl.LoadAvailableClasses();
	m_cExcludeButton.EnableWindow(false);	
	//m_cCommentButton.EnableWindow(false);
	m_cClassListCtrl.EnableWindow(false);
	m_cClassListCtrl.CheckAll();
}

