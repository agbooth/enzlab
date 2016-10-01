//
//  el_ExptRun.m
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_ExptRun.h"

@implementation el_ExptRun

@synthesize n;
@synthesize vol;
@synthesize pH;
@synthesize i;
@synthesize s;
@synthesize v;
@synthesize sel;

- (id)initWithCoder:(NSCoder *)decoder {
    self = [super init];
    if (self != nil) {
        n = [[decoder decodeObjectForKey:@"n"] integerValue];
        vol = [[decoder decodeObjectForKey:@"vol"] floatValue];
        pH = [[decoder decodeObjectForKey:@"pH"] floatValue];
        i = [[decoder decodeObjectForKey:@"i"] floatValue];
        s = [[decoder decodeObjectForKey:@"s"] floatValue];
        v = [[decoder decodeObjectForKey:@"v"] floatValue];
        sel = [[decoder decodeObjectForKey:@"sel"] integerValue];

    }
    return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder {
    [encoder encodeObject:[NSNumber numberWithInteger:n] forKey:@"n"];
    [encoder encodeObject:[NSNumber numberWithFloat:vol] forKey:@"vol"];
    [encoder encodeObject:[NSNumber numberWithFloat:pH] forKey:@"pH"];
    [encoder encodeObject:[NSNumber numberWithFloat:i] forKey:@"i"];
    [encoder encodeObject:[NSNumber numberWithFloat:s] forKey:@"s"];
    [encoder encodeObject:[NSNumber numberWithFloat:v] forKey:@"v"];
    [encoder encodeObject:[NSNumber numberWithInteger:sel] forKey:@"sel"];
 
}

- (id)copyWithZone:(NSZone *)zone
{
    el_ExptRun* copy = [[[self class] alloc] init];
    
    if (copy)
    {
        copy.n = self.n;
        copy.vol = self.vol;
        copy.pH = self.pH;
        copy.i = self.i;
        copy.s = self.s;
        copy.v = self.v;
        copy.sel = self.sel;
        
        return copy;
        
    }
    return Nil;
}

@end
