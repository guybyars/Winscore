// AutoScoreDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscore.h"
#include "AutoScoreDlg.h"
#include "afxdialogex.h"
#include "Global_Prototypes.h"
#include "WinscoreView.h"
#include "WSClass.h"
#include "SSAContestLoginDlg.h"
#include "EmailSettingsDlg.h"
#include "IGCDirectories.h"

#include <CkImap.h>
#include <CkMessageSet.h>
#include <CkEmailBundle.h>
#include <CkEmail.h>
#include <CkDateTime.h>

static 	DWORD iDrives=0;
static bool CheckFlashDrive(CString &strDrive);
UINT __cdecl AutoScoreProc( LPVOID lpParameter);
static BOOL CheckAutoScoreStop(CWinscoreView *pView);

static int nErrors=0;
static int nMAXErrors=10;
static bool bScoreSheetError=false;

// CAutoScoreDlg dialog

IMPLEMENT_DYNAMIC(CAutoScoreDlg, CResizingDialog)

CAutoScoreDlg::CAutoScoreDlg(CWnd* pParent /*=NULL*/)
	: CResizingDialog(CAutoScoreDlg::IDD, pParent)
	, m_strCountdown(_T("Ready"))
	, m_iDuration(30)
	, m_iItem(0)
{
	m_uTimerVal=0;
	m_iTimeRemaining=0;
	m_pView=NULL;
	m_pAutoScoreThread=NULL;
	m_bRunning=false;
	m_bPreContest=false;
	nErrors=0;
	m_nDays=1;

    SetControlInfo(IDSTART,			ANCHORE_BOTTOM);
    SetControlInfo(IDSTOP,			ANCHORE_BOTTOM);
    SetControlInfo(IDCLOSE,			ANCHORE_BOTTOM);
    SetControlInfo(IDCLEAR,			ANCHORE_BOTTOM);
    SetControlInfo(IDC_ANALYZE_CHECK,	RESIZE_HOR);
    SetControlInfo(IDC_EVENT_LIST	,RESIZE_HOR | RESIZE_VER);
}

CAutoScoreDlg::~CAutoScoreDlg()
{
}

BOOL CAutoScoreDlg::OnInitDialog() 
	{
	CDialog::OnInitDialog();
	CIGCDirectories cIGCDirs;

	m_iDuration=GetWinscoreInt( AUTOSCORE_DURATION,	30 );

	m_cMediaCheck.SetCheck(GetWinscoreInt(	AUTOSCORE_MEDIA,	0 ));
	m_cEmailCheck.SetCheck(GetWinscoreInt(	AUTOSCORE_EMAIL,	0 ));
	m_cAnalyzeCheck.SetCheck(GetWinscoreInt( AUTOSCORE_ANALYSIS,	1 ));
	m_cSendScoreslCheck.SetCheck(GetWinscoreInt( AUTOSCORE_SCORES,	1 ));
	m_cAutoSaveCheck.SetCheck(GetWinscoreInt( AUTOSCORE_AUTOSAVE,	1 ));

	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt =LVCFMT_LEFT;

	lvc.iSubItem = 0;
	lvc.pszText = _T("Time");
	lvc.cx = 70;
	m_cListCtrl.InsertColumn(0,&lvc);

	lvc.iSubItem = 0;
	lvc.pszText = _T("Activity");
	lvc.cx = 700;
	m_cListCtrl.InsertColumn(1,&lvc);

    m_cStopButton.EnableWindow(false);
	UpdateData(FALSE);

	CString strText;
	strText+="Analyze and score new logs found in:  ";
	strText+=cIGCDirs.GetFlightLogPath(m_cDate);
	m_cAnalyzeCheck.SetWindowText(strText);

	m_pDocument->LoadDateComboBox(m_cDateCombo,true);

	int nLoaded=LoadContestsFromSSA( &m_cSSACombo, NULL ); 
	if( nLoaded>0 )
		{
		for( int i=0; i<m_cSSACombo.GetCount(); i++)
			{
			if( m_iSSA_ID==(int)m_cSSACombo.GetItemData(i) )
				{
				m_cSSACombo.SetCurSel(i);
				break;
				}
			}
		}


	if( m_strStatusArrayTime.GetCount() >0 )
		{
	    int iItemID;
		for (int iLine=0; iLine<m_strStatusArrayTime.GetCount(); iLine++ )
			{
			// New item
			LV_ITEM lvi;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_PARAM;
			lvi.iItem = m_iItem++;
			lvi.lParam = NULL;
			iItemID=m_cListCtrl.InsertItem(&lvi);
			m_cListCtrl.SetItemText(iItemID,0,m_strStatusArrayTime.GetAt(iLine));
			m_cListCtrl.SetItemText(iItemID,1,m_strStatusArrayData.GetAt(iLine));
			}
		m_cListCtrl.EnsureVisible( iItemID, FALSE);
		}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

void CAutoScoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizingDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEXT_EVENT_STATIC, m_strCountdown);
	DDX_Text(pDX, IDC_NUM_SECONDS_EDIT, m_iDuration);
	DDV_MinMaxInt(pDX, m_iDuration, 10, 1000);
	DDX_Control(pDX, IDC_EMAIL_CHECK, m_cEmailCheck);
	DDX_Control(pDX, IDC_ANALYZE_CHECK, m_cAnalyzeCheck);
	DDX_Control(pDX, IDC_SEND_SCORES_CHECK, m_cSendScoreslCheck);
	DDX_Control(pDX, IDC_EVENT_LIST, m_cListCtrl);
	DDX_Control(pDX, IDSTART, m_cStartButton);
	DDX_Control(pDX, IDSTOP, m_cStopButton);
	DDX_Control(pDX, IDC_NUM_SECONDS_EDIT, m_cNumSecondsEdit);
	DDX_Control(pDX, IDC_DATE_COMBO, m_cDateCombo);
	DDX_Control(pDX, IDC_SSA, m_cSSACombo);
	DDX_Control(pDX, IDC_EMAIL_SETTINGS_BUTTON, m_cEmailSettings);
	DDX_Control(pDX, IDC_TRANSFER_SETTINGS_BUTTON, m_cTransferSettings);
	DDX_Control(pDX, IDC_MEDIA_CHECK, m_cMediaCheck);
	DDX_Control(pDX, IDC_AUTOSAVE_CHECK, m_cAutoSaveCheck);
}


