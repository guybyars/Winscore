//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Contestant.cpp: implementation of the CContestant class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Winscore.h"
#include "Contestant.h"
#include "wsclass.h"
#include <atlbase.h>
#include "global_prototypes.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const char TAB = '\t';


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CContestant, CContestant, VERSIONABLE_SCHEMA|CURRENT_CONTESTANT_SCHEME )


CContestant::CContestant()
	{
	m_iOptions=0;
	m_iSSANumber=0;
	m_fHandicap=0;
	m_fSpan=0;
	m_fWinglet=0;
	m_fWeight=0;
	}

CContestant::~CContestant()
{

}

void CContestant::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
		ar << m_strContestNo;
		ar << m_strLastName;
		ar << m_strMiddleName;
		ar << m_strFirstName;
		ar << m_strGlider;
		ar << m_strAddress1;
		ar << m_strAddress2;
		ar << m_strCity;
		ar << m_strState;
		ar << m_strZipcode1;
		ar << m_strZipcode2;
		ar << m_iSSANumber;
		ar << m_fHandicap;
		ar << (WORD)m_eClass;
		ar << (WORD)m_bGuest;
		ar << (WORD)m_iOptions;
		m_cGliderInfo.Serialize(ar);
		ar << m_fSpan;
		ar << m_fWinglet;
		ar << m_fWeight;
		}
	else
		{
		m_iOptions=0;
		UINT iSchema=ar.GetObjectSchema( );
		WORD w;
		if( iSchema==1 )
			{
			ar >> m_strContestNo;
			ar >> m_strLastName;
			ar >> m_strMiddleName;
			ar >> m_strFirstName;
			ar >> m_strGlider;
			ar >> m_strAddress1;
			ar >> m_strAddress2;
			ar >> m_strCity;
			ar >> m_strState;
			ar >> m_strZipcode1;
			ar >> m_strZipcode2;
			ar >> m_iSSANumber;
			ar >> m_fHandicap;
			ar >> w;
			m_eClass=(EClass)w;
			ar >> w;
			m_bGuest=w!=0?(true):(false);
			}
		else if( iSchema==2 )
			{
			ar >> m_strContestNo;
			ar >> m_strLastName;
			ar >> m_strMiddleName;
			ar >> m_strFirstName;
			ar >> m_strGlider;
			ar >> m_strAddress1;
			ar >> m_strAddress2;
			ar >> m_strCity;
			ar >> m_strState;
			ar >> m_strZipcode1;
			ar >> m_strZipcode2;
			ar >> m_iSSANumber;
			ar >> m_fHandicap;
			ar >> w;
			m_eClass=(EClass)w;
			ar >> w;
			m_bGuest=w!=0?(true):(false);
			ar >> w;
			m_iOptions=w;
			}
		else if( iSchema==3 )
			{
			ar >> m_strContestNo;
			ar >> m_strLastName;
			ar >> m_strMiddleName;
			ar >> m_strFirstName;
			ar >> m_strGlider;
			ar >> m_strAddress1;
			ar >> m_strAddress2;
			ar >> m_strCity;
			ar >> m_strState;
			ar >> m_strZipcode1;
			ar >> m_strZipcode2;
			ar >> m_iSSANumber;
			ar >> m_fHandicap;
			ar >> w;
			m_eClass=(EClass)w;
			ar >> w;
			m_bGuest=w!=0?(true):(false);
			ar >> w;
			m_iOptions=w;

			m_cGliderInfo.Serialize(ar);
			ar >> m_fSpan;
			ar >> m_fWinglet;
			ar >> m_fWeight;
			}
	}

}



CString CContestant::GetFormattedInfo()
{
	CString cTemp1, strClass;

	switch(m_eClass)
		{
	case eStandard:
		strClass="STD";
		break;
	case e15Meter:
		strClass="15M";
		break;
	case eClub:
		strClass="CLB";
		break;
	case eSports:
		strClass="SPT";
		break;
	case e18Meter:
		strClass="18M";
		break;
	case eOpen:
		strClass="OPN";
		break;	
	case eFAIHandicapped:
		strClass="FAI";
		break;		
	}


	cTemp1.Format(_T(" %-30s \t%-15s  \t%-3s   \t%3s"), 
	m_strLastName+_T("), ")+m_strFirstName+_T("  ")+m_strMiddleName,
	m_strGlider,
	m_strContestNo,
	strClass);

	return cTemp1;
}

