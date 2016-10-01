/******************************************************************************

						   EnzLab for Windows

                   (coded by:  Doug Clow, 1992 / 1993)
				    (32-bit version by: Andrew Booth, April 2014)

              MODULE: KINCALC

             PURPOSE: Calculations for kinetic analysis - Marquardt fitting

 DATA ITEMS REQUIRED: NONE

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: NONE


******************************************************************************/



int MMKinetics( int nKinSel, struct exptrun *listhead,
	float *Km, float *Vmax, float *cKm, float *cVmax );

