/******************************************************************************

                           EnzLab for Windows

		  (coded by:  Doug Clow, November / December 1992)
		  (32-bit version by: Andrew Booth, April 2014)

              MODULE: LIMITS		

             PURPOSE: defined constants for limits, ranges etc

 DATA ITEMS REQUIRED: none

 DATA ITEMS RESIDENT: n/a

OTHER MODULES CALLED: n/a


******************************************************************************/

#ifndef __LIMITS_LOADED__
#define __LIMITS_LOADED__ 1

#define PLUSMINUS			0xB1	// ASCII +/- sign

#define BUFLEN				4096	// Default buffer size

#define ENZREFLEN			15		// length of enzyme reference string
#define ENZNAMELEN			15		// largest enzyme type name (Dehydrogenase)

#define ANALYSIS_MIN		5		// Minimum results required to do analyses

#define NSAMPLES			6		// Initial number of samples in freezer

#define ENZRUN_EDITS		4		// Number of edit controls in EnzRun window

#define SAMPLE_SIZE			500		// size of each frozen sample in ul
#define VOL_MIN				5.0		// minimum volume of enzyme
#define VOL_RESIDUE			20.0	// that annoying bit at the bottom you can't pipette out
#define PH_MIN				4.0		// lowest useful pH
#define PH_MAX				10.0	// highest useful pH
#define I_MIN				0.0		// less than this is silly
#define S_MIN				0.0		// minimum useful conc of substrate
#define S_MAX				100.0	// maximum useful conc of substrate

#define KIF					1.5		// KI factor - multiply KN by this to find KI

#define SPLATCHANCE_DAY		90		// chance of a splat before 5:30pm
#define SPLATCHANCE_EEVEN	68		// chance of a splat between 5:30pm and 8pm
#define SPLATCHANCE_LEVEN	18		// chance of a splat between 8pm and 10pm
#define SPLATCHANCE_LATE	9		// chance of a splat after 10pm

#define ERROR_NRANDS		30		// number of evenly distributed random numbers to make
									// one Gaussian random number
#define ERROR_SIGMA			0.03	// s.d. of errors

#define V_SPLAT				-1.0	// v value signalling 'SPLAT!'
#define V_TOOSLOW			0.0		// v value signalling 'too slow'
#define V_TOOFAST			-20.0	// v value signalling 'too fast'
#define V_SLOW				0.2		// any v slower than this is 'too slow'
#define V_FAST				10.0	// any v faster than this is 'too fast'
#define V_NA				-10.0	// v value signalling 'not available' - i.e. at useless S
#define KM_MAX				50.0	// useless S is this many times Km

/*
	EnzGraph limits etc
*/
#define GRAPH_DELAY			2.0		// time (in sec) to draw an experimental graph
#define GRAPH_TIME			2.0		// time (in min) the experimental graph is drawn over
#define GRAPH_A1			2.0		// \ 	the three scales for the graphs:
#define GRAPH_A2			1.0		//  |- 	maximum values plottable, 
#define GRAPH_A3			0.4		//  |-  added by AGB
#define GRAPH_A4            0.2     // /	in absorbance units
#define GRAPH_SCALE			V_FAST / GRAPH_A1		// v * scale -> value in absorbance units
#define GRAPH_XTICS			5		// number of tics on x-axis
#define GRAPH_YTICS			5		// number of tics on y-axis
#define GRAPH_LEFTCHARS		7		// number of chars space on LHS of graph (including labels/tics)
#define GRAPH_BOTTOMCHARS	4		// number of chars space at bottom of graph (including labels/tics)
#define GRAPH_XSPACE		4		// number of chars space on RHS of graph
#define GRAPH_YSPACE		3		// number of chars space at top of graph

// Minimum required experiments
#define PREC_MIN			5		// Minimum number of results to get a meaningful precision
#define KINETICS_MIN		5		// Min number of results to get meaningful kinetic analysis.
#define PHP_MIN				5		// Min number of results to get a pH profile.

// Marquardt fitting parameters
#define CHISQ_TOL			0.1		// Acceptably small change in chi-sq for ending iteration
#define IT_MAX				100		// Max nr of iterations before we give up
#define ALAMBDA_MAX			1e20	// Biggest alambda before we abandon fit


/*
	WINDOW SIZES
	(in characters)
*/
// EnzClock
#define ENZCLK_WD			18
//#define ENZCLK_HT			3
#define ENZCLK_HT			3.2

// EnzSoln
#define ENZSOLN_WD			25
#define ENZSOLN_HT			ENZCLK_HT

// EnzRun
#define ENZRUN_WD			ENZSOLN_WD
#define ENZRUN_HT			16
#define ENZRUN_ITEMS		8

// EnzInfo
#define ENZINFO_WD			38



#endif // __LIMITS_LOADED__