CContestant::CContestant(CContestant *pcContestant)
{
	 m_strContestNo = pcContestant->m_strContestNo;
	 m_strLastName	= pcContestant->m_strLastName;
	 m_strMiddleName= pcContestant->m_strMiddleName;
	 m_strFirstName = pcContestant->m_strFirstName;
	 m_strGlider	= pcContestant->m_strGlider;
	 m_strAddress1	= pcContestant->m_strAddress1;
	 m_strAddress2	= pcContestant->m_strAddress2;
	 m_strCity		= pcContestant->m_strCity;
	 m_strState		= pcContestant->m_strState;
	 m_strZipcode1	= pcContestant->m_strZipcode1;
	 m_strZipcode2	= pcContestant->m_strZipcode2;
	 m_iSSANumber	= pcContestant->m_iSSANumber;
	 m_fHandicap	= pcContestant->m_fHandicap;
	 m_eClass		= pcContestant->m_eClass;
	 m_bGuest		= pcContestant->m_bGuest;
	 m_iOptions		= pcContestant->m_iOptions;
	 m_fSpan		= pcContestant->m_fSpan;
	 m_fWinglet		= pcContestant->m_fWinglet;
	 m_fWeight		= pcContestant->m_fWeight;
	 m_cGliderInfo	= pcContestant->m_cGliderInfo;
}



CContestant::CContestant(	
					CString &strContestNo,
					CString &strLastName,
					CString &strMiddleName,
					CString &strFirstName,
					CString &strGlider,
					CString &strAddress1,
					CString &strAddress2,
					CString &strCity,
					CString &strState,
					CString &strZipcode1,
					CString &strZipcode2,
					int iSSANumber,
					double fHandicap,
					EClass eClass,
					BOOL   bGuest,
					BOOL	bMotorized,
					BOOL    bWithdrew,
					BOOL    bIsInBGroup,
					BOOL    bForeign)
{
	 m_fSpan=0;
	 m_fWinglet=0;
	 m_fWeight=0;
	 m_iOptions=0;

	 m_strContestNo = strContestNo;
	 m_strLastName = strLastName;
	 m_strMiddleName = strMiddleName;
	 m_strFirstName = strFirstName;
	 m_strGlider = strGlider;
	 m_strAddress1 = strAddress1;
	 m_strAddress2 = strAddress2;
	 m_strCity = strCity;
	 m_strState = strState;
	 m_strZipcode1 = strZipcode1;
	 m_strZipcode2 = strZipcode2;
	 m_iSSANumber = iSSANumber;
	 m_fHandicap = fHandicap;
	 m_eClass = eClass;
	 m_bGuest = bGuest?(true):(false);
	 Motorized(bMotorized);
	 Withdrew(bWithdrew);
	 Foreign(bForeign);
	 SetBGroup(bIsInBGroup);

}

	
CString CContestant::SSANumberText()
{
CString cTemp;
cTemp.Format(_T("%d"),  m_iSSANumber);
return cTemp;
}

CString CContestant::NameText()
{
	return	m_strLastName	+_T(", ")+
			m_strFirstName	+_T("  ")+
			m_strMiddleName;
}

CString CContestant::CitizenText()
{
	CString cText;
	if( m_bGuest )
		cText=  _T("Guest");
	else
		cText= _T("Contestant");

	if( HasWithdrawn() ) cText+=_T(" (Withdrew)");

	if( IsForeign() ) cText+=_T(" (Foreign)");


	return cText;
}

CString CContestant::HandicapText()
{
CString cTemp=_T("");	
if ( GetClass(m_eClass).IsHandicapped() ) cTemp.Format(_T("%1.4f"),  m_fHandicap);
return cTemp;
}


CString CContestant::AddressText()
{

CString cTemp=_T("");
if( m_strZipcode1.GetLength()>0 && m_strZipcode2.GetLength()>0) 
	{
	cTemp.Format(_T("%s - %s"), m_strZipcode1,  m_strZipcode2);
	}
else if( m_strZipcode1.GetLength() >0 && m_strZipcode2.GetLength()==0) 
	{
	cTemp.Format(_T("%s"), m_strZipcode1);
	}

return	m_strAddress1+_T("  ")+m_strAddress2+_T("  ")+
		m_strCity+_T(" ")+ m_strState+_T("  ")+cTemp;
}


