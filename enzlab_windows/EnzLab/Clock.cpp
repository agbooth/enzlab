/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: CLOCK		

             PURPOSE: clock functions etc

 DATA ITEMS REQUIRED: STIME EVENTS

 DATA ITEMS RESIDENT: STIME

OTHER MODULES CALLED:


******************************************************************************/


#include "stdafx.h"
#pragma hdrstop

#include "d_events.h"
#include "clock.h"
#include "enzlab.h"
#include "EZFont.h"
#include "func.h"
#include "limits.h"

// STIME data item
unsigned int clkDay;
unsigned int clkHour;
unsigned int clkMin;

typedef BOOL CLOCKFUNC( HWND hWnd );

struct clkevent {
	unsigned hr;
	unsigned min;
	CLOCKFUNC *fn;
} ClkEvents[] = { 
    12, 30, LunchTime,
    17, 30, TeaTime,
    20,  0, EevenTime,
    22,  0, LevenTime,
    24,  0, BedTime
};
#define NREVENTS    (int)(sizeof( ClkEvents )/sizeof( struct clkevent ))


WCHAR szDay[8][55];


// clock on/off switch
BOOL bClkSwitch = TRUE;

// handle to 'EnzClk' child window
HWND hEnzClk;

extern int	xMainClient, yMainClient;
extern long				xChar, yChar;


int	xEnzClk, yEnzClk,
		xcEnzClk, ycEnzClk;

extern HINSTANCE hInstance;

void InitClockWin( HWND hWndParent, HINSTANCE hInstance )
{
	LoadString(hInstance, IDS_SUNDAY, szDay[0], 50);
	LoadString(hInstance, IDS_MONDAY, szDay[1], 50);
	LoadString(hInstance, IDS_TUESDAY, szDay[2], 50);
	LoadString(hInstance, IDS_WEDNESDAY, szDay[3], 50);
	LoadString(hInstance, IDS_THURSDAY, szDay[4], 50);
	LoadString(hInstance, IDS_FRIDAY, szDay[5], 50);
	LoadString(hInstance, IDS_SATURDAY, szDay[6], 50);
	LoadString(hInstance, IDS_ERRORDAY, szDay[7], 50);
	
	if (bClkSwitch == TRUE) {
		// calculate size / position of window
		xcEnzClk = ENZCLK_WD * xChar;
	    ycEnzClk = ENZCLK_HT * yChar; 
		xEnzClk = xMainClient - xcEnzClk;
		yEnzClk = 0;

		// create / display window
		hEnzClk = CreateWindow(
			L"EnzClk",					// window class
			NULL,						// window caption
			WS_CHILD | 					// window style
				WS_CLIPSIBLINGS | WS_BORDER | WS_VISIBLE,
			xEnzClk,	 				// initial x position
			yEnzClk,					// initial y position
			xcEnzClk,					// initial x size
			ycEnzClk,					// initial y size
			hWndParent,					// parent window handle
			NULL,						// window menu handle
			hInstance,					// program instance handle
			NULL						// create parameters
	    );
		UpdateWindow( hEnzClk );
	}
	return;
}


void InitClockVar( HWND hWnd )
{
	ClockSet( hWnd, 0, 9, 0 );

	return;
}



void SizeEnzClk( void )
{
	MoveWindow( hEnzClk, xMainClient - xcEnzClk, yEnzClk, xcEnzClk, ycEnzClk, TRUE );
}



