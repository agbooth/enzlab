/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: RESLIST		

			 PURPOSE: maintain / display list of experimental results
             		  and list of analysis results

 DATA ITEMS REQUIRED: ANLIST LIST EVENTS

 DATA ITEMS RESIDENT: ANLIST LIST

OTHER MODULES CALLED:


******************************************************************************/



#include "stdafx.h"


#include "d_events.h"
#include "enzlab.h"
#include "func.h"
#include "help.h"
#include "htmlhelp.h"
#include "limits.h"
#include "print.h"
#include "struct.h"
#include "Logs.h"

#ifndef NULL
	#define NULL 0
#endif // NULL

// ANLIST data item
struct anlist *anlisthead;
struct anlist *anlisttail;

// LIST data item
struct exptrun *listhead;
struct exptrun *listtail;

extern WCHAR *szHelpFilename;
extern BOOL Win7_or_later;
extern HINSTANCE hInstance;
/*
				LIST
*/

void InitResList( void )
{
	if (listhead != NULL)
		FreeList();

	listhead = NULL;
	listtail = NULL;

	return;
}


void FreeList( void )
{
	struct exptrun *ptr, *nptr;

	ptr = listhead;

	while (ptr != NULL) {
		nptr = ptr->next;
		free( ptr );
        ptr = nptr;
	}

	listhead = NULL;
	listtail = NULL;
}


void AddToList( HWND hWnd, float vol, float ph, float s, float i, float v )
{
	struct exptrun	*e;

    // allocate memory
	if ((e = (struct exptrun *)malloc( sizeof( struct exptrun ) )) == NULL) {
		MessageBox( hWnd, L"There is not enough memory to store the experiment you have just done in the list of results!",
				L"EnzLab: WARNING", MB_ICONEXCLAMATION | MB_OK );
		return;
	}

    // load up result
	e->n = Events.nResults;
	e->vol = vol;
	e->pH = ph;
	e->s = s;
	e->i = i;
	e->v = v;
	e->sel = FALSE;

    // put into the list
	e->next = NULL;
	if (listtail == NULL) {
		e->prev = NULL;
		listhead = listtail = e;
	}
	else {
		e->prev = listtail;  // This wasn't set in Doug's code.
		listtail->next = e;
		listtail = e;
	}

	return;
}


void ResLis( HWND hWnd, HINSTANCE hInstance )
{
	DLGPROC	lpfnDlgProc;

	lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)ResLisDlgProc, hInstance );
	DialogBox( hInstance, L"ResLis", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );

	return;
}



LRESULT CALLBACK ResLisDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	struct exptrun	*ptr;
	WCHAR	buffer[BUFLEN],
			buffer2[BUFLEN];
    //static BOOL	bHelp = FALSE;

	switch (iMessage) {
		case WM_INITDIALOG:
			if (!Win7_or_later)
			{
				LoadString( hInstance, IDS_KIN_TITLE, buffer, 500);
				//lstrcpy(buffer,L" Run No   Vol (µL)       pH        S (mM)       I (mM)    V (µmol/min)");
				SetDlgItemText(hDlg, IDD_TITLE,buffer);
			}
			for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
            	MakeExptLine( buffer, ptr );
				SendDlgItemMessage( hDlg, IDD_RESLISTBOX, LB_ADDSTRING, 0, (LPARAM)buffer );
			} 
			return FALSE;

		case WM_COMMAND:
			switch (wParam) {
				case IDOK:
					//if (bHelp == TRUE)
                    //	WinHelp( hDlg, HELPFILE, HELP_QUIT, 0L );
					EndDialog( hDlg, TRUE );
					break;

				case IDD_PRINT:
                	Print( hDlg, DOC_RESLIST );
					break;

				case IDD_HELP:
					//bHelp = TRUE;
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg000d.htm" );
					//WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_reslis );
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



WCHAR *MakeExptLine( WCHAR *buffer, struct exptrun *ptr )
{
	WCHAR temp1[500];
	WCHAR temp2[500];
	swprintf( buffer, L" %3d\t  %.3g\t  %.1f\t %.3g\t  %.3g\t",
			ptr->n, ptr->vol, ptr->pH, ptr->s, ptr->i );
    if (ptr->v <= 0.0) {
		if (ptr->v == V_TOOFAST)
		{
			LoadString( hInstance, IDS_FAST, temp1, 100);
			swprintf(temp2, L"[%s]",temp1);
			lstrcat( buffer, temp2 );
		}
		else if (ptr->v == V_TOOSLOW) 
		{
			LoadString( hInstance, IDS_SLOW, temp1, 100);
			swprintf(temp2, L"[%s]",temp1);
			lstrcat( buffer, temp2 );
		}
		else if (ptr->v == V_SPLAT)
		{
			LoadString( hInstance, IDS_SPLAT, temp1, 100);
			swprintf(temp2, L"[%s]",temp1);
			lstrcat( buffer, temp2 );
		}
		else if (ptr->v == V_NA)
		{
			LoadString( hInstance, IDS_NA, temp1, 100);
			swprintf(temp2, L"[%s]",temp1);
			lstrcat( buffer, temp2 );
		}
		else
		{
			LoadString( hInstance, IDS_EH, temp1, 100);
			swprintf(temp2, L"[%s]",temp1);
			lstrcat( buffer, temp2 );
		}
    }
	else
		swprintf( buffer + lstrlen( buffer ), L"%7.2f", ptr->v );

	return buffer;
}



/*
				ANLIST
*/

void InitAnList( void )
{
	if (anlisthead != NULL)
		FreeAnList();

	anlisthead = NULL;
	anlisttail = NULL;

	return;
}


void FreeAnList( void )
{
	struct anlist *ptr, *nptr;

	ptr = anlisthead;

	while (ptr != NULL) {
		nptr = ptr->next;
		free( ptr );
        ptr = nptr;
	}

	anlisthead = NULL;
	anlisttail = NULL;
}


void AddToAnList( HWND hWnd, float Km, float cKm, float Vmax, float cVmax, float pH, float i, int nResults )
{
	struct anlist	*e;

    // allocate memory
	if ((e = (struct anlist *)malloc( sizeof( struct anlist ) )) == NULL) {
		MessageBox( hWnd, L"There is not enough memory to store the kinetics analysis you have just done in the list of results!",
				L"EnzLab: WARNING", MB_ICONEXCLAMATION | MB_OK );
		return;
	}

    // load up result
	e->n = Events.nKinetics;
	e->Km = Km;
	e->cKm = cKm;
	e->Vmax = Vmax;
	e->cVmax = cVmax;
	e->pH = pH;
	e->i = i;
	e->nResults = nResults;

    // put into the list
	e->next = NULL;
	if (anlisttail == NULL) {
		e->prev = NULL;
		anlisthead = anlisttail = e;
	}
	else {
		anlisttail->next = e;
		anlisttail = e;
	}

	return;
}


void AnList( HWND hWnd, HINSTANCE hInstance )
{
	DLGPROC	lpfnDlgProc;

	lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)AnListDlgProc, hInstance );
	DialogBox( hInstance, L"AnList", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );

	return;
}



