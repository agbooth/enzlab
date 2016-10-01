//
//  el_EnzChar.h
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#ifndef EnzLab_el_EnzChar_h
#define EnzLab_el_EnzChar_h

#import <Foundation/Foundation.h>

@interface el_EnzChar : NSObject <NSCoding>
@property NSInteger ID;
@property NSString* ref;
@property NSString* name;
@property float mod;
@property float Vn;
@property float Kn;
@property float Ka;
@property float Kb;
@property NSInteger m;
@property NSInteger n;
@property NSInteger p;

@end

#endif