BEGIN_MESSAGE_MAP(CAutoScoreDlg, CResizingDialog)
	ON_BN_CLICKED(IDC_EMAIL_SETTINGS_BUTTON, &CAutoScoreDlg::OnBnClickedEmailSettingsButton)
	ON_BN_CLICKED(IDSTART, &CAutoScoreDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDSTOP, &CAutoScoreDlg::OnBnClickedStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCLOSE, &CAutoScoreDlg::OnClose)
	ON_BN_CLICKED(IDCLEAR, &CAutoScoreDlg::OnClear)
	ON_BN_CLICKED(IDC_TRANSFER_SETTINGS_BUTTON, &CAutoScoreDlg::OnTransferSettingsButton)
	ON_CBN_SELCHANGE(IDC_DATE_COMBO, &CAutoScoreDlg::OnCbnSelchangeDateCombo)
	ON_CBN_SELCHANGE(IDC_SSA, &CAutoScoreDlg::OnCbnSelchangeSsa)
	ON_COMMAND(IDC_CANCEL, &CAutoScoreDlg::OnCancel)
END_MESSAGE_MAP()


// CAutoScoreDlg message handlers




void CAutoScoreDlg::OnBnClickedStart()
	{
	bool bTask=false;
	if( UpdateData()==0 ) return;
	UpdateDate();
	bScoreSheetError=false;

	WriteWinscoreInt( AUTOSCORE_DURATION,	m_iDuration );
	WriteWinscoreInt( AUTOSCORE_MEDIA,		m_cMediaCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_EMAIL,		m_cEmailCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_ANALYSIS,	m_cAnalyzeCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_SCORES,		m_cSendScoreslCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_AUTOSAVE,	m_cAutoSaveCheck.GetCheck() );

	iDrives=GetLogicalDrives();

	if( m_cAnalyzeCheck.GetCheck() )
		{
  		for( int iClass=0; iClass<NUMCLASSES; iClass++)
			{
			if( GetClass(iClass).IsActive() )
				{
				CTask *pcTask=m_pDocument->m_taskList.GetByDateClass(m_cDate, GetClass(iClass).GetType());
				if( pcTask ) 
					{
					bTask=true;
					break;
					}
				}
			}
		if( !bTask ) 
			{
			AfxMessageBox("No tasks have been defined for this day.", MB_ICONSTOP);
			return;
			}
		}

  if( m_cSendScoreslCheck.GetCheck() &&
	  (m_strUsername.GetLength()==0 || m_strPassword.GetLength()==0) )
	{
	AfxMessageBox("Invalid username or password for scoresheet submittal.", MB_ICONSTOP);
	return;
	}

  m_uTimerVal = SetTimer (IDT_TIMER_0, 1000, NULL);
  m_iTimerCount=m_iDuration;
  m_cStartButton.EnableWindow(false);
  m_cStopButton.EnableWindow(true);
  m_cNumSecondsEdit.EnableWindow(false);
  m_cDateCombo.EnableWindow(false);

  m_cEmailCheck.EnableWindow(false);
  m_cAnalyzeCheck.EnableWindow(false);
  m_cSendScoreslCheck.EnableWindow(false);
  m_cAutoSaveCheck.EnableWindow(false);
  m_cSSACombo.EnableWindow(false);
  m_cEmailSettings.EnableWindow(false);
  m_cTransferSettings.EnableWindow(false);
  m_cMediaCheck.EnableWindow(false);
  m_bRunning=true;

  AddStatusRecord("Autoscore Started.");
  UpdateData(FALSE);

}


void CAutoScoreDlg::OnBnClickedStop()
{
	m_bRunning=false;
	if(m_pAutoScoreThread!=NULL ) Sleep(500);
	CSingleLock cLock(&m_cSem_OnBnClickedStop);
	KillTimer(IDT_TIMER_0);
	m_strCountdown="Ready";
	m_uTimerVal= 0;
    m_cStartButton.EnableWindow(true);
    m_cStopButton.EnableWindow(false);
	m_cNumSecondsEdit.EnableWindow(true);
    m_cDateCombo.EnableWindow(true);

	m_cEmailCheck.EnableWindow(true);
	m_cAnalyzeCheck.EnableWindow(true);
	m_cSendScoreslCheck.EnableWindow(true);
	m_cSSACombo.EnableWindow(true);
	m_cAutoSaveCheck.EnableWindow(true);

    m_cEmailSettings.EnableWindow(true);
    m_cTransferSettings.EnableWindow(true);
	m_cMediaCheck.EnableWindow(true);

	AddStatusRecord("Autoscore Stopped.");
}


