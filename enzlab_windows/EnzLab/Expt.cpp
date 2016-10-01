/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: EXPT

             PURPOSE: experiment functions: enzyme solution, runs, graph etc
                      - the main child windows of the application

 DATA ITEMS REQUIRED: ECHAR EVENTS ESOLN LIST STIME

 DATA ITEMS RESIDENT: ESOLN

OTHER MODULES CALLED:


******************************************************************************/

#include "stdafx.h"
#pragma hdrstop
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Clock.h"
#include "d_echar.h"
#include "d_events.h"
#include "d_list.h"
#include "d_stime.h"
#include "enzlab.h"
#include "EZFont.h"
#include "func.h"
#include "help.h"
#include "limits.h"
#include "Logs.h"
#include "htmlhelp.h"
#include "Rand.h"
#include "Struct.h"
#include "wchar.h"

#define GWW_ID               0xF000

// ESOLN data item
int nSamples;
unsigned int nulEnzyme;

extern HINSTANCE hInstance;
extern WCHAR *szHelpFilename;
extern int xMainClient, yMainClient;
extern BOOL Win7_or_later;

// Handles to child windows
HWND hEnzSoln;
HWND hEnzRun;
HWND hEnzGraph;
HWND hEnzInfo;

// handles to child window controls of child windows
HWND hCH_RUNNO;
HWND hSTATIC1[ENZRUN_EDITS];
HWND hEDITS[ENZRUN_EDITS];
HWND hSTATIC2[ENZRUN_EDITS];
HWND hCH_V;
HWND hCH_REPEAT;
HWND hCH_GO;
HWND hActiveRunButton;

// number of edit control which most recently had input focus
int nEditLast = 0;

// pointers to edit control window function etc
WNDPROC lpfnOldEdit[ENZRUN_EDITS];
WNDPROC lpfnEditProc;

long   yTopHeight,
		xMin, yMin,
	 	xChar, yChar,	
		xChar2, yChar2,
		xEnzSoln, yEnzSoln,
		xEnzRun, yEnzRun,
		yEnzGraph,
		xcEnzGraph, ycEnzGraph,
        xEnzInfo, yEnzInfo;

	

void InitExptVar( void )
{
	// initialise ESOLN
	// nSamples = GetPrivateProfileString( "Initialisation", "EnzymeSamples", NSAMPLES, "Enzlab.ini" );
	nSamples = NSAMPLES;
	nulEnzyme = 0;
}


void InitExptWin( HWND hWndParent, HINSTANCE hInstance )
{
	TEXTMETRIC tm;
	HDC hDC;
    int	n;

    // Get text sizes etc
	hDC = GetDC( hWndParent );
	GetTextMetrics( hDC, &tm );
	xChar = tm.tmAveCharWidth;
    xChar2 = xChar / 2;
	yChar = tm.tmHeight + tm.tmExternalLeading;
    yChar2 = yChar / 2;
	ReleaseDC( hWndParent, hDC );
    yTopHeight = GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYMENU );

    // Calculate sizes of windows
	xEnzSoln = ENZSOLN_WD * xChar;
	yEnzSoln = ENZSOLN_HT * yChar;
	xEnzRun = ENZRUN_WD * xChar;
    yEnzRun = ENZRUN_HT * yChar;
