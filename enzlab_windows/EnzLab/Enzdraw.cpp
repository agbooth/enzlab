/******************************************************************************

                           EnzLab for Windows

					(coded by:  Doug Clow, 1992 / 1993)
					(32-bit version by: Andrew Booth, April 2014)

              MODULE: ENZDRAW

			 PURPOSE: graph functions for the child windows

 DATA ITEMS REQUIRED: LIST

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: NONE


******************************************************************************/



#include "stdafx.h"
#pragma hdrstop
#include <time.h>

#include "d_list.h"
#include "EZFont.h"
#include "func.h"
#include "limits.h"
#include "Resource.h"

#include "Logs.h"
/*
		ENZGRAPH
*/

struct exptrun *thisrun;
int multiline;

extern BOOL Win7_or_later;
extern HINSTANCE hInstance;

void create_text_at_point(HDC hdc, LPWSTR text, double X, double Y, double size, int colour, double angle, BOOL italic, int alignment)
{
	HFONT hFont;
	LOGFONT lf;
	TEXTMETRIC tm;

	SaveDC( hdc);

	hFont = EzCreateFont( hdc, TEXT("Arial"), (int)size * 10, 0, italic?EZ_ATTR_ITALIC:0, TRUE);
	GetObject(hFont, sizeof(LOGFONT), &lf);
	if (angle==90.0)
	{
		lf.lfOrientation = 900;
		lf.lfEscapement = 900;
	}
	if (angle==-90.0)
	{
		lf.lfOrientation = 2700;
		lf.lfEscapement = 2700;
	}
	SelectObject( hdc, CreateFontIndirect(&lf));
	GetTextMetrics(hdc, &tm);
	switch (alignment)
	{
	case TA_CENTER:
		SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
		break;
	case TA_RIGHT:
		SetTextAlign(hdc, TA_RIGHT | TA_BASELINE);
		break;
	default:
		SetTextAlign(hdc, TA_LEFT | TA_BASELINE);
		break;


	}
	SetBkMode(hdc, OPAQUE);
	SetTextColor(hdc, colour);
	TextOut(hdc, (ULONG)X,(ULONG)Y, text, lstrlen(text));
	SetBkMode(hdc, TRANSPARENT);
	DeleteObject(SelectObject( hdc, GetStockObject(SYSTEM_FONT)));
	RestoreDC(hdc, -1);
}

BOOL SameConditions( struct exptrun *testrun)
{
	BOOL result;

	if ((testrun == NULL) || (listtail == NULL))
	{
		return FALSE;
	}
	else
	{
		result = ((testrun->vol == listtail->vol) &&
			    (testrun->pH == listtail->pH) &&
				(testrun->s == listtail->s) &&
				(testrun->i == listtail->i));
		return result;
	}
}

