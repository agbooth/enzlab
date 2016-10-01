/******************************************************************************

                           EnzLab for Windows

					(coded by:  Doug Clow, 1992 / 1993)
					(32-bit version by: Andrew Booth, April 2014)

              MODULE: FUNC.h		

			 PURPOSE: function prototypes for functions called by other
             		  functions

 DATA ITEMS REQUIRED: NONE

 DATA ITEMS RESIDENT: n/a

OTHER MODULES CALLED: n/a


******************************************************************************/

#ifndef __FUNC_LOADED__
#define __FUNC_LOADED__ 1

#ifndef _WINDOWS_H
//	#define STRICT
	#include <windows.h>
#endif // __WINDOWS_H

// CLOCK
void InitClockWin( HWND hWndParent, HINSTANCE hInstance );
void InitClockVar( HWND hWnd );
void SizeEnzClk( void );
LRESULT CALLBACK EnzClkWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
void ClockSet( HWND hWnd, unsigned day, unsigned hr, unsigned min );
int ClockFwd( HWND hWnd, enum timeints timeincr );
void ClockInc( void );
void ClockOff( void );
void ShowClock( void );
BOOL LunchTime( HWND hWnd );
BOOL TeaTime( HWND hWnd );
BOOL EevenTime( HWND hWnd );
BOOL LevenTime( HWND hWnd );
BOOL BedTime( HWND hWnd );


// CORE
void ResetEntireScreen( HWND hWnd );


// ENZ
void InitEnz( void );								// Initialises ECHAR
BOOL EnzSel( HWND hWnd, HINSTANCE hInstance );		// invokes EnzSel... dialog box
void EnzInfo( HWND hWnd, HINSTANCE hInstance );		// invokes the EnzInfo message box
LRESULT CALLBACK EnzInfoDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );


// ENZDRAW
void PaintEnzGraph( HWND hWnd, HDC hDC, float delay );
void create_text_at_point(HDC hdc, LPWSTR text, double X, double Y, double size, int colour, double angle, BOOL italic, int alignment);

// EXPT
void InitExptWin( HWND hWndParent, HINSTANCE hInstance );
void InitExptVar( void );
void SizeExptWindows( void );
void GiveEnzRunFocus( void );
void UpdateRunNo( void );
LRESULT CALLBACK EnzSolnWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK EnzRunWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK EditProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK EnzGraphWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK EnzInfoWndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK NewDayDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
void NewDay( HWND hWnd, HINSTANCE hInstance );
double GetErr( void );
BOOL DoEnzRun( HWND hWnd );
void EmptyEnzRunControls( void );


// FILE
void Save( HWND hWnd, HINSTANCE hInstance );
LRESULT CALLBACK SaveDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
void Load( HWND hWnd, HINSTANCE hInstance );
LRESULT CALLBACK LoadDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
void PrintSetup( HWND hWnd, HINSTANCE hInstance );


// HELP
//void About( HWND hWnd, HINSTANCE hInstance );
LRESULT CALLBACK AboutDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
BOOL Intro( HWND hWnd, HINSTANCE hInstance );
LRESULT CALLBACK IntroDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK WelcomeDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
void WhatNext( HWND hWnd );
void More(void);
LRESULT CALLBACK EnterRegistration(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );


// KINDRAW
void KinRes( HWND hWnd, HINSTANCE hInstance, float Km, float Vmax, float cKm, float cVmax, float rpH );
LRESULT CALLBACK KinResDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
void KinExam( HWND hWnd, HINSTANCE hInstance, float aKm, float aVmax, float acKm, float acVmax, float arpH );
LRESULT CALLBACK KinExamDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
void KinResPaintGraphWindow( HWND hPaintWnd );
void KinResPaintWindow( HWND hPaintWnd );
void DrawKinRes( HDC hDC, long xPixels, long yPixels );
DWORD GetTextExtent( HDC hDC, WCHAR *buffer, int length );


// KINETICS
int Kinetics( HWND hWnd, HINSTANCE hInstance );
LRESULT CALLBACK KinSelDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );


// LISTDRAW
void DrawResLis( HDC hDC, short xPixels, short yPixels );
void DrawAnList( HDC hDC, short xPixels, short yPixels );


// PHPDRAW
void PHPRes( HWND hWnd, HINSTANCE hInstance );
LRESULT CALLBACK PHPResDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
void PHPResPaintGraphWindow( HWND hPaintWnd );
void PHPResPaintWindow( HWND hPaintWnd );
void DrawPHPRes( HDC hDC, short xPixels, short yPixels );


// PHPROF
int PHProfile( HWND hWnd, HINSTANCE hInstance );
LRESULT CALLBACK PHPSelDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );


// PREC
int Precision( HWND hWnd, HINSTANCE hInstance );
LRESULT CALLBACK PrecSelDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK PrecResDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );


// PRINT
HDC GetPrinterDC( void );
LRESULT CALLBACK PrintDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK PrintAbortProc( HDC hPrnDC, short nCode );


// RESLIST
void InitResList( void );
void FreeList( void );
void AddToList( HWND hWnd, float vol, float ph, float s, float i, float v );
void ResLis( HWND hWnd, HINSTANCE hInstance );
LRESULT CALLBACK ResLisDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
WCHAR *MakeExptLine( WCHAR *buffer, struct exptrun *ptr );
void InitAnList( void );
void FreeAnList( void );
void AddToAnList( HWND hWnd, float Km, float cKm, float Vmax, float cVmax, float pH, float i, int nResults );
void AnList( HWND hWnd, HINSTANCE hInstance );
LRESULT CALLBACK AnListDlgProc( HWND hDlg, unsigned iMessage, WPARAM wParam, LPARAM lParam );
WCHAR *MakeAnListLine( WCHAR *buffer, struct anlist *ptr );
void Clipboard( HWND hWnd );

#endif // __FUNC_LOADED__