/******************************************************************************

                           EnzLab for Windows

					(coded by:  Doug Clow, 1992 / 1993)
					(32-bit version by: Andrew Booth, April 2014)

              MODULE: PHPDRAW

			 PURPOSE: graphing functions for pH Profile

 DATA ITEMS REQUIRED: LIST ECHAR

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: NONE


******************************************************************************/


#include "stdafx.h"
#include <math.h>


#include "d_echar.h"
#include "d_list.h"
#include "enzlab.h"
#include "EZFont.h"
#include "func.h"
#include "graph.h"
#include "Htmlhelp.h"
#include "Resource.h"
#include "help.h"
#include "print.h"

extern BOOL Win7_or_later;
extern WCHAR *szHelpFilename;
extern HINSTANCE hInstance;
/*
		PHPRES
*/
void PHPRes( HWND hWnd, HINSTANCE hInstance )
{
	DLGPROC lpfnDlgProc;

    lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)PHPResDlgProc, hInstance );
	DialogBox( hInstance, L"PHPRes", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );
}


LRESULT CALLBACK PHPResDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	static HWND hPaintWnd;
	static BOOL	bHelp;

	switch (iMessage) {
		case WM_INITDIALOG:
			hPaintWnd = GetDlgItem( hDlg, IDD_HGRAPHPAINT );
			break;

		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORDLG:
			return (INT_PTR)GetStockObject(WHITE_BRUSH);
			        	
		case WM_COMMAND:
			switch (wParam) {
				case IDOK:
					//if (bHelp == TRUE)
                    //	WinHelp( hDlg, HELPFILE, HELP_QUIT, 0L );
					EndDialog( hDlg, TRUE );
					break;

				case IDD_HELP:
					bHelp = TRUE;
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg0009.htm" );
					//WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_phpres );
					break;

				case IDD_PRINT:
					Print( hDlg, DOC_PHPRES );
					break;

				default:
					return FALSE;
			}
			break;

		case WM_PAINT:
			PHPResPaintGraphWindow( hPaintWnd );
			return FALSE;

		default:
			return FALSE;
	}

    return TRUE;
}


void PHPResPaintGraphWindow( HWND hPaintWnd )
{
	InvalidateRect( hPaintWnd, NULL, TRUE );
	UpdateWindow( hPaintWnd );
	PHPResPaintWindow( hPaintWnd );

    return;
}


void PHPResPaintWindow( HWND hPaintWnd )
{
	HDC		hDC;
	RECT	rect;

	hDC = GetDC( hPaintWnd );
	GetClientRect( hPaintWnd, &rect );

	DrawPHPRes( hDC, rect.right - rect.left, rect.bottom - rect.top );

	ReleaseDC( hPaintWnd, hDC );
}


