// BaroZoom.h: interface for the CBaroZoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAROZOOM_H__0DADF6CE_B0C4_491A_A898_102B57171E45__INCLUDED_)
#define AFX_BAROZOOM_H__0DADF6CE_B0C4_491A_A898_102B57171E45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaroZoom  
{
public:
	void OnMove(CDC &dc, CPoint &cPoint);
	void Cancel(CDC &dc);
	void SetEnd(long lPt);
	void SetBeginning(long lPt);
	CBaroZoom(CRect cWindow);
	virtual ~CBaroZoom();

	void Draw(CDC &dc);

	CRect	m_cGridWindow;
	long	m_lStartPoint;
	long	m_lEndPoint;


};

#endif // !defined(AFX_BAROZOOM_H__0DADF6CE_B0C4_491A_A898_102B57171E45__INCLUDED_)
