/******************************************************************************

                           EnzLab for Windows

					(coded by:  Doug Clow, 1992/1993)
					(32-bit version by: Andrew Booth, April 2014)

              MODULE: PHPROF

			 PURPOSE: pH Profile functions

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

// local functions
static void PHPSelFillBoxes( HWND hDlg, struct exptrun *ptr );
static void PHPSelHighlighting( HWND hDlg );

// number of entries selected
static int		nPHPSel;

// current selection values of parameters
static float 	selVol, selpH, selS, selI;

// setting text flag
static BOOL		bSettingEditText;

extern WCHAR *szHelpFilename;
extern BOOL Win7_or_later;
extern HINSTANCE hInstance;

int PHProfile( HWND hWnd, HINSTANCE hInstance )
{
	DLGPROC lpfnDlgProc;
	BOOL	bChose;
	int		cnt;
	double	sum, sumsq;
	struct exptrun	*ptr;
    WCHAR	buffer[BUFLEN];
	WCHAR	title[BUFLEN];

	// Invoke the selection dialog box
    lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)PHPSelDlgProc, hInstance );
	bChose = DialogBox( hInstance, L"PHPSel", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );

    // Give up now if they pressed Cancel
	if (bChose == FALSE)
		return FALSE;

	// give error if insufficient results
	if (nPHPSel < PHP_MIN) {
		LoadString( hInstance, IDS_PHP_ERROR1, buffer, 200);
		LoadString( hInstance, IDS_ERROR_TITLE, title, 100);
		MessageBox( hWnd, buffer, title, MB_OK );
		return FALSE;
	}

	// display results
	PHPRes( hWnd, hInstance );

	// and it all took half an hour
    LoadString( hInstance, IDS_PHP_OK, buffer, 200);
	LoadString( hInstance, IDS_APP_TITLE, title, 100);
	MessageBox( hWnd, buffer, title, MB_OK );
	ClockFwd( hWnd, CLK_HALFHOUR );


    return TRUE;
}



LRESULT CALLBACK PHPSelDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
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
				LoadString( hInstance, IDS_KIN_TITLE, buffer, 500);
				//lstrcpy(buffer,L" Run No   Vol (µL)       pH        S (mM)       I (mM)    V (µmol/min)");
				SetDlgItemText(hDlg, IDD_TITLE,buffer);
			}
			LoadString( hInstance, IDS_MINIMUM, title, 100);
			wsprintf( buffer, title, PREC_MIN );
        	SetDlgItemText( hDlg, IDD_HMIN, buffer );
			bSettingEditText = FALSE;
			// fill list box
			for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
            	MakeExptLine( buffer, ptr );
				ptr->sel = SendDlgItemMessage( hDlg, IDD_HLISTBOX, LB_ADDSTRING, 0, (LPARAM)buffer );
			}
			PHPSelFillBoxes( hDlg, listtail );
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					nPHPSel = 0;
                    // set sel fields of exptruns to current selection state
					for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
						ptr->sel = SendDlgItemMessage( hDlg, IDD_HLISTBOX, LB_GETSEL, ptr->sel, 0L );
						if (ptr->sel == TRUE)
							nPHPSel++;
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
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg000a.htm" );
					//WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_phpsel );
					break;

				case IDD_HLISTBOX:
					if (HIWORD( wParam ) == LBN_SELCHANGE) {
						// find the run on which they've just clicked
						index = SendDlgItemMessage( hDlg, IDD_HLISTBOX, LB_GETCARETINDEX, 0, 0L );
						if (SendDlgItemMessage( hDlg, IDD_HLISTBOX, LB_GETSEL, (WPARAM)index, 0L ) == TRUE) {
							for (ptr = listhead; ptr->sel != index; ptr = ptr->next)
								;
							if (ptr->v < 0.0)
								SendDlgItemMessage( hDlg, IDD_HLISTBOX, LB_SETSEL, 0, (LONG)index );
                            else
								PHPSelFillBoxes( hDlg, ptr );
						}
						else {
                            nPHPSel = SendDlgItemMessage( hDlg, IDD_HLISTBOX, LB_GETSELCOUNT, 0, 0L );
							SetDlgItemInt( hDlg, IDD_HNRMATCHED, nPHPSel, TRUE );
                        }
					}
                    break;

				case IDD_HVOL:
				case IDD_HS:
				case IDD_HI:
                	if (HIWORD( wParam ) == EN_ERRSPACE) {
						MessageBox( hDlg, L"An edit box has run out of memory space!", L"EnzLab: WARNING", MB_OK | MB_ICONEXCLAMATION );
					}
					else if (HIWORD( wParam ) == EN_CHANGE && bSettingEditText == FALSE) {
						GetDlgItemText( hDlg, LOWORD(wParam), buffer, BUFLEN );
						if (LOWORD(wParam) == IDD_HVOL)
							selVol = (float)_wtof( buffer );
						else if (LOWORD(wParam) == IDD_HS)
							selS = (float)_wtof( buffer );
						else
							selI = (float)_wtof( buffer );
						PHPSelHighlighting( hDlg );
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


void PHPSelFillBoxes( HWND hDlg, struct exptrun *ptr )
{
	WCHAR	buffer[BUFLEN];

    bSettingEditText = TRUE;
	selVol = ptr->vol;
    swprintf( buffer, L"%g", selVol );
	SetDlgItemText( hDlg, IDD_HVOL, buffer );
    selS = ptr->s;
	swprintf( buffer, L"%g", selS );
	SetDlgItemText( hDlg, IDD_HS, buffer );
    selI = ptr->i;
	swprintf( buffer, L"%g", selI );
	SetDlgItemText( hDlg, IDD_HI, buffer );
	bSettingEditText = FALSE;
    PHPSelHighlighting( hDlg );
}


void PHPSelHighlighting( HWND hDlg )
{
	struct exptrun	*ptr;

	nPHPSel = 0;
	for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
		if (ptr->vol == selVol && ptr->i == selI && ptr->s == selS && ptr->v > 0.0) {
			SendDlgItemMessage( hDlg, IDD_HLISTBOX, LB_SETSEL, 1, (LONG)ptr->sel );
			nPHPSel++;
		}
		else
			SendDlgItemMessage( hDlg, IDD_HLISTBOX, LB_SETSEL, 0, (LONG)ptr->sel );
	}
	SetDlgItemInt( hDlg, IDD_HNRMATCHED, nPHPSel, TRUE );
}
