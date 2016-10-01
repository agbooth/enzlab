/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: PRINT

			 PURPOSE: bits and pieces for printing

 DATA ITEMS REQUIRED: none

 DATA ITEMS RESIDENT: none

OTHER MODULES CALLED:

******************************************************************************/

#ifndef __PRINT_LOADED__
#define __PRINT_LOADED__ 1

BOOL Print( HWND hDlg, int iDocument );

enum documents { DOC_KINRES, DOC_RESLIST, DOC_ANLIST, DOC_PHPRES };

#endif // __PRINT_LOADED__