void PaintEnzGraph( HWND hWnd, HDC hDC, float delay )
{
	WCHAR	buffer[BUFLEN];
	WCHAR   temp[105];
	RECT	rect;
	BOOL	bLocalDC;
    static BOOL		bPrevFail = FALSE;
	long	xPixels, yPixels,
			xPos, yPos,
            xSpace, ySpace,
			xLabel, yLabel,
			xHeight, yHeight,
            yChar;
	double	maxy,
			TimeInt,
            gradient;
	static double oldmaxy;
	int		i;
	HFONT hFont;
	TEXTMETRIC	tm;
	LOGFONT lf;
    clock_t	clicks;	

	if (listtail == NULL)
		return;

	if (hDC == NULL) {
		bLocalDC = TRUE;
		hDC = GetDC( hWnd );
	}
	else
    	bLocalDC = FALSE;

	SaveDC( hDC );
	hFont = EzCreateFont( hDC, TEXT("Arial"), 100, 0, 0, TRUE);
	GetObject(hFont, sizeof(LOGFONT), &lf);
	SelectObject( hDC, CreateFontIndirect(&lf));

	GetClientRect( hWnd, &rect );
	xPixels = rect.right - rect.left;
	yPixels = rect.bottom - rect.top;

	SetBkColor( hDC, GetSysColor( COLOR_WINDOW ) );
    SetTextColor( hDC, GetSysColor( COLOR_WINDOWTEXT ) );

	// clear screen if +ve delay (ie Go button pressed or refresh) or current or previous expt was a failure
	if (delay >= 0 || listtail->v <= 0 || bPrevFail) {
		HBRUSH	hBr, hOldBr;
		SelectObject( hDC, GetStockObject( NULL_PEN ) );
		hBr = CreateSolidBrush( GetSysColor( COLOR_WINDOW ) );
		hOldBr = (HBRUSH)SelectObject( hDC, hBr );
		Rectangle( hDC, rect.left, rect.top, rect.right, rect.bottom );
        SelectObject( hDC, hOldBr );
		SelectObject( hDC, GetStockObject( BLACK_PEN ) );
		DeleteObject( hBr );
	}
    // draw graph if current expt was a success
	if (listtail->v > 0) {
		// select scale
		if (listtail->v < GRAPH_SCALE * GRAPH_A4)
			maxy = GRAPH_A4;
		else if (listtail->v < GRAPH_SCALE * GRAPH_A3)
			maxy = GRAPH_A3;
		else if (listtail->v > GRAPH_SCALE * GRAPH_A2)
			maxy = GRAPH_A1;
		else
	    	maxy = GRAPH_A2;

		if (oldmaxy == 0.0)
			oldmaxy = maxy;

		if (delay > 0.0) //it's under new conditions
		{
			oldmaxy = maxy;
		}
		else
			maxy = oldmaxy; //it's a repeat - must not change the scale

		
	    // get size intervals
		GetTextMetrics( hDC, &tm );
        yChar = tm.tmExternalLeading + tm.tmHeight;
		xSpace = GRAPH_XSPACE * tm.tmAveCharWidth;
		ySpace = GRAPH_YSPACE * yChar;
		xLabel = GRAPH_LEFTCHARS * tm.tmAveCharWidth;
		yLabel = GRAPH_BOTTOMCHARS * yChar;
		xHeight = xPixels - xLabel - xSpace;
		yHeight = yPixels - yLabel - ySpace; 
		
		// axes
		MoveToEx( hDC, xLabel,  yPixels - yLabel, NULL );
		LineTo( hDC, xLabel + xHeight, yPixels - yLabel );
		MoveToEx( hDC, xLabel, yPixels - yLabel, NULL );
		LineTo( hDC, xLabel, ySpace );
	
	    // tic labels : x-axis (time)
	    SetTextAlign( hDC, TA_CENTER | TA_TOP );
		LoadString(hInstance, IDS_MINUTES, temp, 100);
		for (i = 0; i < GRAPH_XTICS; i++) {
			xPos = ((i * xHeight)/(GRAPH_XTICS - 1)) + xLabel;
			MoveToEx( hDC, xPos, yPixels - yLabel, NULL );
			yPos = yPixels - yLabel + tm.tmExternalLeading + tm.tmHeight;
			LineTo( hDC, xPos, yPos );
			swprintf( buffer, temp, ((float)i * GRAPH_TIME)/ (GRAPH_XTICS - 1) );
	        TextOut( hDC, xPos, yPos, buffer, lstrlen( buffer ) );
		}
	    // tic labels : y-axis (rate, v)
		SetTextAlign( hDC, TA_RIGHT | TA_BOTTOM );
		for (i = 0; i < GRAPH_YTICS; i++) {
			if (i==2) continue;
			yPos = yPixels - ((i * yHeight)/(GRAPH_YTICS - 1)) - yLabel;
			MoveToEx( hDC, xLabel, yPos, NULL );
			xPos = xLabel - tm.tmAveCharWidth;
			LineTo( hDC, xPos, yPos );
			swprintf( buffer, L"%.2g", ((float)i * maxy) / (GRAPH_YTICS - 1) );
			if (i==0) 
				swprintf(buffer, L"0");
			if (i==4)
				swprintf( buffer, L"%.1f", ((float)i * maxy) / (GRAPH_YTICS - 1) );
	        TextOut( hDC, xPos-(tm.tmAveCharWidth/2), yPos+(yChar/2), buffer, lstrlen( buffer ) );
		}

		// Absorbance label
		yPos = yPixels - ((2 * yHeight)/(GRAPH_YTICS - 1)) - yLabel;
		LoadString(hInstance, IDS_ABSORBANCE, buffer, 100);
		create_text_at_point(hDC, buffer, (double) xPos-(tm.tmAveCharWidth*1.5), (double)yPos, 10.0, RGB(0,0,0), 90.0, FALSE, TA_CENTER);
	
		// the appropriate line(s)
		
		thisrun = listtail;
		multiline = -1;  //flag for title plurality
		
		do
		{
			multiline++;
			if (thisrun == listtail)
			{
				SelectObject( hDC, GetStockObject( DC_PEN));
				SetDCPenColor(hDC, RGB(255,0,0));
			}
			else
			{
				SelectObject( hDC, GetStockObject( BLACK_PEN));
			}
			MoveToEx( hDC, xLabel, yPixels - yLabel, NULL );
			gradient = ((yHeight * thisrun->v) / (GRAPH_SCALE * maxy)) / xHeight;
			delay = (delay < 0.0) ? -delay : delay;
			TimeInt = (delay * CLK_TCK) / xHeight;
			clicks = clock();
			if (gradient > 0.0)// don't try to plot a splat or a N/A
			{
				for (i = 0; i < xHeight; i++) 
				{
				if (delay == 0)
            		i = xHeight - 1;
				xPos = xLabel + i;
				yPos = (long)(yPixels - yLabel - (i * gradient));
				LineTo( hDC, xPos, yPos );
				while (clock() < clicks + (i * TimeInt))
            		;
				}

				// Go over it in black.
				SelectObject( hDC, GetStockObject( BLACK_PEN));
				MoveToEx( hDC, xLabel, yPixels - yLabel, NULL );
				LineTo( hDC, xLabel + xHeight -1, (long)(yPixels - yLabel - ((xHeight -1) * gradient)) );
			}
			delay = 0;
		/*	if (thisrun -> v > 0.0)
			{
				// Go over it in black.
				SelectObject( hDC, GetStockObject( BLACK_PEN));
				MoveToEx( hDC, xLabel, yPixels - yLabel, NULL );
				LineTo( hDC, xLabel + xHeight -1, (long)(yPixels - yLabel - ((xHeight -1) * gradient)) );
			}
		*/	
			thisrun = thisrun->prev;  // go to the previous run
			
		} 
		while( SameConditions(thisrun )); // check the conditions are the same
		
		// value of the rate
		SetTextAlign( hDC, TA_CENTER | TA_BOTTOM );
		LoadString(hInstance, IDS_INITIALRATE, buffer, 100);
		swprintf( buffer, Win7_or_later?L"   Vₒ = %.3g µmol/min  ":buffer, listtail->v );
		TextOut( hDC, xPixels / 2, yPixels - (yChar/2), buffer, lstrlen( buffer ) );

		// title
		SetTextAlign( hDC, TA_CENTER | TA_TOP );
		if (multiline)
			LoadString(hInstance, IDS_EXPLOTS, buffer, 100);
		else
			LoadString(hInstance, IDS_EXPLOT, buffer, 100);;

		TextOut( hDC, xPixels / 2, yChar, buffer, lstrlen( buffer ) );

        bPrevFail = FALSE;
	}
    // write message if current experiment was a failure
	else {
		if (listtail->v == V_SPLAT)
			LoadString(hInstance, IDS_SPLAT, buffer, 100);
		else if (listtail->v == V_TOOSLOW)
			LoadString(hInstance, IDS_SLOW, buffer, 100);
		else if (listtail->v == V_TOOFAST)
			LoadString(hInstance, IDS_FAST, buffer, 100);
		else
			LoadString(hInstance, IDS_NOGRAPH, buffer, 100);
		DrawText( hDC, buffer, lstrlen( buffer ), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
        bPrevFail = TRUE;
	} 

	DeleteObject(SelectObject( hDC, GetStockObject(SYSTEM_FONT)));
	RestoreDC(hDC, -1);

	if (bLocalDC == TRUE) {
		ReleaseDC( hWnd, hDC );
    }
    return;
}
