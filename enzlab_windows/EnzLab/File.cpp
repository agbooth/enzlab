/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: FILE		

             PURPOSE: Load and save operations

 DATA ITEMS REQUIRED: all!  ANLIST, ECHAR, ESOLN, EVENTS, LIST, STIME

 DATA ITEMS RESIDENT: none

OTHER MODULES CALLED:


******************************************************************************/



#include "stdafx.h"

#include "commdlg.h"
#include <dlgs.h>
#include <stdio.h>

#include "d_anlist.h"
#include "d_echar.h"
#include "d_esoln.h"
#include "d_events.h"
#include "d_list.h"
#include "d_stime.h"
#include "enzlab.h"
#include "func.h"
#include "help.h"
#include "Htmlhelp.h"
#include "limits.h"
#include "Logs.h"

// viewed by CORE
unsigned	iCommDlgHelp = 0;

// printer device names, in PRINT
extern HGLOBAL	hPrinterDevNames;
extern HGLOBAL	hPrinterDevMode;
extern HINSTANCE hInstance;

// local functions
BOOL SaveTo( HWND hWnd, WCHAR *file );
BOOL LoadFrom( HWND hWnd, WCHAR *file );
LRESULT CALLBACK CommDlgHookProc( HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam );

// module-global variables
WCHAR   	szFileName[BUFLEN], szFileTitle[BUFLEN];
WCHAR	*szFilter = L"EnzLab files (*.elb)\0*.elb\0\0";

enum CommDlgs { CD_UNKNOWN, CD_SAVE, CD_LOAD, CD_PRINTSETUP };
enum CommDlgs	eWhichCommDlg = CD_UNKNOWN;

extern WCHAR *szHelpFilename;

WCHAR buffer[BUFLEN];
WCHAR title[205];

void Save( HWND hWnd, HINSTANCE hInstance )
{
	static OPENFILENAME	ofn;

    eWhichCommDlg = CD_SAVE;
	iCommDlgHelp = RegisterWindowMessage( HELPMSGSTRING );

	// Set all structure members to zero.
	memset(&ofn, 0, sizeof(OPENFILENAME));
    lstrcpy( szFileName, szFileTitle );

	// Initialize the OPENFILENAME members.
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
	ofn.lpstrFile= szFileName;
	ofn.nMaxFile = sizeof( szFileName );
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof( szFileTitle );
	ofn.lpstrInitialDir = 0;
	LoadString( hInstance, IDS_SAVE, title, 100);
	ofn.lpstrTitle = title;
	ofn.lpstrDefExt = L"elb";
    ofn.lpfnHook = (UINT (CALLBACK *)(HWND, UINT, WPARAM, LPARAM))MakeProcInstance( (FARPROC)CommDlgHookProc, hInstance );
	ofn.Flags =
		OFN_EXPLORER |
		OFN_PATHMUSTEXIST	|
		OFN_HIDEREADONLY	|
		OFN_OVERWRITEPROMPT	|
        OFN_ENABLEHOOK		|
		OFN_SHOWHELP;
	if (GetSaveFileName( &ofn ) == TRUE) {
		if (SaveTo( hWnd, szFileName ) == TRUE) {
			LoadString( hInstance, IDS_GOOD_SAVE, title, 200);
			swprintf( buffer, title, szFileName );
			LoadString( hInstance, IDS_APP_TITLE, title, 200);
			MessageBox( hWnd, buffer, title, MB_OK );
		}
	}
	else {
		if (CommDlgExtendedError() != 0) {
			MessageBox( hWnd, L"A problem was encountered in trying to get the Save Current Data Dialog Box to work. (Possibly we've run out of memory.)",
				L"EnzLab: WARNING", MB_OK );
		}
	}

	FreeProcInstance( (FARPROC)CommDlgHookProc );
	eWhichCommDlg = CD_UNKNOWN;

	return;
}





