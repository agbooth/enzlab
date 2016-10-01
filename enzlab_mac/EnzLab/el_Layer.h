//
//  el_Layer.h
//  EnzLab
//
//  Created by Andrew Booth on 17/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "el_AppDelegate.h"
#import "el_ExptRun.h"

@interface el_Layer : CAShapeLayer

@property float delay;
@property float xHeight;



- (float) getXHeight;
- (void) SetVariables;
@end
