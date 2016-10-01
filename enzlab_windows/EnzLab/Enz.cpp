/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: ENZ		

             PURPOSE: enzyme-only functions: selection, information etc

 DATA ITEMS REQUIRED: ECHAR EVENTS

 DATA ITEMS RESIDENT: ECHAR

OTHER MODULES CALLED:


******************************************************************************/


#include "stdafx.h"

#include "d_events.h"
#include "enzlab.h"
#include "func.h"
#include "help.h"
#include "htmlhelp.h"
#include "limits.h"
#include "struct.h"
#include <time.h>




// ECHAR data item
struct enzchar EnzChar;
extern WCHAR*szHelpFilename;
extern HINSTANCE hInstance;

// initialise ECHAR enzyme characteristics
void InitEnz( void )
{
	EnzChar.ID = IDD_UNKNOWN;
	EnzChar.ref[0] = EnzChar.name[0] = '\0';
	//randomize();
	srand(time(NULL)); // Seed the random number generator
	return;
}

// arrays of data for enzyme characteristics
float knvals[] = { 2.0, 1.0, 0.6, 0.5, 0.3, 0.1, 0.05, 0.02, 0.01, 0.001 };
float vnvals[] = { 1.627, 0.826, 0.438, 0.241, 0.228, 0.103, 0.082, 0.043, 0.0244, 0.028 };
float kavals[] = { 4.3, 5.8, 6.0, 6.2, 6.5 };
float kbvals[] = { 7.7, 7.9, 8.0, 8.5, 9.0 };
float modvals[] = { 1.04, 1.02, 1.0, 0.98, 0.96 };
WCHAR *enzrefs[] = { L"PR", L"DH", L"HY", L"DC", L"PH" };
WCHAR enztypes[5][55];

#define NRKN    (sizeof(knvals)/sizeof(float))
#define NRVN    (sizeof(vnvals)/sizeof(float))
#define NRKA    (sizeof(kavals)/sizeof(float))
#define NRKB    (sizeof(kbvals)/sizeof(float))
#define NRMOD   (sizeof(modvals)/sizeof(float))
#define NRENZR	(sizeof(enzrefs)/sizeof(char *))
#define NRENZT  (sizeof(enztypes)/sizeof(char *))

/*
#if (NRMOD != NRENZT)
	#error The number of modvals[] must equal the number of enztypes[]!
#endif
#if (NRENZR != NRENZT)
    #error The number of enzrefs[] must equal the number of enztypes[]!
#endif
*/
// Generate an enzyme in EnzChar - the ID must be there!
void EnzGen( void )
{
	int     i, kn, vn, ka, kb, mod;

	LoadString( hInstance, IDS_PROTEASE, enztypes[0], 50);
	LoadString( hInstance, IDS_DEHYDROGENASE, enztypes[1], 50);
	LoadString( hInstance, IDS_HYDRATASE, enztypes[2], 50);
	LoadString( hInstance, IDS_DECARBOXYLASE, enztypes[3], 50);
	LoadString( hInstance, IDS_PHOSPHATASE, enztypes[4], 50);

	i = (EnzChar.ID - IDD_UNKNOWN) - 1;
	swprintf( EnzChar.name, L"%s", enztypes[i] );

	EnzChar.mod = modvals[i];

	unsigned int rand_no;
	rand_s(&rand_no);
    kn = (int)(((double)rand_no / (double)UINT_MAX) * (double)NRKN);
	EnzChar.Kn = knvals[kn];

	rand_s(&rand_no);
    vn = (int)(((double)rand_no / (double)UINT_MAX) * (double)NRVN);
	EnzChar.Vn = vnvals[vn];

	rand_s(&rand_no);
    ka = (int)(((double)rand_no / (double)UINT_MAX) * (double)NRKA);
	EnzChar.Ka = kavals[ka];

	rand_s(&rand_no);
    kb = (int)(((double)rand_no / (double)UINT_MAX) * (double)NRKB);
	EnzChar.Kb = kbvals[kb];

	rand_s(&rand_no);
	EnzChar.m = (int)((((double)rand_no / (double)UINT_MAX) * 1.0) + 0.5);
	rand_s(&rand_no);
	EnzChar.n = (int)((((double)rand_no / (double)UINT_MAX) * 1.0) + 0.5);
	rand_s(&rand_no);
	EnzChar.p = (int)((((double)rand_no / (double)UINT_MAX) * 2.0) + 0.5);

	/* DEBUG START 
	swprintf( EnzChar.name, L"%s", enztypes[0] );
	EnzChar.mod = modvals[2];
	EnzChar.Kn = knvals[2];
	EnzChar.Vn = vnvals[2];
	EnzChar.Ka = kavals[2];
	EnzChar.Kb = kbvals[2];
	EnzChar.m = EnzChar.n = EnzChar.p = 1;
	 DEBUG END */

	swprintf( EnzChar.ref, L"%.2s/%d%d-%d%d%d-%d%d%d", enzrefs[i],
		kn, vn, ka, kb, i, EnzChar.m, EnzChar.n, EnzChar.p );

	return;
}


