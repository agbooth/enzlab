// EnzLab.cpp : Defines the entry point for the application.
//

#include "D_esoln.h"
#include "Disaster.h"
#include "stdafx.h"
#include "Func.h"
#include "Struct.h"
#include "Help.h"
#include "Htmlhelp.h"
#include "EnzLab.h"
#include "Limits.h"
#include "Logs.h"
#include "shlobj.h"
#include <time.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
struct sflags	Events;							// EVENTS data item
WCHAR *szHelpFilename;

BOOL IsAdmin;
BOOL Win7_or_later;
HKEY hRegKey;
HKEY hGuidKey;

HINSTANCE	hInstance;
int	xMainClient, yMainClient;	// size of main window client area
int xChar, yChar;

extern long xMin, yMin;
extern unsigned int nulEnzyme;

// Forward declarations of functions included in this code module:
ATOM				RegisterMainClass(HINSTANCE hInstance);
ATOM				RegisterEnzSolnClass(HINSTANCE hInstance);
ATOM				RegisterEnzRunClass(HINSTANCE hInstance);
ATOM				RegisterEnzClkClass(HINSTANCE hInstance);
ATOM				RegisterEnzGraphClass(HINSTANCE hInstance);
ATOM				RegisterEnzInfoClass(HINSTANCE hInstance);

BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void InitEnzLab( HWND, HINSTANCE );

BOOL Is_Win7_or_Later() 
{
   OSVERSIONINFOEX osvi;
   DWORDLONG dwlConditionMask = 0;
   int op=VER_GREATER_EQUAL;

   // Initialize the OSVERSIONINFOEX structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
   osvi.dwMajorVersion = 6;
   osvi.dwMinorVersion = 1;
   osvi.wServicePackMajor = 0;
   osvi.wServicePackMinor = 0;

   // Initialize the condition mask.

   VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, op );
   VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, op );
   VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMAJOR, op );
   VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMINOR, op );

   // Perform the test.

   return VerifyVersionInfo(
      &osvi, 
      VER_MAJORVERSION | VER_MINORVERSION | 
      VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR,
      dwlConditionMask);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Check if we are running as Administrator
	IsAdmin = IsUserAnAdmin();

	// Check Windows version - used by EZFont
	Win7_or_later = Is_Win7_or_Later();
	
	// Copy the help file to the temporary directory

	WCHAR sourcePath[MAX_PATH];
	WCHAR destPath[MAX_PATH];
	WCHAR HelpFilename[200];

	// Get the directory in which the exe file exists
	HMODULE hModule = GetModuleHandleW(NULL);
	GetModuleFileName(hModule, sourcePath, MAX_PATH);
	WCHAR *nameStart = wcsrchr(sourcePath,'\\')+1;
	*nameStart = '\0';

	// Add on the name of the help file
	LoadString(hInst,IDS_HELPFILENAME,HelpFilename, 199);
	lstrcat(sourcePath,HelpFilename);

	// Find the system directory for temporary files 
	// Copy the help file across and record its location
	// Fallback to original location
	if (GetTempPath(MAX_PATH, destPath))
	{
		lstrcat(destPath, HelpFilename);
		if (CopyFile(sourcePath, destPath, FALSE))
			szHelpFilename = destPath;
		else
			szHelpFilename = sourcePath;
	}
	else
		szHelpFilename = sourcePath;


	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ENZLAB, szWindowClass, MAX_LOADSTRING);

	RegisterMainClass(hInstance);
	RegisterEnzSolnClass(hInstance);
	RegisterEnzRunClass(hInstance);
	RegisterEnzClkClass(hInstance);
	RegisterEnzGraphClass(hInstance);
	RegisterEnzInfoClass(hInstance);


	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ENZLAB));


	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: RegisterMainClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM RegisterMainClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENZLAB));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDM_MENU);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ENZLAB));

	return RegisterClassEx(&wcex);
}

ATOM RegisterEnzSolnClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= EnzSolnWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENZLAB));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"EnzSoln";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

ATOM RegisterEnzRunClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= EnzRunWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENZLAB));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"EnzRun";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

ATOM RegisterEnzClkClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= EnzClkWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENZLAB));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"EnzClk";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

ATOM RegisterEnzGraphClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= EnzGraphWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENZLAB));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"EnzGraph";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

