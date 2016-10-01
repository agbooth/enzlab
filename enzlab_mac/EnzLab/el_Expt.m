//
//  el_Expt.m
//  EnzLab
//
//  Created by Andrew Booth on 10/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_Expt.h"
#import "el_Limits.h"
#import "Linreg.h"
#import "Nrc.h"
#import "el_EnzSelController.h"
#import "el_EnzInfoController.h"
#import "el_NewDayController.h"
#import "el_AppDelegate.h"
#import "el_Rand.h"
#import "L.h"
#import "stdlib.h"


@implementation el_Expt

// arrays of data for enzyme characteristics
float knvals[] = { 2.0, 1.0, 0.6, 0.5, 0.3, 0.1, 0.05, 0.02, 0.01, 0.001 };
float vnvals[] = { 1.627, 0.826, 0.438, 0.241, 0.228, 0.103, 0.082, 0.043, 0.0244, 0.028 };
float kavals[] = { 4.3, 5.8, 6.0, 6.2, 6.5 };
float kbvals[] = { 7.7, 7.9, 8.0, 8.5, 9.0 };
float modvals[] = { 1.04, 1.02, 1.0, 0.98, 0.96 };
NSString *enzrefs[] = { @"PR", @"DH", @"HY", @"DC", @"PH" };
NSString *enztypes[5];

#define NRKN    10
#define NRVN    10
#define NRKA    5
#define NRKB    5
#define NRMOD   5
#define NRENZR	5
#define NRENZT  5




- (id)init
{
    self = [super init];
    if (self) {
        enztypes[0] = L(@"protease");
        enztypes[1] = L(@"dehydrogenase");
        enztypes[2] = L(@"hydratase");
        enztypes[3] = L(@"decarboxylase");
        enztypes[4] = L(@"phosphatase");
    }
    return self;
}

- (void) InitExptVar
{
	app.nSamples = NSAMPLES;
	app.nulEnzyme = 0;
}

- (void) InitExptWin
{

}

- (void) InitEnz
{
    app.EnzChar = [[el_EnzChar alloc] init];
    app.EnzChar.ID = IDD_UNKNOWN;
    app.EnzChar.ref = @"";
    
    srand((unsigned int)time(NULL));  // seed the random number generator
}

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

