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



#include "stdafx.h"

#include <math.h>

#include "kincalc.h"
#include "Linreg.h"
#include "limits.h"
#include "Nrc.h"
#include "struct.h"
#include "Logs.h"
#include "Resource.h"

/*
	Explanatory notes for Marquardt fitting of M-M kinetics using NRC routines
	**************************************************************************

					v = Vmax.S / Km+S

		objective:  for the given set of v-S pairs, find best values for
					Vmax and Km


					x	==	S
					y	==	v
					a	==	list of parameters; a[1] = Vmax and a[2] = Km

		thus
					y = a[1].x / a[2] + x

*/

extern HINSTANCE hInstance; 

void MMkin( float x, float a[], float *y, float dyda[], int na )
{
	// v = Vmax.S / Km + S
	*y = (a[1] * x) / (a[2] + x);

	// (partial) dv/dVmax = S / (Km + S)
	dyda[1] = x / (a[2] + x);

	// (partial) dv/dKm = - (Vmax.S) / (Km^2 + 2.Km.S + S^2)
	dyda[2] = - (a[1]*x) / (a[2]*a[2] + 2*a[2]*x + x*x);

	return;
}


int MMKinetics( int nKinSel, struct exptrun *listhead,
	float *Km, float *Vmax, float *cKm, float *cVmax )
{
	BOOL	bAlloc,
			bDone, obDone,
            bAllSame;
	BOOL	error = FALSE;
	int		n,
			lista[3] = {0, 1, 2},
			nIts;
	WCHAR	szErrMsg[BUFLEN];
	WCHAR title[500];
	float	pH, i, vol, s,
    		xi, yi,
			*x, *y, *sig, **covar, **alpha, a[3],
			chisq, ochisq, alambda;
	struct exptrun *ptr;
	struct sums	sum;
    struct line *results;


    // Allocate memory
    bAlloc = TRUE;
	if ((x = vector( 1, nKinSel )) == NULL)
		bAlloc = FALSE;
	if ((y = vector( 1, nKinSel )) == NULL)
    	bAlloc = FALSE;
	if ((sig = vector( 1, nKinSel )) == NULL)
		bAlloc = FALSE;
	if ((covar = matrix( 1, 2, 1, 2 )) == NULL)
		bAlloc = FALSE;
	if ((alpha = matrix( 1, 2, 1, 2 )) == NULL)
		bAlloc = FALSE;
	if (bAlloc == FALSE) {
		MessageBox( NULL, L"There isn't enough free memory to perform kinetic analysis!",
			L"EnzLab: WARNING", MB_OK | MB_ICONEXCLAMATION );
		return FALSE;
	}

	/*
		Do a linear regression first to get initial guesses.
		(using Lineweaver-Burk plot:  1/v = Km/Vmax * 1/S + 1/Vmax)
    */
	//	Find sums of x (1/S),  y (1/v), xy, x^2 and y^2, storing x-y pairs as we go.  
	sum.x = sum.y = sum.xy = sum.xx = sum.yy = 0.0;
	n = 1;
	
    bAllSame = TRUE;
	for (ptr = listhead; ptr != NULL; ptr = ptr->next) {
		if (ptr->sel == TRUE) {
        	// is this a successful expt?
			if (ptr->v <= 0.0) 
			{
				error = TRUE;
				LoadString( hInstance, IDS_KIN_ERROR1, szErrMsg, 500);
				//szErrMsg = L"One (or more) of the experiments you selected was unsuccessful.\nIt is impossible to include such an experiment in the kinetic analysis!";
			}
			// ensure their choices vary only in v and S
			if (n == 1) {
				pH = ptr->pH;
				i = ptr->i;
				vol = ptr->vol;
                s = ptr->s;
            }
			else if (error == FALSE) {
				if (ptr->pH != pH)
				{
					error = TRUE;
					LoadString( hInstance, IDS_KIN_ERROR2, szErrMsg, 500);
					//szErrMsg = L"The experiments you selected were carried out at differing pHs!\nThis makes any kinetic analysis meaningless.";
				}
				else if (ptr->i != i)
				{
					error = TRUE;
					LoadString( hInstance, IDS_KIN_ERROR3, szErrMsg, 500);
					//szErrMsg =  L"The experiments you selected were carried out at differing concentrations of inhibitor!\nThis makes any kinetic analysis meaningless.";
				}
				else if (ptr->vol != vol)
				{
					error = TRUE;
					LoadString( hInstance, IDS_KIN_ERROR4, szErrMsg, 500);
					//szErrMsg = L"The experiments you selected were carried out using differing volumes of enzyme solution!\nThis makes any kinetic analysis meaningless.";
			
				}
			}
		
			if (error == TRUE) {

				LoadString( hInstance, IDS_ERROR_TITLE, title, 100); 
				MessageBox( NULL, szErrMsg, title, MB_OK );
				free_vector( x, 1, nKinSel );
				free_vector( y, 1, nKinSel );
				free_vector( sig, 1, nKinSel );
				free_matrix( covar, 1, 2, 1, 2 );
				free_matrix( alpha, 1, 2, 1, 2 );
				return FALSE;
			}
			if (ptr->s != s)
            	bAllSame = FALSE;
			// accumulate sums
			x[n] = ptr->s;
			y[n] = ptr->v;
			sig[n] = ERROR_SIGMA * y[n];
			xi = 1.0 / ptr->s;
			yi = 1.0 / ptr->v;
			sum.x += xi;
			sum.y += yi;
			sum.xy += xi * yi;
			sum.xx += xi * xi;
			sum.yy += yi * yi;
            n++;
		}
	}

	if (bAllSame == TRUE) {
		LoadString( hInstance, IDS_ERROR_TITLE, title, 100);
		LoadString( hInstance, IDS_KIN_ERROR5, szErrMsg, 500);
		MessageBox( NULL, szErrMsg,
			title , MB_OK );
		return FALSE;
	}


	// do LMSLRA
	results = regress( &sum, nKinSel );
	*Vmax = 1.0 / results->c;
	*Km = results->m * *Vmax;

    /*
		 Now the nonlinear regression
    */
	a[1] = *Vmax;
	a[2] = *Km;

	// initialise mrqmin
    alambda = -1.0;
	if (mrqmin( x, y, sig, nKinSel, a, 2, lista, 2, covar, alpha, &chisq, MMkin, &alambda ) == FALSE) {
		MessageBox( NULL, L"There is a serious internal problem with the nonlinear regression algorithm!",
			L"EnzLab: INTERNAL ERROR", MB_OK | MB_ICONEXCLAMATION );
		return FALSE;
	}

 	// iterate!
	bDone = FALSE;
    nIts = 0;
	do {
    	nIts++;
		ochisq = chisq;
        obDone = bDone;
		if (mrqmin( x, y, sig, nKinSel, a, 2, lista, 2, covar, alpha, &chisq, MMkin, &alambda ) == FALSE) {
			MessageBox( NULL, L"There is a very serious internal problem with the nonlinear regression algorithm!",
				L"EnzLab: INTERNAL ERROR", MB_OK | MB_ICONEXCLAMATION );
			return FALSE;
		}
		
		if ((alambda >= ALAMBDA_MAX) || (a[2] <= 0.0)  || (a[1] < 0)) {
        	WCHAR	buffer[BUFLEN];
			WCHAR title[105];

			LoadString( hInstance, IDS_ERROR_TITLE, title, 100);
			LoadString( hInstance, IDS_KIN_ERROR6, buffer, 500);
			//swprintf( buffer,
			//	L"The selected data don't fit a Michaelis-Menten curve at all.  \nThis is probably due to your enzyme being saturated.  \nTry using lower substrate concentrations.  \n\n(Nonlinear least squares fitting is impossible on this data.)" );
            MessageBox( NULL, buffer, title, MB_OK );
			free_vector( x, 1, nKinSel );
			free_vector( y, 1, nKinSel );
			free_vector( sig, 1, nKinSel );
			free_matrix( covar, 1, 2, 1, 2 );
			free_matrix( alpha, 1, 2, 1, 2 );

			return FALSE;
        }
		if (chisq < ochisq && ochisq - chisq < CHISQ_TOL)
			bDone = TRUE;
		else
			bDone = FALSE;
	} while (!bDone && !obDone && nIts < IT_MAX );

	// converged, so final call
    alambda = 0.0;
	if (mrqmin( x, y, sig, nKinSel, a, 2, lista, 2, covar, alpha, &chisq, MMkin, &alambda ) == FALSE) {
		MessageBox( NULL, L"There is an extremely serious internal problem with the nonlinear regression algorithm!",
			L"EnzLab: INTERNAL ERROR", MB_OK | MB_ICONEXCLAMATION );
		return FALSE;
	}

    // load up results
    *Vmax = a[1];
	*Km = a[2];
	*cVmax = sqrt( covar[1][1] ) * t95(nKinSel - 2);
	*cKm = sqrt( covar[2][2] ) * t95(nKinSel - 2);

    // free memory and depart
	free_vector( x, 1, nKinSel );
	free_vector( y, 1, nKinSel );
	free_vector( sig, 1, nKinSel );
	free_matrix( covar, 1, 2, 1, 2 );
	free_matrix( alpha, 1, 2, 1, 2 );

    return TRUE;
}