void DrawPHPRes( HDC hDC, short xPixels, short yPixels )
{
	int		xChar, yChar,
			xPos, yPos,
			nPts,
            crosslen,
			i;
	float	x, y,
			pH[BUFLEN], v[BUFLEN],
            maxv;
	WCHAR	buffer[BUFLEN];
	WCHAR	temp[BUFLEN];
	struct exptrun	*ptr;
	DWORD	dwExtent;
    TEXTMETRIC	tm;
	HFONT hFont;
	LOGFONT lf;

    SaveDC( hDC );
	hFont = EzCreateFont( hDC, TEXT("Arial"), 100, 0, 0, TRUE);
	GetObject(hFont, sizeof(LOGFONT), &lf);
	SelectObject( hDC, CreateFontIndirect(&lf));
	SetBkColor( hDC, GetSysColor( COLOR_WINDOW ) );
	SetTextColor( hDC, GetSysColor( COLOR_WINDOWTEXT ) );

	GetTextMetrics( hDC, &tm );
	yChar = tm.tmHeight + tm.tmExternalLeading;

    // write title
	SetTextAlign( hDC, TA_CENTER | TA_TOP );
	LoadString( hInstance, IDS_PHP_TITLE, temp, 100);
	swprintf( buffer, temp, EnzChar.ref );
	TextOut( hDC, xPixels / 2.0, 0, buffer, lstrlen( buffer ) );


    // write y-axis label (v / umol per min)
	create_text_at_point(hDC, Win7_or_later?L"Vₒ  (µmol/min)":L"  (µmol/min)  ", (XLEFTGAP - XTICLENGTH) * xPixels, (double)yPixels/2.0, 10.0, RGB(0,0,0), 90.0, FALSE, TA_CENTER);
	

	swprintf( buffer, L"v / umol" );
	dwExtent = GetTextExtent( hDC, buffer, lstrlen( buffer ) );
	xPos = LOWORD( dwExtent );
    SetTextAlign( hDC, TA_LEFT | TA_BASELINE );
	//TextOut( hDC, 0, yPixels / 2, buffer, lstrlen( buffer ) );
	swprintf( buffer, L"per min" );
	dwExtent = GetTextExtent( hDC, buffer, lstrlen( buffer ) );
	if (LOWORD( dwExtent ) > xPos )
		xPos = LOWORD( dwExtent );
	SetTextAlign( hDC, TA_LEFT | TA_TOP );
	//TextOut( hDC, 0, yPixels / 2, buffer, strlen( buffer ) );
	yPos = 2 * yChar;

    // write x-axis label (pH)
	swprintf( buffer, L"pH" );
	SetTextAlign( hDC, TA_CENTER | TA_BASELINE );
    TextOut( hDC, xPixels / 2, yPixels, buffer, lstrlen( buffer ) );


	// resize viewport
	SetViewportOrgEx( hDC, xPos, yPos, NULL );
	xPixels -= xPos;
	yPixels -= (yPos + yChar);

	// scan the data and find biggest value
	maxv = 0.0;
    nPts = 0;
	for (ptr = listhead; ptr != NULL; ptr = ptr->next )
	{
		if (ptr->sel == TRUE) {
			 pH[nPts] = ptr->pH;
			 v[nPts] = ptr->v;
			 if (ptr->v > maxv)
				maxv = ptr->v;
			 nPts++;
		}
	}
	maxv = (float)ceil( maxv );

	// axes
	MoveToEx( hDC, XLEFTGAP * xPixels, (1.0 - YBOTTOMGAP) * yPixels, NULL );
	LineTo( hDC, (XLEFTGAP + XMAIN) * xPixels, (1.0 - YBOTTOMGAP) * yPixels );
	MoveToEx( hDC, XLEFTGAP * xPixels, (1.0 - YBOTTOMGAP) * yPixels, NULL );
	LineTo( hDC, XLEFTGAP * xPixels, YTOPGAP * yPixels );

	// tic labels : x-axis (pH)
    SetTextAlign( hDC, TA_CENTER | TA_TOP ); 
	for (i = 0; i < XTICS_PH; i++) {
		xPos = ((i * XMAIN)/(XTICS_PH - 1) + XLEFTGAP) * xPixels;
		MoveToEx( hDC, xPos, (1.0 - YBOTTOMGAP) * yPixels, NULL );
        yPos = (1.0 - YBOTTOMGAP + YTICLENGTH) * yPixels;
		LineTo( hDC, xPos, yPos );
		swprintf( buffer, L"%.4g", (((float)i / (XTICS_PH - 1.0)) * (PH_MAX-PH_MIN)) + PH_MIN );
        TextOut( hDC, xPos, yPos, buffer, lstrlen( buffer ) );
	}
	// tic labels : y-axis (rate, v)
	SetTextAlign( hDC, TA_RIGHT | TA_BASELINE );
	for (i = 0; i < YTICS; i++) {
		yPos = (1.0 - (((float)i * YMAIN)/(YTICS - 1.0) + YBOTTOMGAP)) * yPixels;
		MoveToEx( hDC, XLEFTGAP * xPixels, yPos, NULL );
        xPos = (XLEFTGAP - (XTICLENGTH/2)) * xPixels;
		LineTo( hDC, xPos, yPos );
		swprintf( buffer, L"%.1f", ((float)i / (YTICS - 1) * maxv) );
        TextOut( hDC, xPos, yPos+(yChar/4), buffer, lstrlen( buffer ) );
	}

	// our experimental points
	for (i = 0; i < nPts; i++) {
		xPos = (((pH[i] - PH_MIN) / (PH_MAX - PH_MIN)) * XMAIN + XLEFTGAP) * xPixels;
		yPos = (1.0 - ((v[i] / maxv) * YMAIN + YBOTTOMGAP)) * yPixels;
        crosslen = YTICLENGTH * yPixels / 2;
		MoveToEx( hDC, xPos - crosslen, yPos + crosslen, NULL );
		LineTo( hDC, xPos + crosslen, yPos - crosslen );
		MoveToEx( hDC, xPos - crosslen, yPos - crosslen, NULL );
		LineTo( hDC, xPos + crosslen, yPos + crosslen );
	}
	DeleteObject(SelectObject( hDC, GetStockObject(SYSTEM_FONT)));
	RestoreDC(hDC, -1);
	return;
}
