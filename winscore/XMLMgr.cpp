// XMLMgr.cpp: implementation of the CXMLMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLMgr.h"
#include <string>
#include <assert.h>
#include <atlbase.h>


#define M_BUFFER_LEN   512

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
HRESULT wi_BSTRToChar(BSTR pSrc , std::string& strBuffer)
{
	if(!pSrc) 
	{
		return E_FAIL;
	}

	DWORD cb , cwch = ::SysStringLen(pSrc);//convert even embeded NULL

	char *szOut = NULL;

	cb = ::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch + 1, NULL, 0, 0, 0);
	
	if(cb)
	{
		szOut = new char[cb];
		
		if(szOut)
		{
			szOut[cb - 1] = '\0';

			if(!::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch + 1, szOut, cb, 0, 0))
			{
				delete [] szOut; //clean up if failed;
				return E_FAIL;
			}
		}

		strBuffer = szOut;
		delete szOut;
	}

	return S_OK;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
//------------------------//
// Convert char * To BSTR //
//------------------------//
HRESULT wi_CharToBSTR(const char *pString, BSTR &_bstr)
{
	int lenA = lstrlenA(pString);
	int lenW = MultiByteToWideChar(CP_ACP, 0, pString, lenA, 0, 0);

	if (lenW > 0)
	{
		// Name of the file to load in BSTR
		_bstr = SysAllocStringLen(0, lenW);

		if (0 != ::MultiByteToWideChar(CP_ACP, 0, pString, lenA, _bstr, lenW) )
		{
			return S_OK;
		}
	}

	return E_FAIL;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

//CXMLMgr* CXMLMgr::m_pThis = NULL;
/*$ ******************************************************************************
					Method Name			CXMLMgr() - Constructor	
					Parameters			None
					Return				None
					Description			It will initlize COM, set the default memory handler
********************************************************************************** */

CXMLMgr::CXMLMgr() : m_pXMLDomDoc(NULL) , m_pXMLRoot(NULL)
{
	m_pBuffer = new _TCHAR [M_BUFFER_LEN];
	memset(m_pBuffer,0,sizeof(_TCHAR[M_BUFFER_LEN]) );
	m_strLastError="";
}

/*$ ******************************************************************************
					Method Name			CXMLMgr() - Constructor	
					Parameters			None
					Return				None
					Description			Destory & Release resources
********************************************************************************** */
CXMLMgr::~CXMLMgr()
{
	Destroy();
}

/*$ ******************************************************************************
					Method Name			Initlize
					Parameters			None

					Return				HERSULT
											S_OK (on success)
											E_FAIL (on error)

					Description			It will initlize Document pointer
********************************************************************************** */
HRESULT CXMLMgr::Initlize()
{

	HRESULT hr = m_pXMLDomDoc.CreateInstance(__uuidof(DOMDocument60));
	if (FAILED(hr) )
		return hr;

   m_pXMLDomDoc->async = VARIANT_FALSE;
   m_pXMLDomDoc->validateOnParse = VARIANT_FALSE;
   m_pXMLDomDoc->resolveExternals = VARIANT_FALSE;

	return S_OK;
}

/*$ ******************************************************************************
					Method Name			Load	
					
					Parameters			[in] LPCSTR 
											Name & path of the .xml file to load
					Return				bool
											true (on success)
											false (on error)

					Description			It will loads a valid xml document
********************************************************************************** */
bool CXMLMgr::Load(LPCSTR lpcstrXMLFileName, LPCSTR lpcstrXMLSchemaFileName)
{
	assert (lpcstrXMLFileName != NULL);

	BSTR			bstr, bsstr;
	VARIANT			var, svar;
	bool			bRetVal = false;
	HRESULT hr=S_FALSE;
	MSXML2::IXMLDOMParseErrorPtr		errPtr	=	NULL;	
	MSXML2::IXMLDOMSchemaCollectionPtr  pXS		=	NULL;
	m_strLastError=_T("");

	wi_CharToBSTR(lpcstrXMLFileName , bstr);

	VariantInit(&var);
	V_BSTR(&var) = SysAllocString(bstr);
	V_VT(&var) = VT_BSTR;

	// Loading xml DOM
;
	if( lpcstrXMLSchemaFileName!=NULL && strlen(lpcstrXMLSchemaFileName)>0 ) 
		{
		VariantInit(&svar);
		wi_CharToBSTR( lpcstrXMLSchemaFileName , bsstr);
		V_BSTR(&svar) = SysAllocString(bsstr);

	   // Create a schema cache and add books.xsd to it.
	   hr = pXS.CreateInstance(__uuidof(XMLSchemaCache60));
	   hr = pXS->add(_T("urn:ssa_hl"), bsstr);

	    // Assign the schema cache to the DOMDocument's
	    // schemas collection.
	    m_pXMLDomDoc->schemas = pXS.GetInterfacePtr();
		}

	hr = m_pXMLDomDoc->load(var);

	errPtr = m_pXMLDomDoc->parseError;
	long lTemp=errPtr->errorCode;
;

   // Return validation results in message to the user.
     if (hr==E_FAIL || errPtr->errorCode != S_OK)
		{

         BSTR bstrErr;
		 hr =errPtr->get_reason(&bstrErr);

         BSTR bstrErr2;
		 hr =errPtr->get_srcText(&bstrErr2);

	 	 long errorCode=0;
		 hr =errPtr->get_errorCode(&errorCode);

		 long lineno=0;
		 hr =errPtr->get_line(&lineno);
		 	 
		 long linepos=0;
		 hr =errPtr->get_line(&linepos);
;
		 m_strLastError.Format(_T("Error Reading : %s\nAt line Number : %d\n%s\n%s"),lpcstrXMLFileName, lineno, CString(bstrErr), CString(bstrErr2) );

		 bRetVal = false;
		}
   else 
		{
		// now that the document is loaded, initialize the root pointer
		m_pXMLDomDoc->get_documentElement(&m_pXMLRoot);
		bRetVal = true;
		}

	SysFreeString(bstr);

	return bRetVal;
}

/*$ ******************************************************************************
					Method Name			Save	

					Parameters			[in] LPCSTR 
											Name & path with which the .xml file is required to be stored

					Return				HRESULT
											S_OK (on success)
											E_FAIL (on error)

					Description			Save the document to the external .xml file
********************************************************************************** */
HRESULT CXMLMgr::Save(LPCSTR lpcstrXMLFileName)
{
	HRESULT hr = E_FAIL;

	assert (lpcstrXMLFileName != NULL);
	_variant_t varString (lpcstrXMLFileName);

	try
		{
		hr = m_pXMLDomDoc->save(varString );
		}
	catch (...)
		{
		return E_FAIL;
		}

	return  hr;
}

/*$ ******************************************************************************
					Method Name			Release		
					Parameters			None
					Return				None
					Description			Release the resources
********************************************************************************** */
void CXMLMgr::Release()
{

//  if (m_pXMLDomDoc)	m_pXMLDomDoc.Release();
//	if (m_pXMLRoot)		m_pXMLRoot.Release();
}

/*$ ******************************************************************************
					Method Name			Release	

					Parameters			[in] MSXML2::IXMLDOMDocument*
											Doucument pointer

					Return				None
					Description			Release document pointer
********************************************************************************** */
void CXMLMgr::Release(MSXML2::IXMLDOMDocument* pXMLDoc)
{
	if (pXMLDoc)
	{
		pXMLDoc->Release();
		pXMLDoc = NULL;
	}
}

/*$ ******************************************************************************
					Method Name			Release		

					Parameters			[in] MSXML2::IXMLDOMElement*
											Element to release

					Return				None
					Description			Release element pointer
********************************************************************************** */
void CXMLMgr::Release(MSXML2::IXMLDOMElement* pXMLElement)
{
	if (pXMLElement)
	{
		pXMLElement->Release();
		pXMLElement = NULL;
	}
}

/*$ ******************************************************************************
					Method Name			GetText	

					Parameters			[in] MSXML2::IXMLDOMNode*
											Element which text is required

					Return				LPCSTR
											Text of the XML Element

					Description			Get the text of the node
********************************************************************************** */
LPCSTR CXMLMgr::GetText(MSXML2::IXMLDOMNode* pElement) const
{
	assert (pElement != NULL);

	BSTR bstr;
	pElement->get_text(&bstr);

	return ReturnBuffer(bstr);
}

/*$ ******************************************************************************
					Method Name			GetNodeName	

					Parameters			[in] MSXML2::IXMLDOMNode*
											Node, which name is required

					Return				LPCSTR
											Name of the ndoe

					Description			Get the name of the node
********************************************************************************** */
LPCSTR CXMLMgr::GetNodeName(MSXML2::IXMLDOMNode* pNode) const
{
	assert (pNode != NULL);

	if( pNode == NULL) return "";

	BSTR bstr;
	pNode->get_nodeName(&bstr);

	return ReturnBuffer(bstr);
}

/*$ ******************************************************************************
					Method Name			ReturnBuffer		

					Parameters			[in] BSTR 
											BSTR string, which is required to be in LPCSTR

					Return				LPCSTR
											Output of BSTR

					Description			Convert BSTR string to LPCSTR and return it
********************************************************************************** */
LPCSTR CXMLMgr::ReturnBuffer(BSTR bstr) const
{
	std::string strTemp;
	wi_BSTRToChar(bstr , strTemp);

	size_t len=sizeof(_TCHAR[M_BUFFER_LEN]);
	strcpy_s( m_pBuffer , len, strTemp.c_str());
	return m_pBuffer;
}

/*$ ******************************************************************************
					Method Name			GetFirstChild		

					Parameters			[in] MSXML2::IXMLDOMNode* 
											Node, which first child is required

					Return				MSXML2::IXMLDOMNode*
											First child of the node

					Description			Get the first element of the node
********************************************************************************** */
MSXML2::IXMLDOMNode* CXMLMgr::GetFirstChild(MSXML2::IXMLDOMNode* pNode) const
{
	assert (pNode != NULL);

	MSXML2::IXMLDOMNode* pElem = NULL;
	pNode->get_firstChild(&pElem);

	return pElem;
}

HRESULT CXMLMgr::SelectChild(LPCSTR lpcstrText, MSXML2::IXMLDOMNode* pParent, MSXML2::IXMLDOMNodePtr &pNode)

{
	assert (pParent != NULL);

	BSTR bstr;
	wi_CharToBSTR(lpcstrText , bstr);

	pNode=pParent->selectSingleNode(bstr);
	if( pNode==NULL ) return S_OK; //This is ok, but the caller needs to check for it!!!

	return S_OK;

}



HRESULT CXMLMgr::SelectChildSTR(MSXML2::IXMLDOMNode* pParent, LPCSTR lpcstrText,  CString &strResult)
{
	assert (pParent != NULL);

	strResult.Empty();

	BSTR bstr;
	wi_CharToBSTR(lpcstrText , bstr);

	MSXML2::IXMLDOMNodePtr pNode = NULL;

	pNode=pParent->selectSingleNode(bstr);
	if( pNode==NULL ) 
		{
		//This is OK, the Child isn't here, just return.
		return S_OK;
		}
	strResult=GetItemText(pNode);

	return S_OK;
}


/*$ ******************************************************************************
					Method Name			GetFirstChild	

					Parameters			None
					Return				MSXML2::IXMLDOMNode*
											First child of the root element

					Desccription		Get the first element of the root node
********************************************************************************** */
MSXML2::IXMLDOMNode* CXMLMgr::GetFirstChild() const
{
	assert(m_pXMLRoot != NULL);

	MSXML2::IXMLDOMNode* pChild = NULL;
	m_pXMLRoot->get_firstChild(&pChild);

	return pChild;
}

/*$ ******************************************************************************
					Method Name			GetLastChild	

					Parameters			[in] MSXML2::IXMLDOMNode*
											Node, which last element is required

					Return				MSXML2::IXMLDOMNode*
											Last element of the node

					Desccription		Get the last element of the node
********************************************************************************** */
MSXML2::IXMLDOMNode * CXMLMgr::GetLastChild(MSXML2::IXMLDOMNode * pNode) const
{
	assert(pNode != NULL);

	MSXML2::IXMLDOMNode* pChild = NULL;
	pNode->get_lastChild(&pChild);

	return pChild;
}

/*$ ******************************************************************************
					Method Name			GetLastChild	

					Parameters			None
					Return				MSXML2::IXMLDOMNode*
											Last child of the root

					Description			Get the last element of the root node
********************************************************************************** */
MSXML2::IXMLDOMNode * CXMLMgr::GetLastChild() const
{
	assert(m_pXMLRoot != NULL);

	MSXML2::IXMLDOMNode* pChild = NULL;
	m_pXMLRoot->get_lastChild(&pChild);

	return pChild;
}

/*$ ******************************************************************************
					Method Name			GetNextSibling	

					Parameters			[in] MSXML2::IXMLDOMNode*
											Node which sibling is required

					Return				MSXML2::IXMLDOMNode*
											Sibling node pointer
											
					Description			Get the next sibling of the node
********************************************************************************** */
MSXML2::IXMLDOMNode * CXMLMgr::GetNextSibling(MSXML2::IXMLDOMNode* pNode) const
{
	assert(pNode != NULL);

	MSXML2::IXMLDOMNode* pSibling = NULL;
	pNode->get_nextSibling(&pSibling);

	return pSibling;
}

/*$ ******************************************************************************
					Method Name			GetChildList	

					Parameters			[in] MSXML2::IXMLDOMNode*
											Node, which child list is required

					Return				MSXML2::IXMLDOMNodeList*
											Child list

					Description			Get the list of the child nodes contained by the node
********************************************************************************** */
MSXML2::IXMLDOMNodeList* CXMLMgr::GetChildList(MSXML2::IXMLDOMNode* pNode) const
{
	assert (pNode != NULL);

	MSXML2::IXMLDOMNodeList* pList=NULL;

	HRESULT hr=pNode->get_childNodes(&pList);
    return pList;
}



MSXML2::IXMLDOMNodeListPtr CXMLMgr::SelectNodeList(LPCSTR lpcstrNode, MSXML2::IXMLDOMNode* pParentNode) const
{
	assert (pParentNode != NULL);


	BSTR bstr;
	wi_CharToBSTR(lpcstrNode , bstr);

	return pParentNode->selectNodes(bstr);
}



/*$ ******************************************************************************
					Method Name			GetNoOfChilds	

					Parameters			[in] MSXML2::IXMLDOMNodeList*
											Child node list pointer
					
					 Return				long
											No. of childs

					Description			Return the no. of childrens of the node list
********************************************************************************** */
const long CXMLMgr::GetNoOfChilds(MSXML2::IXMLDOMNodeList* pList) const
{
	assert (pList != NULL);

	long lLength;
	pList->get_length(&lLength);

	return lLength;
}

/*$ ******************************************************************************
					Method Name			GetItemNode	

					Parameters			[in] const long lIndex
											Index of the node in the list

										[in] MSXML2::IXMLDOMNodeList*
											Pointer of the node list

					Return				MSXML2::IXMLDOMNode*
											Node item in the list

					Description			Return node, which index and child list is provided 
********************************************************************************** */
MSXML2::IXMLDOMNode	* CXMLMgr::GetItemNode(const long lIndex , MSXML2::IXMLDOMNodeList* pList) const
{
	assert(pList != NULL);
	assert(lIndex >= 0);
	assert(lIndex <= GetNoOfChilds(pList));

	MSXML2::IXMLDOMNode	*pNode;
	pList->get_item(lIndex , &pNode);

	return pNode;
}

/*$ ******************************************************************************
					Method Name			GetItemText

					Parameters			MSXML2::IXMLDOMNode*
											Node, which text is required

					Return				LPCSTR
											Text of the node

					Description			Get the text of the node
********************************************************************************** */
LPCSTR CXMLMgr::GetItemText(MSXML2::IXMLDOMNode* pNode) const
{
	assert (pNode != NULL);

//	CString str=GetNodeName(pNode);

	BSTR bstr;
	HRESULT hr = pNode->get_text(&bstr);
	
	return ReturnBuffer(bstr);
}

/*$ ******************************************************************************
					Method Name			CreateRoot	

					Parameters			[in] LPCSTR lpcstrRootName
											Name with which root element is required to be created

										[in] LPCSTR lpcstrRootText
											Text of the root element (can be NULL)

					Return				HRESULT
											S_OK (on Success)
											E_FAIL (on Error)

					Description			Create Root element with root text. Note, if there is no text required for the
										root element, then it can set to NULL.
********************************************************************************** */
HRESULT CXMLMgr::CreateRoot(LPCSTR lpcstrRootName , LPCSTR lpcstrRootText)
{
	assert (lpcstrRootName != NULL);

	HRESULT hr = E_FAIL;

	m_pXMLRoot = CreateNode(lpcstrRootName);

	if (m_pXMLRoot)
	{
		if (lpcstrRootText)
			SetNodeText(lpcstrRootText , m_pXMLRoot);
		
		hr = AppendChildToParent(m_pXMLRoot , m_pXMLDomDoc);
	}

	return hr;	
}

/*$ ******************************************************************************
					Method Name			AppendChildToParent	

					Parameters			[in] MSXML2::IXMLDOMNode* pChild
											Child node to append

										[in] MSXML2::IXMLDOMNode* pParent
											Parent node, to which child will be appended

					Return				HRESULT
											S_OK (on Success)
											E_FAIL (on Error)

					Desccription		Append one node to another
********************************************************************************** */
HRESULT CXMLMgr::AppendChildToParent(MSXML2::IXMLDOMNode *pChild, MSXML2::IXMLDOMNode *pParent)
{
	assert (pChild != NULL);
	assert (pParent != NULL);

	MSXML2::IXMLDOMNodePtr pNode = NULL;

    pNode = pParent->appendChild(pChild);
 
	return S_OK;
}


HRESULT CXMLMgr::CreateChild(MSXML2::IXMLDOMNode *pParent, MSXML2::IXMLDOMNodePtr &pChild, LPCSTR lpcstrName )
{
	assert (pParent != NULL);

	MSXML2::IXMLDOMNodePtr pNode = NULL;

	BSTR bstr;
	wi_CharToBSTR(lpcstrName , bstr);

	pChild = m_pXMLDomDoc->createNode(CComVariant(MSXML2::NODE_ELEMENT), bstr, _T(""));
	assert (pChild != NULL);

    pNode = pParent->appendChild(pChild);

	return S_OK;
}


/*$ ******************************************************************************
					Method Name			InsertChild	

					Parameters			[in] MSXML2::IXMLDOMNode *pNewNode
											New node to insert

										[in] MSXML2::IXMLDOMNode *pRefNode
											The address of the reference node; the newChild parameter is inserted to 
											the left of the refChild parameter. 
											
											If Null, the newChild parameter is inserted at the end of the child list.

										[in] MSXML2::IXMLDOMNode *pParentNode
											Parent node, to which new child is required to be added

					Return				HRESULT
											S_OK (on Success)
											E_FAIL (on Error)

					Desccription		Insert new child 
********************************************************************************** */
HRESULT CXMLMgr::InsertChild(MSXML2::IXMLDOMNode *pNewNode , MSXML2::IXMLDOMNode *pRefNode , MSXML2::IXMLDOMNode *pParentNode)
{
	assert (pNewNode != NULL);

	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMNode *pOutNode = NULL;

	_variant_t varNodeRef;

	if (NULL != pRefNode)
		varNodeRef = pRefNode;

	if (!pParentNode)
		pParentNode = m_pXMLRoot;
	
	pOutNode = pParentNode->insertBefore(pNewNode , varNodeRef);

	return hr;

}

/*$ ******************************************************************************
					Method Name			CreateXMLFileHeader	

					Parameters			None

					Return				HRESULT
											S_OK (on Success)
											E_FAIL (on Error)
											
					Desccription		Create XML Header
********************************************************************************** */
HRESULT CXMLMgr::CreateXMLFileHeader()
{
	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMProcessingInstructionPtr pi = NULL;
	BSTR bstr1 = SysAllocString(L"xml");
	BSTR bstr2 = SysAllocString(L"version='1.0'");

	// Creating the processing instruction, that will create the xml tag
	pi = m_pXMLDomDoc->createProcessingInstruction(bstr1 ,bstr2);

	if (pi == NULL ) return E_FAIL;

	hr = AppendChildToParent(pi , m_pXMLDomDoc);	// Putting the header tag into the DOM

	SysFreeString(bstr1);
	SysFreeString(bstr2);

	return hr;
}



HRESULT CXMLMgr::CreateProcInstruction(LPCSTR lpcstr1, LPCSTR lpcstr2)
{
	HRESULT hr = E_FAIL;

	BSTR bstr1, bstr2;
	wi_CharToBSTR(lpcstr1, bstr1);
	wi_CharToBSTR(lpcstr2, bstr2);

	MSXML2::IXMLDOMProcessingInstructionPtr pi = NULL;

	// Creating the processing instruction, that will create the xml tag
	pi = m_pXMLDomDoc->createProcessingInstruction(bstr1,bstr2);
	if ( pi==NULL ) return E_FAIL;

	hr = AppendChildToParent(pi , m_pXMLDomDoc);	// Putting the header tag into the DOM

	SysFreeString(bstr1);
	SysFreeString(bstr2);

	return hr;
}


LPCSTR  CXMLMgr::GetAttribute(MSXML2::IXMLDOMElement *pNode, LPCSTR  strPropertyName ) const
	{

	BSTR bstr;
	wi_CharToBSTR(strPropertyName , bstr);
	
	 _variant_t var=pNode->getAttribute( bstr);

	return ReturnBuffer(var.bstrVal);
	
	}

HRESULT  CXMLMgr::SetAttribute(MSXML2::IXMLDOMElement *pNode, LPCSTR  strPropertyName, LPCSTR strValue ) const
	{

	BSTR bstrProp;
	wi_CharToBSTR(strPropertyName , bstrProp);
	BSTR bstrValue;
	wi_CharToBSTR(strValue , bstrValue);

	HRESULT hr =pNode->setAttribute( bstrProp, bstrValue);

	return hr;
	
	}

/*$ ******************************************************************************
					Method Name			CreateComments	

					Parameters			[in] LPCSTR
											Comments

					Return				HRESULT
											S_OK (on Success)
											E_FAIL (on Error)

					Desccription		Write particular comments to document
********************************************************************************** */
HRESULT CXMLMgr::CreateComments(LPCSTR lpcstrComment)
{
	assert (lpcstrComment != NULL);
	HRESULT hr = E_FAIL;

	// Comment Interface
	MSXML2::IXMLDOMComment	*pComment = NULL;

	// Comment itself
	BSTR bstr;
	wi_CharToBSTR(lpcstrComment , bstr);
	
	// Creating the comment
	pComment = m_pXMLDomDoc->createComment(bstr);
	if( pComment==NULL ) return E_FAIL;

	hr = AppendChildToParent(pComment, m_pXMLDomDoc);	// Putting it in the DOM object
   
	// Releasing the resources
	SysFreeString(bstr);
	pComment->Release();

	return hr;
}

/*$ ******************************************************************************
					Method Name			CreateElement	

					Parameters			[in] LPCSTR lpcstrElementName
											Name of the element to be created

										[in] LPCSTR lpcstrElementText 
											Text of the element to be created (Can be NULL)

										[in] MSXML2::IXMLDOMNode* 
											Pareent Node, to which the new element is required to be added

										[in] bool  
												true (to immediately add the element to the parent)
												false (just create the element, but don't add now)

										[in] int 
												Indentation level
					
					Return				HRESULT
											S_OK (on Success)
											E_FAIL (on Error)

					Description			Create a new element and add to the parent node (if required)
********************************************************************************** */
HRESULT CXMLMgr::CreateElement( MSXML2::IXMLDOMNode* pParentNode, LPCSTR lpcstrElementName, LPCSTR lpcstrElementText, bool bAdd , int nLevel, bool bNullOK)
{
	assert (lpcstrElementName != NULL);
	
	if( !bNullOK && strlen(lpcstrElementText)<=0 ) return S_OK;

	HRESULT hr = E_FAIL;
	MSXML2::IXMLDOMElementPtr pElement = NULL;

	SetIndentLevel(nLevel , pParentNode);
	pElement = CreateNode(lpcstrElementName);

	if (strlen(lpcstrElementText)>0)
		SetNodeText(lpcstrElementText , pElement);

	if (bAdd)
		hr = AppendChildToParent(pElement, pParentNode);	// Append <node> to <root>.

	if (pElement)
	{
		hr = S_OK;
	}

	return hr;
}


HRESULT CXMLMgr::CreateElementInt( MSXML2::IXMLDOMNode* pParentNode, LPCSTR lpcstrElementName, int iVal, bool bAdd , int nLevel)
{
  TCHAR buff[80];
  _itoa_s(iVal,buff,80,10);
  return CreateElement( pParentNode, lpcstrElementName, buff,  bAdd , nLevel);
}

HRESULT CXMLMgr::CreateElementIntC( MSXML2::IXMLDOMNode* pParentNode, LPCSTR lpcstrElementName, int iVal, bool bAdd , int nLevel)
{
  if( iVal==0 ) return S_OK;
  return CXMLMgr::CreateElementInt( pParentNode, lpcstrElementName, iVal,  bAdd , nLevel);
}

HRESULT CXMLMgr::CreateElementDbl( MSXML2::IXMLDOMNode* pParentNode, LPCSTR lpcstrElementName, double dbl, bool bAdd , int nLevel)
{
	CString strTemp;
	strTemp.Format(_T("%0.14f"),dbl);
	strTemp.TrimRight('0');
	strTemp.TrimRight('.');
	return CXMLMgr::CreateElement( pParentNode, lpcstrElementName, strTemp,  bAdd , nLevel);
}


HRESULT CXMLMgr::CreateElementFlt( MSXML2::IXMLDOMNode* pParentNode, LPCSTR lpcstrElementName, float flt, bool bAdd , int nLevel)
{
	CString strTemp;
	strTemp.Format(_T("%lf"),flt);
	strTemp.TrimRight('0');
	strTemp.TrimRight('.');
	return CXMLMgr::CreateElement( pParentNode, lpcstrElementName, strTemp,  bAdd , nLevel);
}

HRESULT CXMLMgr::CreateElementDblC( MSXML2::IXMLDOMNode* pParentNode, LPCSTR lpcstrElementName, double dbl, bool bAdd , int nLevel)
{
	if( dbl==0.0 ) return S_OK;
	return CXMLMgr::CreateElementDbl( pParentNode, lpcstrElementName, dbl,  bAdd , nLevel);
}


/*$ ******************************************************************************
					Method Name				AddWhiteSpaceToNode

					Parameters				[in] MSXML2::IXMLDOMDocument* pDOMDoc
												Document pointer

											[in] BSTR bstrWs
												White space BSTR
								
											[in] MSXML2::IXMLDOMNode* pNode
												Node to which white space is required to be created

					Return					None

					Description				Create white spaces
********************************************************************************** */
void CXMLMgr::AddWhiteSpaceToNode(MSXML2::IXMLDOMDocument* pDOMDoc, BSTR bstrWs, MSXML2::IXMLDOMNode* pNode)
{
	MSXML2::IXMLDOMTextPtr pws  = NULL;
	MSXML2::IXMLDOMNodePtr pBuf = NULL;
    
	pws=pDOMDoc->createTextNode(bstrWs);
    pBuf=pNode->appendChild(pws);
}

/*$ ******************************************************************************
					Method Name			CreateNode	

					Parameters			[in] LPCSTR
											A string specifying the name for the new element node

										[in] MSXML2::IXMLDOMElement*
											The IXMLDOMElement interface for the new element 

					Return				MSXML2::IXMLDOMElement*
											New element created

					Description			Creates an element node using the specified name
********************************************************************************** */
MSXML2::IXMLDOMElementPtr CXMLMgr::CreateNode(LPCSTR lpcstrRootName )
{
	assert (lpcstrRootName != NULL);

	BSTR bstr;
	wi_CharToBSTR(lpcstrRootName , bstr);
	MSXML2::IXMLDOMElementPtr pNode;
   
	// Putting the name of the element in the DOM
	pNode = m_pXMLDomDoc->createElement(bstr);

	SysFreeString(bstr);

	return pNode;
}

/*$ ******************************************************************************
					Method Name			SetNodeText	

					Parameters			[in] LPCSTR
											Node text to set

										[in] MSXML2::IXMLDOMElement*
											Node, which text is required to be set

					Return				HRESULT
											S_OK (on Success)
											E_FAIL (on Error)

					Description			Set the text of the specified node
********************************************************************************** */
HRESULT CXMLMgr::SetNodeText(LPCSTR lpcstrText , MSXML2::IXMLDOMElement* pNode)
{
	assert (lpcstrText != NULL);
	assert (pNode  != NULL);

	HRESULT hr = E_FAIL;
	BSTR bstr;
	wi_CharToBSTR(lpcstrText , bstr);

	// Writing Element To DOM
	hr = pNode->put_text(bstr);

	SysFreeString(bstr);

	return hr;
}

/*$ ******************************************************************************
					Method Name			SetIndentLevel		

					Parameters			[in] int
											Indend level
											
										[in] MSXML2::IXMLDOMNode*
											Node, which indend level is required to be set
					Return				None

					Description			---
********************************************************************************** */
void CXMLMgr::SetIndentLevel(int nLevel , MSXML2::IXMLDOMNode* pParentNode)
{
	assert (nLevel >= 0);
	assert (pParentNode != NULL);

	BSTR bstr;
	std::string strIndenter = _T("\n");

	for (int nCount = 0; nCount < nLevel; nCount++)
		strIndenter += _T("\t");

	wi_CharToBSTR( strIndenter.c_str() , bstr);

	AddWhiteSpaceToNode(m_pXMLDomDoc, bstr, pParentNode);

	SysFreeString(bstr);
}

/*$ ******************************************************************************
					Method Name			ReplaceElement	

					Parameters			[in] MSXML2::IXMLDOMElement* pNewElement
											New element

										[in] MSXML2::IXMLDOMElement* pOldElement
											Element to be replaced

										[in] MSXML2::IXMLDOMNode* pParent
											Parent which contains the element that is required to be replaced

					Return				HRESULT
											S_OK (on Success)
											E_FAIL (on Error)

					Description			Replace an existing element with the new element.
********************************************************************************** */
HRESULT CXMLMgr::ReplaceElement(MSXML2::IXMLDOMElement* pNewElement , MSXML2::IXMLDOMElement* pOldElement , MSXML2::IXMLDOMNode* pParent)
{
	assert (pNewElement != NULL);
	assert (pOldElement != NULL);

	HRESULT hr = E_FAIL;

	MSXML2::IXMLDOMNode* pElementReplaced = NULL;

	pElementReplaced = pParent->replaceChild(pNewElement , pOldElement);
    if( pElementReplaced == NULL ) return E_FAIL;
	return S_OK;
}

/*$ ******************************************************************************
					Method Name			RemoveElement	

					Parameters			[in] MSXML2::IXMLDOMElement* pElementToRemove
											Element to be removed

										[in] MSXML2::IXMLDOMNode* pParent
											Parent that contains the element, required to be removed

					Return				HRESULT
											S_OK (on Success)
											E_FAIL (on Error)

					Description			Remove specified element from the parent node
********************************************************************************** */
HRESULT CXMLMgr::RemoveElement(MSXML2::IXMLDOMElement* pElementToRemove , MSXML2::IXMLDOMNode* pParent)
{
	assert (pElementToRemove != NULL);
	assert (pParent != NULL);

	MSXML2::IXMLDOMNode * pOutNode = NULL;
	pOutNode = pParent->removeChild(pElementToRemove);
    
	return S_OK;
}

/*$ ******************************************************************************
					Method Name			RemoveElement	

					Parameters			[in] MSXML2::IXMLDOMElement*
											Element to be removed. It will determine its pareent itself

					Return				HRESULT
											S_OK (on Success)
											E_FAIL (on Error)
	
					Description			Remove a specified element
********************************************************************************** */
HRESULT CXMLMgr::RemoveElement(MSXML2::IXMLDOMElement* pElementToRemove)
{
	assert (pElementToRemove != NULL);
	
	MSXML2::IXMLDOMNode* pParent = GetParent(pElementToRemove);
	assert (pParent != NULL);

	HRESULT hr = E_FAIL;

	MSXML2::IXMLDOMNode * pOutNode = pParent->removeChild(pElementToRemove );
	if( pOutNode==NULL ) return E_FAIL;

	return S_OK;
}

/*$ ******************************************************************************
					Method Name			GetParent	

					Parameters			[in] MSXML2::IXMLDOMElement*
											Element to which parent is required

					Return				MSXML2::IXMLDOMNode*
											Parent of the element

					Description			Return the parent of the specified element
********************************************************************************** */
MSXML2::IXMLDOMNode* CXMLMgr::GetParent(MSXML2::IXMLDOMElement* pElement) const
{
	assert (pElement != NULL);

	MSXML2::IXMLDOMNode* pNode = NULL;
	pElement->get_parentNode(&pNode);

	return pNode;
}

/*$ ******************************************************************************
					Method Name			GetChild

					Parameters			[in] const long lIndex
											Index of the child to get
								
										[in] MSXML2::IXMLDOMNodeList*
											Parent list, that contains the child

					Return				MSXML2::IXMLDOMNode*
											child pointer

					Description			Get the child element from a parent list, which index is specified.
********************************************************************************** */
MSXML2::IXMLDOMNode* CXMLMgr::GetChild(const long lIndex , MSXML2::IXMLDOMNodeList* pList) const
{
	assert (lIndex >= 0);
	assert (pList != NULL);

	MSXML2::IXMLDOMNode* pNode;
	pList->get_item(lIndex , &pNode);

	return pNode;
}

/*$ ******************************************************************************
					Method Name			Destroy
					Parameters			None
					Return				None
					Description			Release resources
********************************************************************************** */
void CXMLMgr::Destroy()
{
	Release();

	delete m_pBuffer;
	m_pBuffer = NULL;
}


HRESULT	CXMLMgr::TransformDOM2Obj(IXMLDOMDocument2Ptr &pXSLDoc,  IXMLDOMDocument2Ptr &pXMLOut)
	{
    HRESULT hr = S_OK;
//    BSTR bstrXML = NULL;
    IDispatch *pDisp = NULL;
    VARIANT varDisp;
    VARIANT varFileName;

    VariantInit(&varDisp);
    VariantInit(&varFileName);


    hr=pXMLOut->QueryInterface(IID_IDispatch, (void**)&pDisp);
	if( hr!=S_OK ) goto CleanUp;

    V_VT(&varDisp) = VT_DISPATCH;
    V_DISPATCH(&varDisp) = pDisp;
    pDisp = NULL;

    hr=m_pXMLDomDoc->transformNodeToObject(pXSLDoc, varDisp);
	if( hr!=S_OK ) goto CleanUp;
/*
    hr=pXMLOut->get_xml(&bstrXML);
	if( hr!=S_OK ) goto CleanUp;

    //save to .htm
	BSTR bstr;
	wi_CharToBSTR(LPCSTR(strHTML), bstr);
    hr=pXMLOut->save(bstr);
	*/
CleanUp:
    VariantClear(&varDisp);
    return hr;

	}