#if !defined(SUAUTIL_H)
#define SUAUTIL_H


#ifndef _CAIEXPLR
#include "winscoredoc.h"
#endif

#include "sua.h"

int					ReadSUAFile(CString &cFileName, CStringArray &strArray );
CSUASectionArray*	GetSUAArray( );
#ifndef _CAIEXPLR
int					LoadSUA( CTurnpointArray &turnpointArray );
#endif
#endif