// Enzyme selection dialog box
LRESULT CALLBACK EnzSelDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	static unsigned nChoice;
    //static BOOL bHelp = FALSE;

	switch (iMessage) {
		case WM_INITDIALOG:
			nChoice = EnzChar.ID;
			if (nChoice == IDD_UNKNOWN)
				nChoice = IDD_PROTEASE;
			CheckRadioButton( hDlg, IDD_PROTEASE, IDD_PHOSPHATASE, nChoice );
			SetFocus( GetDlgItem( hDlg, nChoice ) );
			return FALSE;

		case WM_COMMAND:
			switch (wParam) {
				case IDOK:
					EnzChar.ID = nChoice;
					//if (bHelp == TRUE)
                    //	WinHelp( hDlg, HELPFILE, HELP_QUIT, 0L );
					EndDialog( hDlg, TRUE );
					break;

				case IDCANCEL:
					//if (bHelp == TRUE)
                    //	WinHelp( hDlg, HELPFILE, HELP_QUIT, 0L );
					EndDialog( hDlg, FALSE );
					break;

				case IDD_HELP:
					//bHelp = TRUE;
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg0002.htm" );
					break;

				case IDD_PROTEASE:
				case IDD_DECARBOXYLASE:
				case IDD_HYDRATASE:
				case IDD_DEHYDROGENASE:
				case IDD_PHOSPHATASE:
					nChoice = wParam;
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



BOOL EnzSel( HWND hWnd, HINSTANCE hInstance )
{
	DLGPROC lpfnEnzSelDlgProc;
	BOOL bSet;

    // Invoke dialog box
	lpfnEnzSelDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)EnzSelDlgProc, hInstance );
	bSet = DialogBox( hInstance, L"EnzSel", hWnd, lpfnEnzSelDlgProc );
	FreeProcInstance( (FARPROC)lpfnEnzSelDlgProc );

    // if they chose something, generate the enzyme and post ourselves an IDM_EINFO
    if (bSet == TRUE) {
		EnzGen();
		//PostMessage( hWnd, WM_COMMAND, IDM_EINFO, (LPARAM)0L );
		EnzInfo( hWnd, hInstance );
        return TRUE;
	}
    else
		return FALSE;
}


// Print enzyme info
void EnzInfo( HWND hWnd, HINSTANCE hInstance )
{
	DLGPROC lpfnDlgProc;

    lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)EnzInfoDlgProc, hInstance );
	DialogBox( hInstance, L"EnzInfo", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );

	return;
}


LRESULT CALLBACK EnzInfoDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	WCHAR	buffer[BUFLEN];
	WCHAR temp[105];

	switch (iMessage) {
		case WM_INITDIALOG:
			LoadString(hInstance, IDS_ENZNAME, temp, 100);
        	swprintf( buffer, temp, EnzChar.name );
			SetDlgItemText( hDlg, IDD_ENZNAME, buffer );

			LoadString(hInstance, IDS_ENZREF, temp, 100);
			swprintf( buffer, temp, EnzChar.ref );
			SetDlgItemText( hDlg, IDD_ENZREF, buffer );

			LoadString(hInstance, IDS_ENZCONC, temp, 100);
			swprintf( buffer, temp, 2.5 / EnzChar.Vn );
			SetDlgItemText( hDlg, IDD_ENZCONC, buffer );

			LoadString(hInstance, IDS_ENZACT, temp, 100);
			swprintf( buffer, temp, 100.0*EnzChar.Vn );
			SetDlgItemText( hDlg, IDD_ENZACT, buffer );
			return FALSE;

		case WM_COMMAND:
			switch (wParam) {
				case IDOK:
					EndDialog( hDlg, TRUE );
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
