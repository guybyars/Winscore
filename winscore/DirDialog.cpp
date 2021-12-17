//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
///////////////////////////////////////////////////////////////////////////
// DirDialog.cpp: implementation of the CDirDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirDialog.h"
#include "shlobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirDialog::CDirDialog()
{
}

CDirDialog::~CDirDialog()
{
}

int CDirDialog::DoBrowse()
{
		IShellItem *psiFolder;
 		LPWSTR folderpath = NULL; 

		IFileDialog *pfd;
		if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
		{
			DWORD dwOptions;
			if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
				{
				pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
				}

			CStringW strw(m_strSelDir);
			LPCWSTR szFilePath = strw;
			SHCreateItemFromParsingName ( szFilePath, NULL, IID_PPV_ARGS(&psiFolder));
			pfd->SetDefaultFolder ( psiFolder );

			CStringW strTit(m_strTitle);
			LPCWSTR szTitle =strTit;
			if (!SUCCEEDED( pfd->SetTitle(szTitle) ) ) return FALSE;


			if (SUCCEEDED(pfd->Show(NULL)))
			{
				IShellItem *psi;
				if (SUCCEEDED(pfd->GetResult(&psi)))
				{
					if(!SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &folderpath)))
						{
						 return FALSE;
						}
					m_strPath=CString(folderpath);
					psi->Release();
				}
			}
			pfd->Release();
		}


  return (folderpath!=NULL);
}