int CALLBACK CompareContestantName(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn)
{
	int iResult;
	CString str1, str2;
	int iR=0;
	switch( abs(iColumn)-1 )
		{
	case 0:
		iR=strcmp(  ((CContestant*)lParam1)->m_strLastName,
						((CContestant*)lParam2)->m_strLastName  );
		break;
	case 1:
		iR	= strcmp( ((CContestant*)lParam1)->m_strContestNo,
						((CContestant*)lParam2)->m_strContestNo  );
		if( iR==0 ) 
			iR=strcmp(  ((CContestant*)lParam1)->m_strLastName,
						((CContestant*)lParam2)->m_strLastName  );
		break;

	case 2:
		str1=((CContestant*)lParam1)->m_strGlider;
		str2=((CContestant*)lParam2)->m_strGlider;
		iR=strcmp(  str1.MakeLower(),
					str2.MakeLower() );
//		if( iR==0 ) 
//			iR=strcmp(  ((CContestant*)lParam1)->m_strLastName,
//						((CContestant*)lParam2)->m_strLastName  );
		break;
	case 3:
		iR=strcmp(  GetClass(((CContestant*)lParam1)->m_eClass).GetName(),
						GetClass(((CContestant*)lParam2)->m_eClass).GetName()  );	
		if( iR==0 ) 
			iR=strcmp(  ((CContestant*)lParam1)->m_strLastName,
						((CContestant*)lParam2)->m_strLastName  );
		break;
	case 4:
		iResult=strcmp(  ((CContestant*)lParam1)->HandicapText(),
					     ((CContestant*)lParam2)->HandicapText()  );
		iR= -iResult;
		break;
	case 5:
		iR=strcmp(  ((CContestant*)lParam1)->SSANumberText(),
						((CContestant*)lParam2)->SSANumberText() );
		if( iR==0 ) 
			iR=strcmp(  ((CContestant*)lParam1)->m_strLastName,
						((CContestant*)lParam2)->m_strLastName  );
		break;
	case 6:
		iR=strcmp(  ((CContestant*)lParam1)->CitizenText(),
						((CContestant*)lParam2)->CitizenText() );
		break;
	case 7:
		if( ((CContestant*)lParam1)->IsInBGroup() && ((CContestant*)lParam2)->IsInBGroup() )
			iR=0;
		else if( ((CContestant*)lParam1)->IsInBGroup() && !((CContestant*)lParam2)->IsInBGroup() )
			iR=1;
		else 
			iR= -1;
		break;
	case 8:
		iR=	strcmp(  ((CContestant*)lParam1)->AddressText(),
					 ((CContestant*)lParam2)->AddressText() );
	break;

	}
	return (iColumn<0)?(-iR):(iR);
}


CString CContestant::operator =(CContestant * pcContestant)
{
return m_strContestNo;
}


ostream& operator <<(ostream &os,  CContestant& cCont )
{
	PutString( os, cCont.m_strContestNo );
	PutString( os, cCont.m_strLastName );
	PutString( os, cCont.m_strMiddleName );
	PutString( os, cCont.m_strFirstName );
	PutString( os, cCont.m_strGlider );
	PutString( os, cCont.m_strAddress1 );
	PutString( os, cCont.m_strAddress2 );
	PutString( os, cCont.m_strCity );
	PutString( os, cCont.m_strState );
	PutString( os, cCont.m_strZipcode1 );
	PutString( os, cCont.m_strZipcode2 );
		 os<< cCont.m_iSSANumber << TAB
		   << cCont.m_fHandicap << TAB
		   << (WORD)cCont.m_eClass << TAB
		   << (WORD)cCont.m_bGuest << endl;
	return os;
}

CString ExtractString(istream &is)
    {
    size_t size;
    is >> size;
    if (size == 0)
        return _T("");
	else if( size>256 )
		{
		CString str;
		str.Format(_T("%d"),size);
		return str;
		}
    else
        {
        char chBegin;
        is >> chBegin;
        char *psz = new char[size+1];
        is.get(psz, size + 1, '\0');
        CString str(psz);
        delete [] psz;

        return str;
        }
    }

