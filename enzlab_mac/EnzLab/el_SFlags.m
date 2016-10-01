//
//  el_SFlags.m
//  EnzLab
//
//  Created by Andrew Booth on 10/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_SFlags.h"

@implementation el_SFlags
@synthesize bEnzSel, bHelp, nResults, nPrecisions, nKinetics, nPHProfiles, todNow;

- (id)initWithCoder:(NSCoder *)decoder {
    self = [super init];
    if (self != nil) {
        bEnzSel = [[decoder decodeObjectForKey:@"bEnzSel"] integerValue];
        bHelp = [[decoder decodeObjectForKey:@"bHelp"] integerValue];
        nResults = [[decoder decodeObjectForKey:@"nResults"] integerValue];
        nPrecisions = [[decoder decodeObjectForKey:@"nPrecisions"] integerValue];
        nKinetics = [[decoder decodeObjectForKey:@"nKinetics"] integerValue];
        nPHProfiles = [[decoder decodeObjectForKey:@"nPHProfiles"] integerValue];
        todNow = [[decoder decodeObjectForKey:@"todNow"] integerValue];
    }
    return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder {
    [encoder encodeObject:[NSNumber numberWithInteger:bEnzSel] forKey:@"bEnzSel"];
    [encoder encodeObject:[NSNumber numberWithInteger:bHelp] forKey:@"bHelp"];
    [encoder encodeObject:[NSNumber numberWithInteger:nResults] forKey:@"nResults"];
    [encoder encodeObject:[NSNumber numberWithInteger:nPrecisions] forKey:@"nPrecisions"];
    [encoder encodeObject:[NSNumber numberWithInteger:nKinetics] forKey:@"nKinetics"];
    [encoder encodeObject:[NSNumber numberWithInteger:nPHProfiles] forKey:@"nPHProfiles"];
    [encoder encodeObject:[NSNumber numberWithInteger:todNow] forKey:@"todNow"];
}


@end
