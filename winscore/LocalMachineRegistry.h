// LocalMachineRegistry.h: interface for the CLocalMachineRegistry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCALMACHINEREGISTRY_H__13D0D815_023B_11D3_AC38_00C04FA33C8D__INCLUDED_)
#define AFX_LOCALMACHINEREGISTRY_H__13D0D815_023B_11D3_AC38_00C04FA33C8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLocalMachineRegistry  
	{
	public:
		CLocalMachineRegistry(HKEY	m_hRootKey);
		virtual ~CLocalMachineRegistry();
	public:
		void SetRegistryKey(CString cRegistryKey, CString cProfile);
		HKEY GetAppRegistryKey();
		HKEY GetSectionKey(CString cSection);
		UINT GetProfileInt(CString cSection, CString cEntry,int nDefault);
		CString GetProfileString(CString cSection, CString cEntry, CString cDefault= "");
		BOOL GetProfileBinary(CString cSection, CString cEntry,BYTE** ppData, UINT* pBytes);
		BOOL WriteProfileInt(CString cSection, CString cEntry,int nValue);
		BOOL WriteProfileString(CString cSection, CString cEntry, CString cValue);
		BOOL WriteProfileBinary(CString cSection, CString cEntry,LPBYTE pData, UINT nBytes);
	private:
		CString* m_pszRegistryKey;   // used for registry entries
		CString* m_pszProfileName;   // default based on app name
		HKEY	m_hRootKey;

	};

#endif // !defined(AFX_LOCALMACHINEREGISTRY_H__13D0D815_023B_11D3_AC38_00C04FA33C8D__INCLUDED_)
