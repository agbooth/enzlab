/******************************************************************************

                           EnzLab for Windows

					(coded by:  Doug Clow, 1992 / 1993)
					(32-bit version by: Andrew Booth, April 2014)

              MODULE: KINDRAW

			 PURPOSE: graphing functions for Kinetic Analysis

 DATA ITEMS REQUIRED: LIST ECHAR

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: NONE


******************************************************************************/



#include "stdafx.h"

#include <math.h>


#include "d_echar.h"
#include "d_list.h"
#include "enzlab.h"
#include "func.h"
#include "Graph.h"
#include "help.h"
#include "Htmlhelp.h"
#include "EZFont.h"
#include "Logs.h"
#include "print.h"

static int	nOutliers;
static float Km, Vmax, cKm, cVmax, rpH;

extern WCHAR *szHelpFilename;
extern BOOL Win7_or_later;

extern HWND hEnzGraph;
extern HINSTANCE hInstance;

/*
		KINRES
*/


void KinRes( HWND hWnd, HINSTANCE hInstance, float aKm, float aVmax, float acKm, float acVmax, float arpH )
{
	DLGPROC lpfnDlgProc;

	Km = aKm;
	Vmax = aVmax;
	cKm = acKm;
	cVmax = acVmax;
	rpH = arpH;
    nOutliers = 0;
    lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)KinResDlgProc, hInstance );
	DialogBox( hInstance, L"KinRes", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );
}



LRESULT CALLBACK KinResDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	static HWND hPaintWnd;
	static BOOL	bHelp;

	switch (iMessage) {
		case WM_INITDIALOG:
			hPaintWnd = GetDlgItem( hDlg, IDD_GRAPHPAINT );

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
					//WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_kinres );
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg0005.htm" );
					break;

				case IDD_PRINT:
					Print( hDlg, DOC_KINRES );
					break;

				default:
					return FALSE;
			}
			break;

		case WM_PAINT:
			KinResPaintGraphWindow( hPaintWnd );
			return FALSE;

		default:
			return FALSE;
	}

    return TRUE;
}


/*
		KINEXAM
*/


void KinExam( HWND hWnd, HINSTANCE hInstance, float aKm, float aVmax, float acKm, float acVmax, float arpH )
{
	struct exptrun	*ptr;
	DLGPROC lpfnDlgProc;

	Km = aKm;
	Vmax = aVmax;
	cKm = acKm;
	cVmax = acVmax;
    rpH = arpH;

	nOutliers = 0;
	for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
		if ((ptr->sel == TRUE) && (ptr->s / aKm) > KM_MAX) {
			nOutliers++;
		}
    }

	if (nOutliers == 0)
		return;

    lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)KinExamDlgProc, hInstance );
	DialogBox( hInstance, L"KinExam", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );
	

	for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
		if ((ptr->sel == TRUE) && (ptr->s / aKm) > KM_MAX) {
			ptr->v = V_NA;
		}
	}
	PaintEnzGraph( hEnzGraph, NULL, 0.0 );
    return;
}


LRESULT CALLBACK KinExamDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	WCHAR	buffer[BUFLEN];
	WCHAR	temp[BUFLEN];
	static HWND hPaintWnd;
	static BOOL	bHelp;

	switch (iMessage) {
		case WM_INITDIALOG:
			hPaintWnd = GetDlgItem( hDlg, IDD_GRAPHPAINT );
			if (nOutliers == 1) {
				LoadString( hInstance, IDS_ONE_POINT, temp, 500);
				swprintf( buffer, temp, KM_MAX);
            }
            else {
				LoadString( hInstance, IDS_MORE_POINTS, temp, 500);
				swprintf( buffer, temp, nOutliers, KM_MAX);
			} 
            SetDlgItemText( hDlg, IDD_GRAPHTEXT, buffer );
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
					//WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_kinexam );
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg0004.htm" );
					break;

				default:
					return FALSE;
			}
			break;

		case WM_PAINT:
			KinResPaintGraphWindow( hPaintWnd );
			return FALSE;

		default:
			return FALSE;
	}

    return TRUE;
}


void KinResPaintGraphWindow( HWND hPaintWnd )
{
	InvalidateRect( hPaintWnd, NULL, TRUE );
	UpdateWindow( hPaintWnd );
	KinResPaintWindow( hPaintWnd );

    return;
}


void KinResPaintWindow( HWND hPaintWnd )
{
	HDC		hDC;
	RECT	rect;

	hDC = GetDC( hPaintWnd );
	GetClientRect( hPaintWnd, &rect );

	DrawKinRes( hDC, rect.right - rect.left, rect.bottom - rect.top );

	ReleaseDC( hPaintWnd, hDC );
}

DWORD GetTextExtent( HDC hDC, WCHAR *buffer, int length )
{
	return 20;  // Bodge it.
}

