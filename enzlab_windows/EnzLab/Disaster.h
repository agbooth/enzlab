/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)


              MODULE: DISASTER

			 PURPOSE: exception handling - math errors, signals etc

 DATA ITEMS REQUIRED: NONE

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: NONE


******************************************************************************/

#ifndef __DISASTER_LOADED__
#define __DISASTER_LOADED__
#endif

#include <math.h>

void FPECatcher( int bibble, int reglist );
void SetupDisasterCatchers( void );
int matherr( struct exception *e );
