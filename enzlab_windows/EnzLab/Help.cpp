/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: HELP		

             PURPOSE: Functions to give help, and to work the Help... menu

 DATA ITEMS REQUIRED: EVENTS

 DATA ITEMS RESIDENT: none

OTHER MODULES CALLED: 


******************************************************************************/


#include "stdafx.h"
#include "d_events.h"
#include "enzlab.h"
#include "func.h"
#include "help.h"
#include "htmlhelp.h"
#include "Logs.h"
#include "Shellapi.h"

extern WCHAR *szHelpFilename;
extern HINSTANCE hInstance;
extern BOOL IsAdmin;

BOOL Intro( HWND hWnd, HINSTANCE hInstance )
{
	DLGPROC lpfnDlgProc;
	int result;

	// invoke Intro dialog box
	lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)IntroDlgProc, hInstance );
	result = DialogBox( hInstance, L"Intro", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );
	if (result == 1)
	{
		PostQuitMessage(0);
		return FALSE;
	}
    // invoke Welcome dialog box
	lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)WelcomeDlgProc, hInstance );
	result = DialogBox( hInstance, L"Welcome", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );
	if (result == 1)
	{
		PostQuitMessage(0);
		return FALSE;
	}	
	return TRUE;
}



LRESULT CALLBACK IntroDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	WCHAR buffer[BUFLEN];

	if (iMessage == WM_INITDIALOG)
	{
		LoadString( hInstance, IDS_LICENCE, buffer, 300);
		SetDlgItemText( hDlg, IDD_LICENCE, buffer);
		return TRUE;
	}
	else if (iMessage == WM_COMMAND) {
		if (wParam == IDOK) {
			//if (bHelp == TRUE)
            //	HtmlHelp(GetDesktopWindow(), szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"bgrn0001.htm" );
			EndDialog( hDlg, 0 );
			return TRUE;
		}
		else if (wParam == IDD_HELP) {
        	//bHelp = TRUE;
			HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"bgrn0001.htm" );
        }
	}
	else if (iMessage == WM_CLOSE)
	{
		EndDialog( hDlg, 1);
		return TRUE;
	}

	return FALSE;
}



LRESULT CALLBACK WelcomeDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	if (iMessage == WM_INITDIALOG)
		return TRUE;
	else if (iMessage == WM_COMMAND) 
	{
		if (wParam == IDOK) 
		{
			EndDialog( hDlg, 0 );
			return TRUE;
		}
	}
	else if (iMessage == WM_CLOSE)
	{
		EndDialog( hDlg, 1);
		return TRUE;
	}

	return FALSE;
}


void WhatNext( HWND hWnd )
{
	//unsigned long	ulContext;

	if (Events.bEnzSel != TRUE)
    	HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"what0001.htm" );
	else if (Events.nResults <= ANALYSIS_MIN)
		HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"what0002.htm" );
	else if (Events.nKinetics < 1)
		HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"what0003.htm" );
	else if (Events.nPrecisions < 1)
		HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"what0004.htm" );
	else if (Events.nPHProfiles < 1)
    	HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"what0005.htm" );
	else
		HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"what0006.htm" );

    Events.bHelp = TRUE;

}

void More()
{
	ShellExecute(NULL, L"open",L"http://www.agbooth.com/apps/",NULL,NULL, SW_SHOWNORMAL);
}


// Message handler for generic box.
LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_CTLCOLORSTATIC:
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (long) GetStockObject(NULL_BRUSH);

	//case WM_CTLCOLORDLG:
	//	return (long)hDlgBkgBrush;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
