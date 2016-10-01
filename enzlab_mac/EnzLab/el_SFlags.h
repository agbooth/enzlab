//
//  el_SFlags.h
//  EnzLab
//
//  Created by Andrew Booth on 10/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface el_SFlags : NSObject <NSCoding>
@property Boolean bEnzSel;
@property Boolean bHelp;
@property NSInteger nResults;
@property NSInteger nPrecisions;
@property NSInteger nKinetics;
@property NSInteger nPHProfiles;
@property NSInteger todNow;
@end
