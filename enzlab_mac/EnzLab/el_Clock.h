//
//  el_Clock.h
//  EnzLab
//
//  Created by Andrew Booth on 10/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Foundation/Foundation.h>
enum timeints { CLK_HOUR, CLK_HALFHOUR, CLK_NOTIME };

enum timesofday { TOD_UNKNOWN, TOD_MORNING, TOD_AFTERNOON, TOD_EEVEN, TOD_LEVEN,
    TOD_LATE, TOD_HOME };


@interface el_Clock : NSObject

// STIME data item
@property NSUInteger clkDay;
@property NSUInteger clkHour;
@property NSUInteger clkMin;

- (void) InitClockWin;
- (void) InitClockVar;

- (void) PaintClock;
- (void) ClockSetDay: (NSUInteger) day hr: (NSUInteger) hr min: (NSUInteger) min;
- (Boolean) ClockFwd: (NSUInteger) timeincr;
- (void) ClockInc;
- (void) ClockOff;
- (void) ShowClock;

- (Boolean) LunchTime;
- (Boolean) TeaTime;
- (Boolean) EevenTime;
- (Boolean) LevenTime;
- (Boolean) BedTime;
@end