//	yEnzRun = ENZRUN_HT * yChar + yChar/2;
	yEnzGraph = ENZCLK_HT * yChar;
	xcEnzGraph = xMainClient - xEnzRun;
	ycEnzGraph = yMainClient - yEnzGraph;
	xEnzInfo = ENZINFO_WD * xChar;
    yEnzInfo = ENZCLK_HT * yChar;

	xMin = (ENZSOLN_WD + ENZINFO_WD + ENZCLK_WD) * xChar;
	yMin = yEnzSoln + yEnzRun + yTopHeight;

 	if (yMainClient - yEnzSoln > yEnzRun)
		yEnzRun = yMainClient - yEnzSoln;

	if (xMainClient > xEnzSoln + xEnzInfo + (ENZCLK_HT * xChar))
		xEnzInfo = xMainClient - (xEnzSoln + ENZCLK_WD * xChar);

    // Create / display windows
    // EnzSoln
	hEnzSoln = CreateWindow(
		L"EnzSoln",					// window class
		NULL,						// window caption
		WS_CHILD | 					// window style
			WS_CLIPSIBLINGS | WS_BORDER | WS_VISIBLE,
		0,			 				// initial x position
		0,			 				// initial y position
		xEnzSoln,					// initial x size
		yEnzSoln,					// initial y size
		hWndParent,					// parent window handle
		NULL,						// window menu handle
		hInstance,					// program instance handle
		NULL						// create parameters
    );
	UpdateWindow( hEnzSoln );

    // EnzRun
	hEnzRun = CreateWindow(
		L"EnzRun",					// window class
		NULL,						// window caption
		WS_CHILD | 					// window style
			WS_CLIPSIBLINGS | WS_BORDER | WS_VISIBLE,
		0,			 				// initial x position
		yEnzSoln,	 				// initial y position
		xEnzRun,					// initial x size
		yEnzRun,					// initial y size
		hWndParent,					// parent window handle
		NULL,						// window menu handle
		hInstance,					// program instance handle
		NULL						// create parameters
    );
	UpdateWindow( hEnzRun );

    // EnzGraph
	hEnzGraph = CreateWindow(
		L"EnzGraph",					// window class
		NULL,						// window caption
		WS_CHILD | 					// window style
			WS_CLIPSIBLINGS | WS_BORDER | WS_VISIBLE,
		xEnzRun,	 				// initial x position
		yEnzGraph,	 				// initial y position
		xcEnzGraph,					// initial x size
		ycEnzGraph,					// initial y size
		hWndParent,					// parent window handle
		NULL,						// window menu handle
		hInstance,					// program instance handle
		NULL						// create parameters
    );
	UpdateWindow( hEnzGraph );

    // EnzInfo
	hEnzInfo = CreateWindow(
		L"EnzInfo",					// window class
		NULL,						// window caption
		WS_CHILD | 					// window style
			WS_CLIPSIBLINGS | WS_BORDER | WS_VISIBLE,
		xEnzRun,	 				// initial x position
		0,			 				// initial y position
		xEnzInfo,					// initial x size
		yEnzInfo,					// initial y size
		hWndParent,					// parent window handle
		NULL,						// window menu handle
		hInstance,					// program instance handle
		NULL						// create parameters
    );
	UpdateWindow( hEnzInfo );

	// subclass the edit control window function
    lpfnEditProc = (WNDPROC)MakeProcInstance( (FARPROC)EditProc, hInstance );
	for (n = 0; n < ENZRUN_EDITS; n++ ) {
		lpfnOldEdit[n] = (WNDPROC)GetWindowLong( hEDITS[n], GWL_WNDPROC );
		SetWindowLong( hEDITS[n], GWL_WNDPROC, (LONG)lpfnEditProc );
	}

	return;
}

void GiveEnzRunFocus( void )
{
	SetFocus( hEDITS[nEditLast] );
    return;
}


void SizeExptWindows( void )
{
	MoveWindow( hEnzRun, 0, yEnzSoln, xEnzRun, yMainClient - yEnzSoln, TRUE );
	MoveWindow( hEnzGraph, xEnzRun, yEnzGraph, xMainClient - xEnzRun, yMainClient - yEnzGraph, TRUE );
	MoveWindow( hEnzInfo, xEnzRun, 0, xMainClient - (ENZCLK_WD * xChar + xEnzRun), yEnzInfo, TRUE );
}