- (Boolean) MMKinetics: (NSInteger) nKinsel
                  list: (NSMutableArray*) list
                    Km: (float *) Km
                  Vmax: (float *) Vmax
                   cKm: (float *) cKm
                 cVmax: (float *) cVmax
{
    Boolean	bAlloc, bDone, obDone, bAllSame;
	int		n,
    lista[3] = {0, 1, 2},
    nIts;
	NSString*	szErrMsg;
	float	pH = 0, i = 0, vol= 0, s = 0,
    xi, yi,
    *x, *y, *sig, **covar, **alpha, a[3],
    chisq, ochisq;
    double alambda;
    
    el_ExptRun* run;
    struct sums sum;
    struct line *results;
    
    NSAlert* alert;
    
    // Allocate memory
    bAlloc = TRUE;
	if ((x = vector( 1, (int)app.nKinSel )) == NULL)
		bAlloc = FALSE;
	if ((y = vector( 1, (int)app.nKinSel )) == NULL)
    	bAlloc = FALSE;
	if ((sig = vector( 1, (int)app.nKinSel )) == NULL)
		bAlloc = FALSE;
	if ((covar = matrix( 1, 2, 1, 2 )) == NULL)
		bAlloc = FALSE;
	if ((alpha = matrix( 1, 2, 1, 2 )) == NULL)
		bAlloc = FALSE;
	if (bAlloc == FALSE) {
		
        alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab: WARNING")];
        [alert setInformativeText:@"There isn't enough free memory to perform kinetic analysis!"];
        [alert runModal];
		return FALSE;
	}
    
    /*
     Do a linear regression first to get initial guesses.
     (using Lineweaver-Burk plot:  1/v = Km/Vmax * 1/S + 1/Vmax)
     */
	//	Find sums of x (1/S),  y (1/v), xy, x^2 and y^2, storing x-y pairs as we go.
	sum.x = sum.y = sum.xy = sum.xx = sum.yy = 0.0;
	n = 1;
	szErrMsg = NULL;
    bAllSame = TRUE;
	for (int counter=0; counter<app.list.count; counter++)
    {
        run = [app.list objectAtIndex:counter];
        
		if (run.sel == TRUE) {
        	// is this a successful expt?
			if (run.v <= 0.0)
				szErrMsg = L(@"One (or more) of the experiments you selected was unsuccessful.\nIt is impossible to include such an experiment in the kinetic analysis!");
			// ensure their choices vary only in v and S
			if (n == 1)
            {
				pH = run.pH;
				i = run.i;
				vol = run.vol;
                s = run.s;
            }
			else if (szErrMsg == NULL)
            {
				if (run.pH != pH)
					szErrMsg = L(@"The experiments you selected were carried out at differing pHs!\nThis makes any kinetic analysis meaningless.");
				else if (run.i != i)
					szErrMsg =  L(@"The experiments you selected were carried out at differing concentrations of inhibitor!\nThis makes any kinetic analysis meaningless.");
				else if (run.vol != vol)
					szErrMsg = L(@"The experiments you selected were carried out using differing volumes of enzyme solution!\nThis makes any kinetic analysis meaningless.");
			}
			if (szErrMsg != NULL)
            {
                alert = [[NSAlert alloc] init];
                [alert addButtonWithTitle:L(@"OK")];
                [alert setAlertStyle:NSInformationalAlertStyle];
                [alert setMessageText:L(@"EnzLab: Error")];
                [alert setInformativeText:szErrMsg];
                [alert runModal];
                
                
                
				free_vector( x, 1, (int)app.nKinSel );
				free_vector( y, 1, (int)app.nKinSel );
				free_vector( sig, 1, (int)app.nKinSel );
				free_matrix( covar, 1, 2, 1, 2 );
				free_matrix( alpha, 1, 2, 1, 2 );
				return FALSE;
			}
			if (run.s != s)
            	bAllSame = FALSE;
			// accumulate sums
			x[n] = run.s;
			y[n] = run.v;
			sig[n] = ERROR_SIGMA * y[n];
			xi = 1.0 / run.s;
			yi = 1.0 / run.v;
			sum.x += xi;
			sum.y += yi;
			sum.xy += xi * yi;
			sum.xx += xi * xi;
			sum.yy += yi * yi;
            n++;
		}
	}
	if (bAllSame == TRUE)
    {
        alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab: Error")];
        [alert setInformativeText:L(@"You cannot perform a kinetic analysis on experiments that were all carried out at the same substrate concentration!")];
        [alert runModal];
		return FALSE;
	}
    
    // do LMSLRA
	results = regress( &sum, (int)app.nKinSel );
	*Vmax = 1.0 / results->c;
	*Km = results->m * *Vmax;
    
    /*
     Now the nonlinear regression
     */
	a[1] = *Vmax;
	a[2] = *Km;

    
    // initialise mrqmin
    alambda = -1.0;
	if (mrqmin( x, y, sig, (int)app.nKinSel, a, 2, lista, 2, covar, alpha, &chisq, MMkin, &alambda ) == FALSE) {
		
        alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:NSLocalizedString(@"OK",@"")];
        [alert setAlertStyle:NSCriticalAlertStyle];
        [alert setMessageText:NSLocalizedString(@"EnzLab: INTERNAL ERROR",@"")];
        [alert setInformativeText:@"There is a serious internal problem with the nonlinear regression algorithm!"];
        [alert runModal];
		return FALSE;
	}
    
    // iterate!
	bDone = FALSE;
    nIts = 0;
	do
    {
    	nIts++;
		ochisq = chisq;
        obDone = bDone;
		if (mrqmin( x, y, sig, (int)app.nKinSel, a, 2, lista, 2, covar, alpha, &chisq, MMkin, &alambda ) == FALSE)
        {
            alert = [[NSAlert alloc] init];
            [alert addButtonWithTitle:NSLocalizedString(@"OK",@"")];
            [alert setAlertStyle:NSCriticalAlertStyle];
            [alert setMessageText:NSLocalizedString(@"EnzLab: INTERNAL ERROR",@"")];
            [alert setInformativeText:@"There is a very serious internal problem with the nonlinear regression algorithm!"];
            [alert runModal];
			return FALSE;
		}
        
        if ((alambda >= ALAMBDA_MAX) || (a[2] <= 0.0)  || (a[1] < 0))
        {
        	NSString*	buffer;
            
			buffer = L(@"The selected data don't fit a Michaelis-Menten curve at all.\n\nThis is probably due to your enzyme being saturated.  \nTry using lower substrate concentrations.  \n\n(Nonlinear least squares fitting is impossible on this data.)");
            
            alert = [[NSAlert alloc] init];
            [alert addButtonWithTitle:L(@"OK")];
            [alert setAlertStyle:NSInformationalAlertStyle];
            [alert setMessageText:L(@"EnzLab: Error")];
            [alert setInformativeText:buffer];
            [alert runModal];
            
            
            
			free_vector( x, 1, (int)app.nKinSel );
			free_vector( y, 1, (int)app.nKinSel );
			free_vector( sig, 1, (int)app.nKinSel );
			free_matrix( covar, 1, 2, 1, 2 );
			free_matrix( alpha, 1, 2, 1, 2 );
            
			return FALSE;
        }
		if ((chisq < ochisq) && ((ochisq - chisq) < CHISQ_TOL))
			bDone = TRUE;
		else
			bDone = FALSE;
	} while (!bDone && !obDone && (nIts < IT_MAX) );
    
	// converged, so final call
    alambda = 0.0;
	if (mrqmin( x, y, sig, (int)app.nKinSel, a, 2, lista, 2, covar, alpha, &chisq, MMkin, &alambda ) == FALSE)
    {
		alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:NSLocalizedString(@"OK",@"")];
        [alert setAlertStyle:NSCriticalAlertStyle];
        [alert setMessageText:NSLocalizedString(@"EnzLab: INTERNAL ERROR",@"")];
        [alert setInformativeText:@"There is a extremely serious internal problem with the nonlinear regression algorithm!"];
        [alert runModal];
        return FALSE;
	}
    
    // load up results
    *Vmax = a[1];
	*Km = a[2];
	*cVmax = sqrt( covar[1][1] ) * t95((int)app.nKinSel - 2);
	*cKm = sqrt( covar[2][2] ) * t95((int)app.nKinSel - 2);
    
    //NSLog(@" Vmax is %g; Km is %g",*Vmax,*Km);
    
    // free memory and depart
	free_vector( x, 1, (int)app.nKinSel );
	free_vector( y, 1, (int)app.nKinSel );
	free_vector( sig, 1, (int)app.nKinSel );
	free_matrix( covar, 1, 2, 1, 2 );
	free_matrix( alpha, 1, 2, 1, 2 );
    
    return TRUE;
    
}