void PutString(ostream &os, const CString &str)
    {
	int iLen=str.GetLength();
    os << iLen << TAB;
    if ( iLen>0)
        os << '\"' << str << TAB;
    }

istream& operator >>(istream &is,  CContestant& cCont )
{
		WORD w;
	    cCont.m_strContestNo=ExtractString(is); 
		cCont.m_strLastName =ExtractString(is);
		cCont.m_strMiddleName =ExtractString(is);
		cCont.m_strFirstName =ExtractString(is);
		cCont.m_strGlider =ExtractString(is);
		cCont.m_strAddress1 =ExtractString(is);
		cCont.m_strAddress2 =ExtractString(is);
		cCont.m_strCity =ExtractString(is);
		cCont.m_strState =ExtractString(is);
		cCont.m_strZipcode1=ExtractString(is);
		cCont.m_strZipcode2=ExtractString(is);
		is  >> cCont.m_iSSANumber; 
		is  >> cCont.m_fHandicap; 
		is  >> w;
		cCont.m_eClass=(EClass)w;
		is  >> w;
		cCont.m_bGuest=(w==0)?false:true;

	return is;
}

CString	CContestant::GetText()
{
	CString num;
	CString str;
	str+=m_strContestNo+TAB;
	str+=GetClass(m_eClass).GetName()+TAB;
	str+=m_strLastName+TAB;
	str+=m_strMiddleName+TAB;
	str+=m_strFirstName+TAB;
	str+=m_strGlider;
	if( IsMotorized() ) str+=_T(" (Motorized)");
	str+=TAB;
	num.Format(_T("%lf"),m_fHandicap);
	str+=num+TAB;
	str+=CitizenText()+TAB;
	str+=_T("Gate ");
	str+=GetGateText()+TAB;
	num.Format(_T("%d"),m_iSSANumber);
	str+=num+TAB;
	if(m_strAddress1!=_T("")	||
		m_strAddress2!=_T("")	||
		m_strCity!=_T("")		||
		m_strState!=_T("")		||
		m_strZipcode1!=_T("")	||
		m_strZipcode2!=_T("")		)
		{
		str+=m_strAddress1+TAB;
		str+=m_strAddress2+TAB;
		str+=m_strCity+TAB;
		str+=m_strState+TAB;
		str+=m_strZipcode1+TAB;
		str+=m_strZipcode2+TAB;
		}
	else
		str+=TAB;

	str+=GetWeightText()+TAB;
	str+=GetSpanText()+TAB;
	str+=GetWingletText();

	return str;
}


bool CContestant::IsGuest()
{
    return m_bGuest;
}
void CContestant::Foreign(BOOL b)
	{
	if( b )
		m_iOptions|=CON_FOREIGN;
	else
		if( IsForeign() ) m_iOptions^=CON_FOREIGN;
	}

void CContestant::Motorized(BOOL b)
	{
	if( b )
		m_iOptions|=CON_MOTORIZED;
	else
		if( IsMotorized() ) m_iOptions^=CON_MOTORIZED;
	}

void CContestant::SetBGroup(BOOL b)
	{
	if( b )
		m_iOptions|=CON_IN_B_GATE;
	else
		if( IsInBGroup() ) m_iOptions^=CON_IN_B_GATE;
	}

void CContestant::Withdrew(BOOL b)
	{
	if( b )
		m_iOptions|=CON_WITHDRAWN;
	else
		{
		if( HasWithdrawn() ) m_iOptions^=CON_WITHDRAWN;
		}
	}


void CContestant::ExcludeFromFlightFiles(BOOL b)
	{
	if( b )
		m_iOptions|=CON_EXCLUDED;
	else
		if( IsExcludedFromFlightFiles() ) m_iOptions^=CON_EXCLUDED;
	}

CString CContestant::GetGateText()
	{
	return IsInBGroup()?("B"):("A");
	}

CString CContestant::GetWeightText()
	{
	if( m_cGliderInfo.IsNull() || !GetClass(m_eClass).IsHandicapped() ) return _T("");
	CString cTemp;

	cTemp.Format(_T("%4.0flb/%4.0f lb"),  m_cGliderInfo.m_fWeight, m_fWeight);

	return cTemp;
	}

