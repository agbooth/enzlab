//
//  el_Reslist.m
//  EnzLab
//
//  Created by Andrew Booth on 15/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_Reslist.h"
#import "el_AnList.h"
#import "el_ExptRun.h"
#import "el_AppDelegate.h"

@implementation el_Reslist

- (void) InitAnList
{
    app.list = [[NSMutableArray alloc] init];
    
}

- (void) InitResList
{
    app.anlist = [[NSMutableArray alloc] init];

}

- (void) FreeList
{
    [app.list removeAllObjects];
}

- (void) FreeAnList
{
    [app.anlist removeAllObjects];
}

- (void) AddToList: (float) vol ph: (float) ph s: (float) s i: (float) i v: (float) v
{
    el_ExptRun* e = [[el_ExptRun alloc] init];
    
    e.n = app.Events.nResults;
    e.vol = vol;
    e.pH = ph;
    e.s = s;
    e.i = i;
    e.v = v;
    e.sel = FALSE;
    
    [app.list addObject:e];
}

- (void) AddToAnList: (float) Km cKm: (float) cKm Vmax: (float) Vmax cVmax: (float) cVmax pH: (float)pH i: (float) i nResults: (NSInteger) nResults
{
    
    el_AnList* e = [[el_AnList alloc] init];
    
    // load up result
	e.n = app.Events.nKinetics;
	e.Km = Km;
	e.cKm = cKm;
	e.Vmax = Vmax;
	e.cVmax = cVmax;
	e.pH = pH;
	e.i = i;
	e.nResults = nResults;
    
    [app.anlist addObject:e];
    
}



@end
