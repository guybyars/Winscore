// XMLMgr.h: interface for the CXMLMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLMGR_H__6DD2D0F7_D24E_4099_A3BE_A8AEB254A2BE__INCLUDED_)
#define AFX_XMLMGR_H__6DD2D0F7_D24E_4099_A3BE_A8AEB254A2BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "msxml6.dll" no_auto_exclude //raw_interfaces_only

using namespace MSXML2;

class CXMLMgr
{
public:
	CXMLMgr();
	virtual ~CXMLMgr();

public:
	CString m_strLastError;

	HRESULT
		SelectChildSTR(MSXML2::IXMLDOMNode* pParent, LPCSTR lpcstrText,  CString &strResult);

	HRESULT
		TransformDOM2Obj(IXMLDOMDocument2Ptr &pXSLDoc,  IXMLDOMDocument2Ptr &pXMLOut);

	HRESULT 
		SelectChild(LPCSTR lpcstrText, MSXML2::IXMLDOMNode* pParent, MSXML2::IXMLDOMNodePtr &pNode);

	HRESULT 
		InsertChild(MSXML2::IXMLDOMNode* /*pNewNode*/ , MSXML2::IXMLDOMNode* /*pRefNode*/ , MSXML2::IXMLDOMNode* /*pParentNode*/ = NULL );

	MSXML2::IXMLDOMNode* 
		GetChild(const long /*lIndex*/ , MSXML2::IXMLDOMNodeList* /*pList*/) const;


	MSXML2::IXMLDOMNodeListPtr 
		CXMLMgr::SelectNodeList(LPCSTR lpcstrNode, MSXML2::IXMLDOMNode* pParentNode) const;

	LPCSTR 
		GetAttribute(MSXML2::IXMLDOMElement* /*pNode*/, LPCSTR) const;

	HRESULT
	    SetAttribute(MSXML2::IXMLDOMElement *pNode, LPCSTR  strPropertyName, LPCSTR strValue ) const;

	LPCSTR 
		GetNodeName(MSXML2::IXMLDOMNode* /*pNode*/) const;

	LPCSTR 
		GetText(MSXML2::IXMLDOMNode* /*pElement*/) const;

	MSXML2::IXMLDOMNode* 
		GetParent(MSXML2::IXMLDOMElement* /*pElement*/) const;

	HRESULT 
		CXMLMgr::CreateChild(MSXML2::IXMLDOMNode *pParent, MSXML2::IXMLDOMNodePtr &pChild, LPCSTR lpcstrName );


	HRESULT 
		RemoveElement(MSXML2::IXMLDOMElement* /*pElementToRemove*/ , MSXML2::IXMLDOMNode* /*pParent*/);

	HRESULT 
		RemoveElement(MSXML2::IXMLDOMElement* /*pElementToRemove*/);

	HRESULT 
		ReplaceElement(MSXML2::IXMLDOMElement* /*pNewElement*/ , MSXML2::IXMLDOMElement* /*pOldElement*/ , MSXML2::IXMLDOMNode* /*pParent*/);

	HRESULT 
		CreateElement(MSXML2::IXMLDOMNode* /*pParentNode*/ ,LPCSTR /*lpcstrElementName*/, LPCSTR /*lpcstrElementText*/ ,  bool /*bAdd*/ = true , int /*nLevel*/ = 1, bool /*bNullOK*/ = false);

	HRESULT 
		CreateElementInt(MSXML2::IXMLDOMNode* /*pParentNode*/ ,LPCSTR /*lpcstrElementName*/, int /*lpcstrElementText*/ ,  bool /*bAdd*/ = true , int /*nLevel*/ = 1);

	HRESULT 
		CreateElementIntC(MSXML2::IXMLDOMNode* /*pParentNode*/ ,LPCSTR /*lpcstrElementName*/, int ,  bool /*bAdd*/ = true , int /*nLevel*/ = 1);

	HRESULT 
		CreateElementDbl(MSXML2::IXMLDOMNode* /*pParentNode*/ ,LPCSTR /*lpcstrElementName*/, double ,  bool /*bAdd*/ = true , int /*nLevel*/ = 1);
	
