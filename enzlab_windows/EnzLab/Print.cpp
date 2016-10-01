/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		   (32-bit version by: Andrew Booth, April 2014)

              MODULE: PRINT

			 PURPOSE: bits and pieces for printing

 DATA ITEMS REQUIRED: none

 DATA ITEMS RESIDENT: none

OTHER MODULES CALLED:

******************************************************************************/


#include "stdafx.h"

#include "func.h"
#include "limits.h"
#include "print.h"
#include "Resource.h"

// printer device names handle, seen by CORE and FILE
HGLOBAL	hPrinterDevNames = NULL;
HGLOBAL	hPrinterDevMode = NULL;

// [wince]
extern HINSTANCE hInstance;

BOOL bUserAbort;
HWND hPrintDlg;

HDC GetPrinterDC( void )
{
	WCHAR	szPrinter[BUFLEN],
			*szDevice, *szDriver, *szOutput;

	GetProfileString( L"windows", L"device", L"", szPrinter, BUFLEN );

	if (((szDevice = wcstok( szPrinter, L"," )) != NULL) &&
		((szDriver = wcstok(     NULL, L", " )) != NULL) &&
		((szOutput = wcstok(     NULL, L", " )) != NULL))
		return CreateDC( szDriver, szDevice, szOutput, NULL );
	else
		return NULL;
}



LRESULT CALLBACK PrintDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	switch (iMessage) {
		case WM_INITDIALOG:
			EnableMenuItem( GetSystemMenu( hDlg, FALSE ), SC_CLOSE, MF_GRAYED );
            break;

		case WM_COMMAND:
			bUserAbort = TRUE;
			EnableWindow( GetParent( hDlg ), TRUE );
			DestroyWindow( hDlg );
            hPrintDlg = NULL;
			break;

		default:
			return FALSE;
	}

    return TRUE;
}


LRESULT CALLBACK PrintAbortProc( HDC hPrnDC, short nCode )
{
	MSG	msg;

	while (!bUserAbort && PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )) {
		if (!hPrintDlg || !IsDialogMessage( hPrintDlg, &msg )) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

    return !bUserAbort;
}


BOOL Print( HWND hDlg, int iDocument )
{
	WCHAR	buffer[BUFLEN];
	WCHAR	title[BUFLEN];
	HDC		hPrnDC;
	short	xPage, yPage;
	DLGPROC	lpfnPrintDlgProc;
	ABORTPROC	lpfnPrintAbortProc;
    BOOL	bErr = FALSE;

	if ((hPrnDC = GetPrinterDC()) == NULL)
		return TRUE;

	xPage = GetDeviceCaps( hPrnDC, HORZRES );
	yPage = GetDeviceCaps( hPrnDC, VERTRES );

	EnableWindow( hDlg, FALSE );

    bUserAbort = FALSE;
	lpfnPrintDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)PrintDlgProc, hInstance );
	hPrintDlg = CreateDialog( hInstance, L"PrintDlg", hDlg, lpfnPrintDlgProc );

	lpfnPrintAbortProc = (ABORTPROC)MakeProcInstance( (FARPROC)PrintAbortProc, hInstance );
	Escape( hPrnDC, SETABORTPROC, NULL, (LPCSTR)lpfnPrintAbortProc, NULL );
	LoadString( hInstance, IDS_EL_GRAPH, buffer, 100);
    //swprintf( buffer, L"EnzLab graph");
	if (Escape( hPrnDC, STARTDOC, lstrlen(buffer), (LPCSTR)buffer, NULL ) > 0) {
		// print the page!
		switch (iDocument) {
			case DOC_KINRES:
				DrawKinRes( hPrnDC, xPage, yPage );
				break;

			case DOC_RESLIST:
				DrawResLis( hPrnDC, xPage, yPage );
				break;

			case DOC_ANLIST:
				DrawAnList( hPrnDC, xPage, yPage );
				break;

			case DOC_PHPRES:
				DrawPHPRes( hPrnDC, xPage, yPage );
				break;

			default:
				break;
        }
		if (Escape( hPrnDC, NEWFRAME, NULL, NULL, NULL ) > 0)
			Escape( hPrnDC, ENDDOC, NULL, NULL, NULL );
		else
			bErr = TRUE;
	}
	else
		bErr = TRUE;

	if (!bUserAbort) {
		EnableWindow( hDlg, TRUE );
		DestroyWindow( hPrintDlg );
	}

	FreeProcInstance( (FARPROC)lpfnPrintAbortProc );
	FreeProcInstance( (FARPROC)lpfnPrintDlgProc );
	DeleteDC( hPrnDC );

	return bErr || bUserAbort;
}

