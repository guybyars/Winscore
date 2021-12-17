#include "stdafx.h"
#include "Warning.h"


CWarning::CWarning(EWarningType eType, int iPenalty, CString strWarning)
	{
	m_eType		=eType;
	m_strWarning=strWarning;
	m_bActive	=true;
	m_iPenalty	=iPenalty;
	}

CWarning::CWarning(CString strWarningText)
	{
	char cActive;
	char cType[4];
	char cWarn[256];
	char cDum[4];
	memset(cWarn,0,256);

	sscanf_s(LPCTSTR(strWarningText), ("%1c %4c %4c%4i %256c"), &cActive, 1, &cType, 4, &cDum, 4, &m_iPenalty, &cWarn, 256);
	m_strWarning=CString(cWarn);
	m_bActive=cActive=='A';
	m_eType=eUnknownWarning;

	for(int iWarn=0; iWarn<NWARNINGTYPES; iWarn++ )
		{
		if( strncmp( cType, strWarningTypes[iWarn][0], 4) ==0 )
			{
			m_eType=(EWarningType)iWarn;
			break;
			}
		}
	if( m_eType==eUnknownWarning )
		{
		//Legacy, do the best we can...
		m_eType=eInformation;
		m_strWarning=strWarningText;
		m_iPenalty=0;
		m_bActive=true;
		}
	}

CString	CWarning::GetText()
	{
	CString strText;
	strText.Format("%s %s PEN=%4d %s",
					m_bActive?"A":"C",
					strWarningTypes[m_eType][0],
					m_iPenalty,
					m_strWarning);
	return strText;
	}

CString	CWarning::GetTypeText()
	{
	return strWarningTypes[m_eType][1];
	}


CWarning::~CWarning(void)
{
}

