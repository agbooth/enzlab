//
//  el_AnList.m
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_AnList.h"

@implementation el_AnList

@synthesize n;
@synthesize Km;
@synthesize cKm;
@synthesize Vmax;
@synthesize cVmax;
@synthesize pH;
@synthesize i;
@synthesize nResults;


- (id)initWithCoder:(NSCoder *)decoder {
    
    self = [super init];
    if (self != nil) {
        n = [[decoder decodeObjectForKey:@"n"] integerValue];
        Km = [[decoder decodeObjectForKey:@"Km"] floatValue];
        cKm = [[decoder decodeObjectForKey:@"cKm"] floatValue];
        Vmax = [[decoder decodeObjectForKey:@"Vmax"] floatValue];
        cVmax = [[decoder decodeObjectForKey:@"cVmax"] floatValue];
        pH = [[decoder decodeObjectForKey:@"pH"] floatValue];
        i = [[decoder decodeObjectForKey:@"i"] floatValue];
        nResults = [[decoder decodeObjectForKey:@"nResults"] integerValue];
        
    }
    return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder {
    
    [encoder encodeObject:[NSNumber numberWithInteger:n] forKey:@"n"];
    [encoder encodeObject:[NSNumber numberWithFloat:Km] forKey:@"Km"];
    [encoder encodeObject:[NSNumber numberWithFloat:cKm] forKey:@"cKm"];
    [encoder encodeObject:[NSNumber numberWithFloat:Vmax] forKey:@"Vmax"];
    [encoder encodeObject:[NSNumber numberWithFloat:cVmax] forKey:@"cVmax"];
    [encoder encodeObject:[NSNumber numberWithFloat:pH] forKey:@"pH"];
    [encoder encodeObject:[NSNumber numberWithFloat:i] forKey:@"i"];
    [encoder encodeObject:[NSNumber numberWithInteger:nResults] forKey:@"nResults"];
    
}

@end
