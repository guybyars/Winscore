//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Averager.h: interface for the CAverager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVERAGER_H__2CDDD6D8_F7E8_11D2_891F_00C04F79ECB9__INCLUDED_)
#define AFX_AVERAGER_H__2CDDD6D8_F7E8_11D2_891F_00C04F79ECB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAverager  
{
public:
	CAverager(int inPoints);
	void SetValue(double d);
	double Average();
	virtual ~CAverager();

private:
	int m_iLastPoint, m_iMaxPoints;
	double *pdVals;
	bool *pbVals;

};

#endif // !defined(AFX_AVERAGER_H__2CDDD6D8_F7E8_11D2_891F_00C04F79ECB9__INCLUDED_)

