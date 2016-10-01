//
//  el_ExptRun.h
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface el_ExptRun : NSObject <NSCoding, NSCopying>

@property NSInteger n;
@property 	float vol;
@property 	float pH;
@property 	float i;
@property 	float s;
@property 	float v;
@property 	Boolean sel;

@end
