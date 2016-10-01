/****************************************************************************

	Linear regression function stolen from Second Year Statistics
    package written by Doug Clow

****************************************************************************/

#include "stdafx.h"
#include <math.h>
#include "linreg.h"

/******************************************************************************

       function name :  t95

             purpose :  return t95 value for given number of degrees of freedom
                        (uses array if d.o.f. <= MAXT, else returns 1.96)

    input parameters :  
        int dof         number of degrees of freedom

        return value :  the value of t95 for the given number of d.o.f.

******************************************************************************/    
#define MAXT95  31

const float t95s[MAXT95+1] = {
    0.0f,
    12.706f, 4.303f, 3.182f, 2.776f, 2.571f, 2.447f, 2.365f, 2.306f, 2.262f, 2.228f,
    2.201f,  2.179f, 2.160f, 2.145f, 2.131f, 2.120f, 2.110f, 2.101f, 2.093f, 2.086f,
    2.080f,  2.074f, 2.069f, 2.064f, 2.060f, 2.056f, 2.052f, 2.048f, 2.045f, 2.042f,
    2.039f
};

float t95( int dof )
{
    return ((dof > MAXT95) ? 1.960f : t95s[dof]);
}



/******************************************************************************

       function name :  regress

             purpose :  perform LMSLRA

    input parameters :  
        struct sums *sum        pointer to structure holding the sums of
                                x, y, xy, x^2 and y^2
        int n                   number of points sampled

        return value :  pointer to a static structure holding the results
                        of the regression: m, c, sm, sc, 95% confidence 
                        limits and correlation coefficient

******************************************************************************/    
struct line *regress( struct sums *sum, int n )
{
    static struct line  rline;
    float   nssx, se, rd;
    
    nssx = (float)(n*sum->xx - sum->x*sum->x);
    rline.m = (n*sum->xy - sum->x*sum->y) / nssx;
    rline.c = (sum->y - rline.m*sum->x) / n;
    se = (float)sqrt((sum->yy - rline.c*sum->y - rline.m*sum->xy) / (n - 2));
    rline.sm = se / sqrt(nssx / n);
    rline.tsm = t95(n-2) * rline.sm;
    rline.sc = se * sqrt(sum->xx/nssx);
    rline.tsc = t95(n-2) * rline.sc;
    rd = (float)sqrt((n*sum->xx - sum->x*sum->x) * (n*sum->yy - sum->y*sum->y));
    rline.r = (n*sum->xy - sum->x*sum->y) / rd;

    return &rline;
} /*  regress */

