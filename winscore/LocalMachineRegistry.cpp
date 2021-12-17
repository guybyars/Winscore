// LocalMachineRegistry.cpp: implementation of the CLocalMachineRegistry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <winreg.h>
#include "LocalMachineRegistry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocalMachineRegistry::CLocalMachineRegistry(HKEY hRootKey):
	m_hRootKey(hRootKey)
	{
	m_pszProfileName = NULL;
	m_pszRegistryKey = NULL;
	m_pszProfileName = new CString;
	m_pszRegistryKey = new CString;
	}

CLocalMachineRegistry::~CLocalMachineRegistry()
	{
	if (NULL != m_pszProfileName) delete m_pszProfileName;
	if (NULL != m_pszRegistryKey) delete m_pszRegistryKey;
	}

//HKEY_CURRENT_USER\Software\Vold Solutions, Inc.\VSIRotate2\Settings

void CLocalMachineRegistry::SetRegistryKey(CString cRegistryKey, CString cProfileName)
{
	*m_pszRegistryKey = cRegistryKey;
	*m_pszProfileName = cProfileName;
}

// returns key for HKEY_LOCAL_MACHINE\"Software"\RegistryKey\ProfileName
// creating it if it doesn't exist
// responsibility of the caller to call RegCloseKey() on the returned HKEY
HKEY CLocalMachineRegistry::GetAppRegistryKey()
{
	ASSERT(m_pszRegistryKey != NULL);
	ASSERT(m_pszProfileName != NULL);

	HKEY hAppKey = NULL;
	HKEY hSoftKey = NULL;
	HKEY hCompanyKey = NULL;
	if (RegOpenKeyEx(m_hRootKey, _T("Software"), 0, KEY_WRITE|KEY_READ,
		&hSoftKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (RegCreateKeyEx(hSoftKey, *m_pszRegistryKey, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCompanyKey, &dw) == ERROR_SUCCESS)
		{
			RegCreateKeyEx(hCompanyKey, *m_pszProfileName, 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hAppKey, &dw);
		}
	}
	if (hSoftKey != NULL)
		RegCloseKey(hSoftKey);
	if (hCompanyKey != NULL)
		RegCloseKey(hCompanyKey);

	return hAppKey;
}

// returns key for:
//      HKEY_CURRENT_USER\"Software"\RegistryKey\AppName\lpszSection
// creating it if it doesn't exist.
// responsibility of the caller to call RegCloseKey() on the returned HKEY
HKEY CLocalMachineRegistry::GetSectionKey(CString cSection)
{

	HKEY hSectionKey = NULL;
	HKEY hAppKey = GetAppRegistryKey();
	if (hAppKey == NULL)
		return NULL;

	DWORD dw;
	RegCreateKeyEx(hAppKey, cSection, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSectionKey, &dw);
	RegCloseKey(hAppKey);
	return hSectionKey;
}

UINT CLocalMachineRegistry::GetProfileInt(CString cSection, CString cEntry,
	int nDefault)
{
//	ASSERT(lpszSection != NULL);
//	ASSERT(lpszEntry != NULL);
	if (m_pszRegistryKey != NULL) // use registry
	{
		HKEY hSecKey = GetSectionKey(cSection);
		if (hSecKey == NULL)
			return nDefault;
		DWORD dwValue;
		DWORD dwType;
		DWORD dwCount = sizeof(DWORD);
		LONG lResult = RegQueryValueEx(hSecKey, cEntry, NULL, &dwType,
			(LPBYTE)&dwValue, &dwCount);
		RegCloseKey(hSecKey);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_DWORD);
			ASSERT(dwCount == sizeof(dwValue));
			return (UINT)dwValue;
		}
		return nDefault;
	}
	else
	{
		ASSERT(m_pszProfileName != NULL);
		return ::GetPrivateProfileInt(cSection, cEntry, nDefault,
			*m_pszProfileName);
	}
}

CString CLocalMachineRegistry::GetProfileString(CString cSection, CString cEntry,
				CString cDefault)
{
//	ASSERT(lpszSection != NULL);
//	ASSERT(lpszEntry != NULL);
	if (m_pszRegistryKey != NULL)
	{
		HKEY hSecKey = GetSectionKey(cSection);
		if (hSecKey == NULL)
			return cDefault;
		CString strValue;
		DWORD dwType, dwCount;
		LONG lResult = RegQueryValueEx(hSecKey, cEntry, NULL, &dwType,
			NULL, &dwCount);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_SZ);
			lResult = RegQueryValueEx(hSecKey, cEntry, NULL, &dwType,
				(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
			strValue.ReleaseBuffer();
		}
		RegCloseKey(hSecKey);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_SZ);
			return strValue;
		}
		return cDefault;
	}
	else
	{
		ASSERT(m_pszProfileName != NULL);

		if (cDefault == "")
			cDefault = '\0';    // don't pass in NULL
		TCHAR szT[4096];
		DWORD dw = ::GetPrivateProfileString(cSection, cEntry,
			cDefault, szT, sizeof(szT), *m_pszProfileName);
		ASSERT(dw < 4095);
		return szT;
	}
}

