//
//  el_GraphView.h
//  EnzLab
//
//  Created by Andrew Booth on 15/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "el_ExptRun.h"

@interface el_GraphView : NSView

@property float delay;
@property float inc;
@property float xHeight;
- (Boolean) SameConditions: (el_ExptRun*) testrun;
- (float) getXHeight;
@end