CString CContestant::GetSpanText()
{
	if( m_cGliderInfo.IsNull() || !GetClass(m_eClass).IsHandicapped() ) return _T("");

	CString cTemp;

	cTemp.Format(_T("%3.1fm/%3.1fm"),  m_cGliderInfo.m_fSpan, m_fSpan);

	return cTemp;

}

CString CContestant::GetWingletText()
{
	if( m_cGliderInfo.IsNull() || !GetClass(m_eClass).IsHandicapped() ) return _T("");
	CString cTemp;

	cTemp.Format(_T("%4.3fm"),  m_fWinglet);

	return cTemp;

}




bool CContestant::GetXML(CXMLMgr &cMgr, IDispatch *pIdsp )
{
	MSXML2::IXMLDOMNodePtr pIDOMNode(pIdsp);
	MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
	TESTHR(cMgr.CreateChild( pIDOMNode, pIDOMChildNode, _T("Contestant")));

	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("CID"),		LPCSTR(m_strContestNo) ));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("FirstName"),	LPCSTR(m_strFirstName) ));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("MiddleName"),LPCSTR(m_strMiddleName)) );
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("LastName"),	LPCSTR(m_strLastName) )); 
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Glider"),	LPCSTR(m_strGlider)) );
	TESTHR(cMgr.CreateElementInt( pIDOMChildNode, _T("ClassID"),m_eClass));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Class"),		LPCSTR(GetClass(m_eClass).GetName()) ));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Motorized"),	IsMotorized()?_T("yes"):_T("") ));
	if( IsGuest() )
		{
 		TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Status"),		IsForeign()?_T("Foreign"):_T("Guest")) );
		}
	else
 	  TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Status"),		_T("Contestant")) );

	TESTHR(cMgr.CreateElementIntC( pIDOMChildNode, _T("SSANumber"),	m_iSSANumber));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Address1"),	LPCSTR(m_strAddress1)));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Address2"),	LPCSTR(m_strAddress2)));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("City"),		LPCSTR(m_strCity)));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("State"),		LPCSTR(m_strState)));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Zip"),		LPCSTR(m_strZipcode1)));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Zip2"),		LPCSTR(m_strZipcode2)));
	TESTHR(cMgr.CreateElementIntC( pIDOMChildNode, _T("Options"),	m_iOptions));

	if( GetClass(m_eClass).IsHandicapped() )
		{
		if( m_fHandicap>0.0 )
			{
			TESTHR(cMgr.CreateElementDbl( pIDOMChildNode, _T("Handicap"),	m_fHandicap));
			}

		if( !m_cGliderInfo.IsNull() )
			{
			if( m_fHandicap>0.0 )
				{
				double dPercent=100*(m_fHandicap-m_cGliderInfo.m_dHandicap)/m_cGliderInfo.m_dHandicap;
				if( abs(dPercent)>.001 ) 
					{
					CString strPercent;
					strPercent.Format("%3.1f",dPercent);
			        TESTHR(cMgr.CreateElement(pIDOMChildNode, _T("HandicapPercent"), LPCSTR(strPercent)));
					}
				}

			if( m_fWeight>0.0 ) 
				{
				TESTHR(cMgr.CreateElementDbl	( pIDOMChildNode, _T("Weight"),	m_fWeight));
				float fDeltaWeight=m_fWeight-m_cGliderInfo.m_fWeight;
				if( fDeltaWeight!=0.0 )
					{
					CString strDeltaWeight;
					strDeltaWeight.Format("%3.0f",fDeltaWeight);
					strDeltaWeight.TrimLeft();
					TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("WeightDelta"),		LPCSTR(strDeltaWeight)));
					}
				}
			if( m_fWinglet>0.0 ) 
				TESTHR(cMgr.CreateElementFlt	( pIDOMChildNode, _T("Winglet"),(float)m_fWinglet));
			if( m_fSpan>0.0 ) 
				TESTHR(cMgr.CreateElementFlt	( pIDOMChildNode, _T("Span"),	m_fSpan));
			
			MSXML2::IXMLDOMNodePtr pIDOMGliderInfoNode=NULL;
			TESTHR(cMgr.CreateChild( pIDOMChildNode, pIDOMGliderInfoNode, _T("gliderinfo") ));
			TESTHR(cMgr.CreateElementFlt	( pIDOMGliderInfoNode, _T("Weight"), m_cGliderInfo.m_fWeight));
			TESTHR(cMgr.CreateElementFlt	( pIDOMGliderInfoNode, _T("Span"),	 m_cGliderInfo.m_fSpan));
			TESTHR(cMgr.CreateElement 	( pIDOMGliderInfoNode, _T("Manufacturer"),	 LPCSTR(m_cGliderInfo.m_strManufacturer)));
			TESTHR(cMgr.CreateElementDbl	( pIDOMGliderInfoNode, _T("Handicap"),	 m_cGliderInfo.m_dHandicap));
			TESTHR(cMgr.CreateElement 	( pIDOMGliderInfoNode, _T("Model"),	 LPCSTR(m_cGliderInfo.m_strModel)));
			TESTHR(cMgr.CreateElement 	( pIDOMGliderInfoNode, _T("Notes"),	 LPCSTR(m_cGliderInfo.m_strNotes)));
			}
		}

	return true;
}


