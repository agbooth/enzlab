/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: CLOCK		

             PURPOSE: clock functions etc

 DATA ITEMS REQUIRED: none

 DATA ITEMS RESIDENT: none

OTHER MODULES CALLED: errr


******************************************************************************/

#ifndef __CLOCK_LOADED__
#define __CLOCK_LOADED__ 1

enum timeints { CLK_HOUR, CLK_HALFHOUR, CLK_NOTIME };

enum timesofday { TOD_UNKNOWN, TOD_MORNING, TOD_AFTERNOON, TOD_EEVEN, TOD_LEVEN,
				  TOD_LATE, TOD_HOME };

// function prototypes will live here eventually

#endif // __CLOCK_LOADED__