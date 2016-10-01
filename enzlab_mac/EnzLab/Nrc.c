/******************************************************************************

						   EnzLab for Windows

                   (coded by:  Doug Clow, 1992 / 1993)


              MODULE: NRC

             PURPOSE: Numerical Recipes in C functions

 DATA ITEMS REQUIRED: NONE

 DATA ITEMS RESIDENT: NONE

OTHER MODULES CALLED: NONE


******************************************************************************/


#include "nrc.h"
#include <stdlib.h>

/*
	Utility functions used only by this module and KINCALC
*/
float *vector( int nl, int nh )
{
	float	*v;

	v = (float *)malloc((unsigned)(nh-nl+1)*sizeof(float));
	if (!v)
		return NULL; // nrerror("allocation failure in vector()");
	return (v - nl);
}

int	*ivector( int nl, int nh )
{
	int	*v;

	v = (int *)malloc((unsigned)(nh-nl+1)*sizeof(int));
	if (!v)
		return NULL; // nrerror("allocation failure in ivector()");
	return (v - nl);
}

float **matrix( int nrl, int nrh, int ncl, int nch )
{
	int	i;
	float	**m;

	m = (float **)malloc((unsigned)((nrh-nrl+1)*sizeof(float*)));
	if (!m)
		return NULL; // nrerror("allocation failure 1 in matrix()");
	m -= nrl;
	for (i = nrl; i <= nrh; i++) {
		m[i] = (float *)malloc((unsigned)((nch-ncl+1)*sizeof(float)));
		if (!m[i])
			return NULL; // nrerror("allocation failure 2 in matrix()");
		m[i] -= ncl;
	}
	return(m);
}


void free_vector( float *v, int nl, int nh )
{
	free((char*)(v+nl));
}


void free_ivector( int *v, int nl, int nh )
{
	free((char*)(v+nl));
}


void free_matrix( float **m, int nrl, int nrh, int ncl, int nch )
{
	int	i;

	for (i = nrh; i >= nrl; i--)
		free((char*)(m[i]+ncl));
	free((char*)(m+nrl));
}



/*
	mrqmin - Marquardt fitting
*/
int mrqmin( float x[], float y[], float sig[], int ndata, float a[], int ma,
	int lista[], int mfit, float **covar, float **alpha,
	float *chisq, void (*funcs)(float, float *, float *, float *, int), double *alamda )
{
	int	k, kk, j, ihit;
	static float	*da, *atry, **oneda, *beta, ochisq;

	if (*alamda < 0.0) {
		if ((oneda = matrix(1, mfit, 1, 1)) == NULL)
        	return 0;
		if ((atry = vector(1, ma)) == NULL)
			return 0;
		if ((da = vector(1, ma)) == NULL)
        	return 0;
		if ((beta = vector(1, ma)) == NULL)
        	return 0;
		kk = mfit+1;
		for (j = 1; j <= ma; j++) {
			ihit = 0;
			for (k = 1; k <= mfit; k++)
				if (lista[k] == j)
					ihit++;
			if (ihit == 0)
				lista[kk++] = j;
			else if (ihit > 1)
				return 0; // nrerror("Bad LISTA permutation in MRQMIN-1");
		}
		if (kk != ma+1)
			return 0; // nrerror("Bad LISTA permutation in MRQMIN-2");
		*alamda = 0.001f;
		mrqcof(x, y, sig, ndata, a, ma, lista, mfit, alpha, beta, chisq, funcs);
		ochisq = (*chisq);
	}
	for (j = 1; j <= mfit; j++) {
		for (k = 1; k <= mfit; k++)
			covar[j][k] = alpha[j][k];
		covar[j][j] = (float)alpha[j][j]*(1.0f+(*alamda));
		oneda[j][1] = beta[j];
	}
	if (gaussj(covar, mfit, oneda, 1) == 0)
    	return 0; // error in gaussj
	for (j = 1; j <= mfit; j++)
		da[j] = oneda[j][1];
	if (*alamda == 0.0) {
		covsrt(covar, ma, lista, mfit);
		free_vector(beta, 1, ma);
		free_vector(da, 1, ma);
		free_vector(atry, 1, ma);
		free_matrix(oneda, 1, mfit, 1, 1);
		return 1;
	}
	for (j = 1; j <= ma; j++)
		atry[j]=a[j];
	for (j = 1; j <= mfit; j++)
		atry[lista[j]] = a[lista[j]]+da[j];
	mrqcof(x, y, sig, ndata, atry, ma, lista, mfit, covar, da, chisq, funcs);
	if (*chisq < ochisq) {
		*alamda *= 0.1f;
		ochisq = (*chisq);
		for (j = 1; j <= mfit; j++) {
			for (k = 1; k <= mfit; k++)
				alpha[j][k] = covar[j][k];
			beta[j] = da[j];
			a[lista[j]] = atry[lista[j]];
		}
	} else {
		*alamda *= 10.0;
		*chisq = ochisq;
	}
	return 1;
}





