//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
#include "stdafx.h"
#include "WinscoreFonts.h"

#define MAX_FONTS  500

static CFont*  pcFonts[MAX_FONTS+1];
static int nFonts=0;
static int aiSize[MAX_FONTS+1];
static int aiBold[MAX_FONTS+1];
static int abUnderline[MAX_FONTS+1];


void  DeleteFonts()
	{
	if( nFonts==0 ) return;
	for( int iFont=0; iFont<nFonts; iFont++ ) delete pcFonts[iFont];
	nFonts=0;
	}

void  SelectFont( CDC *pDC, int iSize, int iBold, BOOL bUnderline )
	{

	// First check if we have already created this font before
	for( int iFont=0; iFont<nFonts; iFont++ )
		{
		if( iSize==aiSize[iFont]  &&
			iBold==aiBold[iFont] &&
			bUnderline==abUnderline[iFont]  )
			{
			pDC->SelectObject( pcFonts[iFont] );
			return;
			}
		}

	pcFonts[nFonts]=new CFont;
	pcFonts[nFonts]->CreateFont(
							iSize,		//int nHeight, 
							0,		// int nWidth, 
							0,		// int nEscapement, 
							0,		//  int nOrientation, 
							iBold,	//int nWeight, 
							FALSE,	// BYTE bItalic, 
							bUnderline,	// BYTE bUnderline, 
							FALSE,	// BYTE cStrikeOut, 
							ANSI_CHARSET,	// BYTE nCharSet, 
							OUT_DEVICE_PRECIS,		// BYTE nOutPrecision, 
							CLIP_DEFAULT_PRECIS,	// BYTE nClipPrecision, 
							DEFAULT_QUALITY,	// BYTE nQuality, 
							DEFAULT_PITCH,	// BYTE nPitchAndFamily, 
							NULL);	//	 LPCTSTR lpszFacename );
		pDC->SelectObject( pcFonts[nFonts] );
		aiSize[nFonts]=iSize;
		aiBold[nFonts]=iBold;
		abUnderline[nFonts]=bUnderline;
		ASSERT(nFonts<MAX_FONTS);
		if( nFonts<MAX_FONTS ) nFonts++;
	}

