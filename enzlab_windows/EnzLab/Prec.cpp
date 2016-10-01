/******************************************************************************

                           EnzLab for Windows

					(coded by:  Doug Clow, 1992/1993)
					(32-bit version by: Andrew Booth, April 2014)

              MODULE: PREC

			 PURPOSE: Analysis of experimental precision

 DATA ITEMS REQUIRED: ECHAR LIST EVENTS

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: Ask me later


******************************************************************************/

#include "stdafx.h"
#include <math.h>
#include "d_echar.h"
#include "d_events.h"
#include "d_list.h"
#include "enzlab.h"
#include "func.h"
#include "help.h"
#include "Htmlhelp.h"
#include "limits.h"
#include "Logs.h"

// local functions
static void PrecSelFillBoxes( HWND hDlg, struct exptrun *ptr );
static void PrecSelHighlighting( HWND hDlg );

// number of entries selected
static int		nPrecSel;

// current selection values of parameters
static float 	selVol, selpH, selS, selI;

// setting text flag
static BOOL		bSettingEditText;

// mean and sd
static double	meanV, sdV;

extern WCHAR *szHelpFilename;
extern BOOL Win7_or_later;
extern HINSTANCE hInstance;

int Precision( HWND hWnd, HINSTANCE hInstance )
{
	DLGPROC lpfnDlgProc;
	BOOL	bChose;
    int		cnt;
	double	sum, sumsq;
	struct exptrun	*ptr;
    WCHAR	buffer[BUFLEN];
	WCHAR	title[BUFLEN];

	// Invoke the selection dialog box
    lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)PrecSelDlgProc, hInstance );
	bChose = DialogBox( hInstance, L"PrecSel", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );

    // Give up now if they pressed Cancel
	if (bChose == FALSE)
		return FALSE;

    /*
		Analyse for precision!
	*/
    // find selected runs, and keep sums
	sum = sumsq = 0.0;
    cnt = 0;
    for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
		if (ptr->sel == TRUE && ptr->v > 0.0) {
            cnt++;
			sum += ptr->v;
			sumsq += ptr->v * ptr->v;
        }
	}
    // did we get enough?
	if (cnt == 0) {
		LoadString( hInstance, IDS_PREC_ERROR1, buffer, 500);
		LoadString( hInstance, IDS_ERROR_TITLE, title, 200);
		MessageBox( hWnd, buffer, title, MB_OK );
		return FALSE;
	}
	else if (cnt < PREC_MIN) {
		if (cnt == 1)
		{
			LoadString( hInstance, IDS_PREC_ERROR2, title, 500);
			swprintf( buffer, title, PREC_MIN );
		}
		else
		{
			LoadString( hInstance, IDS_PREC_ERROR2, title, 500);
			swprintf( buffer, title, cnt, PREC_MIN );
		}
		LoadString( hInstance, IDS_ERROR_TITLE, title, 100);
		MessageBox( hWnd, buffer, title, MB_OK );
        return FALSE;
	}

	// got enough ...
	meanV = sum / cnt;
	sdV = sqrt((sumsq - meanV*sum) / (cnt - 1));
	nPrecSel = cnt;

    // display results dialog
	lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)PrecResDlgProc, hInstance );
	DialogBox( hInstance, L"PrecRes", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );

	// and it took us half an hour
    LoadString( hInstance, IDS_PREC_OK, buffer, 500);
	LoadString( hInstance, IDS_APP_TITLE, title, 200);
	MessageBox( hWnd, buffer, title, MB_OK );
	ClockFwd( hWnd, CLK_HALFHOUR );
    return TRUE;
}



