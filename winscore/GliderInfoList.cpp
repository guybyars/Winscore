// GliderInfoList.cpp: implementation of the CGliderInfoList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "winscore.h"
#include "winscoredefs.h"
#include "GliderInfoList.h"
#include "GliderInfo.h"
#include <fstream>
#include <string.h>
#include <stdio.h>
#include "xmlmgr.h"
#include "Global_Prototypes.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		MAXLINELEN	256

bool SortDescendingString(const CString& s1, 
                          const CString& s2);


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGliderInfoList::CGliderInfoList()
{



}

CGliderInfoList::~CGliderInfoList()
	{
	if( IsEmpty() !=0  )  return;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
		if( pcInfo ) delete pcInfo;
		}
	RemoveAll();
	}

bool CGliderInfoList::operator ==(CGliderInfoList &cInfoList)
	{
	if( GetCount() !=cInfoList.GetCount() ) return false;

	if( m_strTitle != cInfoList.m_strTitle ) return false;

	POSITION pos1 = GetHeadPosition();
	POSITION pos2 = cInfoList.GetHeadPosition();
	while(pos1!=NULL && pos2!=NULL)
		{
		CGliderInfo* pcInfo1=(CGliderInfo*)GetNext(pos1);
		CGliderInfo* pcInfo2=(CGliderInfo*)cInfoList.GetNext(pos2);
		if( pcInfo1==NULL || pcInfo2==NULL ) return false;
		if ( *pcInfo1==*pcInfo2 ) continue;
		return false;
		}
	return true;
	}

bool CGliderInfoList::ExportXML(CString strFileName,bool bSilent)
	{
	if( GetCount()==0 ) return true;

	try
		{
		CXMLMgr cMgr;
		cMgr.Initlize();
		cMgr.CreateXMLFileHeader();

		cMgr.CreateRoot(_T("HandicapList"));
		MSXML2::IXMLDOMNodePtr pRoot=  cMgr.GetRoot();
		MSXML2::IXMLDOMNodePtr pIDOMNode(pRoot);

		MSXML2::IXMLDOMElementPtr peRoot=cMgr.GetRoot();
		cMgr.SetAttribute(peRoot, "Title", m_strTitle);


		MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
		TESTHR(cMgr.CreateChild( pIDOMNode, pIDOMChildNode, _T("Codes")));
		for( int i=0; i<m_strCodeIDs.GetCount(); i++ )
			{
		    MSXML2::IXMLDOMNodePtr pIDOMCodeNode=NULL;
			MSXML2::IXMLDOMNodePtr pIDOMNode(pIDOMChildNode);
			TESTHR(cMgr.CreateChild( pIDOMNode, pIDOMCodeNode, _T("Code")));

			TESTHR(cMgr.CreateElement 	 ( pIDOMCodeNode, _T("ID"),	 LPCSTR(m_strCodeIDs.GetAt(i))));
			TESTHR(cMgr.CreateElement 	 ( pIDOMCodeNode, _T("Description"),	 LPCSTR(m_strCodeDescriptions.GetAt(i))));
			}

		MSXML2::IXMLDOMNodePtr pIDOMAircraftListNode=NULL;
		TESTHR(cMgr.CreateChild( pIDOMNode, pIDOMAircraftListNode, _T("AircraftList")));

		POSITION pos = GetHeadPosition();
		while(pos!=NULL)
			{
			CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
			pcInfo->GetXML(cMgr, pIDOMAircraftListNode);
			}

		TESTHR(cMgr.Save(strFileName) );

		}
    catch (...)
		{
		return false;
		}

	return true;
	}

