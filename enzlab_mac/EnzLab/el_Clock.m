//
//  el_Clock.m
//  EnzLab
//
//  Created by Andrew Booth on 10/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_Clock.h"
#import "el_AppDelegate.h"
#import "L.h"

typedef Boolean CLOCKFUNC();



@implementation el_Clock

// STIME data item
@synthesize clkDay;
@synthesize clkHour;
@synthesize clkMin;

struct clkevent {
	NSUInteger hr;
	NSUInteger min;
	NSUInteger fn;
} ClkEvents[] = {
    12, 30, 0,
    17, 30, 1,
    20,  0, 2,
    22,  0, 3,
    24,  0, 4
};

#define NREVENTS    (int)(sizeof( ClkEvents )/sizeof( struct clkevent ))


NSString *szDay[8];

// clock on/off switch
BOOL bClkSwitch = TRUE;

- (id)init
{
    self = [super init];
    if (self) {
        szDay[0] = L(@"Sunday");
        szDay[1] = L(@"Monday");
        szDay[2] = L(@"Tuesday");
        szDay[3] = L(@"Wednesday");
        szDay[4] = L(@"Thursday");
        szDay[5] = L(@"Friday");
        szDay[6] = L(@"Saturday");
        szDay[7] = L(@"[error]");
        
    }
    return self;
}

- (void) InitClockWin
{
   // Nothing to do as the window sizing is handled by the system.
}

- (void) InitClockVar
{
    [self ClockSetDay: 0 hr: 9 min: 0];
}

- (void) PaintClock
{
    NSString* timeString;
    
    if (clkDay == 0)
    {
        timeString = LF(@"  Simulation time:  \r\n %2lu:%02lu  \r\n   [not started] ", (unsigned long)clkHour, clkMin );
        
    }
    else
    {
        timeString = LF(@"  Simulation time:  \r\n %2lu:%02lu  \r\n  %@ (day %lu)  ",
                      (unsigned long)clkHour, clkMin, szDay[clkDay % 7], (unsigned long)clkDay );
    }
    
    [app.Clock_Label setStringValue:timeString];
}


- (void) ClockSetDay: (NSUInteger) day hr: (NSUInteger) hr min: (NSUInteger) min
{
    clkDay = day;
    clkHour = hr;
    clkMin = min;
    
    if (clkHour >= 12 && clkMin > 0)
    {
        //Should never happen.
        
        NSAlert* alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:NSLocalizedString(@"OK",@"")];
        [alert setAlertStyle:NSCriticalAlertStyle];
        [alert setMessageText:@"EnzLab: INTERNAL ERROR"];
        [alert setInformativeText:NSLocalizedString(@"Don't know what time of day it is!\n[I can't start days except in the morning.]",@"")];
        [alert runModal];

    }
    
    app.Events.todNow = TOD_MORNING;
    [self PaintClock];
    
}

- (Boolean) ClockFwd: (NSUInteger) timeincr
{
    NSInteger i;
    
    if (bClkSwitch == TRUE)
    {
        if (timeincr == CLK_HALFHOUR)
            i = 30;
        else if (timeincr == CLK_HOUR)
            i = 60;
        else
            i = 0;
        
        while (i-- > 0)
        {
            [self ClockInc];
            [self ShowClock];
        }
        
        for (i = 0; i < NREVENTS; i++)
        {
            if ((ClkEvents[i].hr == clkHour) && (ClkEvents[i].min == clkMin))
            {
                switch (ClkEvents[i].fn)
                {
                    case 0: return [self LunchTime];
                    case 1: return [self TeaTime];
                    case 2: return [self EevenTime];
                    case 3: return [self LevenTime];
                    case 4: return [self BedTime];
                    default: return TRUE;
                }
            }
        }
    }
    
    return TRUE;
}

- (void) ClockInc
{
	clkMin++;
    if (clkMin == 60)
    {
        clkMin = 0;
		clkHour++;
    }
}


- (void) ClockOff
{
    bClkSwitch = FALSE;
}


- (void) ShowClock
{
	if (bClkSwitch == TRUE)
    	[self PaintClock];
}



- (Boolean) LunchTime
{
    NSAlert* alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"OK")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:L(@"Time for lunch.\nBreak for an hour.")];
    [alert runModal];
    
    [self ClockFwd: CLK_HOUR];
    app.Events.todNow = TOD_AFTERNOON;

    return TRUE;
    
}

- (Boolean) TeaTime
{
    NSAlert* alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"No")];
    [alert addButtonWithTitle:L(@"Yes")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:L(@"Everybody else is going home.\nDo you want to work late?")];
    NSInteger result = [alert runModal];
    
    if (result==1001)          // Yes
    {
        NSAlert* alert2 = [[NSAlert alloc] init];
        [alert2 addButtonWithTitle:L(@"OK")];
        [alert2 setAlertStyle:NSInformationalAlertStyle];
        [alert2 setMessageText:L(@"EnzLab")];
        [alert2 setInformativeText:L(@"Spend half an hour having tea.")];
        [alert2 runModal];
        
        [self ClockFwd: CLK_HOUR];
        app.Events.todNow = TOD_EEVEN;
        
        return TRUE;
    }
    else
    {
        [app.commands GoHomeNow];
        return FALSE;
    }

    
    
}

- (Boolean) EevenTime
{
    NSAlert* alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"No")];
    [alert addButtonWithTitle:L(@"Yes")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:L(@"It's getting quite late.\nDo you want to continue working?")];
    NSInteger result = [alert runModal];
    
    if (result==1001)          // Yes
    {
        app.Events.todNow = TOD_LEVEN;
        
        return TRUE;
    }
    else
    {
        [app.commands GoHomeNow];
        return FALSE;
    }
    
}

- (Boolean) LevenTime
{
    app.Events.todNow = TOD_LATE;
    return TRUE;
    
}

- (Boolean) BedTime
{
    NSAlert* alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"OK")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:L(@"You are too tired to work any later than this!")];
    [alert runModal];
    
    
    [app.commands GoHomeNow];
    return FALSE;
    
}

@end
