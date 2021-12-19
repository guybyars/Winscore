// GliderInfo.cpp: implementation of the CGliderInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "winscore.h"
#include "xmlmgr.h"
#include "GliderInfo.h"
#include "global_prototypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CGliderInfo, CGliderInfo, VERSIONABLE_SCHEMA | 1)

CGliderInfo::CGliderInfo()
	{
	m_bNull=true;
	}

CGliderInfo::CGliderInfo(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pGlider)
	{
	cMgr.SelectChildSTR( pGlider, _T("Manufacturer"), m_strManufacturer);
	cMgr.SelectChildSTR( pGlider, _T("Model"), m_strModel);
	cMgr.SelectChildSTR( pGlider, _T("Notes"),m_strNotes);
	cMgr.SelectChildSTR( pGlider, _T("Modified"),m_strModified);
	GET_XML_DBL	( cMgr, pGlider, _T("Span"), float, m_fSpan, 0.0);
	GET_XML_DBL	( cMgr, pGlider, _T("Handicap"), double, m_dHandicap, 0.0);
	GET_XML_DBL	( cMgr, pGlider, _T("Weight"), float, m_fWeight, 0.0);
	m_bNull=false;
	}


CGliderInfo::CGliderInfo(	CString strManufacturer,
							CString strModel,
							CString strNotes,
							float fSpan,
							double dHandicap,
							float fWeight	):
	m_strManufacturer(strManufacturer),
	m_strModel(strModel),
	m_strNotes(strNotes),
	m_fSpan(fSpan),
	m_dHandicap(dHandicap),
	m_fWeight(fWeight)
	{
	m_bNull=false;
	}

CGliderInfo::~CGliderInfo()
{

}
bool CGliderInfo::operator ==(CGliderInfo &cInfo)
		{
		if( m_strManufacturer   !=cInfo.m_strManufacturer ) return false;
		if( m_strModel			!=cInfo.m_strModel ) return false;
		if( m_strNotes			!=cInfo.m_strNotes ) return false;
		if( m_fSpan				!=cInfo.m_fSpan ) return false;
		if( m_dHandicap			!=cInfo.m_dHandicap ) return false;
		if( m_fWeight			!=cInfo.m_fWeight ) return false;
		if( m_bNull				!=cInfo.m_bNull ) return false;
		return true;
		}

CGliderInfo& CGliderInfo::operator =(CGliderInfo &cInfo)
	{
	m_strManufacturer	=cInfo.m_strManufacturer;
	m_strModel			=cInfo.m_strModel;
	m_strNotes			=cInfo.m_strNotes;
	m_fSpan				=cInfo.m_fSpan;
	m_dHandicap			=cInfo.m_dHandicap;
	m_fWeight			=cInfo.m_fWeight;
	m_bNull				=cInfo.m_bNull;
	return cInfo;
	}

bool CGliderInfo::IsNull()
{
return m_bNull;
}

bool CGliderInfo::GetXML(CXMLMgr &cMgr, IDispatch *pIdsp )
{
	MSXML2::IXMLDOMNodePtr pIDOMNode(pIdsp);
	MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
	TESTHR(cMgr.CreateChild( pIDOMNode, pIDOMChildNode, _T("Aircraft")));

	TESTHR(cMgr.CreateElement 	 ( pIDOMChildNode, _T("Manufacturer"),	 LPCSTR(m_strManufacturer)));
	TESTHR(cMgr.CreateElement 	 ( pIDOMChildNode, _T("Model"),	 LPCSTR(m_strModel)));
	TESTHR(cMgr.CreateElementFlt ( pIDOMChildNode, _T("Span"),	 m_fSpan));
	TESTHR(cMgr.CreateElementFlt ( pIDOMChildNode, _T("Weight"), m_fWeight));
	TESTHR(cMgr.CreateElement 	 ( pIDOMChildNode, _T("Notes"),	 LPCSTR(m_strNotes),true,1,true));
	TESTHR(cMgr.CreateElementDbl ( pIDOMChildNode, _T("Handicap"),	 m_dHandicap));
	TESTHR(cMgr.CreateElement 	 ( pIDOMChildNode, _T("Modified"),	 LPCSTR(m_strModified),true,1,true));

	return true;
}