void mrqcof( float x[], float y[], float sig[], int ndata, float a[],
	int ma, int lista[], int mfit, float **alpha, float beta[],
	float *chisq, void (*funcs)(float, float *, float *, float *, int) )
{
	int	k, j, i;
	float	ymod, wt, sig2i, dy, *dyda;

	dyda = vector(1, ma);
	for (j = 1; j <= mfit; j++) {
		for (k = 1; k <= j; k++)
			alpha[j][k] = 0.0;
		beta[j] = 0.0;
	}
	*chisq = 0.0;
	for (i = 1; i <= ndata; i++) {
		(*funcs)(x[i], a, &ymod, dyda, ma);
		sig2i = 1.0f/(sig[i]*sig[i]);
		dy = y[i] - ymod;
		for (j = 1; j <= mfit; j++) {
			wt = dyda[lista[j]]*sig2i;
			for (k = 1; k <= j; k++)
				alpha[j][k] += wt*dyda[lista[k]];
			beta[j] += dy*wt;
		}
		(*chisq) += dy*dy*sig2i;
	}
	for (j = 2; j <= mfit; j++)
		for (k = 1; k <= j-1; k++)
			alpha[k][j] = alpha[j][k];
	free_vector(dyda, 1, ma);
}




void covsrt( float **covar, int ma, int lista[], int mfit )
{
	int	i, j;
	float	swap;

	for (j = 1; j < ma; j++)
		for (i = j+1; i <= ma; i++)
			covar[i][j] = 0.0;
	for (i = 1; i < mfit; i++)
		for (j = i+1; j <= mfit; j++) {
			if (lista[j] > lista[i])
				covar[lista[j]][lista[i]] = covar[i][j];
			else
				covar[lista[i]][lista[j]] = covar[i][j];
		}
	swap = covar[1][1];
	for (j = 1; j <= ma; j++) {
		covar[1][j] = covar[j][j];
		covar[j][j] = 0.0;
	}
	covar[lista[1]][lista[1]] = swap;
	for (j = 2; j <= mfit; j++)
		covar[lista[j]][lista[j]] = covar[1][j];
	for (j = 2; j <= ma; j++)
		for (i = 1; i <= j-1; i++)
			covar[i][j] = covar[j][i]; 
}


#include <math.h>

#define SWAP(a,b) {float temp=(a);(a)=(b);(b)=temp;}

int gaussj( float **a, int n, float **b, int m )
{
	int	*indxc, *indxr, *ipiv;
	int	i, icol = 0, irow = 0, j, k, l, ll;
	float	big, dum, pivinv;

	indxc = ivector(1, n);
	indxr = ivector(1, n);
	ipiv = ivector(1, n);
	for (j = 1; j <= n; j++)
		ipiv[j] = 0;
	for (i = 1; i <= n; i++) {
		big = 0.0;
		for (j = 1; j <= n; j++)
			if (ipiv[j] != 1)
				for (k = 1; k <= n; k++) {
					if (ipiv[k] == 0) {
						if (fabs(a[j][k]) >= big) {
							big = fabs(a[j][k]);
							irow = j;
							icol = k;
						}
					} else if (ipiv[k] > 1)
						return 0; // nrerror("GAUSSJ: Singular Matrix-1");
				}
		++(ipiv[icol]);
		if (irow != icol) {
			for (l = 1; l <= n; l++)
				SWAP(a[irow][l], a[icol][l])
			for (l = 1; l <= m; l++)
				SWAP(b[irow][l], b[icol][l])
		}
		indxr[i] = irow;
		indxc[i] = icol;
		if (a[icol][icol] == 0.0)
			return 0; // nrerror("GAUSSJ: Singular Matrix-2");
		pivinv = 1.0f/a[icol][icol];
		a[icol][icol] = 1.0;
		for (l = 1; l <= n; l++)
			a[icol][l] *= pivinv;
		for (l = 1; l <= m; l++)
			b[icol][l] *= pivinv;
		for (ll = 1; ll <= n; ll++)
			if (ll != icol) {
				dum = a[ll][icol];
				a[ll][icol] = 0.0;
				for (l = 1; l <= n; l++)
					a[ll][l] -= a[icol][l]*dum;
				for (l = 1; l <= m; l++)
					b[ll][l] -= b[icol][l]*dum;
			}
	}
	for (l = n; l >= 1; l--) {
		if (indxr[l] != indxc[l])
			for (k = 1; k <= n; k++)
				SWAP(a[k][indxr[l]], a[k][indxc[l]]);
	}
	free_ivector(ipiv, 1, n);
	free_ivector(indxr, 1, n);
	free_ivector(indxc, 1, n);

    return 1;
}