bool CContestant::GetSSAXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr pIDOMNode )
{

	MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
	TESTHR(cMgr.CreateChild( pIDOMNode, pIDOMChildNode, _T("Contestant")));

	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("CID"),		LPCSTR(m_strContestNo) ));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("FirstName"),	LPCSTR(m_strFirstName) ));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("MiddleName"),LPCSTR(m_strMiddleName)) );
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("LastName"),	LPCSTR(m_strLastName) )); 
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Glider"),	LPCSTR(m_strGlider)) );
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Class"),		LPCSTR(GetClass(m_eClass).GetName()) ));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Motorized"),	IsMotorized()?_T("yes"):_T("") ));
	if( IsGuest() )
		{
 		TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Status"),		IsForeign()?_T("Foreign"):_T("Guest") ));
		}
	else
 	  TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Status"),		_T("Contestant") ));

	TESTHR(cMgr.CreateElementIntC( pIDOMChildNode, _T("SSANumber"),	m_iSSANumber));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Address1"),	LPCSTR(m_strAddress1)));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Address2"),	LPCSTR(m_strAddress2)));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("City"),		LPCSTR(m_strCity)));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("State"),		LPCSTR(m_strState)));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Zip"),		LPCSTR(m_strZipcode2)));
	TESTHR(cMgr.CreateElement( pIDOMChildNode, _T("Zip2"),		LPCSTR(m_strZipcode2)));

	return true;
}


