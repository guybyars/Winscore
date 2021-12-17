

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_WSVIEWLib,0xE0748843,0x5814,0x11D3,0xBA,0x96,0x00,0x10,0xA4,0x04,0x32,0x81);


MIDL_DEFINE_GUID(IID, DIID__DWSBaro,0x49134334,0x1CE2,0x11D4,0xB1,0xD2,0x54,0xF8,0x5E,0x00,0x00,0x00);


MIDL_DEFINE_GUID(IID, DIID__DWSBaroEvents,0x49134335,0x1CE2,0x11D4,0xB1,0xD2,0x54,0xF8,0x5E,0x00,0x00,0x00);


MIDL_DEFINE_GUID(CLSID, CLSID_WSBaro,0x49134336,0x1CE2,0x11D4,0xB1,0xD2,0x54,0xF8,0x5E,0x00,0x00,0x00);


MIDL_DEFINE_GUID(IID, DIID__DWSView,0xE0748844,0x5814,0x11D3,0xBA,0x96,0x00,0x10,0xA4,0x04,0x32,0x81);


MIDL_DEFINE_GUID(IID, DIID__DWSViewEvents,0xE0748845,0x5814,0x11D3,0xBA,0x96,0x00,0x10,0xA4,0x04,0x32,0x81);


MIDL_DEFINE_GUID(CLSID, CLSID_WSView,0xE0748846,0x5814,0x11D3,0xBA,0x96,0x00,0x10,0xA4,0x04,0x32,0x81);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



