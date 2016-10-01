/*

	RAND.C

		random generator of normally-distributed numbers with mu = 0 and sigma = 1

		multiply result by your desired sigma, then add your desired mu
*/

#pragma hdrstop
#include <math.h>
#include "el_Rand.h"

#define M1 259200L
#define IA1 7141L
#define IC1 54773L
#define RM1 (1.0/M1)
#define M2 134456L
#define IA2 8121L
#define IC2 28411L
#define RM2 (1.0/M2)
#define M3 243000L
#define IA3 4561L
#define IC3 51349L

float ran1( int *idum )
{
	static long	ix1, ix2, ix3;
	static float	r[98];
	float	temp;
	static int	iff = 0;
	int	j;

	if (*idum < 0 || iff == 0) {
		iff = 1;
		ix1 = (IC1 - (*idum)) % M1;
		ix1 = (IA1*ix1 + IC1) % M1;
		ix2 = ix1 % M2;
		ix1 = (IA1*ix1 + IC1) % M1;
		ix3 = ix1 % M3;
		for (j = 1; j <= 97; j++) {
			ix1 = (IA1*ix1 + IC1) % M1;
			ix2 = (IA2*ix2 + IC2) % M2;
			r[j] = (ix1 + ix2*RM2)*RM1;
		}
		*idum = 1;
	}
	ix1 = (IA1*ix1 + IC1) % M1;
	ix2 = (IA2*ix2 + IC2) % M2;
	ix3 = (IA3*ix3 + IC3) % M3;
	j = (int)(1 + ((97*ix3) / M3));
	if (j > 97 || j < 1) 
		return 0.0;
	temp = r[j];
	r[j] = (ix1 + ix2*RM2)*RM1;
	return	temp;
}

#undef M1
#undef IA1
#undef IC1
#undef RM1
#undef M2
#undef IA2
#undef IC2
#undef RM2
#undef M3
#undef IA3
#undef IC3


float gasdev( int *idum )
{
	static int iset=0;
	static float	gset;
	float	fac, r, v1, v2;

	if  (iset == 0) {
		do {
			v1 = 2.0*ran1(idum) - 1.0;
			v2 = 2.0*ran1(idum) - 1.0;
			r = v1*v1 + v2*v2;
		} while (r >= 1.0);
		fac = sqrt(-2.0*log(r)/r);
		gset = v1*fac;
		iset = 1;
		return v2*fac;
	} else {
		iset = 0;
		return gset;
	}
}
