/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: NRC

			 PURPOSE: Numerical Recipes in C header file

 DATA ITEMS REQUIRED: NONE

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: NONE


******************************************************************************/


float *vector( int nl, int nh );
int *ivector( int nl, int nh );
float **matrix( int nrl, int nrh, int ncl, int nch );
void free_vector( float *v, int nl, int nh );
void free_ivector( int *v, int nl, int nh );
void free_matrix( float **m, int nrl, int nrh, int ncl, int nch );
int mrqmin( float x[], float y[], float sig[], int ndata, float a[], int ma,
	int lista[], int mfit, float **covar, float **alpha,
	float *chisq, void (*funcs)(float, float *, float *, float *, int), double *alamda );
void mrqcof( float x[], float y[], float sig[], int ndata, float a[],
	int ma, int lista[], int mfit, float **alpha, float beta[],
	float *chisq, void (*funcs)(float, float *, float *, float *, int) );
void covsrt( float **covar, int ma, int lista[], int mfit );
int gaussj( float **a, int n, float **b, int m );