ATOM RegisterEnzInfoClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= EnzInfoWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENZLAB));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"EnzInfo";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
  
   // initialise variables, child windows etc
   // ESOLN should be initialised before Expt windows
   InitExptVar();

   InitExptWin( hWnd, hInstance );
   // clock window must be initialised before clock variables and after Expt windows
   InitClockWin( hWnd, hInstance );
   InitClockVar( hWnd );

   // do the startup bits (select enzyme etc)
   InitEnzLab( hWnd, hInstance );


   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	PAINTSTRUCT ps;
	HDC hdc;
	WORD	wEnable;
	MINMAXINFO FAR	*lpMinMax;
	WCHAR	buffer[BUFLEN];
	WCHAR title[205];

	switch (message)
	{
	case WM_CREATE:
		// Introductory dialog
		if (Intro( hWnd, hInst ) == FALSE)
		{
			PostQuitMessage(0);
			exit(0);
		}
        break;

	case WM_GETMINMAXINFO:
		lpMinMax = (MINMAXINFO FAR *)lParam;
		lpMinMax->ptMinTrackSize.x = xMin;
		lpMinMax->ptMinTrackSize.y = yMin;
        break;

	case WM_SIZE:
		xMainClient = LOWORD(lParam);
		yMainClient = HIWORD(lParam);
		SizeExptWindows();
		SizeEnzClk();
		break;

	case WM_INITMENU:
			// disable Experiment if enzyme not selected
			EnableMenuItem( (HMENU)wParam, IDM_EXPT, (Events.bEnzSel == FALSE) ? MF_GRAYED : MF_ENABLED );
			break;

	case WM_INITMENUPOPUP:
            // Gray menu items / alter items if no enzyme selected
			wEnable = (Events.bEnzSel == FALSE) ? MF_GRAYED : MF_ENABLED;
			EnableMenuItem( (HMENU)wParam, IDM_SAVE,	wEnable );
			EnableMenuItem( (HMENU)wParam, IDM_RESTART,	wEnable );
			EnableMenuItem( (HMENU)wParam, IDM_EINFO,	wEnable );
			EnableMenuItem( (HMENU)wParam, IDM_EXPT,	wEnable );
			EnableMenuItem( (HMENU)wParam, IDM_GOHOME,	wEnable );
			LoadString(hInstance, IDS_SELECT, title, 100);
			LoadString(hInstance, IDS_RESELECT, buffer, 100);
			ModifyMenu( (HMENU)wParam, IDM_ESELECT, MF_BYCOMMAND, IDM_ESELECT,
				(Events.bEnzSel == FALSE) ? title : buffer );
			// Gray menu items if insufficient results obtained
			EnableMenuItem( (HMENU)wParam, IDM_RESLIST, (Events.nResults > 1) ? MF_ENABLED : MF_GRAYED );
			EnableMenuItem( (HMENU)wParam, IDM_CLIPBOARD, (Events.nResults > 1) ? MF_ENABLED : MF_GRAYED );
			EnableMenuItem( (HMENU)wParam, IDM_ANLIST, (Events.nKinetics > 0) ? MF_ENABLED : MF_GRAYED );
            wEnable = (Events.nResults > ANALYSIS_MIN) ? MF_ENABLED : MF_GRAYED;
			EnableMenuItem( (HMENU)wParam, IDM_PRECISION,	wEnable );
			EnableMenuItem( (HMENU)wParam, IDM_KINETICS,	wEnable );
			EnableMenuItem( (HMENU)wParam, IDM_PHPROFILE,	wEnable );
			// Redraw menu

			break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_SAVE:
            Save( hWnd, hInstance );
			break;

		case IDM_LOAD:
			Load( hWnd, hInstance );
			break;

		case IDM_ABOUT:
			HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"About.html" );
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_EINFO:
			EnzInfo( hWnd, hInstance );
			break;

		case IDM_RESTART:
			LoadString(hInstance, IDS_RESTART_WARN, buffer, 500);
			LoadString(hInstance, IDS_RESTART_TITLE, title, 100);
			if (MessageBox( hWnd, buffer,
							title, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) == IDYES) {
				InitExptVar();
				InitClockVar( hWnd );
				InitEnzLab( hWnd, hInstance );
            }
			break;

		case IDM_PRINTSETUP:
					PrintSetup( hWnd, hInstance );
					break;

		case IDM_ESELECT:
			if (Events.bEnzSel == FALSE) {
			// Get 'em to choose the enzyme and start the first day if they do
				if (EnzSel( hWnd, hInstance ) == TRUE) {
					Events.bEnzSel = TRUE;
					ResetEntireScreen( hWnd );
					NewDay( hWnd, hInstance );
				}
			}
			else 
			{	
				LoadString(hInstance, IDS_RESEL_WARN, buffer, 500);
				LoadString(hInstance, IDS_RESEL_TITLE, title, 100);
				if (MessageBox( hWnd, buffer,
								title, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDYES) 
				{
								
					InitExptVar();
					InitClockVar( hWnd );
					InitEnzLab( hWnd, hInstance );
				}																																			
            }
            break;

		case IDM_EXPT:
			GiveEnzRunFocus();
			break;

		case IDM_RESLIST:
			ResLis( hWnd, hInstance );
			break;

		case IDM_CLIPBOARD:
			Clipboard( hWnd );
			break;

		case IDM_ANLIST:
			AnList( hWnd, hInstance );
            break;

		case IDM_PRECISION:
			if (Precision( hWnd, hInstance ))
                Events.nPrecisions++;
			break;

		case IDM_KINETICS:
			Kinetics( hWnd, hInstance );
			break;


		case IDM_PHPROFILE:
			if (PHProfile( hWnd, hInstance ) == TRUE)
				Events.nPHProfiles++;
            break;

		case IDM_GOHOME:
			LoadString(hInstance, IDS_GOHOME_TITLE, title, 100);
			if (nulEnzyme > VOL_RESIDUE)
				LoadString(hInstance, IDS_GOHOME1, buffer, 500);
			else
				LoadString(hInstance, IDS_GOHOME2, buffer, 500);
    
			if (MessageBox( hWnd, buffer, title, MB_YESNO | MB_ICONQUESTION ) == IDYES)
				PostMessage( hWnd, WM_COMMAND, IDM_GOHOMENOW, 0L );
            break;

		case IDM_GOHOMENOW:
			LoadString( hInstance, IDS_WORK_AGAIN, buffer, 500);
			LoadString( hInstance, IDS_APP_TITLE, title, 100);
			if (MessageBox( hWnd, buffer,
                    		title, MB_ICONQUESTION | MB_YESNO ) == IDYES) 
			{
				NewDay( hWnd, hInstance );
			}
			else
			{
				LoadString( hInstance, IDS_SURE_EXIT, buffer, 500);
				LoadString( hInstance, IDS_EXIT_TITLE, title, 100);
				if (MessageBox( hWnd, buffer, title,
				MB_ICONQUESTION | MB_OKCANCEL ) == IDOK) 
				{
					//if (Events.bHelp == TRUE)
					//	HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"howt000g.htm" );
                    //WinHelp( hWnd, HELPFILE, HELP_QUIT, 0L );
					DestroyWindow( hWnd );
				}
				else
				{
					NewDay( hWnd, hInstance );
				}
			}
			break;

		case IDM_HCONTENTS:
			Events.bHelp = TRUE;
			HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"cont0001.htm" );
			//WinHelp( hWnd, HELPFILE, HELP_CONTENTS, 0L );
			break;

		case IDM_HWHATNEXT:
            WhatNext( hWnd );
			break;

		case IDM_HPROGRAM:
			Events.bHelp = TRUE;
			HtmlHelp(NULL, szHelpFilename, HH_DISPLAY_TOPIC, (DWORD)L"bgrn0003.htm" );
			//WinHelp( hWnd, HELPFILE, HELP_CONTEXT, bg_program );
			break;

		case IDM_MORE:
			More();
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		//if (hPrinterDevNames != NULL)
		//	GlobalFree( hPrinterDevNames );
		//if (hPrinterDevMode != NULL)
        //    GlobalFree( hPrinterDevMode );
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void InitEnzLab( HWND hWnd, HINSTANCE hInstance )
{
	// Initialise EVENTS
	Events.bEnzSel = FALSE;
	Events.bHelp = FALSE;
	Events.nResults = 1;
	Events.nPrecisions = 0;
	Events.nKinetics = 0;
    Events.nPHProfiles = 0;
	Events.todNow = TOD_UNKNOWN;

	// Initialise the results lists and enzyme
	InitResList();
    InitAnList();
	InitEnz();

	ResetEntireScreen( hWnd );

	// Get 'em to choose the enzyme and start the first day if they do
	if (EnzSel( hWnd, hInstance ) == TRUE) {
		Events.bEnzSel = TRUE;
		ResetEntireScreen( hWnd );
		NewDay( hWnd, hInstance );
    }

	return;
}


void ResetEntireScreen( HWND hWnd )
{
	InvalidateRect( hWnd, NULL, TRUE );
	SendMessage( hWnd, WM_INITMENU, (WPARAM)GetMenu( hWnd ), 0L );
	DrawMenuBar( hWnd );
	EmptyEnzRunControls();
}