int CGliderInfoList::ImportXML(CString strFileName,bool bSilent)
{
	int nLinesRead=0;
	Purge();
	try
		{

		CXMLMgr cMgr;
		cMgr.Initlize();

		bool bLoad=(cMgr.Load(strFileName));
		if( !bLoad ) 
			{
			if( !bSilent ) AfxMessageBox(cMgr.m_strLastError);
			return 0;
			}

		CString strRoot=cMgr.GetNodeName(cMgr.GetRoot());
		if(strRoot!=_T("HandicapList") )
			{
			if( !bSilent ) AfxMessageBox( _T("Not a valid Handicap List XML file."),  MB_ICONSTOP   );
			return 0;
			}
		m_strTitle=cMgr.GetAttribute(cMgr.GetRoot(), _T("Title"));


		MSXML2::IXMLDOMNodePtr pCodesNode;
		HRESULT HR= cMgr.SelectChild(_T("Codes"),cMgr.GetRoot(), pCodesNode);
		if( pCodesNode==NULL ) return 0;
		MSXML2::IXMLDOMNodeListPtr pCodeNode=cMgr.SelectNodeList(_T("Code"),pCodesNode);
		if( pCodeNode==NULL ) return 0;
		long codelength=0;
		TESTHR(pCodeNode->get_length(&codelength));
		if( codelength>0 ) 
			{
			for( int i=0; i<codelength; i++ )
				{
				MSXML2::IXMLDOMNodePtr pCode;
				TESTHR(pCodeNode->get_item(i,&pCode));
				if( pCode==NULL ) continue;

				CString strID;
				cMgr.SelectChildSTR( pCode, _T("ID"), strID);
				m_strCodeIDs.Add(strID);

				CString strDescription;
				cMgr.SelectChildSTR( pCode, _T("Description"), strDescription);
				m_strCodeDescriptions.Add(strDescription);
				}
			}


		MSXML2::IXMLDOMNodePtr pAircraftListNode;
		HR= cMgr.SelectChild(_T("AircraftList"),cMgr.GetRoot(), pAircraftListNode);
		if( pAircraftListNode==NULL ) return 0;


		MSXML2::IXMLDOMNodeListPtr pAircraftList=cMgr.SelectNodeList(_T("Aircraft"),pAircraftListNode);
		if( pAircraftList==NULL ) return 0;

		long length=0;
		TESTHR(pAircraftList->get_length(&length));
		if( length==0 ) return 0;

		for( int i=0; i<length; i++ )
			{
			MSXML2::IXMLDOMNodePtr pAircraft;
			TESTHR(pAircraftList->get_item(i,&pAircraft));
			if( pAircraft==NULL ) continue;

			CString strNode=cMgr.GetNodeName(pAircraft);
			if( strNode.Compare(_T("Aircraft"))!=0 ) continue;

			CGliderInfo* pGliderInfo=NULL;
			try
				{
				pGliderInfo = new CGliderInfo( cMgr, pAircraft);
				if( pGliderInfo->m_dHandicap==0.0 ) 
					{
					delete pGliderInfo;
					continue;
					}
				}
			catch(...)
				{
				ASSERT(FALSE);
				if(pGliderInfo) delete pGliderInfo;
				continue;
				}
			AddTail( pGliderInfo );
			nLinesRead++;
			}
		}
	catch (...)
		{
		CString strError;
		strError.Format(_T("Error Reading: %s, handicap list not read."), strFileName);
		if( !bSilent ) AfxMessageBox( strError,  MB_ICONSTOP   );
		}

	return nLinesRead;
}


