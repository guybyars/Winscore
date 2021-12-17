
#include "stdafx.h"
//#include <afxdllx.h>
#include "FTPClient.h"
//#include "FTPDownload.h"
#include "FTPUpload.h"
#include "ProgressDlg.h"
#include "StringHelper.h"
//#include "FileInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if 0

void DoEvents()
{
	MSG msg;
	
	// window message         
	while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))         
	{            
		TranslateMessage(&msg);            
		DispatchMessage(&msg);         
	}      
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFTPClient::CFTPClient ()
{
	// Set the timeout value to 1000 milli-seconds
	m_InternetSession.SetOption (INTERNET_OPTION_CONNECT_TIMEOUT, 1000);
	m_InternetSession.SetOption (INTERNET_OPTION_RECEIVE_TIMEOUT, 1000);
	m_InternetSession.SetOption (INTERNET_OPTION_SEND_TIMEOUT, 1000);

	m_pFtpConnection = NULL;
}

CFTPClient::~CFTPClient()
{
	if (m_pFtpConnection != NULL)
	{
		m_pFtpConnection->Close ();
		delete m_pFtpConnection;
		m_pFtpConnection = NULL;
		m_sFTPRootFolder.Empty ();
	}
}

bool CFTPClient::IsInitialized	()
{
	if (m_pFtpConnection != NULL)
		return true;
	else
		return false;
}


void CFTPClient::Login (const CString &sHost, const CString &sUsername, 
						const CString &sPassword, BOOL bPASV, int nPort, 
						int nRetries, int nRetryDelay)
{
	if (m_pFtpConnection != NULL)
		throw new CFTPException (CFTPException::FTP_AlreadyConnected, "Another session is not disconnected yet. Logoff before trying to connect again!");
	
	
	HANDLE hEventKill = CreateEvent (NULL, TRUE, FALSE, NULL);
	
	// set animation to 'Downloading'
	CProgressDlg dlg;
	dlg.m_nAnimationID = IDR_AVI1;
	
	// Create the progress dialog box.
	if (!dlg.Create (NULL, hEventKill))
	{
		ASSERT (FALSE);
		throw new CFTPException (CFTPException::FTP_Unknown, "Unable to create the progress dialog. Please contact the program vendor for more information.");
	}
	
	CString sStatus;
	int nRetryCount = 0;
	int iTimeout=1000;

	while (nRetryCount < nRetries)
	{
		nRetryCount ++;
		try
		{
			// Create new ftp connection to retrieve file
			sStatus.Format ("Connecting to %s (Attempt %d of %d)", sHost, nRetryCount, nRetries);
			dlg.SetStatus (sStatus);
			
			// Connect to FTP Server
			m_pFtpConnection = m_InternetSession.GetFtpConnection (sHost, sUsername, sPassword, nPort, bPASV);
			
			m_pFtpConnection->GetCurrentDirectory (m_sFTPRootFolder);
			StringHelper::RemoveWSFromFileName (m_sFTPRootFolder);
			StringHelper::AddBSToFolderName (m_sFTPRootFolder, '/');
			m_sFTPRootFolder.Replace	("\\", "/");

			// Connected! Exit the while loop.
			break;
		}
		catch (CInternetException* pEx)
		{
			m_pFtpConnection = NULL;
			
			// catch errors from WinINet
			pEx->GetErrorMessage (sStatus.GetBuffer (MAX_PATH), MAX_PATH);
			pEx->Delete ();
			sStatus.ReleaseBuffer ();
			
			if (nRetryCount < nRetries)
			{
				// Increase the timeout.  They might have a satellite connection
				iTimeout+=1000;
				m_InternetSession.SetOption (INTERNET_OPTION_CONNECT_TIMEOUT, iTimeout);
				m_InternetSession.SetOption (INTERNET_OPTION_RECEIVE_TIMEOUT, iTimeout);
				m_InternetSession.SetOption (INTERNET_OPTION_SEND_TIMEOUT, iTimeout);

				sStatus.Format ("Failed to connect to %s (Attempt %d of %d)", sHost, nRetryCount, nRetries); 
				dlg.SetStatus(sStatus);
				
				sStatus.Format ("Retrying after %d seconds...", nRetryDelay); 
				dlg.SetSecondStatus(sStatus);
				
				DWORD dwRet;
				DWORD dwMaxTick = GetTickCount () + nRetryDelay * 1000;
				
				while (1)
				{
					// wait for event or message, if it's a message, process it and return to waiting state
					dwRet = MsgWaitForMultipleObjects (1, &hEventKill, FALSE, dwMaxTick - GetTickCount (), QS_ALLINPUT);
					if (dwRet == WAIT_OBJECT_0)
					{
						// user canceled
						throw new CFTPException (CFTPException::FTP_UserAborted, "Login is aborted by the user");
					}   
					else if (dwRet == WAIT_OBJECT_0 + 1)
					{
						DoEvents ();
					}  
					else
					{
						// timed out!
						break;
					}
				}
				dlg.SetSecondStatus ("");
			}
			else
			{
				throw new CFTPException (CFTPException::FTP_LoginFailed, "Cannot login to the server " + sHost +"\nReason: " +sStatus);
			}
		}
	}
	
	dlg.DestroyWindow ();
}