BOOL CLocalMachineRegistry::GetProfileBinary(CString cSection, CString cEntry,
	BYTE** ppData, UINT* pBytes)
{
//	ASSERT(lpszSection != NULL);
//	ASSERT(lpszEntry != NULL);
	ASSERT(ppData != NULL);
	ASSERT(pBytes != NULL);
	*ppData = NULL;
	*pBytes = 0;
	if (m_pszRegistryKey != NULL)
	{
		HKEY hSecKey = GetSectionKey(cSection);
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		LONG lResult = RegQueryValueEx(hSecKey, cEntry, NULL, &dwType,
			NULL, &dwCount);
		*pBytes = dwCount;
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			*ppData = new BYTE[*pBytes];
			lResult = RegQueryValueEx(hSecKey, cEntry, NULL, &dwType,
				*ppData, &dwCount);
		}
		RegCloseKey(hSecKey);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			return TRUE;
		}
		else
		{
			delete [] *ppData;
			*ppData = NULL;
		}
		return FALSE;
	}
	else
	{
		ASSERT(m_pszProfileName != NULL);

		CString str = GetProfileString(cSection, cEntry, CString(""));
		if (str.IsEmpty())
			return FALSE;
		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();
		*pBytes = nLen/2;
		*ppData = new BYTE[*pBytes];
		for (int i=0;i<nLen;i+=2)
		{
			(*ppData)[i/2] = (BYTE)
				(((str[i+1] - 'A') << 4) + (str[i] - 'A'));
		}
		return TRUE;
	}
}

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

BOOL CLocalMachineRegistry::WriteProfileInt(CString cSection, CString cEntry,
	int nValue)
{
//	ASSERT(lpszSection != NULL);
//	ASSERT(lpszEntry != NULL);
	if (m_pszRegistryKey != NULL)
	{
		HKEY hSecKey = GetSectionKey(cSection);
		if (hSecKey == NULL)
			return FALSE;
		LONG lResult = RegSetValueEx(hSecKey, cEntry, NULL, REG_DWORD,
			(LPBYTE)&nValue, sizeof(nValue));
		RegCloseKey(hSecKey);
		return lResult == ERROR_SUCCESS;
	}
	else
	{
		ASSERT(m_pszProfileName != NULL);

		TCHAR szT[16];
		wsprintf(szT, _T("%d"), nValue);
		return ::WritePrivateProfileString(cSection, cEntry, szT,
			*m_pszProfileName);
	}
}

BOOL CLocalMachineRegistry::WriteProfileString(CString cSection, CString cEntry,
			CString cValue)
{
//	ASSERT(lpszSection != NULL);
	if (m_pszRegistryKey != NULL)
	{
		LONG lResult;
		if (cEntry == "") //delete whole section
		{
			HKEY hAppKey = GetAppRegistryKey();
			if (hAppKey == NULL)
				return FALSE;
			lResult = ::RegDeleteKey(hAppKey, cSection);
			RegCloseKey(hAppKey);
		}
		else if (cValue == "")
		{
			HKEY hSecKey = GetSectionKey(cSection);
			if (hSecKey == NULL)
				return FALSE;
			// necessary to cast away const below
			lResult = ::RegDeleteValue(hSecKey, cEntry);
			RegCloseKey(hSecKey);
		}
		else
		{
			HKEY hSecKey = GetSectionKey(cSection);
			if (hSecKey == NULL)
				return FALSE;
#ifdef _UNICODE
			lResult = RegSetValueEx(hSecKey, cEntry, NULL, REG_SZ,
				(const unsigned char *)LPCTSTR(cValue), (cValue.GetLength()+1)*sizeof(TCHAR));
#else
			lResult = RegSetValueEx(hSecKey, cEntry, NULL, REG_SZ,
				(LPBYTE)LPCTSTR(cValue), (cValue.GetLength()+1)*sizeof(TCHAR));
#endif
	
			RegCloseKey(hSecKey);
		}
		return lResult == ERROR_SUCCESS;
	}
	else
	{
		ASSERT(m_pszProfileName != NULL);
		ASSERT(lstrlen(*m_pszProfileName) < 4095); // can't read in bigger
		return ::WritePrivateProfileString(cSection, cEntry, cValue,
			*m_pszProfileName);
	}
}

BOOL CLocalMachineRegistry::WriteProfileBinary(CString cSection, CString cEntry,
	LPBYTE pData, UINT nBytes)
{
//	ASSERT(lpszSection != NULL);
	if (m_pszRegistryKey != NULL)
	{
		LONG lResult;
		HKEY hSecKey = GetSectionKey(cSection);
		if (hSecKey == NULL)
			return FALSE;
		lResult = RegSetValueEx(hSecKey, cEntry, NULL, REG_BINARY,
			pData, nBytes);
		RegCloseKey(hSecKey);
		return lResult == ERROR_SUCCESS;
	}

	// convert to string and write out
	LPTSTR lpsz = new TCHAR[nBytes*2+1];
	UINT i=0;
	for ( i = 0; i < nBytes; i++)
	{
		lpsz[i*2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
		lpsz[i*2+1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
	}
	lpsz[i*2] = 0;

	ASSERT(m_pszProfileName != NULL);

	BOOL bResult = WriteProfileString(cSection, cEntry, CString(lpsz) );
	delete[] lpsz;
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
