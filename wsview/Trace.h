//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Trace.h: interface for the CTrace class.
//
//////////////////////////////////////////////////////////////////////

#include "WinscoreDefs.h"	// Added by ClassView
#include "fix.h"
#include "3DTransformation.h"
#include "hitlist.h"

#if !defined(AFX_TRACE_H__3720CEC2_5819_11D3_BA96_0010A4043281__INCLUDED_)
#define AFX_TRACE_H__3720CEC2_5819_11D3_BA96_0010A4043281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace Winscore {

class CTrace  
{
public:
	bool m_bAltitudeMeters;
	CString m_strAdditionalText;
	double GetDistanceToLabel(CPoint &point);
	void Highlight(CDC* pDC);
	void SetColor(COLORREF lColor);
	EClass m_eClass;
	int GetNumFixes();
	bool m_bVisible;
	bool IsPilotVisible();
	void SetVisible( bool bVisible);
	void LoadLabel( CDC *pDC, CPoint &cPoint );
//	short m_sMaxAltitude;
//	short m_sMinAltitude;

	short m_sLastIndexPainted;
	bool m_bGLUpToDate;
	void Draw(CDC* pDC, C3DTransformation &c3DTransformation, CTime &cBegin, CTime &end, long lDisplayLength, BOOL bPointDisplay, short sAltMax, short sAltMin, CHitList &cHitList );
	void Draw(CDC* pDC, C3DTransformation &c3DTransformation, short sTimeIndexStart, short sTimeIndexStop, short sMaxAlt, short sMinAlt, CHitList &cHitList);
	CFix* Get(CTime  &cTime);
	void AddFix( CFix &cFix );
	void SetTime( CTime &cStartTime, CTime &cFinishTime);

	CTrace( CString strContestNo, CString strLastName, CString strFirstName, EClass eClass, long lColor, void* pCtrl   );
	CTrace( CTrace &cTrace  );

	void LoadVertex(CDC* pDC, C3DTransformation &c3DTransformation, CPoint &cPoint, int iVertexSize=2);


	virtual ~CTrace();

	CString		m_strLastName;
	CString		m_strFirstName;
	CString		m_strContestNo;
	CTime		m_cFinishTime;
	CTime		m_cStartTime;

	double		m_dLabelPositionX;
	double		m_dLabelPositionY;
	short		m_sAltitude;

	unsigned char		m_R, m_G, m_B;

	bool		m_bHighlighted;
	void	   *m_pWSViewCtl;


private:

	CPen* m_pPen;
	CPtrList m_cPtrList;
};

}
#endif // !defined(AFX_TRACE_H__3720CEC2_5819_11D3_BA96_0010A4043281__INCLUDED_)