void CAutoScoreDlg::OnTimer(UINT TimerVal)
	{
	CStringArray strArray;
	CStringArray strFiles, strFilePaths;
	int nNew=0;
	bool bFound=false;
	CString strError;
	CString strDrive;
	UpdateDate();
	CIGCDirectories cIGCDirs;

	if( m_cMediaCheck.GetCheck() && CheckFlashDrive(strDrive) )
		{
		CString strLogPath	=cIGCDirs.GetFlightLogPath(m_cDate);
		// New flash drive inserted, check for files

		//Search for igc files regardless of date.
		bFound = CIGCDirectories::FindIGCFilesInDirectory( strDrive, _T("igc"),  0, 1, strFilePaths, strFiles, 1 )>0;
		if( !bFound )
			{
			CString strMedia;
			strMedia.Format("MEDIA - EMPTY - No .IGC logs found on removable drive %s.",strDrive);
			AddStatusRecord(strMedia);
			}
		else
			{
			// Something was found...
			ShowWindow(SW_RESTORE);
			UpdateWindow();
			for( int iFile=0; iFile<strFiles.GetCount(); iFile++ )
				{

				// Check for proper date
				CIGCFile igcFile(strFilePaths[iFile]);
				if( !igcFile.m_bValid ) continue;
				if( igcFile.m_iYear		!= m_cDate.GetYear()	||
					igcFile.m_iMonth	!= m_cDate.GetMonth()	||
					igcFile.m_iDay		!= m_cDate.GetDay()		 ) continue;

				CString strOutputFileName, strStatus;
				if( CIGCDirectories::ImportIGC( strFilePaths[iFile], strFiles[iFile], strLogPath, strOutputFileName, strStatus ) )
					{
					CString strFlash("MEDIA - SUCCESS - ");
					strFlash+=strStatus;
					AddStatusRecord(strFlash);
					DWORD dwFreq=1000;
					DWORD dwDuration=500;
					Beep( dwFreq,  dwDuration);
					}
				else
					{
					if( strStatus.Find("already")>0 ) 
						{
						CString strFlash("MEDIA - ALREADY - ");
						strFlash+=strStatus;
						AddStatusRecord(strFlash);
						}
					else
						{
						CString strFlash("MEDIA - FAIL - ");
						strFlash+=strStatus;
						AddStatusRecord(strFlash);
						}
					}
				}
			}
		}

	if( m_iTimerCount==m_iDuration ||  m_iTimerCount<=0 ) 
		{
		// The time is nie, do something
		
		bool bThreaded=true;
		if( bThreaded )
			{
			if( m_pAutoScoreThread==NULL )
				{
				m_pAutoScoreThread=AfxBeginThread(&AutoScoreProc, m_pView);
				m_strCountdown="Processing Autoscore.";
				}
			else
				{
				TRACE0("AutoScore Already running.\n");
				}
			}
		else
			AutoScoreProc(m_pView);

		m_iTimerCount=m_iDuration-1;
		}


	if( m_pAutoScoreThread==NULL ) 
		{
		m_strCountdown.Format("Next event in %i seconds.",m_iTimerCount+1);
		m_iTimerCount--;
		}
	else
		{
		if( m_strCountdown.GetLength()<30 ) m_strCountdown+=".";
		}

	UpdateData(FALSE);
	CDialog::OnTimer(TimerVal);
	}


void CAutoScoreDlg::OnClose()
{
	UpdateData();
	WriteWinscoreInt( AUTOSCORE_DURATION,	m_iDuration );
	WriteWinscoreInt( AUTOSCORE_MEDIA,		m_cMediaCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_EMAIL,		m_cEmailCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_ANALYSIS,	m_cAnalyzeCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_SCORES,		m_cSendScoreslCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_AUTOSAVE,	m_cAutoSaveCheck.GetCheck() );

	KillTimer(IDT_TIMER_0);
	CDialog::OnCancel();
	DestroyWindow();
}


void CAutoScoreDlg::OnClear()
{
	m_strStatusArrayTime.RemoveAll();
	m_strStatusArrayData.RemoveAll();

	m_cListCtrl.DeleteAllItems();
	m_iItem=0;
}

int CAutoScoreDlg::AddStatusRecord(CString strText, int iItem)
	{
	CSingleLock cLock(&m_cSem_AddStatusRecord);
	int iItemID;

	if( iItem==-1 )
		{
		// New item
		LV_ITEM lvi;
		lvi.iSubItem = 0;
		lvi.mask = LVIF_PARAM;
		lvi.iItem = m_iItem++;
		lvi.lParam = NULL;
		iItemID=m_cListCtrl.InsertItem(&lvi);
		}
	else
		iItemID=iItem;

	CTime tnow = CTime::GetCurrentTime();
	CString strTime=tnow.Format(_T("%H : %M : %S"));
	m_cListCtrl.SetItemText(iItemID,0,strTime);
	m_cListCtrl.SetItemText(iItemID,1,strText);

	m_strStatusArrayTime.Add(strTime);
	m_strStatusArrayData.Add(strText);

	m_cListCtrl.EnsureVisible( iItemID, FALSE);
	m_cListCtrl.Invalidate(); 
	m_cListCtrl.UpdateWindow(); 



	return iItemID;
	}