LRESULT CALLBACK EnzClkWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	HDC 	hDC;
	HFONT hFont;
	LOGFONT lf;
	PAINTSTRUCT	ps;
    RECT 	rect;
    WCHAR	buffer[BUFLEN];
	WCHAR temp [210];
	HBRUSH hBrush;
             
	switch (iMessage) {
		case WM_PAINT:
			if (clkDay == 0) {
				LoadString( hInstance, IDS_SIMTIME1, temp, 200); 
				swprintf( buffer, temp, clkHour, clkMin );
			}
			else {   
				LoadString( hInstance, IDS_SIMTIME2, temp, 200); 
				swprintf( buffer, temp,
						clkHour, clkMin, szDay[clkDay % 7], clkDay );
			}   
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
			Rectangle( hDC, rect.left-1, rect.top, rect.right, rect.bottom+1);
			DrawText( hDC, buffer, lstrlen( buffer ), &rect, DT_CENTER );

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


void ClockSet( HWND hWnd, unsigned day, unsigned hr, unsigned min )
{
	clkDay = day;
	clkHour = hr;
    clkMin = min;
	if (clkHour >= 12 && clkMin > 0) {
		MessageBox( hWnd, L"Don't know what time of day it is!\n[I can't start days except in the morning.]", L"EnzLab: INTERNAL ERROR", MB_ICONEXCLAMATION );
    }
	Events.todNow = TOD_MORNING;

	InvalidateRect( hEnzClk, NULL, FALSE );
    return;
}


int ClockFwd( HWND hWnd, enum timeints timeincr )
{
    int     i;
    
	if (bClkSwitch == TRUE) {
		if (timeincr == CLK_HALFHOUR)
			i = 30;
		else if (timeincr == CLK_HOUR)
			i = 60;
		else
			i = 0;

        while (i-- > 0) {
            ClockInc();
            ShowClock();
		}

        for (i = 0; i < NREVENTS; i++) {
            if (ClkEvents[i].hr == clkHour && ClkEvents[i].min == clkMin)
            	return (*ClkEvents[i].fn)( hWnd );
        }
	}

    return TRUE;
}


void ClockInc( void )
{
	clkMin++;
    if (clkMin == 60) {
        clkMin = 0;
		clkHour++;
    }

    return;
}


void ClockOff( void )
{
    bClkSwitch = FALSE;
    return;
}


void ShowClock( void )
{
	if (bClkSwitch == TRUE)
    	InvalidateRect( hEnzClk, NULL, FALSE );

    return;
}


/*
    Period of day change action functions
*/

BOOL LunchTime( HWND hWnd )
{
	WCHAR message[105];
	WCHAR title[55];

	LoadString( hInstance, IDS_LUNCH, message, 100);
	LoadString( hInstance, IDS_APP_TITLE, title, 50);
    MessageBox( hWnd, message, title, MB_OK );

	ClockFwd( hWnd, CLK_HOUR );
	Events.todNow = TOD_AFTERNOON;

    return TRUE;
}

BOOL TeaTime( HWND hWnd )
{
	HWND	hParent;
	WCHAR message[105];
	WCHAR title[55];

	LoadString( hInstance, IDS_TEATIME, message, 100);
	LoadString( hInstance, IDS_APP_TITLE, title, 50);

	if (MessageBox( hWnd, message,
			title, MB_YESNO ) == IDYES) {
		LoadString( hInstance, IDS_TEA, message, 100);
		MessageBox( hWnd, message, title, MB_OK );
		ClockFwd( hWnd, CLK_HALFHOUR );
        Events.todNow = TOD_EEVEN;
		return TRUE;
	}
	else {
		hParent = GetParent( hWnd );
        hParent = (hParent == NULL) ? hWnd : hParent;
		SendMessage( hParent, WM_COMMAND, IDM_GOHOMENOW, 0L );
		return FALSE;
    }
}


BOOL EevenTime( HWND hWnd )
{
	HWND	hParent;
	WCHAR message[105];
	WCHAR title[55];

	LoadString( hInstance, IDS_EVEN, message, 100);
	LoadString( hInstance, IDS_APP_TITLE, title, 50);

	if (MessageBox( hWnd, message, title, MB_YESNO ) == IDYES) {
        Events.todNow = TOD_LEVEN;
        return TRUE;
    }
	else {
		hParent = GetParent( hWnd );
        hParent = (hParent == NULL) ? hWnd : hParent;
		SendMessage( hParent, WM_COMMAND, IDM_GOHOMENOW, 0L );
		return FALSE;
    }
}


BOOL LevenTime( HWND hWnd )
{
	Events.todNow = TOD_LATE;

    return TRUE;
}


BOOL BedTime( HWND hWnd )
{
	HWND	hParent;
	WCHAR message[105];
	WCHAR title[55];

	LoadString( hInstance, IDS_BED, message, 100);
	LoadString( hInstance, IDS_APP_TITLE, title, 50);

	MessageBox( hWnd, message, title, MB_OK );
	hParent = GetParent( hWnd );
 	hParent = (hParent == NULL) ? hWnd : hParent;
	SendMessage( hParent, WM_COMMAND, IDM_GOHOMENOW, 0L );

	return FALSE;
}