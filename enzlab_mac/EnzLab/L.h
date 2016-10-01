//
//  L.h
//  EnzLab
//
//  Created by Andrew Booth on 09/06/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#ifndef EnzLab_L_h
#define EnzLab_L_h

#define L(X) NSLocalizedString(X, nil)
#define LF(X, ...) [NSString stringWithFormat:L(X), __VA_ARGS__]
#define LU(X) [NSURL URLWithString:L(X)]

#endif
