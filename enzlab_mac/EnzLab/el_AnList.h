//
//  el_AnList.h
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface el_AnList : NSObject <NSCoding>

@property NSInteger n;
@property float Km;
@property float cKm;
@property float Vmax;
@property float cVmax;
@property float pH;
@property float i;
@property NSInteger	nResults;

@end