int CGliderInfoList::ReadFromFile(CString strFileName)
{
	TCHAR seps[]   = _T("\t");
	char  cRecord[MAXLINELEN] ;
	int m_nLinesRead=0;
	ifstream	cIfstream;
	bool	bHeaderFound=false;

	//  Use the ifstream class to do I/O on the ascii files.
	cIfstream.open(strFileName, ios::binary  );
	if (cIfstream.is_open()==0)
		{
		return 0;
		}
	
	while( !cIfstream.eof() )
		{
		cIfstream.getline( cRecord, MAXLINELEN );
		if( cIfstream.fail() ) break;
		CString strRecord(cRecord);
		if( strRecord.GetLength()<=0 ) continue;

		if( !bHeaderFound &&
			strRecord.Find(_T("Manufacturer"))>=0 &&
			strRecord.Find(_T("Model"))>=0		&&
			strRecord.Find(_T("Notes"))>=0		&&
			strRecord.Find(_T("Span"))>=0		&&
			strRecord.Find(_T("Handicap"))>=0	&&
			strRecord.Find(_T("Weight"))>=0			)
			{
			bHeaderFound=true;
			continue;
			}
		if(!bHeaderFound ) continue;

	   int iPos=strRecord.Find( _T("\t") );
	   CString strMan = strRecord.Left(iPos);
	   strRecord=strRecord.Right(strRecord.GetLength()-iPos-1);

	   iPos=strRecord.Find( _T("\t") );
	   CString strMod = strRecord.Left(iPos);
	   strRecord=strRecord.Right(strRecord.GetLength()-iPos-1);

	   iPos=strRecord.Find( _T("\t") );
	   CString strNot = strRecord.Left(iPos);
	   strRecord=strRecord.Right(strRecord.GetLength()-iPos-1);

	   iPos=strRecord.Find( _T("\t") );
	   CString strSpn = strRecord.Left(iPos);
	   strRecord=strRecord.Right(strRecord.GetLength()-iPos-1);

	   iPos=strRecord.Find( _T("\t") );
	   CString strHnd = strRecord.Left(iPos);
	   strRecord=strRecord.Right(strRecord.GetLength()-iPos-1);

	   CString strWgt = strRecord;
 
		CGliderInfo *pGliderInfo = new CGliderInfo( strMan,
													strMod,
													strNot,
													(float)atof(strSpn),
													(float)atof(strHnd),
													(float)atof(strWgt)	);

		AddTail( pGliderInfo );
		m_nLinesRead++;
	}

			
	cIfstream.close();

	return m_nLinesRead;
}

bool CGliderInfoList::IsManInList(CStringArray &strArray, CString strManu)
	{
	if( strArray.GetSize() ==0  )  return false;
	for( int i=0; i<strArray.GetSize(); i++ )
		{
		if( strManu.CompareNoCase(strArray[i])==0 ) return true;
		}
	return false;
	}


int CGliderInfoList::GetManufacturers(CStringArray &strArray, CString strNotes, float fMaxSpan)
{
	int nList=0;
	if( IsEmpty() !=0  )  return 0;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);

		if( (strNotes.GetLength()>0 && pcInfo->m_strNotes.Find(strNotes)<0) ) continue;

		if( pcInfo->m_fSpan<=fMaxSpan					 &&
			!IsManInList(strArray, pcInfo->m_strManufacturer)  )
			{
			strArray.Add(pcInfo->m_strManufacturer);
			nList++;
			}
		}
	return nList;
}

int CGliderInfoList::GetModels(CString strManufacturer, CPtrArray &ptrArray)
{
	int nList=0;
	if( IsEmpty() !=0  )  return 0;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
		if( strManufacturer == pcInfo->m_strManufacturer )
			{
			ptrArray.Add(pcInfo);
			nList++;
			}
		}
	return nList;
}

int CGliderInfoList::GetModels(CString strManufacturer, CStringArray &strArray, CString strNotes, float fMaxSpan)
{
	int nList=0;
	if( IsEmpty() !=0  )  return 0;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
		
		if( (strNotes.GetLength()>0 && pcInfo->m_strNotes.Find(strNotes)<0) ) continue;

		if( pcInfo->m_fSpan<=fMaxSpan					 &&
			strManufacturer == pcInfo->m_strManufacturer &&
			!IsManInList(strArray, pcInfo->m_strModel)  )
			{
			strArray.Add(pcInfo->m_strModel);
			nList++;
			}
		}
	return nList;
}