void CFTPClient::Logoff ()
{
	if (m_pFtpConnection == NULL)
		throw new CFTPException (CFTPException::FTP_NotConnected, "Not connected yet!");
	else
	{
		m_pFtpConnection->Close ();
		delete m_pFtpConnection;
		m_pFtpConnection = NULL;
		m_sFTPRootFolder.Empty ();
	}
}

void CFTPClient::SetCurrentDir(const CString &sDir)
{
	CString sTemp;
	// set current directory
	if (!m_pFtpConnection->SetCurrentDirectory (sDir))
	{
		sTemp.Empty ();
		DWORD dwLength = 255, dwError;
		InternetGetLastResponseInfo (&dwError, sTemp.GetBuffer (dwLength), &dwLength);
		sTemp.ReleaseBuffer();
		throw new CFTPException (CFTPException::FTP_Remote_SetCurDirFailed, sTemp); 
	}
}

void CFTPClient::GetFileList (const CString &sRemoteFolder, bool bFullPathGiven, CStringArray &sFileNameArray, bool bRecursive, bool bExcludeHidden)
{
	if (m_pFtpConnection == NULL)
		throw new CFTPException (CFTPException::FTP_NotConnected, "Not connected to any FTP server!");

	CFtpFileFind ftpFind (m_pFtpConnection);
	CStringArray sDirectoryArray;
	
	CString sTemp;
	if (bFullPathGiven)
		sTemp = sRemoteFolder;
	else
	{
		sTemp = sRemoteFolder;
		RemoveFirstBS (sTemp);
		sTemp = (m_sFTPRootFolder + sTemp);
	}

	// go to requested directory
	if (!m_pFtpConnection->SetCurrentDirectory (sTemp))
	{
		CString sTemp;
		DWORD dwLength = 255, dwError;
		InternetGetLastResponseInfo (&dwError, sTemp.GetBuffer (dwLength), &dwLength);
		sTemp.ReleaseBuffer ();
		throw new CFTPException (CFTPException::FTP_Remote_SetCurDirFailed, sTemp); 
	}
	
	// add directory (without filename), so we can create the directory structure later
	CString sPath, sFileName;
	
	m_pFtpConnection->GetCurrentDirectory (sPath);
	
	StringHelper::RemoveWSFromFileName (sPath);
	StringHelper::AddBSToFolderName (sPath, '/');
	
	sFileName.Format ("|%s|0", sPath);
	sFileNameArray.Add (sFileName);
	
	BOOL bContinue = ftpFind.FindFile (NULL);//"*");
	if (!bContinue)
	{
		// the directory is empty; just close up and return.
		ftpFind.Close ();
		return;
	}
	
	// add all files in lpszPath
	while (bContinue)
	{
		bContinue = ftpFind.FindNextFile ();
		
		if (ftpFind.IsDots ())
			continue;
		
		// is selected item a directory ?
		if (ftpFind.IsDirectory ())
		{
			sDirectoryArray.Add (ftpFind.GetFilePath ());
			// If not recursive, add the directory names
			if (!bRecursive)
			{
				sFileName.Format ("|%s|0", ftpFind.GetFilePath ());
				sFileNameArray.Add (sFileName);
			}				
		}
		else
		{
			if (! (bExcludeHidden && ftpFind.IsHidden ()))
			{
				sFileName.Format ("%s|%s|%u", ftpFind.GetFileName (), sPath, ftpFind.GetLength ());
				sFileNameArray.Add (sFileName);
			}
		}
	}
	ftpFind.Close ();
	
	if (bRecursive)
	{
		// get contents of directories
		for (int i = 0; i < sDirectoryArray.GetSize(); i++)
			GetFileList (sDirectoryArray.GetAt (i), true, sFileNameArray, bRecursive);
	}
}

// Full name can be a folder name or a full file name (wildcards are allowed)
// The function finds the Folder name and the filename.
// If sFullName ends with a cFolderSeperator, then 
// cFolderSeperator is '/' for FTP and '\\' for local files...
void CFTPClient::SeperateFilename (const CString &sFullName, CString &sFolder, CString &sFilename, TCHAR cFolderSeperator)
{
	if (sFullName.GetLength () > 0)
	{
		if (sFullName.GetAt (sFullName.GetLength () - 1) != cFolderSeperator)
		{
			int nPos = sFullName.ReverseFind (cFolderSeperator);
			if (nPos != -1)
			{
				sFolder = sFullName.Left (nPos + 1);
				sFilename = sFullName.Mid (nPos + 1);
			}
			else
			{
				sFolder = "";
				sFilename = sFullName;
			}
		}
		// If it is a folder name. Thus just add the whole 
		// folder name and set the 
		else
		{
			sFolder = sFullName;
			sFilename = "";
		}
	}
	else
	{
		sFolder = "";
		sFilename = "";
	}
}

void CFTPClient::RemoveFirstBS (CString &sFilename)
{
	int nLength = sFilename.GetLength ();
	if (nLength > 0)
	{
		if (sFilename.GetAt (0) == '\\' ||
			sFilename.GetAt (0) == '/')
		{
			if (nLength > 1)
				sFilename = sFilename.Right (nLength - 1);
			else
				sFilename = "";
		}
	}
}
#endif

