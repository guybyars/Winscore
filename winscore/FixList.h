//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// FixList.h: interface for the CFixList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIXLIST_H__A84C0723_6B54_11D3_B10C_0080C84B0161__INCLUDED_)
#define AFX_FIXLIST_H__A84C0723_6B54_11D3_B10C_0080C84B0161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFixList : public CObList  
{
public:
	void Purge();
	CFixList();
	int ReadRealtime( int iYear, int iMonth, int iDay, CString strfile);
	virtual ~CFixList();

};

#endif // !defined(AFX_FIXLIST_H__A84C0723_6B54_11D3_B10C_0080C84B0161__INCLUDED_)

