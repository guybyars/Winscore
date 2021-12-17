// WSSingleDocTemplate.cpp: implementation of the CWSSingleDocTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "winscore.h"
#include "WSSingleDocTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CWSSingleDocTemplate::~CWSSingleDocTemplate()
{

}

CWSSingleDocTemplate::CWSSingleDocTemplate(	UINT nIDResource, 
											CRuntimeClass* pDocClass,
											CRuntimeClass* pFrameClass, 
											CRuntimeClass* pViewClass ) :
      CSingleDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
      { };
	

BOOL CWSSingleDocTemplate::GetDocString(CString& rString, enum DocStringIndex i) const
   {
        CString strTemp,strLeft,strRight;
   //     int nFindPos;
        AfxExtractSubString(strTemp, m_strDocStrings, (int)i);

      if( i == CDocTemplate::filterExt/* ||	i == CDocTemplate::filterName*/)  
			{
		    rString=".wscx";
//			  nFindPos=strTemp.Find(';');
//			if(-1 != nFindPos) 
//				{
//				//string contains two extensions
//			    strLeft=strTemp.Left(nFindPos+1);
//			    strRight="*"+strTemp.Right(lstrlen((const char*)strTemp)-nFindPos-1);
//				strTemp=strLeft+strRight;
//				}
			}
 //     else if( i == CDocTemplate::filterName)  
//			{
//		    rString="WinscoreFile (*.wsc)";
//			rString+=(TCHAR)'\0';
//		    rString+="WinscoreFile (*.xml)";
//
//
//			nFindPos=strTemp.Find(';');
//			  if(-1 != nFindPos) 
//				{
//				//string contains two extensions
//				strLeft=strTemp.Left(nFindPos+1);
//				strRight="\0"+strTemp.Right(lstrlen((const char*)strTemp)-nFindPos-1);
//				strTemp=strLeft;
//				strTemp+= (TCHAR)'\0';
//				strTemp+=strRight;
//				}
//			}
		else     
			rString = strTemp;

    return TRUE;
   }

CDocTemplate::Confidence CWSSingleDocTemplate::MatchDocType(const
        char* pszPathName, CDocument*& rpDocMatch)
   {
        ASSERT(pszPathName != NULL);
        rpDocMatch = NULL;

        // go through all documents
        POSITION pos = GetFirstDocPosition();
        while (pos != NULL)
        {
           CDocument* pDoc = GetNextDoc(pos);
           if (pDoc->GetPathName() == pszPathName) {
              // already open
              rpDocMatch = pDoc;
              return yesAlreadyOpen;
           }
        }  // end while

        // see if it matches either suffix
        CString strFilterExt;
        if (GetDocString(strFilterExt, CDocTemplate::filterExt) &&
          !strFilterExt.IsEmpty())
        {
           // see if extension matches
           ASSERT(strFilterExt[0] == '.');
           CString ext1,ext2;
           int nDot = CString(pszPathName).ReverseFind('.');
           const char* pszDot = nDot < 0 ? NULL : pszPathName + nDot;

           int nSemi = strFilterExt.Find(';');
           if(-1 != nSemi)   {
             // string contains two extensions
             ext1=strFilterExt.Left(nSemi);
             ext2=strFilterExt.Mid(nSemi+2);
             // check for a match against either extension
             if (nDot >= 0 && (lstrcmpi(pszPathName+nDot, ext1) == 0
                 || lstrcmpi(pszPathName+nDot,ext2) ==0))
               return yesAttemptNative; // extension matches
           }
           else
           { // string contains a single extension
             if (nDot >= 0 && (lstrcmpi(pszPathName+nDot,
                 strFilterExt)==0))
             return yesAttemptNative;  // extension matches
           }
        }
        return yesAttemptForeign; //unknown document type
   }
   