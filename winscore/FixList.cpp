//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FixList.cpp: implementation of the CFixList class.
//
//////////////////////////////////////////////////////////////////////

using namespace std;

#include "stdafx.h"
#include "winscore.h"
#include "FixList.h"
#include "Fix.h"
#include <fstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	MAXLINELEN   256

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFixList::CFixList()
{

}

CFixList::~CFixList()
	{
	Purge();
	}

void CFixList::Purge()
	{
	if( IsEmpty() !=0  )  return;
	POSITION pos = GetHeadPosition();
	while(pos!=NULL)
		{
		CFix* pcFix=(CFix*)GetNext(pos);
		if( pcFix ) delete pcFix;
		}
	RemoveAll();
	}


int CFixList::ReadRealtime( int iYear, int iMonth, int iDay, CString strfile)

	{
	CString strRecord;
	int nAdded=0;
	std::ifstream	cIfstream;
	char  cRecord[MAXLINELEN];

	cIfstream.open( strfile );
	if (cIfstream.is_open()==0)
		{
		return 0;
		}

	while( !cIfstream.eof() )
		{
		cIfstream.getline( cRecord, MAXLINELEN );
		CString strRecord(cRecord);
		strRecord.TrimLeft();
		if( strRecord.GetLength()<=0 ) continue;

		AddTail( new CFix( iYear, iMonth, iDay, strRecord ) );
		nAdded++;
		}// end loop on records

	cIfstream.close();
	return nAdded;
	}