void Load( HWND hWnd, HINSTANCE hInstance )
{
	static OPENFILENAME	ofn;

	iCommDlgHelp = RegisterWindowMessage( HELPMSGSTRING );
    eWhichCommDlg = CD_LOAD;

	// Set all structure members to zero.
	memset(&ofn, 0, sizeof(OPENFILENAME));
    lstrcpy( szFileName, szFileTitle );

	// Initialize the OPENFILENAME members.
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
	ofn.lpstrFile= szFileName;
	ofn.nMaxFile = sizeof( szFileName );
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof( szFileTitle );
	ofn.lpstrInitialDir = 0;
	LoadString( hInstance, IDS_LOAD, title, 100);
	ofn.lpstrTitle = title;
	ofn.lpstrDefExt = L".elb";
    ofn.lpfnHook = (UINT (CALLBACK *)(HWND, UINT, WPARAM, LPARAM))MakeProcInstance( (FARPROC)CommDlgHookProc, hInstance );
	ofn.Flags =
		OFN_EXPLORER |
    	OFN_FILEMUSTEXIST	|
		OFN_PATHMUSTEXIST	|
		OFN_HIDEREADONLY	|
        OFN_ENABLEHOOK		|
		OFN_SHOWHELP;
	if (GetOpenFileName( &ofn ) == TRUE) {
		if (LoadFrom( hWnd, szFileName ) == TRUE) {
			LoadString( hInstance, IDS_GOOD_LOAD, title, 200);
			swprintf( buffer, title, szFileName );
			LoadString( hInstance, IDS_APP_TITLE, title, 200);
			MessageBox( hWnd, buffer, title, MB_OK );
		}
	}
	else {
		if (CommDlgExtendedError() != 0) {
			MessageBox( hWnd, L"A problem was encountered in trying to get the Load Data Dialog Box to work. (Possibly we've run out of memory.)",
				L"EnzLab: WARNING", MB_OK );
		}
	}

	FreeProcInstance( (FARPROC)CommDlgHookProc );
	eWhichCommDlg = CD_UNKNOWN;

	return;
}



LRESULT CALLBACK CommDlgHookProc( HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	OPENFILENAME *oFN;
	OFNOTIFY *ofn;
	switch (iMessage) {
		case WM_INITDIALOG:
			return TRUE;

		case WM_NOTIFY:
			ofn = (OFNOTIFY *) lParam;
			oFN = ofn->lpOFN;
			if (ofn->hdr.code == CDN_HELP)
			{
				//Log(L"Title is %s\n",oFN->lpstrTitle);

				Events.bHelp = TRUE;
				LoadString( hInstance, IDS_SAVE, buffer, 200);
				LoadString( hInstance, IDS_LOAD, title, 200);
				if (lstrcmp(oFN->lpstrTitle, buffer)==0)
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg000e.htm" );
					//WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_save );
				else if (lstrcmp(oFN->lpstrTitle, title)==0)
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg0007.htm" );
					//WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_load );
				else
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"cont0001.htm" );
			}
			return TRUE;

	}
	return FALSE;
}


/*
		The actual file i/o
*/

//#define HEADSTRING  L"This file is an environment saved from EnzLab for Windows,\nby Doug Clow, John Garratt, Peter Groves and Andrew Booth.\n\x1A%c\n"

BOOL SaveTo( HWND hWnd, WCHAR *file )
{
	FILE	*outfile;
	struct exptrun	*lptr;
    struct anlist 	*aptr;
    unsigned	nItems;

	if ((outfile = _wfopen( file, L"wb" )) == NULL) {
		LoadString( hInstance, IDS_NO_WRITE, title, 200);
		swprintf( buffer, title, file );
		LoadString( hInstance, IDS_ERROR_TITLE, title, 200);
		MessageBox( hWnd, buffer, title, MB_OK );
		return FALSE;
	}

	//fwprintf( outfile, HEADSTRING );

	fwrite( &EnzChar, sizeof( struct enzchar ), 1, outfile );
	fwrite( &Events, sizeof( struct sflags ), 1, outfile );
	fwrite( &clkDay, sizeof( unsigned int ), 1, outfile );
	fwrite( &clkHour, sizeof( unsigned int ), 1, outfile );
	fwrite( &clkMin, sizeof( unsigned int ), 1, outfile );
	fwrite( &nSamples, sizeof( unsigned int ), 1, outfile );
	fwrite( &nulEnzyme, sizeof( unsigned int ), 1, outfile );

	// LIST linked list
	for (lptr = listhead, nItems = 0; lptr != NULL; lptr = lptr->next, nItems++)
		;
    fwrite( &nItems, sizeof( unsigned ), 1, outfile );
	for (lptr = listhead; lptr != NULL; lptr = lptr->next)
		fwrite( lptr, sizeof( struct exptrun ), 1, outfile );

	// ANLIST linked list
	for (aptr = anlisthead, nItems = 0; aptr != NULL; aptr = aptr->next, nItems++)
		;
	fwrite( &nItems, sizeof( unsigned ), 1, outfile );
	for (aptr = anlisthead; aptr != NULL; aptr = aptr->next)
    	fwrite( aptr, sizeof( struct anlist ), 1, outfile );

	fclose( outfile ); 
	return TRUE;
}


