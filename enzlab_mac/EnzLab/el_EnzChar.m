//
//  el_EnzChar.m
//  EnzLab
//
//  Created by Andrew Booth on 10/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//


#import "el_EnzChar.h"

@implementation el_EnzChar
@synthesize ID, ref, name, mod, Vn, Kn, Ka, Kb, m, n, p;

- (id)initWithCoder:(NSCoder *)decoder {
    self = [super init];
    if (self != nil) {
        ID = [[decoder decodeObjectForKey:@"ID"] integerValue];
        ref = [decoder decodeObjectForKey:@"ref"];
        name = [decoder decodeObjectForKey:@"name"];
        mod = [[decoder decodeObjectForKey:@"mod"] floatValue];
        Vn = [[decoder decodeObjectForKey:@"Vn"] floatValue];
        Kn = [[decoder decodeObjectForKey:@"Kn"] floatValue];
        Ka = [[decoder decodeObjectForKey:@"Ka"] floatValue];
        Kb = [[decoder decodeObjectForKey:@"Kb"] floatValue];
        m = [[decoder decodeObjectForKey:@"m"] integerValue];
        n = [[decoder decodeObjectForKey:@"n"] integerValue];
        p = [[decoder decodeObjectForKey:@"p"] integerValue];
    }
    return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder {
    [encoder encodeObject:[NSNumber numberWithInteger:ID] forKey:@"ID"];
    [encoder encodeObject:ref forKey:@"ref"];
    [encoder encodeObject:name forKey:@"name"];
    [encoder encodeObject:[NSNumber numberWithFloat:mod] forKey:@"mod"];
    [encoder encodeObject:[NSNumber numberWithFloat:Vn] forKey:@"Vn"];
    [encoder encodeObject:[NSNumber numberWithFloat:Kn] forKey:@"Kn"];
    [encoder encodeObject:[NSNumber numberWithFloat:Ka] forKey:@"Ka"];
    [encoder encodeObject:[NSNumber numberWithFloat:Kb] forKey:@"Kb"];
    [encoder encodeObject:[NSNumber numberWithInteger:m] forKey:@"m"];
    [encoder encodeObject:[NSNumber numberWithInteger:n] forKey:@"n"];
    [encoder encodeObject:[NSNumber numberWithInteger:p] forKey:@"p"];
}

@end
