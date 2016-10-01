/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: STRUCT.H		

             PURPOSE: structures for the entire program

 DATA ITEMS REQUIRED: n/a

 DATA ITEMS RESIDENT: n/a

OTHER MODULES CALLED: n/a


******************************************************************************/

#ifndef __STRUCT_LOADED__
#define __STRUCT_LOADED__ 1


#ifndef __CLOCK_LOADED__
	#include "clock.h"
#endif // __CLOCK_LOADED__

#ifndef __LIMITS_LOADED__
	#include "limits.h"
#endif // __LIMITS_LOADED__

// ECHAR structure

struct enzchar {
	int	ID;
	WCHAR ref[ENZREFLEN];
	WCHAR name[ENZNAMELEN];
	float mod;
	float Vn;
	float Kn;
	float Ka;
	float Kb;
	short m, n, p;
};


// LIST structure

struct exptrun {
	int n;
	float vol;
	float pH;
	float i;
	float s;
	float v;
	short sel;
	struct exptrun *next;
	struct exptrun *prev;
};


// ANLIST structure

struct anlist {
	int n;
	float Km, cKm;
	float Vmax, cVmax;
	float pH;
	float i;
	int	nResults;
	struct anlist *next;
	struct anlist *prev;
};


// EVENTS structure

struct sflags {
	int bEnzSel;
	int bHelp;
	int nResults;
	int nPrecisions;
	int nKinetics;
	int nPHProfiles;
	enum timesofday todNow;
};


#endif // __STRUCT_LOADED__