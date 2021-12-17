// Median.cpp: implementation of the CMedian class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "winscore.h"
#include "Median.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMedian::CMedian()
{

}

CMedian::~CMedian()
{

}

void CMedian::AddSample(int iVal)
	{

// Do some filtering.  Reject samples that are more than 2000 ft from the average

//	if( m_AltList.size()>5 )
//		{
//		int iTest=iVal-Average();
//		if( abs(iTest)>2000 ) return;
//		}


	m_AltList.push_back(iVal);
	}

int CMedian::GetMedian()
{
	m_AltList.sort();
	
	// Now remove from the list 33% of the greatest and least values in the list.  This
	// should remove any erronious values.  Then average the middle 3rd.

//	int iSize=m_AltList.size();
//	if( iSize==0 ) return 0;
//	int iDrop=iSize/3;
//	for( int i=0; i<iDrop; i++ )
//		{
//		m_AltList.pop_back();
//		m_AltList.pop_front();
//		}
//	return Average();

	while (1)
		{
		if( m_AltList.size()==1 ) break;
		m_AltList.pop_back();
		if( m_AltList.size()==1 ) break;
		m_AltList.pop_front();
		}


	return m_AltList.front();

}

int CMedian::Average()
{
	int iAlt=0;
	int iVal=0;
	for( std::list<int>::iterator iter=m_AltList.begin();
		 iter != m_AltList.end();
		 iter++)
		 {
	     iVal= *iter;
		 iAlt+= iVal;
		 }
	iAlt = int(float(iAlt)/m_AltList.size());

	return iAlt;
}


double CMedian::dAverage()
{
	double dAlt=0.0;
	int iVal=0;
	for( std::list<int>::iterator iter=m_AltList.begin();
		 iter != m_AltList.end();
		 iter++)
		 {
	     iVal= *iter;
		 dAlt+= iVal;
		 }
	dAlt /= m_AltList.size();

	return dAlt;
}


int CMedian::Mode()
{
	int iMode=0;
	int iModeCnt=0;

	for( std::list<int>::iterator iter=m_AltList.begin();
		 iter != m_AltList.end();
		 iter++)
		 {
	     int iVal= *iter;
		 if( iModeCnt==0 )
			{
			 iMode=iVal;
			 iModeCnt=1;
			}

		 // Count this value
		 int iCnt=0;
		 for( std::list<int>::iterator iter2=m_AltList.begin();
			 iter2 != m_AltList.end();
			 iter2++)
			 {
			 int iVal2= *iter2;
			 if( iVal2==iVal ) iCnt++;
			 }

		if( iCnt>iModeCnt ) 
			{
			iMode=iVal;
			iModeCnt=iCnt;
			}
		}
		return iMode;
}


int CMedian::Median2(void)
{
	m_AltList.sort();
	
	// Now remove from the list 33% of the greatest and least values in the list.  This
	// should remove any erronious values.  Then average the middle 3rd.

	int iSize=m_AltList.size();
	if( iSize==0 ) return 0;
	int iDrop=iSize/2;
	for( int i=0; i<iDrop; i++ )
		{
		m_AltList.pop_back();
//		m_AltList.pop_front();
		}
	return Average();
}


int CMedian::GetMax(void)
{
	int iMax=0;
	for( std::list<int>::iterator iter=m_AltList.begin();
		 iter != m_AltList.end();
		 iter++)
		 {
	     int iVal= *iter;
		 iMax=max(iMax,iVal);
		 }
	return iMax;
}


int CMedian::GetMin(void)
{
	int iMin=0;
	bool bFirst=true;
	for( std::list<int>::iterator iter=m_AltList.begin();
		 iter != m_AltList.end();
		 iter++)
		 {
	     int iVal= *iter;
		 if( bFirst )
			{
			iMin=iVal;
			bFirst=false;
			}
		 else
			{
			iMin=min(iMin,iVal);
			}
		 }
	return iMin;
}


double CMedian::StdDeviation()
	{
    double sum = 0.0, mean, standardDeviation = 0.0;

	for( std::list<int>::iterator iter=m_AltList.begin();
		 iter != m_AltList.end();
		 iter++)
			{
			int iVal= *iter;
			sum += double(iVal);
			}

    mean = sum/double(m_AltList.size());

	for( std::list<int>::iterator iter=m_AltList.begin();
		 iter != m_AltList.end();
		 iter++)
		{
		int iVal= *iter;
        standardDeviation += pow(double(iVal) - mean, 2);
		}

	return sqrt(standardDeviation / m_AltList.size());


}