// Generate an enzyme in EnzChar - the ID must be there!
- (void) EnzGen
{
	NSInteger     i, kn, vn, ka, kb;
    
	i = (app.EnzChar.ID - IDD_UNKNOWN) - 1;
	
    app.EnzChar.name = [NSString stringWithFormat:@"%@", enztypes[i]];
    
	app.EnzChar.mod = modvals[i];
    
    
    kn = (NSInteger)(((double)arc4random() / (double)ARC4RANDOM_MAX) * (double)NRKN);
    
	app.EnzChar.Kn = knvals[kn];
 
    vn = (NSInteger)(((double)arc4random() / (double)ARC4RANDOM_MAX) * (double)NRVN);
	app.EnzChar.Vn = vnvals[vn];
 
    ka = (NSInteger)(((double)arc4random() / (double)ARC4RANDOM_MAX) * (double)NRKA);
	app.EnzChar.Ka = kavals[ka];

    kb = (NSInteger)(((double)arc4random() / (double)ARC4RANDOM_MAX) * (double)NRKB);
	app.EnzChar.Kb = kbvals[kb];

	app.EnzChar.m = (NSInteger)((((double)arc4random() / (double)ARC4RANDOM_MAX) * 1.0) + 0.5);
	app.EnzChar.n = (NSInteger)((((double)arc4random() / (double)ARC4RANDOM_MAX) * 1.0) + 0.5);
	app.EnzChar.p = (NSInteger)((((double)arc4random() / (double)ARC4RANDOM_MAX) * 2.0) + 0.5);
    
    /* DEBUG START
    app.EnzChar.name = [NSString stringWithFormat:@"%@", enztypes[0]];
    app.EnzChar.mod = modvals[2];
    app.EnzChar.Kn = knvals[2];
    app.EnzChar.Vn = vnvals[2];
    app.EnzChar.Ka = kavals[2];
    app.EnzChar.Kb = kbvals[2];
    app.EnzChar.m = app.EnzChar.n = app.EnzChar.p = 1;
     DEBUG END */
    
	app.EnzChar.ref = [NSString stringWithFormat: @"%@/%ld%ld-%ld%ld%ld-%ld%ld%ld", enzrefs[i],
             (long)kn, (long)vn, (long)ka, (long)kb, (long)i, (long)app.EnzChar.m, (long)app.EnzChar.n, (long)app.EnzChar.p ];
    
	return;
}

- (void) EnzInfo
{
    el_EnzInfoController *ezc = [[el_EnzInfoController alloc] initWithNibName:NULL bundle:NULL];
    [ezc showDialog];
    
}

