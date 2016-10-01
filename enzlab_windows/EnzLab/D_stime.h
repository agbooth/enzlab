/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: STIME		

             PURPOSE: STIME data item external declarations

 DATA ITEMS REQUIRED: n/a

 DATA ITEMS RESIDENT: n/a

OTHER MODULES CALLED: n/a


******************************************************************************/

#ifndef __D_STIME_LOADED__
#define __D_STIME_LOADED__ 1

#ifndef __CLOCK_LOADED__
	#include "clock.h"
#endif // __CLOCK_LOADED__

// STIME data item external declarations
extern unsigned int clkDay;
extern unsigned int clkHour;
extern unsigned int clkMin;

#endif // __D_STIME_LOADED__