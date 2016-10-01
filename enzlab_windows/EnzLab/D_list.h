/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: LIST		

             PURPOSE: LIST data item external declarations

 DATA ITEMS REQUIRED: n/a

 DATA ITEMS RESIDENT: n/a

OTHER MODULES CALLED: n/a


******************************************************************************/


#ifndef __D_LIST_LOADED__
#define __D_LIST_LOADED__ 1

#ifndef __STRUCT_LOADED__
	#include "struct.h"
#endif // __STRUCT_LOADED__

// LIST data item extern declarations
extern struct exptrun *listhead;
extern struct exptrun *listtail;

#endif // __D_LIST_LOADED__