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

#if !defined(AFX_STRINGHELPER_H__AF93E88F_4BEF_4A22_A8E9_D6DECAF859A3__INCLUDED_)
#define AFX_STRINGHELPER_H__AF93E88F_4BEF_4A22_A8E9_D6DECAF859A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class StringHelper  
{
public:
	static bool WildcardCompareNoCase	(LPCSTR strWild, LPCSTR strText);
	static bool WildcardCompare			(LPCSTR strWild, LPCSTR strText);
	
	static void AddBSToFolderName		(CString &sFolderName, TCHAR cSlash = '\\');
	static void RemoveWSFromFileName	(CString &sFileName);
};

#endif // !defined(AFX_STRINGHELPER_H__AF93E88F_4BEF_4A22_A8E9_D6DECAF859A3__INCLUDED_)