void CAutoScoreDlg::OnTransferSettingsButton()
{
	CSSAContestLoginDlg dlg;
	dlg.m_strPassword=m_strPassword;
	dlg.m_strUsername=m_strUsername;

	dlg.m_bRemember=GetWinscoreInt( SSACONTESTREMEMBER, 0 );
	if( dlg.m_bRemember )
		{
		dlg.m_strUsername	=GetWinscoreString( SSACONTESTUSERNAME, "" );
		//dlg.m_strPassword	=GetWinscoreStringDecrypt( SSACONTESTPASSWORD, "" );
		dlg.m_strPassword	=AfxGetApp()->GetProfileString( REGSECTION, SSACONTESTPASSWORD, "" );
		
		}

	if( dlg.DoModal()==IDOK ) 
		{
		WriteWinscoreInt( SSACONTESTREMEMBER, (int)dlg.m_bRemember );
		if( dlg.m_strPassword.GetLength()<=1 ) 
			AfxMessageBox(_T("Invalid Password."));
		else
			{
			m_strPassword=dlg.m_strPassword;
			//if( dlg.m_bRemember ) WriteWinscoreStringEncrypt( SSACONTESTPASSWORD, dlg.m_strPassword );
			if( dlg.m_bRemember ) AfxGetApp()->WriteProfileString( REGSECTION, SSACONTESTPASSWORD, dlg.m_strPassword );
			}

		if( dlg.m_strUsername.GetLength()<=1 ) 
			AfxMessageBox(_T("Invalid Username." ));
		else
			{
  			m_strUsername= dlg.m_strUsername;
			if( dlg.m_bRemember ) WriteWinscoreString( SSACONTESTUSERNAME, dlg.m_strUsername );
			}
	}
}


void CAutoScoreDlg::UpdateDate(void)
	{
	int iSel=m_cDateCombo.GetCurSel();
	CTime *pcTime=(CTime*)m_cDateCombo.GetItemDataPtr(iSel);
	m_cDate= *pcTime;

	CString strText;
	strText+="Analyze and score new logs found in:  ";
	strText+=m_cIGCDirs.GetFlightLogPath(m_cDate, iSel==0);
	m_cAnalyzeCheck.SetWindowText(strText);

	m_bPreContest=iSel==0;

	if( iSel==0 ) 
		{
		m_nDays=m_pDocument->GetNumPreContestDays();
		}
	else
		{
		m_nDays=1;
		}
	}


void CAutoScoreDlg::OnCbnSelchangeDateCombo()
{
UpdateDate();
}


void CAutoScoreDlg::OnCbnSelchangeSsa()
{
	int iIndex=m_cSSACombo.GetCurSel();
	m_iSSA_ID=m_cSSACombo.GetItemData(iIndex);
}



void CAutoScoreDlg::OnBnClickedEmailSettingsButton()
	{

	CEmailSettingsDlg dlg;
	dlg.m_strServer		=GetWinscoreString( EMAILSERVER,	_T("")  );
	dlg.m_strUsername	=GetWinscoreString( EMAILUSERNAME,	_T("")  );
	dlg.m_strPassword	=GetWinscoreStringDecrypt(  EMAILPASSWORD,	_T("")  );
	dlg.m_strMailbox	=GetWinscoreString(  EMAILMAILBOX,	_T("Inbox")  );
	dlg.m_iPortID		=GetWinscoreInt(  EMAILPORTID,	993 );
	dlg.m_bUseSSL		=GetWinscoreInt(  EMAILSSL,	1 );

	dlg.m_pParentDlg=this;

	if( dlg.DoModal() )
		{
		WriteWinscoreString( EMAILSERVER, dlg.m_strServer);
		WriteWinscoreString( EMAILUSERNAME, dlg.m_strUsername);
		WriteWinscoreStringEncrypt( EMAILPASSWORD, dlg.m_strPassword);
		WriteWinscoreString( EMAILMAILBOX, dlg.m_strMailbox);
		WriteWinscoreInt(	 EMAILPORTID, dlg.m_iPortID);
		WriteWinscoreInt(	 EMAILSSL, dlg.m_bUseSSL	);

		}

//	CString		strIMAPServer("imap.gmail.com"); 
//	CString strUsername("ssa.scoring@gmail.com");
//	CString strPassword("Rem0te$c0rin9");
//	CString strMailbox("Inbox");
	}