void redrawControlInDialog(HWND hCtrl, HWND hDlg)
{
	RECT rect;	
	GetClientRect(hCtrl, &rect);
	InvalidateRect(hCtrl, &rect, TRUE);
	MapWindowPoints(hCtrl, hDlg, (POINT *) &rect, 2);
	RedrawWindow(hDlg, &rect, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void UpdateRunNo( void )
{
	WCHAR	buffer[BUFLEN];
	WCHAR temp[105];

	LoadString( hInstance, IDS_RUNNO, temp, 100);

	swprintf( buffer, temp, Events.nResults );
	SetWindowText( hCH_RUNNO, buffer );
	

}


LRESULT CALLBACK EnzSolnWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	HDC		hDC;
	HFONT hFont;
	LOGFONT lf;
	PAINTSTRUCT	ps;
    RECT	rect;
    WCHAR	buffer[BUFLEN];
	WCHAR temp[205];
	HBRUSH hBrush;
             
	switch (iMessage) {
		case WM_PAINT:
			hDC = BeginPaint( hWnd, &ps );

			hBrush = (HBRUSH)GetSysColorBrush(COLOR_MENU);
			SaveDC( hDC );
			hFont = EzCreateFont( hDC, TEXT("Arial"), 100, 0, 0, TRUE);
			GetObject(hFont, sizeof(LOGFONT), &lf);
			SelectObject( hDC, CreateFontIndirect(&lf));
		
			SelectObject(hDC, hBrush);
			SetBkMode(hDC,TRANSPARENT);
			
            SetTextColor( hDC, GetSysColor( COLOR_WINDOWTEXT ) );
			GetClientRect( hWnd, &rect );
			Rectangle(hDC, rect.left, rect.top, rect.right+1, rect.bottom+1 );
			if (nulEnzyme == 0)
				LoadString( hInstance, IDS_ENZ_REMAIN1, buffer, 200);
            else
			{
				LoadString( hInstance, IDS_ENZ_REMAIN2, temp, 200);
				swprintf( buffer, temp, nulEnzyme );
			}
			DrawText( hDC, buffer, lstrlen( buffer ), &rect, DT_CENTER );
			SetBkMode(hDC, OPAQUE);

			DeleteObject(SelectObject( hDC, GetStockObject(SYSTEM_FONT)));
			RestoreDC(hDC, -1);
			EndPaint( hWnd, &ps );

			break;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc( hWnd, iMessage, wParam, lParam );
	}

	return 0L;
}


WCHAR *szStaticText1[] = { L"", L"Vol:", L"pH:", L"S:", L"I:" };
WCHAR *szStaticText2[] = { L"", L"µL", L"", L"mM", L"mM" };

LRESULT CALLBACK EnzRunWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	static long	xSp, ySp,
					xClient, yClient,
                    yEdit,
                    yLine;
	long 	n,
			yStart;
	CREATESTRUCT FAR	*lpCS;
	WCHAR	buffer[BUFLEN];
	WCHAR temp[205];
	PAINTSTRUCT	ps;
	HDC hDC,hdcStatic;
	static HFONT hFont;
	LOGFONT lf;
	HBRUSH hBrush;
	RECT rect;
		             
	switch (iMessage) {
	case WM_CTLCOLORSTATIC:
			hdcStatic = (HDC) wParam;
			SetTextColor(hdcStatic, RGB(0,0,0));
			SetBkColor(hdcStatic, GetSysColor(COLOR_MENU));
			return (INT_PTR)GetSysColorBrush(COLOR_MENU);

		case WM_PAINT:
			hDC = BeginPaint( hWnd, &ps );
		//	hBrush = CreateSolidBrush(GetSysColor(COLOR_MENU));

			hBrush = (HBRUSH)GetSysColorBrush(COLOR_MENU);

			SelectObject(hDC, hBrush);
			GetClientRect( hWnd, &rect );
			Rectangle(hDC, rect.left, rect.top-1, rect.right+1, rect.bottom );

			//DeleteObject( hBrush);
			
			EndPaint( hWnd, &ps );
			break;

		case WM_CREATE:

			hFont = EzCreateFont( GetDC(hWnd), TEXT("Arial"), 100, 0, 0, TRUE);

			lpCS = (CREATESTRUCT FAR *)lParam;
			xClient = lpCS->cx;
			yClient = lpCS->cy;
			yLine = 2 * yChar;
			yEdit = (3 * yChar) / 2; 
			ySp = (yClient - yChar * ENZRUN_HT) / ENZRUN_ITEMS;




        	// Create the child window controls
			LoadString( hInstance, IDS_RUNNO1, buffer, 100);
			hCH_RUNNO = CreateWindow( L"static",buffer, WS_CHILD | WS_VISIBLE | SS_CENTER,
					5, ySp + yChar2, xClient, yChar,
					hWnd, (HMENU)IDCH_RUNNO, hInstance, NULL );
			for (n = 1; n <= ENZRUN_EDITS; n++ ) {
				yStart = n * yLine + (n + 1)*ySp;
				hSTATIC1[n-1] = CreateWindow( L"static", szStaticText1[n], WS_CHILD | WS_VISIBLE | SS_CENTER,
						xChar2, yStart + yChar2, 4 * xChar, yChar,
						hWnd, (HMENU)(n * 10), hInstance, NULL );
				hEDITS[n-1] = CreateWindow( L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL | ES_RIGHT,
						5 * xChar, yStart + yChar2/2, (18 * xChar)/2, yEdit,   
						hWnd, (HMENU)((n * 10) + 1), hInstance, NULL );
				hSTATIC2[n-1] = CreateWindow( L"static", szStaticText2[n], WS_CHILD | WS_VISIBLE | SS_LEFT,
						(29 * xChar)/2, yStart + yChar2, 4 * xChar, yChar,
						hWnd, (HMENU)((n * 10) + 2), hInstance, NULL );
			}
			hCH_V = CreateWindow( L"static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER,
					5, 5 * yLine + 6 * ySp + yChar2, xClient, yChar,
					hWnd, (HMENU)IDCH_V, hInstance, NULL );
			LoadString( hInstance, IDS_REPEAT, buffer, 100);
			hCH_REPEAT = CreateWindow( L"button", buffer, WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON,
					xChar2,  6 * yLine + 7 * ySp + yChar2, 8 * xChar, 2 * yChar,
					hWnd, (HMENU)IDCH_REPEAT, hInstance, NULL );
			LoadString( hInstance, IDS_GO, buffer, 100);
			hCH_GO = CreateWindow( L"button", buffer, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED,
					(19 * xChar)/2, 6 * yLine + 7 * ySp + yChar2, 8 * xChar, 2 * yChar,
					hWnd, (HMENU)IDCH_GO, hInstance, NULL );
            hActiveRunButton = NULL;
            break;

		case WM_SIZE:
			xClient = LOWORD(lParam);
			yClient = HIWORD(lParam);
			ySp = (yClient - yChar * ENZRUN_HT) / ENZRUN_ITEMS;
            // the original design is for an 18 char wide box - we add some extra space if we have more!
            xSp = (xClient - (18 * xChar)) / 2;

        	// Move the child window controls
			MoveWindow( hCH_RUNNO,
					5, ySp + yChar2, xClient, yChar,
                    TRUE );
			SendMessage( hCH_RUNNO, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
			for (n = 1; n <= ENZRUN_EDITS; n++ ) {
				yStart = n * yLine + (n + 1)*ySp;
				MoveWindow( hSTATIC1[n-1],
						xSp + xChar2, yStart + yChar2, 4 * xChar, yChar,
                        TRUE );
				SendMessage( hSTATIC1[n-1], WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
				MoveWindow( hEDITS[n-1],
						xSp + 5 * xChar, yStart + yChar2/2, (18 * xChar)/2, yEdit,   
                		TRUE );
				SendMessage(  hEDITS[n-1], WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
				MoveWindow( hSTATIC2[n-1],
						xSp + (29 * xChar)/2, yStart + yChar2, 4 * xChar, yChar,
                        TRUE );
				SendMessage( hSTATIC2[n-1], WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
			}
			MoveWindow( hCH_V,
					5, 5 * yLine + 6 * ySp + yChar2, xClient, yChar,
            		TRUE );
			SendMessage( hCH_V, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
			MoveWindow( hCH_REPEAT,
					xSp + xChar2,  6 * yLine + 7 * ySp + yChar2, 8 * xChar, 2 * yChar,
                    TRUE );
			SendMessage( hCH_REPEAT, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
			MoveWindow( hCH_GO,
					xSp + (19 * xChar)/2, 6 * yLine + 7 * ySp + yChar2, 8 * xChar, 2 * yChar,
                    TRUE );
			SendMessage( hCH_GO, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
            break;

		case WM_SETFOCUS:
			SetFocus( hEDITS[nEditLast] );
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDCH_REPEAT:
					UpdateRunNo();
					DoEnzRun( hWnd );
					break;

				case IDCH_GO:
					if (DoEnzRun( hWnd ) == TRUE) {
                    	hActiveRunButton = hCH_REPEAT;
						EnableWindow( hCH_REPEAT, TRUE );
						EnableWindow( hCH_GO, FALSE );
                    }
                    break;

				case IDCH_VOLEDIT:
				case IDCH_PHEDIT:
				case IDCH_SEDIT:
				case IDCH_IEDIT:
					switch (HIWORD( wParam )) {
                    	case EN_ERRSPACE:
							MessageBox( hWnd, L"An edit box has run out of memory space!", L"EnzLab: WARNING", MB_OK | MB_ICONEXCLAMATION );
							break;

						case EN_CHANGE:
							
                        	if (Events.bEnzSel == TRUE) {
	                        	hActiveRunButton = hCH_GO;
								EnableWindow( hCH_REPEAT, FALSE );
								EnableWindow( hCH_GO, TRUE );
								SetWindowText( hCH_V, L"" );
								UpdateRunNo();
                            }
							break;
                    }
					break;

				//default:
					//swprintf( buffer, L"Ooops! An unidentifiable WM_COMMAND message was received by EnzRunWndProc.\n[wParam = %ld.]", wParam );
					//MessageBox( hWnd, buffer, L"EnzLab: INTERNAL ERROR", MB_ICONEXCLAMATION );
			}
			break;

		case WM_DESTROY:
			DeleteObject(SelectObject( GetDC(hWnd), GetStockObject(SYSTEM_FONT)));
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc( hWnd, iMessage, wParam, lParam );
	}

	return 0L;
}

LRESULT CALLBACK EditProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	static BOOL	bButtonDown = FALSE;

	switch (iMessage) {
		case WM_KEYDOWN:
			if (Events.bEnzSel == TRUE && wParam == VK_RETURN) {
 				bButtonDown = TRUE;
				SendMessage( hActiveRunButton, BM_SETSTATE, (WPARAM)TRUE, 0L );
				return 0L;
            }
			break;

		case WM_KEYUP:
			if (Events.bEnzSel == TRUE && wParam == VK_RETURN && bButtonDown == TRUE) {
            	bButtonDown = FALSE;
				SendMessage( hActiveRunButton, BM_SETSTATE, (WPARAM)FALSE, 0L );
                if (hActiveRunButton == hCH_GO)
					PostMessage( hEnzRun, WM_COMMAND, (WPARAM)IDCH_GO, 0L );
				else
					PostMessage( hEnzRun, WM_COMMAND, (WPARAM)IDCH_REPEAT, 0L );
                return 0L;
			}
            break;

		case WM_CHAR:
			if (wParam == VK_TAB) {
				if (GetKeyState( VK_SHIFT ) < 0)
					nEditLast += (ENZRUN_EDITS - 1);
				else
					nEditLast++;
                nEditLast %= ENZRUN_EDITS;
				SetFocus( hEDITS[nEditLast] );
                return 0L;
			}
			else if ((wParam < '0' || wParam > '9') && wParam != '.' && wParam != VK_BACK)
				return 0L;
			break;

		case WM_SETFOCUS:
			nEditLast = ((GetWindowLong(hWnd, GWL_ID) - 1) / 10) - 1;
            break;
	}
	return CallWindowProc( lpfnOldEdit[nEditLast], hWnd, iMessage, wParam, lParam );
}


LRESULT CALLBACK EnzInfoWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	HDC		hDC;
	HBRUSH hBrush;
	HFONT hFont;
	LOGFONT lf;
	PAINTSTRUCT	ps;
    RECT	rect;
	WCHAR	text[BUFLEN];
             
	switch (iMessage) {
		case WM_PAINT:
			hDC = BeginPaint( hWnd, &ps );

			SaveDC( hDC );
			hFont = EzCreateFont( hDC, TEXT("Arial"), 100, 0, 0, TRUE);
			GetObject(hFont, sizeof(LOGFONT), &lf);
			SelectObject( hDC, CreateFontIndirect(&lf));

			hBrush = (HBRUSH)GetSysColorBrush(COLOR_MENU);
			SelectObject(hDC, hBrush);
			SetBkMode(hDC,TRANSPARENT);
			
            SetTextColor( hDC, GetSysColor( COLOR_WINDOWTEXT ) );
			GetClientRect( hWnd, &rect );
			Rectangle(hDC, rect.left-1, rect.top, rect.right+1, rect.bottom+1);
			if (Win7_or_later)
				LoadString( hInstance, IDS_FUNCTIONS1, text, 200);
			else
				LoadString( hInstance, IDS_FUNCTIONS2, text, 200);
			//lstrcpy(text, Win7_or_later?L"           \t\tF1 - Help Contents  \r\n     Press:\tF2 - How the program works  \r\n           \t\tF3 - What to do next  ":L"           \t\tF1 - Help Contents  \r\n           \t\tF2 - How the program works  \r\n           \t\tF3 - What to do next  ");
			DrawText( hDC, text, lstrlen( text ), &rect, DT_LEFT | DT_EXPANDTABS);

			DeleteObject(SelectObject( hDC, GetStockObject(SYSTEM_FONT)));
			RestoreDC(hDC, -1);
			EndPaint( hWnd, &ps );
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc( hWnd, iMessage, wParam, lParam );
	}

	return 0L;
}


LRESULT CALLBACK EnzGraphWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	HDC		hDC;
	PAINTSTRUCT	ps;
	
             
	switch (iMessage) {
		case WM_PAINT:
			hDC = BeginPaint( hWnd, &ps );
			PaintEnzGraph( hWnd, hDC, 0.0 );
			EndPaint( hWnd, &ps );
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc( hWnd, iMessage, wParam, lParam );
	}

	return 0L;
}


LRESULT CALLBACK NewDayDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	WCHAR	buffer[BUFLEN];
	WCHAR	temp[105];
	static int		nToThaw;
	static BOOL		bIsOk,
    				bHelp = FALSE;

	switch (iMessage) {
		case WM_INITDIALOG:
			LoadString( hInstance, IDS_ITS_DAY, temp, 100);
        	swprintf( buffer, temp, clkDay );
			SetDlgItemText( hDlg, IDD_DAY, buffer );
			LoadString( hInstance, IDS_YOU_HAVE, temp, 100);
			swprintf( buffer, temp, nSamples );
			SetDlgItemText( hDlg, IDD_NRSAMPLES, buffer );
			EnableWindow( GetDlgItem( hDlg, IDOK ), FALSE );
			SetFocus( GetDlgItem( hDlg, IDD_THAWSAMPLES ) );
			if (nSamples == 0) {
				LoadString( hInstance, IDS_NONE_LEFT, buffer, 100);
				SetDlgItemText( hDlg, IDD_NRSAMPLES, buffer );
				//swprintf( buffer, L"(You have none left!)" );
				SetDlgItemText( hDlg, IDD_THAWSAMPLES, L"0" );
				nToThaw = 0;
                bIsOk = TRUE;
                EnableWindow( GetDlgItem( hDlg, IDOK ), TRUE );
			}
			return FALSE;

		case WM_COMMAND:
			
			switch (LOWORD(wParam)) {
				
			
				case IDOK:
					nSamples -= nToThaw;
					nulEnzyme = nToThaw * SAMPLE_SIZE;
                    InvalidateRect( hEnzSoln, NULL, FALSE );
					//if (bHelp == TRUE)
					//	HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"howt000g.htm" );
					EndDialog( hDlg, TRUE );
					break;

				case IDD_HELP:
					bHelp = TRUE;
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg0008.htm" );
					break;

				case IDD_THAWSAMPLES:
					
					switch (HIWORD( wParam )) {
						case EN_ERRSPACE:
							MessageBox( hDlg, L"An edit box has run out of memory space!", L"EnzLab: WARNING", MB_OK | MB_ICONEXCLAMATION );
							break;

						case EN_CHANGE:
							
							nToThaw = (int)GetDlgItemInt( hDlg, IDD_THAWSAMPLES, &bIsOk, TRUE );
							
							if (bIsOk) {
								if (nToThaw > nSamples)
									bIsOk = FALSE;
							}
							EnableWindow( GetDlgItem( hDlg, IDOK ), bIsOk );
							break;

						
                    }
					break;

				default:
					return FALSE;
			}
			break;

		default:
			return FALSE;
	}

    return TRUE;
}



void NewDay( HWND hWnd, HINSTANCE hInstance )
{
	DLGPROC lpfnDlgProc;
	WCHAR buffer[BUFLEN];
	WCHAR temp[205];

	ClockSet( hWnd, clkDay+1, 9, 0 );
	nulEnzyme = 0;
	InvalidateRect( hEnzSoln, NULL, FALSE );

    lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)NewDayDlgProc, hInstance );
	DialogBox( hInstance, L"NewDay", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );


	LoadString( hInstance, IDS_SHOULD_PLAN, buffer, 500);
	LoadString( hInstance, IDS_APP_TITLE, temp, 100);
	MessageBox( hWnd,buffer,temp, MB_OK );

	LoadString( hInstance, IDS_PLAN_TIME, buffer, 500);
	MessageBox( hWnd, buffer, temp, MB_OK );
	ClockFwd( hWnd, CLK_HOUR );
}