	HRESULT 
		CreateElementFlt(MSXML2::IXMLDOMNode* /*pParentNode*/ ,LPCSTR /*lpcstrElementName*/, float ,  bool /*bAdd*/ = true , int /*nLevel*/ = 1);

	HRESULT 
		CreateElementDblC(MSXML2::IXMLDOMNode* /*pParentNode*/ ,LPCSTR /*lpcstrElementName*/, double ,  bool /*bAdd*/ = true , int /*nLevel*/ = 1);

	HRESULT 
		CreateComments(LPCSTR /*lpcstrComment*/);

	HRESULT 
		CreateXMLFileHeader();

	HRESULT 
		CreateProcInstruction(LPCSTR,LPCSTR);

	HRESULT 
		CreateRoot(LPCSTR /*lpcstrRootName*/ , LPCSTR /*lpcstrRootText*/ = NULL);

	MSXML2::IXMLDOMElementPtr 
		GetRoot() const { return m_pXMLRoot; }

	IXMLDOMDocument2Ptr 
		GetDocumentPtr() const { return m_pXMLDomDoc; }

	MSXML2::IXMLDOMNode * 
		GetFirstChild() const;
	
	MSXML2::IXMLDOMNode * 
		GetFirstChild(MSXML2::IXMLDOMNode* /*pNode*/) const;

	MSXML2::IXMLDOMNode * 
		GetLastChild(MSXML2::IXMLDOMNode* /*pNode*/) const;

	MSXML2::IXMLDOMNode * 
		GetLastChild() const;

	MSXML2::IXMLDOMNode * 
		GetNextSibling(MSXML2::IXMLDOMNode* /*pNode*/) const;

	MSXML2::IXMLDOMNodeList	* 
		GetChildList(MSXML2::IXMLDOMNode* /*pNode*/) const;

	MSXML2::IXMLDOMNode	* 
		GetItemNode(const long /*lIndex */, MSXML2::IXMLDOMNodeList* /*pList*/) const;	

	const long 
		GetNoOfChilds(MSXML2::IXMLDOMNodeList* /*pList*/) const;
	
	LPCSTR 
		GetItemText(MSXML2::IXMLDOMNode* /*pNode*/) const;

	bool	
		Load(LPCSTR /*lpcstrXMLFileName*/, LPCSTR lpcstrXMLSchemaFileName=NULL);

	HRESULT 
		Save(LPCSTR /*lpcstrXMLFileName*/);
	
	HRESULT 
		Initlize();

	void	
		Release();


	static int XMLMemoryHandler(size_t /* size */);

private:
	void 
		Release(MSXML2::IXMLDOMDocument*	/*pXMLDoc*/);	
	void 
		Release(MSXML2::IXMLDOMElement*		/*m_pXMLRoot*/);

	HRESULT 
		AppendChildToParent(MSXML2::IXMLDOMNode* /*pChild*/, MSXML2::IXMLDOMNode* /*pParent*/);

	void 
		AddWhiteSpaceToNode(MSXML2::IXMLDOMDocument* /*pDOMDoc*/, BSTR /*bstrWs*/, MSXML2::IXMLDOMNode* /*pNode*/);

	void 
		Destroy();

	void 
		SetIndentLevel(int /*nLevel*/ , MSXML2::IXMLDOMNode* /*pParentNode*/);
	
	HRESULT 
		SetNodeText(LPCSTR /*lpcstrText*/ , MSXML2::IXMLDOMElement* /*pNode*/);
	
	MSXML2::IXMLDOMElementPtr 
		CreateNode(LPCSTR /*lpcstrRootName*/ );

	LPCSTR 
		ReturnBuffer(BSTR /*bstr*/) const;

private:
	IXMLDOMDocument2Ptr			m_pXMLDomDoc;		// NULL;

	MSXML2::IXMLDOMElementPtr	m_pXMLRoot;			// NULL;

	_TCHAR*	m_pBuffer;

};

#endif // !defined(AFX_XMLMGR_H__6DD2D0F7_D24E_4099_A3BE_A8AEB254A2BE__INCLUDED_)