- (Boolean) EnzSel
{

    el_EnzSelController *ezc = [[el_EnzSelController alloc] initWithNibName:NULL bundle:NULL];
    [ezc showDialog];
    
    Boolean result = [ezc getResult];
    app.Events.bEnzSel = result;
    
    if (result == TRUE)
    {
        [self EnzGen];
        [self EnzInfo];
        
        return TRUE;
    }
    else
        return FALSE;
}

- (void) UpdateRunNo
{
    NSString *buffer = LF(@"Run no. %ld",(long)app.Events.nResults);
    [app.Run_label setStringValue:buffer];
    
}

- (void) UpdateEnzSoln
{
    if (app.nulEnzyme == 0)
    {
        [app.Vol_remaining_label setStringValue:L(@"Enzyme \r\nSolution Remaining: \r\n [none]")];
    }
    else
    {
        [app.Vol_remaining_label setStringValue:LF(@"Enzyme \r\nSolution Remaining: \r\n %ld µL",(long)app.nulEnzyme)];
    }
}

- (void) EmptyEnzRunControls
{
    [app.Vol_box setStringValue:@""];
    [app.pH_box setStringValue:@""];
    [app.Substrate_box setStringValue:@""];
    [app.Inhibitor_box setStringValue:@""];
    [app.Rate_label setStringValue:@""];
    
    [self UpdateRunNo];
    
    if (app.Events.bEnzSel == FALSE)
    {
        [app.Repeat_button setEnabled:FALSE];
        [app.Go_button setEnabled:FALSE];
    }
    [app.window makeFirstResponder:app.Vol_box];
}

- (void) NewDay
{
    [app.clock ClockSetDay:app.clock.clkDay+1 hr:9 min:0];
    app.nulEnzyme = 0;
    [self UpdateEnzSoln];
    
    el_NewDayController* ndc = [[el_NewDayController alloc] init];
    [ndc showDialog];
    
    NSAlert* alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"OK")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:L(@"You should now plan your experimental tactics for today. You need to decide what experimental runs to carry out, what analyses to perform, and so on.")];
    [alert runModal];
    
    NSAlert* alert2 = [[NSAlert alloc] init];
    [alert2 addButtonWithTitle:L(@"OK")];
    [alert2 setAlertStyle:NSInformationalAlertStyle];
    [alert2 setMessageText:L(@"EnzLab")];
    [alert2 setInformativeText:L(@"The planning has taken you an hour.")];
    [alert2 runModal];
    
    [app.clock ClockFwd:CLK_HOUR];
    
}

- (void)ClearGraph  // N.B. Only to be used immediately before reinitializing.
{
    [app.lists AddToList:0 ph: 0 s: 0 i: 0 v: V_CLEAR ];
    [app.ec PaintEnzGraph: 0.0 ];
}