CGliderInfo* CGliderInfoList::GetModel( CString strModel, BOOL bMotorPreference, float fSpan)
	{
	int nList=0;
	CString strPrevious;
	if( IsEmpty() !=0  )  return NULL;
	strModel.TrimRight();
	strModel.TrimLeft();
	strModel.MakeLower();
	CPtrArray cPtrModels;

	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
		CString strTest=pcInfo->m_strModel;
		strTest.TrimLeft();
		strTest.TrimRight();
		if( strModel.CompareNoCase(strTest)==0 )
			{
			cPtrModels.Add(pcInfo);
			}
		}
	if( cPtrModels.GetCount()==1 ) 
		{
		return (CGliderInfo*)cPtrModels.GetAt(0);
		}
	else if( cPtrModels.GetCount()>1 ) 
		{
		if( bMotorPreference ) 
			{
			for( int i=0; i<cPtrModels.GetCount(); i++ )
				{
				CGliderInfo* pcInfo=(CGliderInfo*)cPtrModels.GetAt(i);
				if( pcInfo->m_strNotes.Find("M")>=0 ) 
					{
					return pcInfo;
					}
				}
			}
		else if( fSpan>0.0 ) 
			{
			for( int i=0; i<cPtrModels.GetCount(); i++ )
				{
				CGliderInfo* pcInfo=(CGliderInfo*)cPtrModels.GetAt(i);
				if( abs(pcInfo->m_fSpan-fSpan)<.5 ) 
					{
					return pcInfo;
					}
				}
			}
		// No help from span or motor, just return the first one.
		return (CGliderInfo*)cPtrModels.GetAt(0);
		}
	else
		{
		// Trap garbage in
		if( strModel.GetLength()<3 ) return NULL;


	    // No Match, Try again less strict search
		pos = GetHeadPosition();
		while(pos!=NULL)
			{
			CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
			CString strTest=pcInfo->m_strModel;
			strTest.TrimLeft();
			strTest.TrimRight();
			strTest.MakeLower();
			if( strTest.Find(strModel)>=0 )
				{
				return pcInfo;
				}
			}

		//Try a different approach.  See if a manufacture is present in the model name.  If so, remove it, then look for the 
		//matches under that manufacturer.
		pos = GetHeadPosition();
		while(pos!=NULL)
			{
			CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
			CString strTest=pcInfo->m_strManufacturer;
			strTest.TrimLeft();
			strTest.TrimRight();
			strTest.MakeLower();
			int iPos=strModel.Find(strTest);
			if( iPos>=0 )
				{
				// Found the manufacturer  Now remove that from the name, and check for model matches.
				CString strModel2;
				strModel2=strModel.Right(strModel.GetLength()-strTest.GetLength());
				strModel2.TrimLeft();
				CGliderInfo *pcInfo2 =GetModel( strModel2);
				if( pcInfo2 ) return pcInfo2;
				}
			}

		// Try again with fewer characters.
		strModel=strModel.Left(strModel.GetLength()-1);
		while( strModel.GetLength()>2 )
			{
			pos = GetHeadPosition();
			while(pos!=NULL)
				{
				CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
				CString strTest=pcInfo->m_strModel;
				strTest.TrimLeft();
				strTest.TrimRight();
				strTest.MakeLower();
				if( strTest.Find(strModel)>=0 )
					{
					return pcInfo;
					}
				}
			strModel=strModel.Left(strModel.GetLength()-1);
			}

		return NULL;
		}
	};

CGliderInfo* CGliderInfoList::Get(CString strManufacturer, CString strModel,  CString strNotes)
	{
	int nList=0;
	if( IsEmpty() !=0  )  return NULL;

	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
		if( strModel==pcInfo->m_strModel &&	strManufacturer==pcInfo->m_strManufacturer )
			{
			if( strNotes.GetLength()==0 )
			// Ignore the notes, just return this one
				return pcInfo;
			else
				{
				if( strNotes==pcInfo->m_strNotes )
					{
					return pcInfo;
					}
				}
			}
		}
	return NULL;
	}


void CGliderInfoList::Purge(void)
	{
	if( IsEmpty() !=0  )  return;

	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
		delete pcInfo;
		}
	RemoveAll();
	m_strTitle="";
	m_strCodeIDs.RemoveAll();
	m_strCodeDescriptions.RemoveAll();
	}


int CGliderInfoList::GetAllModels(CStringArray &strArray)
{
	int nList=0;
	if( IsEmpty() !=0  )  return 0;
	POSITION pos = GetHeadPosition();
	CGliderInfo* pcPrevInfo=NULL;
	while(pos!=NULL)
		{
		CGliderInfo* pcInfo=(CGliderInfo*)GetNext(pos);
		if (pcPrevInfo==NULL || pcInfo->m_strModel!=pcPrevInfo->m_strModel )
			{
			strArray.Add(pcInfo->m_strModel);
			nList++;
			}
		pcPrevInfo=pcInfo;
		}

// Sort the CStringArray
	std::sort(strArray.GetData(), 
			  strArray.GetData() + strArray.GetSize(),
			  SortDescendingString);

	return nList;
}
bool SortDescendingString(const CString& s1, 
                          const CString& s2)
{
   return s1 > s2;
}