BOOL LoadFrom( HWND hWnd, WCHAR *file )
{
	FILE	*infile;
    BOOL	done;
	unsigned	nItems,
				i;
	struct exptrun	*lptr;
    struct anlist	*aptr;

	if ((infile = _wfopen( file, L"rb" )) == NULL) {
		LoadString( hInstance, IDS_NO_READ, title, 200);
		swprintf( buffer, title, file );
		LoadString( hInstance, IDS_ERROR_TITLE, title, 200);
		MessageBox( hWnd, buffer, title, MB_OK );
		return FALSE;
	}


    // fseek past the Ctrl-Z
	//fseek( infile, lstrlen( HEADSTRING ) * sizeof( WCHAR ) - 1, SEEK_SET );

    // read in the normal data
	fread( &EnzChar, sizeof( struct enzchar ), 1, infile );
	fread( &Events, sizeof( struct sflags ), 1, infile );
	fread( &clkDay, sizeof( unsigned int ), 1, infile );
	fread( &clkHour, sizeof( unsigned int ), 1, infile );
	fread( &clkMin, sizeof( unsigned int ), 1, infile );
	fread( &nSamples, sizeof( unsigned int ), 1, infile );
	fread( &nulEnzyme, sizeof( unsigned int ), 1, infile );

	// read in the list of experiments
    fread( &nItems, sizeof( unsigned ), 1, infile );
	FreeList();
    for (i = 0; i < nItems; i++) {
        // allocate memory
		if ((lptr = (struct exptrun *)malloc( sizeof( struct exptrun ) )) == NULL ) {
			swprintf( buffer ,L"There is not enough memory to load in the experimental data from the file: %s", file );
			MessageBox( hWnd, buffer, L"EnzLab: WARNING", MB_ICONEXCLAMATION | MB_OK );
			fclose( infile );
			return FALSE;
		}
        // read in data
		if (fread( lptr, sizeof( struct exptrun ), 1, infile ) == 1) {
			lptr->next = NULL;
			if (listtail == NULL) {
				lptr->prev = NULL;
				listhead = listtail = lptr;
			}
			else {
				lptr->prev = listtail;  // This wasn't set in Doug's code.
				listtail->next = lptr;
				listtail = lptr;
			}
		}
		else {
			free( lptr );
            fclose( infile );
			LoadString( hInstance, IDS_READ_ERROR, title, 200);
			swprintf( buffer, title, file );
			LoadString( hInstance, IDS_ERROR_TITLE, title, 200);
			MessageBox( hWnd, buffer, title, MB_OK );
			return FALSE;
		}
    }


	// read in the list of analyses
    fread( &nItems, sizeof( unsigned ), 1, infile );
	FreeAnList();
    for (i = 0; i < nItems; i++) {
        // allocate memory
		if ((aptr = (struct anlist *)malloc( sizeof( struct anlist ) )) == NULL ) {
			swprintf( buffer, L"There is not enough memory to load in kinetic analysis data from the file: %s", file );
			MessageBox( hWnd, buffer, L"EnzLab: WARNING", MB_ICONEXCLAMATION | MB_OK );
			fclose( infile );
			return FALSE;
		}
        // read in data
		if (fread( aptr, sizeof( struct anlist ), 1, infile ) == 1) {
			aptr->next = NULL;
			if (anlisttail == NULL) {
				aptr->prev = NULL;
				anlisthead = anlisttail = aptr;
			}
			else {
				anlisttail->next = aptr;
				anlisttail = aptr;
			}
		}
		else {
			free( aptr );
            fclose( infile );
			LoadString( hInstance, IDS_KIN_ERROR, title, 200);
			swprintf( buffer, title, file );
			LoadString( hInstance, IDS_ERROR_TITLE, title, 200);
			MessageBox( hWnd, buffer, title, MB_OK );
			return FALSE;
		}
    }

	fclose( infile );
    ResetEntireScreen( hWnd );

	return TRUE;
}



/*
		Print Setup dialog
*/

void PrintSetup( HWND hWnd, HINSTANCE hInstance )
{
	WCHAR	buffer[BUFLEN];
	static PRINTDLG		pd;

	iCommDlgHelp = RegisterWindowMessage( HELPMSGSTRING );
    eWhichCommDlg = CD_PRINTSETUP;

	// Set all structure members to zero.
	memset( &pd, 0, sizeof( PRINTDLG ) );

	// Initialize the PRINTDIALOG members.
    pd.lStructSize = sizeof( PRINTDLG );
	pd.hwndOwner = hWnd;
	pd.hDevNames = hPrinterDevNames;
	pd.hDevMode = hPrinterDevMode;
	pd.Flags = PD_ENABLESETUPHOOK | PD_PRINTSETUP;
	pd.lpfnSetupHook = (UINT (CALLBACK *)(HWND, UINT, WPARAM, LPARAM))MakeProcInstance( (FARPROC)CommDlgHookProc, hInstance );

	// Call the function!
	PrintDlg( &pd );
	
}