BOOL DoEnzRun( HWND hWnd )
{
	WCHAR    buffer[BUFLEN];
	WCHAR temp[200];
//	struct ExptRun  *r;
//	struct ExptRun  *prev;
	int     j, m, n, p,
			nSplatChance;
    float   h, ka, kb, ki, fac, km, vmax;
	float   vol, ph, i, s, v;
    static int	idnum = -1;

	LoadString( hInstance, IDS_APP_TITLE, temp, 100);

    // do they have any thawed enzyme left?
	if (nulEnzyme == 0) {
		if (nSamples == 0)
			LoadString( hInstance, IDS_NOWT_LEFT, buffer, 500);
			//swprintf( buffer, L"You have no enzyme left, thawed or otherwise.\nYou can do no more experimental work." );
		else
			LoadString( hInstance, IDS_NO_THAWED, buffer, 500);
			//swprintf( buffer, L"You have no thawed enzyme left: you can do no more experimental work today.\n(You could thaw some more tomorrow morning.)" );
		
		MessageBox( hWnd, buffer, temp, MB_OK );
		return FALSE;
    }

	/*
		 Get the values from the boxes and check they're OK, returning FALSE if they're not
	*/
	// vol
	
	GetWindowText( hEDITS[0], buffer, 300);
	vol = (float)_wtof( buffer );
	if (vol > (nulEnzyme - VOL_RESIDUE)) {
		LoadString( hInstance, IDS_NOT_ENOUGH, temp, 200);
		swprintf( buffer, temp, vol );
		if (vol <= nulEnzyme)
		{
			LoadString( hInstance, IDS_LAST_BIT, temp, 200);
			swprintf( buffer, temp, vol, VOL_RESIDUE );
		}
		LoadString( hInstance, IDS_APP_TITLE, temp, 100);
		MessageBox( hWnd, buffer, temp, MB_OK );
		return FALSE;
    }
	else if (vol < VOL_MIN) {
		LoadString( hInstance, IDS_NEED_MORE, buffer, 200);
		LoadString( hInstance, IDS_APP_TITLE, temp, 100);
		MessageBox( hWnd, buffer, temp, MB_OK );
		return FALSE;
    }

	// pH 
	GetWindowText( hEDITS[1], buffer, 300);
	ph = (float)_wtof( buffer  );
	if (ph > PH_MAX || ph < PH_MIN) {
		LoadString (hInstance, IDS_BAD_PH, temp, 200);
		swprintf( buffer, temp, PH_MIN, PH_MAX );
		LoadString( hInstance, IDS_APP_TITLE, temp, 100);
		MessageBox( hWnd, buffer, temp, MB_OK );
		return FALSE;
    }

	// S
	GetWindowText( hEDITS[2], buffer, 300);
	s = (float)_wtof( buffer  );
	if (s < S_MIN) {
		LoadString (hInstance, IDS_MORE_SUBSTRATE, buffer, 200);
		LoadString( hInstance, IDS_APP_TITLE, temp, 100);
		MessageBox( hWnd, buffer, temp, MB_OK );
		return FALSE;
	}
	else if (s > S_MAX) {
		LoadString (hInstance, IDS_WASTE, buffer, 200);
		LoadString( hInstance, IDS_APP_TITLE, temp, 100);
		MessageBox( hWnd, buffer, temp, MB_OK );
		return FALSE;
    }

	// I
	GetWindowText( hEDITS[3], buffer, 300);
	i = (float)_wtof( buffer  );
	if (i < I_MIN) {
		LoadString (hInstance, IDS_SILLY, buffer, 200);
		LoadString( hInstance, IDS_APP_TITLE, temp, 100);
		MessageBox( hWnd, buffer, temp, MB_OK );
		return FALSE;
    }


	/*
		 Do the run!
    */
	// Remove the enzyme we're using from the bottle
	nulEnzyme -= vol;
	InvalidateRect( hEnzSoln, NULL, FALSE );

	// calculate Km and Vmax */
    h = (float)(powf( 10.0, -ph ) / 1000.0);

    ka = (float)(powf( 10.0, -EnzChar.Ka ) / 1000.0);
	kb = (float)(powf( 10.0, -EnzChar.Kb ) / 1000.0);
    ki = (float)(KIF * EnzChar.Kn);
    m = 1 - EnzChar.m;
    n = 1 - EnzChar.n;
    if (EnzChar.p == 2 || EnzChar.p == 0)
        p = 1;
    else
        p = 0;
    fac = (1 + (h/ka)*m + (kb/h)*n) * (1 + (i/ki)*p);
    km = EnzChar.Kn * fac;
        
    m = EnzChar.m;
    n = EnzChar.n;
    if (EnzChar.p == 2 || EnzChar.p == 1)
        p = 1;
    else
        p = 0;
    fac = (1 + (h/ka)*m + (kb/h)*n) * (1 + (i/ki)*p);
    vmax = (float)((0.25 * vol * EnzChar.mod) / fac);

    // calculate v
	v = (vmax * s) / (km + s);

	v *= (float)((gasdev( &idnum ) * ERROR_SIGMA) + 1);

	// adjust precision
	swprintf( buffer, L"%0.4f", v );
    v = (float)_wtof( buffer );


	/*
		Check for a splat, and take appropriate action
	*/
    // set splat chance to 1 in ...
	switch (Events.todNow) {
		case TOD_MORNING:
		case TOD_AFTERNOON:
			nSplatChance = SPLATCHANCE_DAY;
			break;
		case TOD_EEVEN:
			nSplatChance = SPLATCHANCE_EEVEN;
			break;
		case TOD_LEVEN:
			nSplatChance = SPLATCHANCE_LEVEN;
			break;
		case TOD_LATE:
			nSplatChance = SPLATCHANCE_LATE;
			break;
		default:
			MessageBox( hWnd, L"Unknown time of day in EnzRun function!\n[This should never happen!]",
				L"EnzLab: INTERNAL ERROR", MB_ICONEXCLAMATION | MB_OK );
			nSplatChance = SPLATCHANCE_DAY;
	};
    // if there's a splat, say so and set v and the v text appropriately
	// no splat on first run or it will mess up the display

	unsigned int rand_no;
	rand_s(&rand_no);
	if ((rand_no % nSplatChance == 0) && (Events.nResults > 1)) { 

		LoadString (hInstance, IDS_OOPS, buffer, 500);
		LoadString( hInstance, IDS_APP_TITLE, temp, 100);
		MessageBox( hWnd, buffer, temp, MB_OK );

		LoadString( hInstance, IDS_SPLAT, buffer, 100);

		v = V_SPLAT;
	}
	// otherwise, check if too fast/slow, and set the v text properly
	else {
		if (v > V_FAST) {
			v = V_TOOFAST;
			LoadString( hInstance, IDS_FAST, buffer, 100);
        }
		else if (v < V_SLOW) {
			v = V_TOOSLOW;
			LoadString( hInstance, IDS_SLOW, buffer, 100);
        }
        else
		{
			swprintf( buffer, Win7_or_later?L"Vₒ = %0.3g µmol/min":L"V = %0.3g µmol/min", v );
		}
    }

	// put the results into the result list then move on to next expt run
	AddToList( hWnd, vol, ph, s, i, v );

	Events.nResults++;

	// draw the graph
    // set graph_delay negative if it's a repeat

	PaintEnzGraph( hEnzGraph, NULL, (hActiveRunButton == hCH_REPEAT) ? -GRAPH_DELAY : GRAPH_DELAY );
    PaintEnzGraph( hEnzGraph, NULL, 0.0 ); // This gets rid of artifacts caused by overwriting.
	// show our results in numerical form
	SetWindowText( hCH_V, buffer );

	// it's all taken us half an hour
    return ClockFwd( hWnd, CLK_HALFHOUR );
}


void EmptyEnzRunControls( void )
{
	int		n;

	for (n = 1; n <= ENZRUN_EDITS; n++ )
    	SetWindowText( hEDITS[n-1], L"" );
	SetWindowText( hCH_V, L"" );
	UpdateRunNo();
	if (Events.bEnzSel == FALSE) {
		hActiveRunButton = NULL;
		EnableWindow( hCH_REPEAT, FALSE );
		EnableWindow( hCH_GO, FALSE );
	}
}