LRESULT CALLBACK AnListDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	struct anlist	*ptr;
	WCHAR	buffer[BUFLEN],
			buffer2[BUFLEN];
    //static BOOL	bHelp = FALSE;

	switch (iMessage) {
		case WM_INITDIALOG:
			for (ptr = anlisthead; ptr != NULL; ptr = ptr->next) {
            	MakeAnListLine( buffer, ptr );
				SendDlgItemMessage( hDlg, IDD_ANLISTBOX, LB_ADDSTRING, 0, (LPARAM)buffer );
			} 
			return FALSE;

		case WM_COMMAND:
			switch (wParam) {
				case IDOK:
					//if (bHelp == TRUE)
                    //	WinHelp( hDlg, HELPFILE, HELP_QUIT, 0L );
					EndDialog( hDlg, TRUE );
					break;

				case IDD_PRINT:
					Print( hDlg, DOC_ANLIST );
					break;

				case IDD_HELP:
					//bHelp = TRUE;
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg0001.htm" );
					//WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_anlist );
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



WCHAR *MakeAnListLine( WCHAR *buffer, struct anlist *ptr )
{
	swprintf( buffer, L" %3d\t%3d\t%6.1f\t\t%.3g\t%.3g %c %-.3g\t%.3g %c %-.3g",
		ptr->n, ptr->nResults, ptr->pH, ptr->i, ptr->Km, PLUSMINUS, ptr->cKm, ptr->Vmax, PLUSMINUS, ptr->cVmax );

	return buffer;
}



void Clipboard( HWND hWnd )
{
	HGLOBAL	hGlblMem;
	WCHAR	buffer[BUFLEN];
	WCHAR	line[BUFLEN];
	DWORD length;
	struct exptrun	*ptr;

	swprintf( buffer, L"No.\tVol\tpH\tS\tI\tVₒ\r\n" );
	for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
		swprintf( line, L"%d\t%g\t%.3f\t%g\t%g",
			ptr->n, ptr->vol, ptr->pH, ptr->s, ptr->i );
		lstrcat( buffer,line);
		if (ptr->v <= 0.0)
		{
			swprintf( line, L"\t%d\r\n", -1 );
			lstrcat( buffer, line);
		}
		else
		{
			swprintf( line, L"\t%.4f\r\n", ptr->v );
			lstrcat( buffer, line);
		}
	}

	//Log(L"buffer is %s",buffer);

	length = wcslen(buffer);

	hGlblMem = GlobalAlloc( GMEM_MOVEABLE, (length+1)*sizeof(WCHAR) );
	
	if (hGlblMem == NULL) {
		MessageBox( hWnd, L"Not enough memory to write to clipboard!",
			L"EnzLab: WARNING", MB_OK | MB_ICONSTOP );
		return;
	}

	memcpy(GlobalLock(hGlblMem), buffer, length * sizeof(WCHAR));
	
	GlobalUnlock( hGlblMem );

	OpenClipboard( 0 );
	EmptyClipboard();
	SetClipboardData( CF_UNICODETEXT, hGlblMem );
	CloseClipboard();

	LoadString( hInstance, IDS_CLIP_OK, buffer, 200);
	LoadString( hInstance, IDS_APP_TITLE, line, 100);
	MessageBox( hWnd, buffer, line, MB_OK );
	return;
}
	  

