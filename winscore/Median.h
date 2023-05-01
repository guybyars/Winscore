// Median.h: interface for the CMedian class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEDIAN_H__171B06AB_4714_47E7_8709_F89EB5A5C463__INCLUDED_)
#define AFX_MEDIAN_H__171B06AB_4714_47E7_8709_F89EB5A5C463__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

class CMedian  
{
public:
	int Mode();
	int Average();
	int CountValue( int iVal);
	int NumSamples();
	double dAverage();
	double StdDeviation();
	int GetMedian();
	void AddSample(int iVal);
	CMedian();
	virtual ~CMedian();

private:

	std::list<int> m_AltList;

public:
	int Median2(void);
	int GetMax(void);
	int GetMin(void);
};

#endif // !defined(AFX_MEDIAN_H__171B06AB_4714_47E7_8709_F89EB5A5C463__INCLUDED_)
