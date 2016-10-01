/****************************************************************************

	Linear regression function stolen from Second Year Statistics Package
	written by Doug Clow

****************************************************************************/

#ifndef __LINREG_LOADED__
#define __LINREG_LOADED__ 1

#include <stdlib.h>
#include <time.h>


/****************************************************************************
*    
*   Structures For Function regress()
*   
*       The function regress() [in LINREG.CPP] is called with a pointer to a
*       structure of type sums, containing the sums of x, y, xy, x^2 and
*       y^2, and the number of points sampled.  It returns a pointer to a 
*       structure of type line defined below, which contains the gradient m, 
*       the intercept c, the errors in each, sm and sc, the 95% confidence
*       limits, tsm and tsc, and the correlation coefficient r.
*       
****************************************************************************/

struct sums {
    double x;
    double y;
    double xy;
    double xx;
    double yy;
};

struct line {
    double m;
    double c;
    double sm;
    double tsm;
    double sc;
    double tsc;
    double r;
};


/****************************************************************************
*
*   Function Prototypes
*
*       The prototypes for all the functions in STATS.C are here so that they
*       will be visible to the programs (currently STDEV.C, LINES.C and 
*       NONLIN.C) if they #include this file.
*
*       t95()
*           Called with the number of degrees of freedom, will return the
*           appropriate value of t95.
*            
*       regress()
*           Called with a pointer to a structure of type sums (containing the 
*           sums of x, y, xy, x^2 and y^2) and the number of points sampled, 
*           will return a pointer to a structure of type line (containing the 
*           gradient, intercept, errors, confidence limits and correlation 
*           coefficient determined by least mean squares linear regression 
*           analysis).
*
****************************************************************************/

float t95( int dof );
struct line *regress( struct sums *sum, int n );

#endif  /*  __LINREG_LOADED__  */