int CAutoScoreDlg::ScanEmailForLogs( CTime cDate, int nDates, CString strIMAPServer, CString strUsername, CString strPassword, CString strMailbox,  int iPortID, BOOL bUseSSL, CString strLogDir, CString &strError, bool bTestOnly )
	{
    CkImap imap;

    bool success;
	int nLogs=0;
	CFileStatus rStatus;
	CWaitCursor cWait;
	CString POORFILEDIR("\\RenamedFiles");
	CString strStatus;

    //  unlock the component 
    success = imap.UnlockComponent("GBYARS.IM11117_hzSh89cg06pM");
    if (success != true) {
       strError= imap.lastErrorText();
       return -1;
    }

    //  Connect to an IMAP server.
    //  Use TLS
    imap.put_Ssl(bUseSSL==1?(true):(false));
    imap.put_Port(iPortID);
    success = imap.Connect(strIMAPServer);
    if (success != true) {
        strError=imap.lastErrorText();
        return -1;
    }

    //  Login
    success = imap.Login(strUsername,strPassword);
    if (success != true) {
       strError=imap.lastErrorText();
       return -1;
    }

    //  Select an IMAP mailbox
    success = imap.SelectMailbox(strMailbox);
    if (success != true) {
       strError=imap.lastErrorText();
       return -1;
    }
	if( bTestOnly ) 
		{
		imap.Disconnect();
		return 0;
		}

    CkMessageSet *messageSet = 0;
    bool fetchUids = true;

    //  Get the message IDs of all the emails in the mailbox

    //  Search for messages sent before a date, and SENTSINCE finds
    //  email sent on or after a date. 
	CString strSince;
	strSince.Format("SENTSINCE %s",cDate.Format(_T("%d-%b-%Y")));
    messageSet = imap.Search(strSince,fetchUids);
//    messageSet = imap.Search("ALL",fetchUids);
    if (messageSet == 0 ) {
        strError=imap.lastErrorText();
        return -1;
    }

    //  Fetch the email headers into a bundle object:
    CkEmailBundle *bundle = 0;
    bundle = imap.FetchHeaders(*messageSet);
    if (bundle == 0 ) {
        delete messageSet;
        strError=imap.lastErrorText();
        return -1;
    }

    //  Scan for emails with attachments, and save the attachments
    int i;
	int nMails=bundle->get_MessageCount();
    for (i = 0; i <=  nMails- 1; i++) {
        CkEmail *email = 0;
        email = bundle->GetEmail(i);

//		SYSTEMTIME sTime;
//		CkDateTime *ckDate = email->GetDt();
//		ckDate->GetAsSystemTime(true, sTime);
//		delete ckDate;

        //  Does this email have attachments?
        //  Use GetMailNumAttach because the attachments
        //  are not actually in the email object because
        //  we only downloaded headers.
        //  (Had we downloaded the full emails by
        //  calling mailman.FetchBundle, we could look
        //  at the email object's NumAttachments property.)
        int numAttach = imap.GetMailNumAttach(*email);

        if (numAttach > 0) 
			{
            //  Download the entire email and save the
            //  attachments. (Remember, we
            //  need to download the entire email because
            //  only the headers were previously downloaded.
            //  If the entire emails were downloaded by
            //  calling FetchBundle instead of FetchHeaders,
            //  this would not be necessary.



			//Check to see if this is a valid igc file
			bool bLogs=false;
			for (int j = 0; j <= numAttach - 1; j++) 
				{
				//const char *filename = imap.getMailAttachFilename(*email,j);
				string filename = imap.getMailAttachFilename(*email,j);
				CString strAttach(filename.c_str());
				strAttach.MakeLower();
				if( strAttach.Find(".igc")<0 ) continue;
				
				//Check if the file already exists
				CFileStatus rStatus;
				CString strFileName=strLogDir+"\\"+filename.c_str();
				if( CFile::GetStatus( strFileName, rStatus ) ) continue;

				//Check to make sure this log is the date of interest
				for( int iCheckDate=0; iCheckDate<nDates; iCheckDate++ )
					{
					CTime cCheckDate=cDate+CTimeSpan(iCheckDate,0,0,0);

					CString strLongPrefix=CIGCFile::GetLongDatePrefix(cCheckDate.GetYear(), cCheckDate.GetMonth(), cCheckDate.GetDay());
					CString strShortPrefix=CIGCFile::GetDatePrefix(cCheckDate.GetYear(), cCheckDate.GetMonth(), cCheckDate.GetDay());

					if( strAttach.GetLength()==12 )
						{
						// Short IGC name, check if its valid
						if( strAttach.Find(strShortPrefix)<0 ) continue;
						}
					else if( strAttach.GetLength()==25 || strAttach.GetLength()==28 )
						{// Long IGC name
						if( strAttach.Find(strLongPrefix)<0 ) continue;
						}
					else
						{
						//invalid file name, see if we got it already
						strFileName=strLogDir+POORFILEDIR+"\\"+filename.c_str();
						if( CFile::GetStatus( strFileName, rStatus ) ) continue;	
						}
					// Found at least one new valid log, set flag
					bLogs=true;
					break;
					}
				// Found at least one new valid log, set flag
				if (bLogs) break;			
				}

			// If there are no new logs in this email, skip, don't download the full email
			if( !bLogs ) 
				{
				delete email;
				continue;
				}

           //  The ckx-imap-uid header field is added when
            //  headers are downloaded.  This makes it possible
            //  to get the UID from the email object.
            const char *uidStr = email->getHeaderField("ckx-imap-uid");
            int uid = atoi(uidStr);

            CkEmail *fullEmail = 0;
            fullEmail = imap.FetchSingle(uid,true);
            if (!(fullEmail == 0 )) 
				{
				CString strFrom(fullEmail->ck_from());
				for (int j = 0; j <= numAttach - 1; j++) 
					{
					bool bOddball=false;
					CString strFileName;
					const char *filename = imap.getMailAttachFilename(*fullEmail,j);
					CString strAttach(filename);
					strAttach.MakeLower();
					if( strAttach.Find(".igc")<=0 ) continue;

					//Check to make sure this log is the date of interest
					for( int iCheckDate=0; iCheckDate<nDates; iCheckDate++ )
						{
						CTime cCheckDate=cDate+CTimeSpan(iCheckDate,0,0,0);

						CString strLongPrefix=CIGCFile::GetLongDatePrefix(cCheckDate.GetYear(), cCheckDate.GetMonth(), cCheckDate.GetDay());
						CString strShortPrefix=CIGCFile::GetDatePrefix(cCheckDate.GetYear(), cCheckDate.GetMonth(), cCheckDate.GetDay());

						if( strAttach.GetLength()==12 )
							{
							// Short IGC name
							if( strAttach.Find(strShortPrefix)<0 ) continue;
							}
						else if( strAttach.GetLength()==25 || strAttach.GetLength()==28)
							{// Long IGC name
							if( strAttach.Find(strLongPrefix)<0 ) continue;
							}
						else
							{
							//Some idiot renamed his log to something stupid,
							bOddball=true;
							}

						if( bOddball )
							{
							//Some idiot renamed his log to something stupid, lets download it to a temp directory and rename it.
							CString strLostPath=strLogDir+POORFILEDIR;
							CreateDirectory(strLostPath,NULL);
							if( fullEmail->SaveAttachedFile(j, strLostPath) ) 
								{
								// We downloaded this poorly named file, lets read the A and HDTE records to synthize a proper name.
								strFileName=strLostPath+"\\"+filename;
								CString strLongName;
								if( CIGCFile::GetARecord(strFileName, strLongName) )
									{
									// File was opened and we created a valid file name, check the date
									if( strLongName.Find(strLongPrefix)>=0 ) 
										{
										//This is a good file on todays date.  Some dufus renamed it to something stupid, 
										//copy over to the good directory, but leave a copy in the temp directory so we won't try and download it again
										CString strNewName=strLogDir+"\\"+strLongName;
										if( CopyFile(strFileName, strNewName,TRUE) ) 
											{
											nLogs++;
											strStatus.Format("%s - received from %s (Renamed to %s)",filename,strFrom,strLongName);
											AddStatusRecord(strStatus);
											}
										}
									}
								}
							}
						else
							{
							//Check if we already have this log.
							strFileName=strLogDir+"\\"+filename;
							if( CFile::GetStatus( strFileName, rStatus ) ) continue;

							if( fullEmail->SaveAttachedFile(j, strLogDir) ) 
								{
								nLogs++;
								strStatus.Format("%s - received from %s",filename,strFrom);
								AddStatusRecord(strStatus);
								}
							}
						}
					}

                delete fullEmail;
				}
			}

        delete email;
    }

    //  Disconnect from the IMAP server.
    success = imap.Disconnect();

    delete messageSet;
    delete bundle;
	return nLogs;
	}

void CAutoScoreDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizingDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	
}

void CAutoScoreDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CResizingDialog::OnPaint();
}

bool CheckFlashDrive(CString &strDrive)
	{
	DWORD iNewDrives=GetLogicalDrives();
	if( iNewDrives<iDrives ) 
		{
		// A drive was removed, reset the saved status and return false;
		iDrives=iNewDrives;
		return false;
		}
	else if( iNewDrives==iDrives )
		{
		// No change from last time, nothing to do.
		return false;
		}
	else
		{
		// Something was added. Return the added drive letter;
		DWORD iAddedDrive=iNewDrives-iDrives;
		iDrives=iNewDrives;

		TCHAR buff[512];
		ZeroMemory(buff, 512);
		DWORD iret=GetLogicalDriveStrings( 512, buff);
		if( iret>0 )
			{
			char *pDrv=buff;
			int	iBit=0;
			while( iAddedDrive>0 && iBit<64 )
				{
				if( iAddedDrive & 1) 
					{
					strDrive=pDrv;
					return (strDrive.GetLength()>0)?true:false;
					}
				iAddedDrive=iAddedDrive>>1;

				if(iNewDrives & (1 << iBit++) ) pDrv+=4;
				}
			}
		}

	return false;
}


void CAutoScoreDlg::PostNcDestroy() 
{	
    CDialog::PostNcDestroy();
	if( m_pView ) ((CWinscoreView*)m_pView)->m_pModlessAutoScoreDlg=NULL;
    delete this;
}

void CAutoScoreDlg::OnCancel()
{
	UpdateData();
	WriteWinscoreInt( AUTOSCORE_DURATION,	m_iDuration );
	WriteWinscoreInt( AUTOSCORE_MEDIA,		m_cMediaCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_EMAIL,		m_cEmailCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_ANALYSIS,	m_cAnalyzeCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_SCORES,		m_cSendScoreslCheck.GetCheck() );
	WriteWinscoreInt( AUTOSCORE_AUTOSAVE,	m_cAutoSaveCheck.GetCheck() );

    DestroyWindow();
}


