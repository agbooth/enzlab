//
//  el_Expt.h
//  EnzLab
//
//  Created by Andrew Booth on 10/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "linreg.h"
#import "nrc.h"

@interface el_Expt : NSObject

- (void) InitExptVar;
- (void) InitExptWin;
- (void) InitEnz;
- (void) EnzGen;
- (Boolean) EnzSel;
- (void) EnzInfo;
- (void) UpdateRunNo;
- (void) UpdateEnzSoln;
- (void) EmptyEnzRunControls;
- (void) NewDay;
- (void)ClearGraph;
- (Boolean) DoEnzRun;
- (Boolean) MMKinetics: (NSInteger) nKinsel
                  list: (NSMutableArray*) list
                    Km: (float *) Km
                  Vmax: (float *) Vmax
                   cKm: (float *) cKm
                 cVmax: (float *) cVmax;
@end
