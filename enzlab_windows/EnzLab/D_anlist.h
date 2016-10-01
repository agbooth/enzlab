/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		   (32-bit version by: Andrew Booth, April 2014)

              MODULE: ANLIST		

             PURPOSE: ANLIST data item external declarations

 DATA ITEMS REQUIRED: n/a

 DATA ITEMS RESIDENT: n/a

OTHER MODULES CALLED: n/a


******************************************************************************/


#ifndef __D_ANLIST_LOADED__
#define __D_ANLIST_LOADED__ 1

#ifndef __STRUCT_LOADED__
	#include "struct.h"
#endif // __STRUCT_LOADED__

// ANLIST data item extern declarations
extern struct anlist *anlisthead;
extern struct anlist *anlisttail;

#endif // __D_ANLIST_LOADED__