UINT __cdecl AutoScoreProc( LPVOID lpParameter)
	{
	int nNew=0;
	CString strError;
	CWinscoreView *pView=(CWinscoreView*)lpParameter;
	CStringArray strArray;
	CString strFail;
	CoInitialize(NULL);

	if( CheckAutoScoreStop(pView) ) return 0;

	if( pView->m_pModlessAutoScoreDlg->m_cEmailCheck.GetCheck() )
		{
		CString	strIMAPServer	=GetWinscoreString( EMAILSERVER,	_T("")  );
		CString strUsername		=GetWinscoreString(  EMAILUSERNAME,	_T("")  );
		CString strPassword		=GetWinscoreStringDecrypt(  EMAILPASSWORD,	_T("")  );
		CString strMailbox		=GetWinscoreString(  EMAILMAILBOX,	_T("Inbox")  );
		int		iPortID			=GetWinscoreInt(  EMAILPORTID,	993 );
		BOOL	bUseSSL			=GetWinscoreInt(  EMAILSSL,	1 );


		if( strUsername.GetLength()==0 ||
			strPassword.GetLength()==0 ||
			strMailbox.GetLength()==0		)
			{
			if( CheckAutoScoreStop(pView) ) return 0;
			strError="ERROR - Invalid username/password for "+strIMAPServer;
			pView->m_pModlessAutoScoreDlg->AddStatusRecord(strError);
			pView->m_pModlessAutoScoreDlg->OnBnClickedStop();
			pView->m_pModlessAutoScoreDlg->m_pAutoScoreThread=NULL;
			return 0;
			}

		//AddStatusRecord("Checking Email "+strIMAPServer);

		CString strError;
		CString strLogPath	=pView->m_pModlessAutoScoreDlg->m_cIGCDirs.GetFlightLogPath(pView->m_pModlessAutoScoreDlg->m_cDate,
																						pView->m_pModlessAutoScoreDlg->m_bPreContest);

		if( CheckAutoScoreStop(pView) ) return 0;
		int nLogs=pView->m_pModlessAutoScoreDlg->ScanEmailForLogs( pView->m_pModlessAutoScoreDlg->m_cDate,  pView->m_pModlessAutoScoreDlg->m_nDays, strIMAPServer, strUsername, strPassword, strMailbox, iPortID, bUseSSL, strLogPath, strError );
		if( nLogs<0 ) 
			{
			strError="ERROR - Checking email at "+strIMAPServer;
			pView->m_pModlessAutoScoreDlg->AddStatusRecord(strError);
			if( nErrors++>nMAXErrors ) 
				{
				pView->m_pModlessAutoScoreDlg->OnBnClickedStop();
				pView->m_pModlessAutoScoreDlg->m_pAutoScoreThread=NULL;
				return 0;
				}
			}

		}
	
	if( CheckAutoScoreStop(pView) ) return 0;

	if( pView->m_pModlessAutoScoreDlg->m_cAnalyzeCheck.GetCheck() )
		{
//			AddStatusRecord("Checking Analysis");

		//Get all the IGC files for this date, and remove any that are already associated with a flight
		pView->m_pModlessAutoScoreDlg->m_cIGCDirs.FindIGCFiles( pView->m_pModlessAutoScoreDlg->m_cDate, 1,  strArray ); 

		for( int iIGC=0; iIGC<strArray.GetCount(); iIGC++ )
			{
			if( CheckAutoScoreStop(pView) ) return 0;
			CFlight* pcFlight=pView->m_pModlessAutoScoreDlg->m_pDocument->m_FlightList.FindByIGCFileName(strArray[iIGC] );
			if( pcFlight )
				{
				if( pcFlight->GetStatus()!=eNotAnalyzed ) 
					{
					//Already here and analyzed, remove from list.
					strArray[iIGC]="";
					}
				}
			}

//			bool bShown=false;	
//			CWaitCursor cWait;

			for( int iIGC=0; iIGC<strArray.GetCount(); iIGC++ )
				{
				if( strArray[iIGC].GetLength()==0 ) continue;
				CString strMessage;

				if( CheckAutoScoreStop(pView) ) return 0;

				// See if this IGC file is present in the flight list, if so, remove it to remove duplicates.
				CFlight* pcExistingFlight=pView->m_pModlessAutoScoreDlg->m_pDocument->m_FlightList.FindByIGCFileName(strArray[iIGC] );
				if( pcExistingFlight )
					{
					pView->m_pModlessAutoScoreDlg->m_pDocument->m_FlightList.RemoveByPointer(pcExistingFlight);
					}

				CFlight* pcFlight=pView->m_pModlessAutoScoreDlg->m_pDocument->m_FlightList.LoadIGC(strArray[iIGC], pView->m_pModlessAutoScoreDlg->m_pDocument->m_contestantList );

				if( pcFlight->GetStatus()!=eNotAnalyzed) continue;

				nNew++;

				if( !pcFlight->IsContestant() ) 
					{
					strMessage.Format("NOT A CONTESTANT - %s, %s - %s is not a contestant.", pcFlight->m_strFileName, pcFlight->m_strCID, pcFlight->m_strPilot);
					pView->m_pModlessAutoScoreDlg->AddStatusRecord(strMessage);
					continue;
					}
				if( !pcFlight->ReadFlight() )
					{
					pcFlight->SetIGCFileMissing();
					continue;
					}
				
				if( pView->m_pModlessAutoScoreDlg==NULL ) return 0;
				CTask *pcTask=pView->m_pModlessAutoScoreDlg->m_pDocument->m_taskList.GetByDateClass(pView->m_pModlessAutoScoreDlg->m_cDate, pcFlight->m_eClass);
                if( !pcTask )
					{
					pcFlight->SetStatus(eNoTaskSpecified);
					continue;
					}

				try
					{
					if( pView->m_pModlessAutoScoreDlg==NULL ) return 0;
					bool bRet=	pcFlight->Analyze(	pcTask, 
													pView->m_pModlessAutoScoreDlg->m_pDocument->m_turnpointArray,
													&pView->m_pModlessAutoScoreDlg->m_pDocument->m_contestantList,
													pView->m_pModlessAutoScoreDlg->m_pDocument->m_eUnits,
													true);  //true for silent mode
					if( !bRet ) 
						{
						strMessage.Format("%s, %s - %s analysis FAILED!",pcFlight->m_strFileName,pcFlight->m_strCID,pcFlight->m_strPilot,pcFlight->NumberOfWarnings());
						}
					else
						{
						strMessage.Format("%s, %s - %s analyzed with %i warning(s)",pcFlight->m_strFileName,pcFlight->m_strCID,pcFlight->m_strPilot,pcFlight->NumberOfWarnings());
						pcFlight->FreePositionData();
						}
					if( CheckAutoScoreStop(pView) ) return 0;
					pView->UpdateFlightLogs( pView->m_pModlessAutoScoreDlg->m_pFrame, pView->m_pModlessAutoScoreDlg->m_cDate, pView->m_pModlessAutoScoreDlg->m_eClass, pcFlight);
					}
				catch(...)
					{
					strMessage.Format("%s, %s - %s analysis FAILED!",pcFlight->m_strFileName,pcFlight->m_strCID,pcFlight->m_strPilot,pcFlight->NumberOfWarnings());
					}
				if( CheckAutoScoreStop(pView) ) return 0;
				pView->m_pModlessAutoScoreDlg->AddStatusRecord(strMessage);
				}
		if( CheckAutoScoreStop(pView) ) return 0;
		pView->m_pModlessAutoScoreDlg->m_pDocument->SetModifiedFlag();
		}

	if( CheckAutoScoreStop(pView) ) return 0;

	if( pView->m_pModlessAutoScoreDlg->m_cSendScoreslCheck.GetCheck() && (nNew>0 || bScoreSheetError) )
		{
		if( CheckAutoScoreStop(pView) ) return 0;
		pView->m_pModlessAutoScoreDlg->AddStatusRecord("Sending Scoresheet to ssa.org...");

		int nClassesToExport=0;
		EClass		aeClasses[NUMCLASSES];

  		for( int iClass=0; iClass<NUMCLASSES; iClass++)
			{
			if( GetClass(iClass).IsActive() )
				{
				aeClasses[nClassesToExport++]=GetClass(iClass).GetType();
				}
			}
		
		CString strPathName=GetScoresheetPathName(pView->m_pModlessAutoScoreDlg->m_iSSA_ID);

		bool bExported;
		if( CheckAutoScoreStop(pView) ) return 0;
		bExported=pView->m_pModlessAutoScoreDlg->m_pDocument->ExportDayXML(strPathName, nClassesToExport, aeClasses, pView->m_pModlessAutoScoreDlg->m_cDate, strFail);
		if( !bExported )
			{
			if( CheckAutoScoreStop(pView) ) return 0;
			pView->m_pModlessAutoScoreDlg->AddStatusRecord("ERROR - Could not create scoresheet.  Transfer Aborted.");
			if( strFail.GetLength()>0 ) 
				{
				CString strOut;
				strOut.Format("Failure Reason: %s",strFail);
		        pView->m_pModlessAutoScoreDlg->AddStatusRecord(strOut);
				}
			if( nErrors++>nMAXErrors ) 
				{
				pView->m_pModlessAutoScoreDlg->OnBnClickedStop();
				pView->m_pModlessAutoScoreDlg->m_pAutoScoreThread=NULL;
				return 0;
				}
			}
		CString strError;
		bool bSuccess= uploadfile(strPathName,pView->m_pModlessAutoScoreDlg->m_strUsername,pView->m_pModlessAutoScoreDlg->m_strPassword,true,strError);
		DeleteFile(strPathName);
		if( !bSuccess )
			{
			if( CheckAutoScoreStop(pView) ) return 0;
			pView->m_pModlessAutoScoreDlg->AddStatusRecord("ERROR sending scoresheet - "+strError);
			bScoreSheetError=true;
			if( nErrors++>nMAXErrors ) 
				{
				pView->m_pModlessAutoScoreDlg->OnBnClickedStop();
				pView->m_pModlessAutoScoreDlg->m_pAutoScoreThread=NULL;
				return 0;
				}
			}
		else
			{		
			if( CheckAutoScoreStop(pView) ) return 0;
			pView->m_pModlessAutoScoreDlg->AddStatusRecord(_T("Scoresheet Successfully Transferred"));
			bScoreSheetError=false;
			if( pView->m_pModlessAutoScoreDlg->m_cAutoSaveCheck.GetCheck() )
				{
				if( pView->m_pModlessAutoScoreDlg->m_pDocument->OnSaveDocument("") )
					pView->m_pModlessAutoScoreDlg->AddStatusRecord(_T("Winscore File Successfully Saved"));
				else
					pView->m_pModlessAutoScoreDlg->AddStatusRecord(_T("ERROR - Saving Winscore File"));
				}
			}

		}
	pView->m_pModlessAutoScoreDlg->m_pAutoScoreThread=NULL;
	return 0;
	}


BOOL CheckAutoScoreStop(CWinscoreView *pView)
	{
	if( pView->m_pModlessAutoScoreDlg==NULL ) return true;

	if( pView->m_pModlessAutoScoreDlg->m_bRunning==false ) 
		{
		pView->m_pModlessAutoScoreDlg->m_pAutoScoreThread=NULL;
		return true;
		}
	return false;
	}