CContestant::CContestant(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pContestant, bool bSSA)
	{
	int iOptions=0;

	cMgr.SelectChildSTR( pContestant, _T("CID"), m_strContestNo);
	cMgr.SelectChildSTR( pContestant, _T("FirstName"), m_strFirstName);
	cMgr.SelectChildSTR( pContestant, _T("MiddleName"),m_strMiddleName);
	cMgr.SelectChildSTR( pContestant, _T("LastName"),	m_strLastName);
	cMgr.SelectChildSTR( pContestant, _T("Glider"), m_strGlider);
	if( !bSSA )
		{
		GET_XML_INT	( cMgr, pContestant, _T("ClassID"), EClass, m_eClass, eStandard);
		}
	else
		{

		m_eClass=eStandard;
		CString strClass;
		cMgr.SelectChildSTR( pContestant, _T("Class"), strClass);

		strClass.MakeLower();
		if( (strClass.Find(_T("15"))>=0 && strClass.Find(_T("std"))>=0) || strClass.Find("fai")>=0 ) 
			{
			// Search for a 15/std class in the user defined section.
			bool bFound=false;
   			for( int j=0; j<NUMCLASSES; j++ )
    			{
				if( !GetClass(j).IsActive() ) continue;
				CString strCheckName=GetClass(j).GetName();
				strCheckName.MakeLower();
				if( strCheckName.Find(_T("15"))>=0 && strCheckName.Find(_T("std"))>=0)
					{
					// there is a 15/std class defined in Winscore, use it.
					m_eClass=GetClass(j).GetType();
					bFound=true;
					break;
					}
				}
			  if( !bFound ) m_eClass=eFAIHandicapped;
			}
		else if( strClass.Find(_T("15"))>=0 ) 
			{
			m_eClass=e15Meter;
			}
		else if( strClass.Find(_T("stand"))>=0 || strClass.Find(_T("std"))>=0) 
			{
			m_eClass=eStandard;
			}
		else if( strClass.Find(_T("18"))>=0 ) 
			{
			m_eClass=e18Meter;
			}
		else if( strClass.Find(_T("club"))>=0 ) 
			{
			m_eClass=eClub;
			}
		else if( strClass.Find(_T("sport"))>=0 ) 
			{
			m_eClass=eSports;
			}
		else if( strClass.Find(_T("open"))>=0 || strClass.Find(_T("opn"))>=0) 
			{
			m_eClass=eOpen;
			}
		else
			{
   			for( int j=0; j<NUMCLASSES; j++ )
    			{
				if( !GetClass(j).IsActive() ) continue;
				CString strCheckName=GetClass(j).GetName();
				strCheckName.MakeLower();
				strCheckName.TrimRight(" class");
				strClass.MakeLower();
				int ipos=strClass.Find(strCheckName);
				if( ipos>=0 )
					{
					m_eClass=GetClass(j).GetType();
					break;
					}					
    			}
		}

//		Need to work on the glider name to get it under 10 char.
//		Some pilots get very verbose when entering their glider name.
//		The following attempts to strip out the manufacturer name.
//		Lengthy names on the scoresheet make it very crowed and messy.
		int iLen=m_strGlider.GetLength();
		if( iLen>10 ) 
			{
			CString strSub;
			CString strGlider=m_strGlider;
			strGlider.MakeLower();
			int iPos=strGlider.Find(_T("cher ")); //Schleicher
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("rth ")); //Schempp hirth
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-4);

			iPos=strGlider.Find(_T("ider ")); //Rolladen Schneider
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("bau ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-4);

			iPos=strGlider.Find(_T("zer ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-4);

			iPos=strGlider.Find(_T("land ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("irks ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("ance ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("cija ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("cjai ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("iaca ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("gel ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-4);

			iPos=strGlider.Find(_T("eder ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("dnik ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("vion ")); 
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			iPos=strGlider.Find(_T("gmbh ")); //GMBH
			if( iPos>0 ) 
				strSub=m_strGlider.Right(iLen-iPos-5);

			if( strSub.GetLength()>0 ) m_strGlider=strSub.Left(15);
			}
		}
	GET_XML_DBL	( cMgr, pContestant, _T("Handicap"), double, m_fHandicap, 0.0);
	
	//Options coming from SSA are unreliable, selectively check them.
	m_iOptions=0;
	GET_XML_INT	( cMgr, pContestant, _T("Options"),  int, iOptions, 0);
	if( iOptions & CON_MOTORIZED	)	m_iOptions|=CON_MOTORIZED;
	if( iOptions & CON_WINGLETS		) 	m_iOptions|=CON_WINGLETS;
	if( iOptions & CON_ROOTFARINGS	) 	m_iOptions|=CON_ROOTFARINGS;
	if( iOptions & CON_TURBULATION	) 	m_iOptions|=CON_TURBULATION;
	if( iOptions & CON_WITHDRAWN	) 	m_iOptions|=CON_WITHDRAWN;
	

	CString strStatus;
	cMgr.SelectChildSTR( pContestant, _T("Status"), strStatus);
	m_bGuest=strStatus.Find(_T("Guest"))>=0 || strStatus.GetLength()==0;
	if( strStatus.Find(_T("Foreign"))>=0 )
		{
		m_bGuest=true;
		Foreign(true);
		}
	GET_XML_INT ( cMgr, pContestant, _T("SSANumber"), int, m_iSSANumber, 0);
	cMgr.SelectChildSTR( pContestant, _T("Address1"),	m_strAddress1);
	cMgr.SelectChildSTR( pContestant, _T("Address2"),	m_strAddress2);
	cMgr.SelectChildSTR( pContestant, _T("City"),		m_strCity);
	cMgr.SelectChildSTR( pContestant, _T("State"),		m_strState);
	cMgr.SelectChildSTR( pContestant, _T("Zip"),		m_strZipcode1);
	cMgr.SelectChildSTR( pContestant, _T("Zip2"),		m_strZipcode2);

	GET_XML_DBL	( cMgr, pContestant, _T("Weight"), float, m_fWeight,		0.0);
	GET_XML_DBL	( cMgr, pContestant, _T("Winglet"),double, m_fWinglet,	    0.0);
	GET_XML_DBL	( cMgr, pContestant, _T("Span"),	 float, m_fSpan,		0.0);

	MSXML2::IXMLDOMNodePtr pGliderInfo;
	TESTHR(cMgr.SelectChild(_T("gliderinfo"), pContestant, pGliderInfo));
	if( pGliderInfo!=NULL )
		{
		m_cGliderInfo.m_bNull		=false;
		GET_XML_DBL	( cMgr, pGliderInfo, _T("Weight"),   float,	m_cGliderInfo.m_fWeight,	0.0);
		GET_XML_DBL	( cMgr, pGliderInfo, _T("Span"),	 float,	m_cGliderInfo.m_fSpan,		0.0);
		GET_XML_DBL	( cMgr, pGliderInfo, _T("Handicap"), double,m_cGliderInfo.m_dHandicap,	0.0);

		cMgr.SelectChildSTR ( pGliderInfo, _T("Manufacturer"),	m_cGliderInfo.m_strManufacturer);
		cMgr.SelectChildSTR ( pGliderInfo, _T("Model"),			m_cGliderInfo.m_strModel);
		cMgr.SelectChildSTR ( pGliderInfo, _T("Notes"),			m_cGliderInfo.m_strNotes);
		}

	}

void CContestant::UpdateHandicap(CGliderInfoList &cGIList)
		{
		CGliderInfo *pGliderInfo=NULL;

		if( m_cGliderInfo.IsNull() )
			{
			// No glider information came down from the SSA, do the best we can,
			// all we have is the glider name, try to find that in the handicap list
			pGliderInfo=cGIList.GetModel(m_strGlider, IsMotorized(),m_fSpan);
			}
		else
			//Glider information came down from the SSA. Look up based on manufacturer and model.
			{
			pGliderInfo=cGIList.Get(	m_cGliderInfo.m_strManufacturer,
										m_cGliderInfo.m_strModel, 
										m_cGliderInfo.m_strNotes );
			if( pGliderInfo==NULL )
				{
				// No match, omit the notes
				pGliderInfo=cGIList.Get(	m_cGliderInfo.m_strManufacturer,
											m_cGliderInfo.m_strModel	);
				}
			if( pGliderInfo==NULL )
				{
				//Still no match, drop back to the glider name only.
				pGliderInfo=cGIList.GetModel(m_strGlider, IsMotorized(),m_fSpan);
				}
			}

		if( pGliderInfo )
			{
			//Found a match in the handicap list, use this information from it:
			m_cGliderInfo=*pGliderInfo;

			// Now tweak the handicap based on the weight and span (ignore winglets, turbulators... etc for now.)
			if( m_eClass==eSports || m_eClass==eClub )
				{
				m_fHandicap=GetAdjustedHandicap(	pGliderInfo, 
													m_fWeight, 
													m_fSpan, 
													CheckOption(CON_ROOTFARINGS) , 
													CheckOption(CON_TURBULATION) , 
													CheckOption(CON_WINGLETS)  );
				if( m_eClass==eClub )
					{
					m_fHandicap=max( m_fHandicap, 0.898);
					m_fHandicap=min( m_fHandicap, 1.020);
					}
				}
			else
				{
				m_fWeight=0.0;
				m_fSpan=0.0;
				m_fHandicap=pGliderInfo->m_dHandicap;
				if( m_eClass==eStandard ) m_fHandicap=min( m_fHandicap, .950);
				}

			if( pGliderInfo->m_strNotes.Find("M")>=0 )
				{
				Motorized(true);
				}
			}
		else
			{
			// Lookup into the handicap list failed.  No way to know what we have here. Force it to be 0.0!
			AfxMessageBox(_T("Could not determine handicap for: ") + m_strContestNo, MB_OK | MB_ICONINFORMATION);
			m_fHandicap=0.0;
			}
		}

