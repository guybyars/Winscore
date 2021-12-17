/*********************************************************************
	Copyright (C) 2002 Mustafa Demirhan :)
	
	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	3. This notice may not be removed or altered from any source distribution.

	http://www.macroangel.com
	admin@macroangel.com
**********************************************************************/

#if !defined(AFX_FTPUPLOAD_H__A01390EE_E22B_4D30_B933_90D90F2C18EE__INCLUDED_)
#define AFX_FTPUPLOAD_H__A01390EE_E22B_4D30_B933_90D90F2C18EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProgressDlg.h"

#define BUF_SIZE 4096

#define WM_FTP_STATUS WM_USER+130
#define WM_UPLOAD_FINISHED WM_USER+132

class CFTPClient;
class CFTPUpload
{
public:
	CFTPUpload(CFTPClient *pClient);
	~CFTPUpload();

	void UploadAllFiles (bool *pbAbort, bool bRecursive);
	void UploadSingleFileFromText (const CString &sOrgSource, const CString &sOrgDestDir, bool *pbAbort);
	void UploadSingleFile		(const CString &sOrgSource, const CString &sOrgDestDir, bool *pbAbort);
	
	DWORD	m_dwTransferType;
	
	CString m_sLocalName;
	CString m_sRemoteFolder;
	CString m_sFTPRootFolder;
	bool	m_bRecursive;
	
	DWORD	m_dwFileLength;
	
	HANDLE	m_hEventKill;
	
protected:
	void WaitForProgressDialog	();
	
	CProgressDlg m_ProgressDlg;
	CFTPClient	 *m_pFTPClient;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPUPLOAD_H__A01390EE_E22B_4D30_B933_90D90F2C18EE__INCLUDED_)