LRESULT CALLBACK PrecSelDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	WCHAR	buffer[BUFLEN];
	WCHAR	title[BUFLEN];
	struct exptrun	*ptr;
	int		index;
    static BOOL bHelp = FALSE;

	switch (iMessage) {
		case WM_INITDIALOG:
			if (!Win7_or_later)
			{
				LoadString( hInstance, IDS_KIN_TITLE, buffer, 200);
				//lstrcpy(buffer,L" Run No   Vol (µL)       pH        S (mM)       I (mM)    V (µmol/min)");
				SetDlgItemText(hDlg, IDD_TITLE,buffer);
			}
			LoadString( hInstance, IDS_MINIMUM, title, 100);
			swprintf( buffer, title, PREC_MIN );
        	SetDlgItemText( hDlg, IDD_PMIN, buffer );
			bSettingEditText = FALSE;
			// fill list box
			for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
            	MakeExptLine( buffer, ptr );
				ptr->sel = SendDlgItemMessage( hDlg, IDD_PLISTBOX, LB_ADDSTRING, 0, (LPARAM)buffer );
			}
			PrecSelFillBoxes( hDlg, listtail );
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					nPrecSel = 0;
                    // set sel fields of exptruns to current selection state
					for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
						ptr->sel = SendDlgItemMessage( hDlg, IDD_PLISTBOX, LB_GETSEL, ptr->sel, 0L );
						if (ptr->sel == TRUE)
							nPrecSel++;
					}
					//if (bHelp == TRUE)
                    //	WinHelp( hDlg, HELPFILE, HELP_QUIT, 0L );
					EndDialog( hDlg, TRUE );
					break;

				case IDCANCEL:
                	// clear the sel fields of exptruns
					for (ptr = listhead; ptr != NULL; ptr = ptr->next)
                    	ptr->sel = FALSE;
					//if (bHelp == TRUE)
                    //	WinHelp( hDlg, HELPFILE, HELP_QUIT, 0L );
					EndDialog( hDlg, FALSE );
                    break;

				case IDD_HELP:
					bHelp = TRUE;
                   // WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_precsel );
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg000c.htm" );
					break;

				case IDD_PLISTBOX:
					if (HIWORD( wParam ) == LBN_SELCHANGE) {
						// find the run on which they've just clicked
						index = SendDlgItemMessage( hDlg, IDD_PLISTBOX, LB_GETCARETINDEX, 0, 0L );
						if (SendDlgItemMessage( hDlg, IDD_PLISTBOX, LB_GETSEL, (WPARAM)index, 0L ) == TRUE) {
							for (ptr = listhead; ptr->sel != index; ptr = ptr->next)
								;
							if (ptr->v < 0.0)
								SendDlgItemMessage( hDlg, IDD_PLISTBOX, LB_SETSEL, 0, (LONG)index );
                            else
								PrecSelFillBoxes( hDlg, ptr );
						}
						else {
                            nPrecSel = SendDlgItemMessage( hDlg, IDD_PLISTBOX, LB_GETSELCOUNT, 0, 0L );
							SetDlgItemInt( hDlg, IDD_PNRMATCHED, nPrecSel, TRUE );
                        }
					}
                    break;

				case IDD_PVOL:
				case IDD_PPH:
				case IDD_PS:
				case IDD_PI:
					
                	if (HIWORD( wParam ) == EN_ERRSPACE) {
						MessageBox( hDlg, L"An edit box has run out of memory space!", L"EnzLab: WARNING", MB_OK | MB_ICONEXCLAMATION );
					}
					else if ((HIWORD( wParam ) == EN_CHANGE) && (bSettingEditText == FALSE)) {
						
						GetDlgItemText( hDlg, LOWORD(wParam), buffer, BUFLEN );
						if (LOWORD(wParam) == IDD_PVOL)
							selVol = _wtof( buffer );
						else if (LOWORD(wParam) == IDD_PPH)
							selpH = _wtof( buffer );
						else if (LOWORD(wParam) == IDD_PS)
							selS = _wtof( buffer );
						else
							selI = _wtof( buffer );
						PrecSelHighlighting( hDlg );
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


void PrecSelFillBoxes( HWND hDlg, struct exptrun *ptr )
{
	WCHAR	buffer[BUFLEN];

    bSettingEditText = TRUE;
	selVol = ptr->vol;
    swprintf( buffer, L"%g", selVol );
	SetDlgItemText( hDlg, IDD_PVOL, buffer );
    selpH = ptr->pH;
	swprintf( buffer, L"%0.1f", selpH );
	SetDlgItemText( hDlg, IDD_PPH, buffer );
    selS = ptr->s;
	swprintf( buffer, L"%g", selS );
	SetDlgItemText( hDlg, IDD_PS, buffer );
    selI = ptr->i;
	swprintf( buffer, L"%g", selI );
	SetDlgItemText( hDlg, IDD_PI, buffer );
	bSettingEditText = FALSE;
    PrecSelHighlighting( hDlg );
}


void PrecSelHighlighting( HWND hDlg )
{
	struct exptrun	*ptr;

	nPrecSel = 0;
	for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
		if (ptr->vol == selVol && ptr->pH == selpH && ptr->i == selI && ptr->s == selS && ptr->v > 0.0) {
			SendDlgItemMessage( hDlg, IDD_PLISTBOX, LB_SETSEL, 1, (LONG)ptr->sel );
			nPrecSel++;
		}
		else
			SendDlgItemMessage( hDlg, IDD_PLISTBOX, LB_SETSEL, 0, (LONG)ptr->sel );
	}
	SetDlgItemInt( hDlg, IDD_PNRMATCHED, nPrecSel, TRUE );								  
}


LRESULT CALLBACK PrecResDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	WCHAR	buffer[BUFLEN];
	WCHAR	title[BUFLEN];
	struct exptrun	*ptr;
    static BOOL	bHelp = FALSE;

	switch (iMessage) {
		case WM_INITDIALOG:
			if (!Win7_or_later)
			{
				LoadString( hInstance, IDS_KIN_TITLE, buffer, 500);
				//lstrcpy(buffer,L" Run No   Vol (µL)       pH        S (mM)       I (mM)    V (µmol/min)");
				SetDlgItemText(hDlg, IDD_TITLE,buffer);
			}
			swprintf( buffer, L"%.3f µmol/min", meanV );
			SetDlgItemText( hDlg, IDD_PRMEAN, buffer );
			swprintf( buffer, L"%.3f µmol/min", sdV );
			SetDlgItemText( hDlg, IDD_PRSD, buffer );
			LoadString( hInstance, IDS_PREC_SEL, title, 100);
			swprintf( buffer, title, nPrecSel );
            SetDlgItemText( hDlg, IDD_PRNR, buffer );
			for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
				if (ptr->sel == TRUE) { 
					MakeExptLine( buffer, ptr );
					SendDlgItemMessage( hDlg, IDD_PRLISTBOX, LB_ADDSTRING, 0, (LPARAM)buffer );
                }
			}
			return FALSE;

		case WM_COMMAND:
			switch (wParam) {
				case IDOK:
					//if (bHelp == TRUE)
                    //	WinHelp( hDlg, HELPFILE, HELP_QUIT, 0L );
					EndDialog( hDlg, TRUE );
					break;

				case IDD_HELP:
					bHelp = TRUE;
					//WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_precres );
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg000b.htm" );
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
