#if !defined(SUAUTIL_H)
#define SUAUTIL_H

#include "winscoredoc.h"

#include "sua.h"

int					ReadSUAFile(CString &cFileName, CStringArray &strArray );
CSUASectionArray*	GetSUAArray( );
int					LoadSUA( CTurnpointArray &turnpointArray );

#endif