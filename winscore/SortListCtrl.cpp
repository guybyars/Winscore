//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// SortListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SortListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl

CSortListCtrl::CSortListCtrl()
{
}

CSortListCtrl::~CSortListCtrl()
{
}


// SortTextItems        - Sort the list based on column text
                // Returns              - Returns true for success
                // nCol                 - column that contains the text to be sorted
                // bAscending           - indicate sort order
                // low                  - row to start scanning from - default row is 0
                // high                 - row to end scan. -1 indicates last row
BOOL CSortListCtrl::SortTextItems( int nCol, BOOL bAscending, 
                                 int low /*= 0*/,
								 int high /*= -1*/  )
                {
                        if( nCol >= ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount() )
                                return FALSE;

                        if( high == -1 ) high = GetItemCount() - 1;

                        int lo = low;
                        int hi = high;
                        CString midItem;

                        if( hi <= lo ) return FALSE;

                        midItem = GetItemText( (lo+hi)/2, nCol );

                        // loop through the list until indices cross
                        while( lo <= hi )
                        {
                                // rowText will hold all column text for one row
                                CStringArray rowText;

                                // find the first element that is greater than or equal to 
                                // the partition element starting from the left Index.
                                if( bAscending )
                                        while( ( lo < high ) && ( GetItemText(lo, nCol) < midItem ) )
                                                ++lo;
                                else
                                        while( ( lo < high ) && ( GetItemText(lo, nCol) > midItem ) )
                                                ++lo;

                                // find an element that is smaller than or equal to 
                                // the partition element starting from the right Index.
                                if( bAscending )
                                        while( ( hi > low ) && ( GetItemText(hi, nCol) > midItem ) )
                                                --hi;
                                else
                                        while( ( hi > low ) && ( GetItemText(hi, nCol) < midItem ) )
                                                --hi;

                                // if the indexes have not crossed, swap
                                // and if the items are not equal
                                if( lo <= hi )
                                {
                                        // swap only if the items are not equal
                                        if( GetItemText(lo, nCol) != GetItemText(hi, nCol))
                                        {
                                                // swap the rows
                                                LV_ITEM lvitemlo, lvitemhi;
                                                int nColCount = 
                                                        ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount();
                                                rowText.SetSize( nColCount );
                                                int i;
                                                for( i=0; i<nColCount; i++)
                                                        rowText[i] = GetItemText(lo, i);
                                                lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
                                                lvitemlo.iItem = lo;
                                                lvitemlo.iSubItem = 0;
                                                lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED | 
                                                                LVIS_FOCUSED |  LVIS_SELECTED | 
                                                                LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;

                                                lvitemhi = lvitemlo;
                                                lvitemhi.iItem = hi;

                                                GetItem( &lvitemlo );
                                                GetItem( &lvitemhi );

                                                for( i=0; i<nColCount; i++)
                                                        SetItemText(lo, i, GetItemText(hi, i));

                                                lvitemhi.iItem = lo;
                                                SetItem( &lvitemhi );

                                                for( i=0; i<nColCount; i++)
                                                        SetItemText(hi, i, rowText[i]);

                                                lvitemlo.iItem = hi;
                                                SetItem( &lvitemlo );
                                        }

                                        ++lo;
                                        --hi;
                                }
                        }

                        // If the right index has not reached the left side of array
                        // must now sort the left partition.
                        if( low < hi )
                                SortTextItems( nCol, bAscending , low, hi);

                        // If the left index has not reached the right side of array
                        // must now sort the right partition.
                        if( lo < high )
                                SortTextItems( nCol, bAscending , lo, high );

                        return TRUE;
                }


BOOL CSortListCtrl::SortIntItems( int nCol, BOOL bAscending, 
                                 int low /*= 0*/,
								 int high /*= -1*/  )
                {
                        if( nCol >= ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount() )
                                return FALSE;

                        if( high == -1 ) high = GetItemCount() - 1;

                        int lo = low;
                        int hi = high;
                        int midItem;

                        if( hi <= lo ) return FALSE;

                        midItem = atoi(GetItemText( (lo+hi)/2, nCol ));

                        // loop through the list until indices cross
                        while( lo <= hi )
                        {
                                // rowText will hold all column text for one row
                                CStringArray rowText;

                                // find the first element that is greater than or equal to 
                                // the partition element starting from the left Index.
                                if( bAscending )
                                        while( ( lo < high ) && ( atoi(GetItemText(lo, nCol)) < midItem ) )
                                                ++lo;
                                else
                                        while( ( lo < high ) && ( atoi(GetItemText(lo, nCol)) > midItem ) )
                                                ++lo;

                                // find an element that is smaller than or equal to 
                                // the partition element starting from the right Index.
                                if( bAscending )
                                        while( ( hi > low ) && ( atoi(GetItemText(hi, nCol)) > midItem ) )
                                                --hi;
                                else
                                        while( ( hi > low ) && ( atoi(GetItemText(hi, nCol)) < midItem ) )
                                                --hi;

                                // if the indexes have not crossed, swap
                                // and if the items are not equal
                                if( lo <= hi )
                                {
                                        // swap only if the items are not equal
                                        if( atoi(GetItemText(lo, nCol)) != atoi(GetItemText(hi, nCol)))
                                        {
                                                // swap the rows
                                                LV_ITEM lvitemlo, lvitemhi;
                                                int nColCount = 
                                                        ((CHeaderCtrl*)GetDlgItem(0))->GetItemCount();
                                                rowText.SetSize( nColCount );
                                                int i;
                                                for( i=0; i<nColCount; i++)
                                                        rowText[i] = GetItemText(lo, i);
                                                lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
                                                lvitemlo.iItem = lo;
                                                lvitemlo.iSubItem = 0;
                                                lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED | 
                                                                LVIS_FOCUSED |  LVIS_SELECTED | 
                                                                LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;

                                                lvitemhi = lvitemlo;
                                                lvitemhi.iItem = hi;

                                                GetItem( &lvitemlo );
                                                GetItem( &lvitemhi );

                                                for( i=0; i<nColCount; i++)
                                                        SetItemText(lo, i, GetItemText(hi, i));

                                                lvitemhi.iItem = lo;
                                                SetItem( &lvitemhi );

                                                for( i=0; i<nColCount; i++)
                                                        SetItemText(hi, i, rowText[i]);

                                                lvitemlo.iItem = hi;
                                                SetItem( &lvitemlo );
                                        }

                                        ++lo;
                                        --hi;
                                }
                        }

                        // If the right index has not reached the left side of array
                        // must now sort the left partition.
                        if( low < hi )
                                SortIntItems( nCol, bAscending , low, hi);

                        // If the left index has not reached the right side of array
                        // must now sort the right partition.
                        if( lo < high )
                                SortIntItems( nCol, bAscending , lo, high );

                        return TRUE;
                }




BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSortListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl message handlers

