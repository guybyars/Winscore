

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Apr 15 19:33:14 2021
 */
/* Compiler settings for WSView.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __WSView_h_h__
#define __WSView_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DWSBaro_FWD_DEFINED__
#define ___DWSBaro_FWD_DEFINED__
typedef interface _DWSBaro _DWSBaro;
#endif 	/* ___DWSBaro_FWD_DEFINED__ */


#ifndef ___DWSBaroEvents_FWD_DEFINED__
#define ___DWSBaroEvents_FWD_DEFINED__
typedef interface _DWSBaroEvents _DWSBaroEvents;
#endif 	/* ___DWSBaroEvents_FWD_DEFINED__ */


#ifndef __WSBaro_FWD_DEFINED__
#define __WSBaro_FWD_DEFINED__

#ifdef __cplusplus
typedef class WSBaro WSBaro;
#else
typedef struct WSBaro WSBaro;
#endif /* __cplusplus */

#endif 	/* __WSBaro_FWD_DEFINED__ */


#ifndef ___DWSView_FWD_DEFINED__
#define ___DWSView_FWD_DEFINED__
typedef interface _DWSView _DWSView;
#endif 	/* ___DWSView_FWD_DEFINED__ */


#ifndef ___DWSViewEvents_FWD_DEFINED__
#define ___DWSViewEvents_FWD_DEFINED__
typedef interface _DWSViewEvents _DWSViewEvents;
#endif 	/* ___DWSViewEvents_FWD_DEFINED__ */


#ifndef __WSView_FWD_DEFINED__
#define __WSView_FWD_DEFINED__

#ifdef __cplusplus
typedef class WSView WSView;
#else
typedef struct WSView WSView;
#endif /* __cplusplus */

#endif 	/* __WSView_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __WSVIEWLib_LIBRARY_DEFINED__
#define __WSVIEWLib_LIBRARY_DEFINED__

/* library WSVIEWLib */
/* [control][helpstring][helpfile][version][uuid] */ 


DEFINE_GUID(LIBID_WSVIEWLib,0xE0748843,0x5814,0x11D3,0xBA,0x96,0x00,0x10,0xA4,0x04,0x32,0x81);

#ifndef ___DWSBaro_DISPINTERFACE_DEFINED__
#define ___DWSBaro_DISPINTERFACE_DEFINED__

/* dispinterface _DWSBaro */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DWSBaro,0x49134334,0x1CE2,0x11D4,0xB1,0xD2,0x54,0xF8,0x5E,0x00,0x00,0x00);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("49134334-1CE2-11D4-B1D2-54F85E000000")
    _DWSBaro : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWSBaroVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWSBaro * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWSBaro * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWSBaro * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWSBaro * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWSBaro * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWSBaro * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWSBaro * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DWSBaroVtbl;

    interface _DWSBaro
    {
        CONST_VTBL struct _DWSBaroVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DWSBaro_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DWSBaro_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DWSBaro_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DWSBaro_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DWSBaro_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DWSBaro_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DWSBaro_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DWSBaro_DISPINTERFACE_DEFINED__ */


#ifndef ___DWSBaroEvents_DISPINTERFACE_DEFINED__
#define ___DWSBaroEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DWSBaroEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DWSBaroEvents,0x49134335,0x1CE2,0x11D4,0xB1,0xD2,0x54,0xF8,0x5E,0x00,0x00,0x00);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("49134335-1CE2-11D4-B1D2-54F85E000000")
    _DWSBaroEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWSBaroEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWSBaroEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWSBaroEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWSBaroEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWSBaroEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWSBaroEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWSBaroEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWSBaroEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DWSBaroEventsVtbl;

    interface _DWSBaroEvents
    {
        CONST_VTBL struct _DWSBaroEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DWSBaroEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DWSBaroEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DWSBaroEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DWSBaroEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DWSBaroEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DWSBaroEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DWSBaroEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DWSBaroEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_WSBaro,0x49134336,0x1CE2,0x11D4,0xB1,0xD2,0x54,0xF8,0x5E,0x00,0x00,0x00);

#ifdef __cplusplus

class DECLSPEC_UUID("49134336-1CE2-11D4-B1D2-54F85E000000")
WSBaro;
#endif

#ifndef ___DWSView_DISPINTERFACE_DEFINED__
#define ___DWSView_DISPINTERFACE_DEFINED__

/* dispinterface _DWSView */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DWSView,0xE0748844,0x5814,0x11D3,0xBA,0x96,0x00,0x10,0xA4,0x04,0x32,0x81);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E0748844-5814-11D3-BA96-0010A4043281")
    _DWSView : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWSViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWSView * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWSView * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWSView * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWSView * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWSView * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWSView * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWSView * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DWSViewVtbl;

    interface _DWSView
    {
        CONST_VTBL struct _DWSViewVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DWSView_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DWSView_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DWSView_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DWSView_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DWSView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DWSView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DWSView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DWSView_DISPINTERFACE_DEFINED__ */


#ifndef ___DWSViewEvents_DISPINTERFACE_DEFINED__
#define ___DWSViewEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DWSViewEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DWSViewEvents,0xE0748845,0x5814,0x11D3,0xBA,0x96,0x00,0x10,0xA4,0x04,0x32,0x81);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E0748845-5814-11D3-BA96-0010A4043281")
    _DWSViewEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWSViewEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWSViewEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWSViewEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWSViewEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWSViewEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWSViewEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWSViewEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWSViewEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DWSViewEventsVtbl;

    interface _DWSViewEvents
    {
        CONST_VTBL struct _DWSViewEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DWSViewEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DWSViewEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DWSViewEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DWSViewEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DWSViewEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DWSViewEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DWSViewEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DWSViewEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_WSView,0xE0748846,0x5814,0x11D3,0xBA,0x96,0x00,0x10,0xA4,0x04,0x32,0x81);

#ifdef __cplusplus

class DECLSPEC_UUID("E0748846-5814-11D3-BA96-0010A4043281")
WSView;
#endif
#endif /* __WSVIEWLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


