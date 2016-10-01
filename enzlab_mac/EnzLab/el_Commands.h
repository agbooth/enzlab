//
//  el_Commands.h
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface el_Commands : NSObject
@property NSInteger nOutliers;
@property float Km;
@property float Vmax;
@property float cKm;
@property float cVmax;

- (void) dispatch:(id)sender;
-(void) GoHome;
-(void) GoHomeNow;
@end