void DrawKinRes( HDC hDC, long xPixels, long yPixels )
{
	int		xChar, yChar,
			xPos, yPos,
			nPts,
            crosslen,
			i;
	float	x, y,
			s[BUFLEN], v[BUFLEN],
            maxs, maxv;
	WCHAR	buffer[BUFLEN];
	WCHAR	temp[BUFLEN];
	struct exptrun	*ptr;
	DWORD	dwExtent;
	TEXTMETRIC	tm;
	HFONT hFont;
	LOGFONT lf;
    HPEN	hPen, hOldPen;

	SaveDC( hDC );
	hFont = EzCreateFont( hDC, TEXT("Arial"), 100, 0, 0, TRUE);
	GetObject(hFont, sizeof(LOGFONT), &lf);
	SelectObject( hDC, CreateFontIndirect(&lf));
	SetBkColor( hDC, GetSysColor( COLOR_WINDOW ) );
	SetTextColor( hDC, GetSysColor( COLOR_WINDOWTEXT ) );

	// write the results
	GetTextMetrics( hDC, &tm );
	yChar = tm.tmHeight + tm.tmExternalLeading;
    if (nOutliers == 0) {
		SetTextAlign( hDC, TA_CENTER | TA_BASELINE );
		LoadString( hInstance, IDS_RES_LSR, buffer, 200);
		//swprintf( buffer, L"Results obtained from nonlinear least squares regression:" );
		TextOut( hDC, xPixels / 2, yPixels - 3 * yChar, buffer, lstrlen( buffer ) );
		LoadString( hInstance, IDS_95_CONF, buffer, 200);
		//swprintf( buffer, L"(with 95%% confidence limits)" );
	    TextOut( hDC, xPixels / 2, yPixels -  2 * yChar, buffer, lstrlen( buffer ) );
		LoadString( hInstance, IDS_WITH_CONF, temp, 200);
		swprintf( buffer, temp, Km, PLUSMINUS, cKm, Vmax, PLUSMINUS, cVmax, rpH );
		TextOut( hDC, xPixels / 2, yPixels - yChar/2, buffer, lstrlen( buffer ) );
	    yPixels -= 5 * yChar;
	}

	// if we've a negative Km, best not to draw the graph
	if (Km < 0.0) {
		// bounding rectangle
		MoveToEx( hDC, 0, 0 , NULL);
		LineTo( hDC, xPixels, 0 );
		LineTo( hDC, xPixels, yPixels );
		LineTo( hDC, 0, yPixels );
		LineTo( hDC, 0, 0 );
        // graph unavailable message
		LoadString( hInstance, IDS_KDRAW_ERROR1, buffer, 100);
		//swprintf( buffer, L"[Graph unavailable - Km < 0 !]" );
		dwExtent = GetTextExtent( hDC, buffer, lstrlen( buffer ) );
		xPos = (xPixels - LOWORD( dwExtent )) / 2;
		yPos = (yPixels - HIWORD( dwExtent )) / 2;
		TextOut( hDC, xPos, yPos, buffer, lstrlen( buffer ) );
		DeleteObject(SelectObject( hDC, GetStockObject(SYSTEM_FONT)));
		RestoreDC(hDC, -1);
		return;
    }

    // write labels
	SetTextAlign( hDC, TA_CENTER | TA_TOP );
	LoadString( hInstance, IDS_KDRAW_TITLE, buffer, 200);
	//swprintf( buffer, L"Initial Rate Against Substrate Concentration" );
	TextOut( hDC, xPixels / 2, 0, buffer, lstrlen( buffer ) );
	LoadString( hInstance, IDS_KDRAW_ENZYME, temp, 200);
	swprintf( buffer, temp, EnzChar.ref );
	TextOut( hDC, xPixels / 2, yChar, buffer, lstrlen( buffer ) ); 

	create_text_at_point(hDC, Win7_or_later?L"Vₒ  (µmol/min)":L"  (µmol/min)  "  , (XLEFTGAP - XTICLENGTH) * xPixels, (double)yPixels/2.0, 10.0, RGB(0,0,0), 90.0, FALSE, TA_CENTER);
	

	swprintf( buffer, L"v / µmol" );
	dwExtent = GetTextExtent( hDC, buffer, lstrlen( buffer ) );
	xPos = LOWORD( dwExtent );
    //SetTextAlign( hDC, TA_LEFT | TA_BASELINE );
	//TextOut( hDC, 0, yPixels / 2, buffer, lstrlen( buffer ) );
	swprintf( buffer, L"per min" );
	dwExtent = GetTextExtent( hDC, buffer, lstrlen( buffer ) );
	if (LOWORD( dwExtent ) > xPos )
		xPos = LOWORD( dwExtent );
	//SetTextAlign( hDC, TA_LEFT | TA_TOP );
	//TextOut( hDC, 0, yPixels / 2, buffer, lstrlen( buffer ) );
	yPos = 2 * yChar;

	swprintf( buffer, L"S (mM)" );
	SetTextAlign( hDC, TA_CENTER | TA_BASELINE );
    TextOut( hDC, xPixels / 2, yPixels, buffer, lstrlen( buffer ) );

	// resize viewport
	SetViewportOrgEx( hDC, xPos, yPos, NULL );
	xPixels -= xPos;
	yPixels -= (yPos + yChar);

	// scan the data and find biggest values
	maxs = maxv = 0.0;
    nPts = 0;
	for (ptr = listhead; ptr != NULL; ptr = ptr->next )
	{
		if (ptr->sel == TRUE) {
			 s[nPts] = ptr->s;
			 v[nPts] = ptr->v;
			 if (ptr->s > maxs)
				maxs = ptr->s;
			 if (ptr->v > maxv)
				maxv = ptr->v;
			 nPts++;
		}
	}
    maxv = (float)ceil( maxv );

	// axes
	MoveToEx( hDC, (int)(XLEFTGAP * xPixels), (int)((1.0 - YBOTTOMGAP) * yPixels), NULL );
	LineTo( hDC, (int)((XLEFTGAP + XMAIN) * xPixels), (int)((1.0 - YBOTTOMGAP) * yPixels) );
	MoveToEx( hDC, (int)(XLEFTGAP * xPixels), (int)((1 - YBOTTOMGAP) * yPixels), NULL );
	LineTo( hDC, (int)(XLEFTGAP * xPixels), (int)(YTOPGAP * yPixels) );

	// tic labels
    SetTextAlign( hDC, TA_CENTER | TA_TOP ); 
	for (i = 0; i < XTICS; i++) {
		xPos = (int)(((i * XMAIN)/(XTICS - 1) + XLEFTGAP) * xPixels);
		MoveToEx( hDC, xPos, (int)((1.0 - YBOTTOMGAP) * yPixels), NULL );
        yPos = (int)((1.0 - YBOTTOMGAP + YTICLENGTH) * yPixels);
		LineTo( hDC, xPos, yPos );
		swprintf( buffer, L"%.4g", ((float)i / (XTICS - 1)) * maxs );
        TextOut( hDC, xPos, yPos, buffer, lstrlen( buffer ) );
	}

	SetTextAlign( hDC, TA_RIGHT | TA_BASELINE );
	for (i = 1; i < YTICS; i++) {
		yPos = (int)((1.0 - ((i * YMAIN)/(YTICS - 1) + YBOTTOMGAP)) * yPixels);
		MoveToEx( hDC, (int)(XLEFTGAP * xPixels), yPos, NULL );
        xPos = (int)((XLEFTGAP - (XTICLENGTH/2)) * xPixels);
		LineTo( hDC, xPos, yPos );
		swprintf( buffer, L"%.1f", ((float)i / (YTICS - 1) * maxv) );
        TextOut( hDC, xPos, yPos+(yChar/4), buffer, lstrlen( buffer ) );
	}

    // the smooth curve with our values of Km and Vmax
	xPos = (int)(XMAIN * xPixels);
    MoveToEx( hDC, (int)(XLEFTGAP * xPixels), (int)((1.0 - YBOTTOMGAP) * yPixels), NULL );
	for (i = 0; i < xPos; i += 2) {
		x = (i / (XMAIN * xPixels)) * maxs;
		y = (Vmax * x) / (Km + x); 
		LineTo( hDC, i + (XLEFTGAP * xPixels), (1.0 - ((y / maxv)*YMAIN + YBOTTOMGAP)) * yPixels );
	}

	// and our experimental points upon it
	for (i = 0; i < nPts; i++) {
		xPos = ((s[i] / maxs) * XMAIN + XLEFTGAP) * xPixels;
		yPos = (1.0 - ((v[i] / maxv) * YMAIN + YBOTTOMGAP)) * yPixels;
        crosslen = YTICLENGTH * yPixels / 2;
		MoveToEx( hDC, xPos - crosslen, yPos + crosslen, NULL );
		LineTo( hDC, xPos + crosslen, yPos - crosslen );
		MoveToEx( hDC, xPos - crosslen, yPos - crosslen, NULL );
		LineTo( hDC, xPos + crosslen, yPos + crosslen );
	}

	// plus a line at 50Km if appropriate
	if (nOutliers != 0) {
		xPos = (((KM_MAX * Km) / maxs) * XMAIN + XLEFTGAP) * xPixels;
		hPen = CreatePen( PS_DOT, 1, RGB( 0, 0, 0 ) );
        hOldPen = (HPEN)SelectObject( hDC, hPen );
		MoveToEx( hDC, xPos, (YMAIN + YBOTTOMGAP) * yPixels, NULL );
		LineTo( hDC, xPos, YBOTTOMGAP * yPixels );
		SelectObject( hDC, hOldPen );
		DeleteObject( hPen );
		swprintf( buffer, L"%gKm", KM_MAX );
		SetTextAlign( hDC, TA_LEFT | TA_BASELINE );
	    TextOut( hDC, xPos, (YMAIN/2.0 + YBOTTOMGAP) * yPixels, buffer, lstrlen( buffer ) );
	}
	DeleteObject(SelectObject( hDC, GetStockObject(SYSTEM_FONT)));
	RestoreDC(hDC, -1);
	return;
} 
