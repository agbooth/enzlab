/******************************************************************************

                           EnzLab for Windows

                   (coded by:  Doug Clow, 1992/1993)
				   (32-bit version by: Andrew Booth, April 2014)

              MODULE: KINETICS

             PURPOSE: Kinetic analysis functions

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
#include "kincalc.h"
#include "limits.h"

// local functions
static void KinSelFillBoxes( HWND hDlg, struct exptrun *ptr );
static void KinSelHighlighting( HWND hDlg );


// number of entries selected
static int		nKinSel;

// current selection values of parameters
static float 	selVol, selpH, selS, selI;

// setting text flag
static BOOL		bSettingEditText;

extern WCHAR *szHelpFilename;
extern BOOL Win7_or_later;
extern HINSTANCE hInstance;



int Kinetics( HWND hWnd, HINSTANCE hInstance )
{
	WCHAR buffer[BUFLEN];
	WCHAR title[BUFLEN];
	DLGPROC lpfnDlgProc;
	BOOL	bChose;
	struct exptrun *ptr;
    float	Km, Vmax, cKm, cVmax;

    // Invoke the selection dialog box
    lpfnDlgProc = (DLGPROC)MakeProcInstance( (FARPROC)KinSelDlgProc, hInstance );
	bChose = DialogBox( hInstance, L"KinSel", hWnd, lpfnDlgProc );
	FreeProcInstance( (FARPROC)lpfnDlgProc );

    // Give up now if they pressed Cancel
	if (bChose == FALSE)
		return FALSE;

	// Have they chosen enough?
	if (nKinSel < KINETICS_MIN) {
		LoadString( hInstance, IDS_KIN_ERROR7, buffer, 200);
		LoadString( hInstance, IDS_ERROR_TITLE, title, 100);
		MessageBox( hWnd, buffer, title, MB_OK );
		return FALSE;
	}

	// Do the analysis
	if (MMKinetics( nKinSel, listhead, &Km, &Vmax, &cKm, &cVmax ) == FALSE) {
		LoadString( hInstance, IDS_KIN_FAILED, buffer, 200);
		LoadString( hInstance, IDS_APP_TITLE, title, 100);
		MessageBox( hWnd, buffer, title, MB_OK );
		ClockFwd( hWnd, CLK_HALFHOUR );
		return FALSE;
	}

	// print the results
	for (ptr = listhead; ptr != NULL; ptr = ptr->next ) {
		if (ptr->sel == TRUE)
        	break;
	}
	if (ptr == NULL) {
		MessageBox( hWnd, L"There is a problem in the kinetic analysis routine.",
			L"EnzLab: INTERNAL ERROR", MB_OK );
		return FALSE;
	}
	KinRes( hWnd, hInstance, Km, Vmax, cKm, cVmax, ptr->pH );
    KinExam( hWnd, hInstance, Km, Vmax, cKm, cVmax, ptr->pH );

	// move on to next kinetic analysis and put the results into the list
	Events.nKinetics++;
	AddToAnList( hWnd, Km, cKm, Vmax, cVmax, ptr->pH, ptr->i, nKinSel );

	// and it all took half an hour
    LoadString( hInstance, IDS_KIN_OK, buffer, 200);
	LoadString( hInstance, IDS_APP_TITLE, title, 100);
	MessageBox( hWnd, buffer, title, MB_OK );
	ClockFwd( hWnd, CLK_HALFHOUR );
    return TRUE;
}




LRESULT CALLBACK KinSelDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam )
{
	WCHAR buffer[BUFLEN];
	WCHAR title[BUFLEN];
	struct exptrun	*ptr;
	int		index;
    static BOOL	bHelp = FALSE;

	switch (iMessage) {
		case WM_INITDIALOG:
			if (!Win7_or_later)
			{
				LoadString( hInstance, IDS_KIN_TITLE, buffer, 200);
				//lstrcpy(buffer,L" Run No   Vol (µL)       pH        S (mM)       I (mM)    V (µmol/min)");
				SetDlgItemText(hDlg, IDD_TITLE,buffer);
			}
			LoadString( hInstance, IDS_MINIMUM, title, 200);
        	swprintf( buffer, title, KINETICS_MIN );
        	SetDlgItemText( hDlg, IDD_KMIN, buffer );
			bSettingEditText = FALSE;
			// fill list box
			for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
            	MakeExptLine( buffer, ptr );
				ptr->sel = (short)SendDlgItemMessage( hDlg, IDD_KINLISTBOX, LB_ADDSTRING, 0, (LPARAM)buffer );
			}
			KinSelFillBoxes( hDlg, listtail );
			return FALSE;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					nKinSel = 0;
                    // set sel fields of exptruns to current selection state
					for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
						ptr->sel = (short)SendDlgItemMessage( hDlg, IDD_KINLISTBOX, LB_GETSEL, ptr->sel, 0L );
						if (ptr->sel == TRUE)
							nKinSel++;
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
                    //WinHelp( hDlg, HELPFILE, HELP_CONTEXT, dlg_kinsel );
					HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"dlg0006.htm" );
					break;

				case IDD_KINLISTBOX:
					if (HIWORD( wParam ) == LBN_SELCHANGE) {
						// find the run on which they've just clicked
						index = SendDlgItemMessage( hDlg, IDD_KINLISTBOX, LB_GETCARETINDEX, 0, 0L );
						if (SendDlgItemMessage( hDlg, IDD_KINLISTBOX, LB_GETSEL, (WPARAM)index, 0L ) == TRUE) {
							for (ptr = listhead; ptr->sel != index; ptr = ptr->next)
								;
							if (ptr->v < 0.0)
								SendDlgItemMessage( hDlg, IDD_KINLISTBOX, LB_SETSEL, 0, (LONG)index );
                            else
								KinSelFillBoxes( hDlg, ptr );
						}
						else {
                            nKinSel = SendDlgItemMessage( hDlg, IDD_KINLISTBOX, LB_GETSELCOUNT, 0, 0L );
							SetDlgItemInt( hDlg, IDD_KNRMATCHED, nKinSel, TRUE );
                        }
					}
                    break;

				case IDD_KVOL:
				case IDD_KPH:
				case IDD_KS:
				case IDD_KI:
                	if (HIWORD( wParam ) == EN_ERRSPACE) {
						MessageBox( hDlg, L"An edit box has run out of memory space!", L"EnzLab: WARNING", MB_OK | MB_ICONEXCLAMATION );
					}
					else if (HIWORD( wParam ) == EN_CHANGE && bSettingEditText == FALSE) {
						GetDlgItemText( hDlg, LOWORD(wParam), buffer, BUFLEN );
						if (LOWORD(wParam) == IDD_KVOL)
							selVol = (float)_wtof( buffer );
						else if (LOWORD(wParam) == IDD_KPH)
							selpH = (float)_wtof( buffer );
						else if (LOWORD(wParam) == IDD_KS)
							selS = (float)_wtof( buffer );
						else
							selI = (float)_wtof( buffer );
						KinSelHighlighting( hDlg );
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


void KinSelFillBoxes( HWND hDlg, struct exptrun *ptr )
{
	WCHAR	buffer[BUFLEN];

    bSettingEditText = TRUE;
	selVol = ptr->vol;
    swprintf( buffer, L"%g", selVol );
	SetDlgItemText( hDlg, IDD_KVOL, buffer );
    selpH = ptr->pH;
	swprintf( buffer, L"%0.1f", selpH );
	SetDlgItemText( hDlg, IDD_KPH, buffer );
    selI = ptr->i;
	swprintf( buffer, L"%g", selI );
	SetDlgItemText( hDlg, IDD_KI, buffer );
	bSettingEditText = FALSE;
    KinSelHighlighting( hDlg );
}


void KinSelHighlighting( HWND hDlg )
{
	struct exptrun	*ptr;

	nKinSel = 0;
	for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
		if (ptr->vol == selVol && ptr->pH == selpH && ptr->i == selI && ptr->v > 0.0) {
			SendDlgItemMessage( hDlg, IDD_KINLISTBOX, LB_SETSEL, 1, (LONG)ptr->sel );
			nKinSel++;
		}
		else
			SendDlgItemMessage( hDlg, IDD_KINLISTBOX, LB_SETSEL, 0, (LONG)ptr->sel );
	}
	SetDlgItemInt( hDlg, IDD_KNRMATCHED, nKinSel, TRUE );								  
}
