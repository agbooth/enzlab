#pragma once

#include "resource.h"
#ifndef __ENZLAB_LOADED__
#define __ENZLAB_LOADED__ 1


/*
	DIALOG VALUES
*/
// AnList	[RESLIST]
#define IDD_ANLISTBOX		310

// EnzInfo	[ENZ]
#define IDD_ENZNAME			320
#define IDD_ENZREF			321
#define IDD_ENZACT			322
#define IDD_ENZCONC			323

// EnzRun	[EXPT]
#define IDD_VOL				330
#define IDD_PH				331
#define IDD_S				332
#define IDD_I				333
#define IDD_REPEAT			334
#define IDD_GO				335
#define IDD_TITLE			336

// EnzSel  [ENZ]
#define IDD_UNKNOWN			340
#define IDD_PROTEASE		341
#define IDD_DEHYDROGENASE	342
#define IDD_HYDRATASE		343
#define IDD_DECARBOXYLASE	344
#define IDD_PHOSPHATASE		345

// KinRes & KinExam	[KINETICS]
#define IDD_GRAPHPAINT		350
#define IDD_GRAPHTEXT		351

// KinSel	[KINETICS]
#define IDD_KINLISTBOX		360
#define IDD_KVOL			361
#define IDD_KPH				362
#define IDD_KS				363
#define IDD_KI				364
#define IDD_KNRMATCHED		365
#define IDD_KMIN			366

// Load		[FILE]
#define IDD_LOADNAME		370

// NewDay	[EXPT]
#define IDD_TIME			380
#define IDD_DAY				381
#define IDD_NRSAMPLES		382
#define IDD_THAWSAMPLES		383

// PHPSel	[PHPROF]
#define IDD_HGRAPHPAINT		390
#define IDD_HGRAPHTEXT		391

// PHPRes 	[PHPROF]
#define IDD_HLISTBOX		400
#define IDD_HVOL			401
#define IDD_HPH				402
#define IDD_HS				403
#define IDD_HI				404
#define IDD_HNRMATCHED		405
#define IDD_HMIN			406

// PrecSel	[PREC]
#define IDD_PVOL			410
#define IDD_PPH				411
#define IDD_PS				412
#define IDD_PI				413
#define IDD_PNRMATCHED		414
#define IDD_PLISTBOX		415
#define IDD_PMIN			416

// PrecRes	[PREC]
#define IDD_PRMEAN			420
#define IDD_PRSD			421
#define IDD_PRNR			422
#define IDD_PRLISTBOX		423

// ResLis	[RESLIST]
#define IDD_RESLISTBOX		430

// Save		[FILE]
#define IDD_SAVENAME		440


// all things
#define IDD_PRINT			998
#define IDD_HELP			999
#define IDD_LICENCE			997


/*
	MENU VALUES
*/

// File menu
#define IDM_MENU		300
#define IDM_SAVE		301
#define IDM_LOAD		302
#define IDM_RESTART		303
#define IDM_PRINTSETUP	304
#define IDM_EXIT		305

// Enzyme menu
#define IDM_EINFO		306
#define IDM_ESELECT		307

// Experiment menu
#define IDM_EXPT		308

// Results menu
#define IDM_RESLIST		309
#define IDM_CLIPBOARD	310
#define IDM_ANLIST		311

// Analysis menu
#define IDM_PRECISION	312
#define IDM_KINETICS	313
#define IDM_PHPROFILE	314

// Go Home menu
#define IDM_GOHOME		315
#define IDM_GOHOMENOW	316

// Help menu
#define IDM_HCONTENTS	317
#define IDM_HPROGRAM	318
#define IDM_HWHATNEXT	319
#define IDM_HSEARCH		320
#define IDM_HHOWTOUSE	321
#define IDM_ABOUT		322
#define IDM_MORE		325

// Spare / test menu items
#define IDM_TEST0		500
#define IDM_TEST1		501
#define IDM_TEST2		502
#define IDM_TEST3		503
#define IDM_TEST4		504

/*
	CHILD WINDOW IDs
*/

// children of EnzRun
#define IDCH_RUNNO		0
#define IDCH_VOLEDIT	11
#define IDCH_PHEDIT		21
#define IDCH_SEDIT		31
#define IDCH_IEDIT		41
#define IDCH_V			50
#define IDCH_REPEAT		60
#define IDCH_GO			61

#endif // __ENZLAB_LOADED__
