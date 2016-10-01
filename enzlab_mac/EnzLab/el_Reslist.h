//
//  el_Reslist.h
//  EnzLab
//
//  Created by Andrew Booth on 15/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface el_Reslist : NSObject

- (void) InitAnList;
- (void) InitResList;
- (void) FreeList;
- (void) FreeAnList;
- (void) AddToList: (float) vol ph: (float) ph s: (float) s i: (float) i v: (float) v;
- (void) AddToAnList: (float) Km cKm: (float) cKm Vmax: (float) Vmax cVmax: (float) cVmax pH: (float)pH i: (float) i nResults: (NSInteger) nResults;

@end
