/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: GRAPH

			 PURPOSE: graphing functions

 DATA ITEMS REQUIRED: NONE

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: NONE


******************************************************************************/

#ifndef __GRAPH_LOADED__
#define __GRAPH_LOADED__ 1


// x axis
#define XLEFTGAP	0.1
#define XMAIN		0.85
#define XRIGHTGAP	0.05
#define XTICLENGTH	0.02
#define XTICS		6
#define XTICS_PH	7				// 7 tics on pH X axis makes it neater!

// y axis
#define YBOTTOMGAP	0.08
#define YMAIN		0.85
#define YTOPGAP		0.05
#define YTICLENGTH	0.02
#define YTICS		6

#endif // __GRAPH_LOADED__