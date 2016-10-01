/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: LISTDRAW

			 PURPOSE: graphing function for the two lists

 DATA ITEMS REQUIRED: ANLIST LIST

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: NONE


******************************************************************************/



#include "stdafx.h"


#include "d_anlist.h"
#include "d_list.h"
#include "func.h"
#include "Resource.h"


/*
		RESLIS
*/

extern HINSTANCE hInstance;

void DrawResLis( HDC hDC, short xPixels, short yPixels )
{
	int		yChar,
    		yPos;
	WCHAR	buffer[BUFLEN];
    struct exptrun	*ptr;
	TEXTMETRIC	tm;
    DWORD	dwExtent;

	SelectObject( hDC, GetStockObject( DEVICE_DEFAULT_FONT ) );

	LoadString( hInstance, IDS_LIST_TITLE, buffer, 200);
	//swprintf( buffer, L" Run No   Vol (µL)      pH       S (mM)      I (mM)    Vₒ (µmol/min)" );
	TabbedTextOut( hDC, 0, 0, buffer, lstrlen( buffer ), 0, NULL, 0 );

	GetTextMetrics( hDC, &tm );
	yChar = tm.tmHeight + tm.tmExternalLeading;
	yPos = 2 * yChar;
	for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
    	MakeExptLine( buffer, ptr );
		TabbedTextOut( hDC, 0, yPos, buffer, lstrlen( buffer ), 0, NULL, 0 );
        yPos += yChar;
	}
}


void DrawAnList( HDC hDC, short xPixels, short yPixels )
{
	int		yChar,
    		yPos;
	WCHAR	buffer[BUFLEN];
    struct anlist	*ptr;
	TEXTMETRIC	tm;
    DWORD	dwExtent;

	SelectObject( hDC, GetStockObject( DEVICE_DEFAULT_FONT ) );
	LoadString( hInstance, IDS_ANLIST_TITLE, buffer, 200);
	//swprintf( buffer, L" No.    Points used\t    pH\t\tI (mM)\t\tKm\t      Vmax " );
	TabbedTextOut( hDC, 0, 0, buffer, lstrlen( buffer ), 0, NULL, 0 );

	GetTextMetrics( hDC, &tm );
	yChar = tm.tmHeight + tm.tmExternalLeading;
	yPos = 2 * yChar;
	for (ptr = anlisthead; ptr != NULL; ptr = ptr->next) {
    	MakeAnListLine( buffer, ptr );
		TabbedTextOut( hDC, 0, yPos, buffer, lstrlen( buffer ), 0, NULL, 0 );
        yPos += yChar;
	}
}
