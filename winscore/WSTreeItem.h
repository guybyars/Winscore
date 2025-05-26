#pragma once

#include "WinscoreDefs.h"

class CWSTreeItem
{

public:	
	typedef enum   
	{
	eNone			,
	eContestRoot	,
	eControlPoints	,
	eContestants	,
	eTask 			,
	eTasks			,
	eDistanceGrid   ,
	eDate			,
	eClass			,
	eGridPositions	,
	eScores			,
	eLogs			,
	eSetup          ,
	ePreContest
	}	ETreeType;	

	CWSTreeItem(ETreeType eTreeType, CTime cTime=CTime(0), EClass eClass=eStandard);
	CWSTreeItem();
	~CWSTreeItem(void);

//  Members

	ETreeType  m_eTreeType;
	CTime      m_cTime;
	EClass	   m_eClass;
};