- (Boolean) DoEnzRun
{
    NSString *buffer;
    NSInteger m, n, p, nSplatChance;
    float h, ka, kb, ki, fac, km, vmax;
    float vol, ph, i, s, v;
    
    NSAlert *alert;
    
    static int idnum = -1;
    
    // do they have any thawed enzyme left?
	if (app.nulEnzyme == 0)
    {
		if (app.nSamples == 0)
			buffer = L(@"You have no enzyme left, thawed or otherwise.\nYou can do no more experimental work.");
		else
			buffer = L(@"You have no thawed enzyme left: you can do no more experimental work today.\n(You could thaw some more tomorrow morning.)");
		alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab")];
        [alert setInformativeText:buffer];
        [alert runModal];
		return FALSE;
    }
    
    /*
     Get the values from the boxes and check they're OK, returning FALSE if they're not
     */
	// vol
	
    vol = [app.Vol_box floatValue];
	if (vol > (app.nulEnzyme - VOL_RESIDUE))
    {
		buffer = LF(@"You don't have enough enzyme to use %g µL!", vol );
		if (vol <= app.nulEnzyme)
			buffer = LF(@"You don't have enough enzyme to use\n%g µL as the last %g µL cannot be used.", vol, VOL_RESIDUE );
		alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab")];
        [alert setInformativeText:buffer];
        [alert runModal];
		return FALSE;
    }
	else if (vol < VOL_MIN)
    {
        alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab")];
        [alert setInformativeText:L(@"You'll need to use more enzyme than that!")];
        [alert runModal];
		return FALSE;
    }
    
    // pH
    ph = [app.pH_box floatValue];

	if ((ph > (float)PH_MAX) || (ph < (float)PH_MIN))
    {
		buffer = LF(@"pH must be between %.2g and %.2g!", PH_MIN, PH_MAX );
		alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab")];
        [alert setInformativeText:buffer];
        [alert runModal];
		return FALSE;
    }
        // S
	s = [app.Substrate_box floatValue];
	if (s < S_MIN) {
		alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab")];
        [alert setInformativeText:L(@"You'll need more substrate than that!")];
        [alert runModal];
		return FALSE;
	}
	else if (s > S_MAX) {
		alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab")];
        [alert setInformativeText:L(@"Think!  You are wasting substrate.")];
        [alert runModal];
		return FALSE;
    }
    
    // I
	i = [app.Inhibitor_box floatValue];
	if (i < I_MIN) {
		alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab")];
        [alert setInformativeText:L(@"You are trying to use a silly amount of inhibitor!")];
        [alert runModal];

		return FALSE;
    }

    /*
     Do the run!
     */
	// Remove the enzyme we're using from the bottle
	app.nulEnzyme -= vol;
	[self UpdateEnzSoln];

    
    // calculate Km and Vmax */
    h = (float)(powf( 10.0, -ph ) / 1000.0);

    ka = (float)(powf( 10.0, -app.EnzChar.Ka ) / 1000.0);
	kb = (float)(powf( 10.0, -app.EnzChar.Kb ) / 1000.0);
    ki = (float)(KIF * app.EnzChar.Kn);
    m = 1 - app.EnzChar.m;
    n = 1 - app.EnzChar.n;
    if ((app.EnzChar.p == 2) || (app.EnzChar.p) == 0)
        p = 1;
    else
        p = 0;
    fac = (1 + (h/ka)*m + (kb/h)*n) * (1 + (i/ki)*p);
    km = app.EnzChar.Kn * fac;
    
    m = app.EnzChar.m;
    n = app.EnzChar.n;
    if (app.EnzChar.p == 2 || app.EnzChar.p == 1)
        p = 1;
    else
        p = 0;
    fac = (1 + (h/ka)*m + (kb/h)*n) * (1 + (i/ki)*p);
    vmax = (float)((0.25 * vol * app.EnzChar.mod) / fac);
    
    // calculate v
	v = (vmax * s) / (km + s);

	v *= (float)((gasdev( &idnum ) * ERROR_SIGMA) + 1);

    // adjust precision
	buffer = [NSString stringWithFormat:@"%0.4f", v ];
    v = [buffer floatValue];
  
    /*
     Check for a splat, and take appropriate action
     */
    // set splat chance to 1 in ...
	switch (app.Events.todNow) {
		case TOD_MORNING:
		case TOD_AFTERNOON:
			nSplatChance = SPLATCHANCE_DAY;
			break;
		case TOD_EEVEN:
			nSplatChance = SPLATCHANCE_EEVEN;
			break;
		case TOD_LEVEN:
			nSplatChance = SPLATCHANCE_LEVEN;
			break;
		case TOD_LATE:
			nSplatChance = SPLATCHANCE_LATE;
			break;
		default:  // Should never happen
			nSplatChance = SPLATCHANCE_DAY;
	};
    // if there's a splat, say so and set v and the v text appropriately
	// no splat on first run or it will mess up the display
    
	if ((arc4random() % nSplatChance == 0) && (app.Events.nResults > 1))
    {
		alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"Ooops!")];
        [alert setInformativeText:L(@"You've just had one of those irritating accidents (spilled the enzyme solution, broke a pipette, forgot the buffer, or whatever) that inevitably happen occasionally, especially when you're tired.")];
        [alert runModal];

        
		buffer =  L(@"[SPLAT!]");
		v = V_SPLAT;
	}
	// otherwise, check if too fast/slow, and set the v text properly
	else {
		if (v > V_FAST)
        {
			v = V_TOOFAST;
			buffer = L(@"[TOO FAST]");
        }
		else if (v < V_SLOW) {
			v = V_TOOSLOW;
			buffer = L(@"[TOO SLOW]");
        }
        else
			buffer = LF(@"Vₒ = %0.3g µmol/min", v);
    }
    
	// put the results into the result list then move on to next expt run
	
    [app.lists AddToList:vol ph: ph s: s i: i v: v ];
    
	app.Events.nResults++;
    
    app.ec.velocityString = buffer;
    
	// draw the graph
    // set graph_delay negative if it's a repeat
    
	[app.ec PaintEnzGraph: [app.Repeat_button isEnabled] ? -GRAPH_DELAY : GRAPH_DELAY ];
 	
    
    // show our results in numerical form
	//[app.Rate_label setStringValue: buffer ];
    
	// it's all taken us half an hour
    //return [app.clock ClockFwd: CLK_HALFHOUR];
    return TRUE;
}


@end
