/*********************************************************************
	Copyright (C) 2002 Mustafa Demirhan :)
	
	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	3. This notice may not be removed or altered from any source distribution.

	http://www.macroangel.com
	admin@macroangel.com
**********************************************************************/

#include "stdafx.h"
#include "StringHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Adds a \ character at the end of the folder name
// if it does not exist!
void StringHelper::AddBSToFolderName (CString &sFolderName, TCHAR cSlash)
{
	if (sFolderName.GetLength () > 0)
	{
		int nLast = sFolderName.GetLength () - 1;
		if (sFolderName.GetAt (nLast) != '\\' &&
			sFolderName.GetAt (nLast) != '/')
			sFolderName += cSlash;
		// Fix /'s and \'s
		else if (sFolderName.GetAt (nLast) != cSlash)
			sFolderName.SetAt (nLast, cSlash);
	}
	else
		sFolderName += cSlash;

}

void StringHelper::RemoveWSFromFileName (CString &sFileName)
{
	int nStartIdx = 0, nLength = 0;
	TCHAR cTemp;
	
	bool bStarted = false;

	for (int i = 0; i < sFileName.GetLength (); i++)
	{
		cTemp = sFileName.GetAt (i);
		if (cTemp == ' ' || cTemp == '\t' || cTemp == '\r' || cTemp == '\n')
		{
			if (bStarted)
				++ nLength;
		}
		else
		{
			bStarted = true;
			++ nLength;
		}
	}

	if (nLength > 0)
		sFileName = sFileName.Mid (nStartIdx, nLength);
	else
		sFileName.Empty ();
}

bool StringHelper::WildcardCompareNoCase (LPCSTR strWild, LPCSTR strText)
{
	int cp = 0, mp = 0;
	int idxWild = 0, idxString = 0;
	int nStrLen = strlen (strText);
	int nWildLen = strlen (strWild);

	while (idxString < nStrLen)
	{
		if (idxWild >= nWildLen)
			break;
		if (strWild [idxWild] == '*')
			break;
		
		if ((toupper (strWild [idxWild]) != toupper (strText [idxString]) && (strWild [idxWild] != '?')))
			return false;
		
		++ idxWild;
		++ idxString;
	}
	
	while (idxString < nStrLen) 
	{
		if (idxWild >= nWildLen)
			break;
		
		if (strWild [idxWild] == '*') 
		{
			++ idxWild;
			if (idxWild >= nWildLen)
				return true;
			
			mp = idxWild;
			cp = idxString + 1;
		} 
		else if ((toupper (strWild [idxWild]) == toupper (strText [idxString])) || (strWild [idxWild] == '?')) 
		{
			++ idxWild;
			++ idxString;
		}
		else 
		{
			idxWild = mp;
			idxString = cp ++;
		}
	}
	
	while (idxWild < nWildLen)
	{
		if (strWild [idxWild] != '*')
			break;
		++ idxWild;
	}
	
	if (idxWild < nWildLen)
		return false;
	else
		return true;
}

bool StringHelper::WildcardCompare (LPCSTR strWild, LPCSTR strText)
{
	int cp = 0, mp = 0;
	int idxWild = 0, idxString = 0;
	int nStrLen = strlen (strText);
	int nWildLen = strlen (strWild);
	
	while (idxString < nStrLen)
	{
		if (idxWild >= nWildLen)
			break;
		if (strWild [idxWild] == '*')
			break;
		
		if ((strWild [idxWild] != strText [idxString] && (strWild [idxWild] != '?')))
			return false;
		
		++ idxWild;
		++ idxString;
	}
	
	while (idxString < nStrLen) 
	{
		if (idxWild >= nWildLen)
			break;
		
		if (strWild [idxWild] == '*') 
		{
			++ idxWild;
			if (idxWild >= nWildLen)
				return true;
			
			mp = idxWild;
			cp = idxString + 1;
		} 
		else if ((strWild [idxWild] == strText [idxString]) || (strWild [idxWild] == '?')) 
		{
			++ idxWild;
			++ idxString;
		}
		else 
		{
			idxWild = mp;
			idxString = cp ++;
		}
	}
	
	while (idxWild < nWildLen)
	{
		if (strWild [idxWild] != '*')
			break;
		++ idxWild;
	}
	
	if (idxWild < nWildLen)
		return false;
	else
		return true;
}

/*
 * matches a string against a wildcard string * such as "*.*"
 * or "bl?h.*" etc. This is good for file globbing or to match
 * hostmasks.  
 *
 * Example:
 *
 * if (wildcmp("bl?h.*", "blah.jpg")) {
 *     //we have a match!
 * } else {
 *     //no match =(
 * }
 */
/*int wildcmp(char *wild, char *string) {
	char *cp, *mp;
	
	while ((*string) && (*wild != '*')) {
		if ((*wild != *string) && (*wild != '?')) {
			return 0;
		}
		wild++;
		string++;
	}
	
	while (*string) {
		if (*wild == '*') {
			if (!*++wild) {
				return 1;
			}
			mp = wild;
			cp = string+1;
		} else if ((*wild == *string) || (*wild == '?')) {
			wild++;
			string++;
		} else {
			wild = mp;
			string = cp++;
		}
	}
	
	while (*wild == '*') {
		wild++;
	}
	return !*wild;
}
*/
/*int CStepBackup::wildcmp (LPCSTR wild, LPCSTR string) 
{
	int cp = 0, mp = 0;
	int idxWild = 0, idxString = 0;
	
	while ((string[idxString]) && (wild[idxWild] != '*')) {
		if ((wild [idxWild] != string [idxString]) && (wild [idxWild] != '?')) {
			return 0;
		}
		idxWild++;
		idxString++;
	}
	while (string[idxString]) {
		if (wild[idxWild] == '*') {
			if (!wild[++idxWild]) {
				return 1;
			}
			mp = idxWild;
			cp = idxString+1;
		} else if ((wild[idxWild] == string[idxString]) || (wild[idxWild] == '?')) {
			idxWild++;
			idxString++;
		} else {
			idxWild = mp;
			idxString = cp++;
		}
	}
	while (wild[idxWild] == '*') {
		idxWild++;
	}
	return !wild[idxWild];
}*/
