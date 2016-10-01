//
//  el_Commands.m
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_Commands.h"
#import "el_Limits.h"
#import "el_GraphView.h"
#import "el_ResListController.h"
#import "el_AnListController.h"
#import "el_PrecSelController.h"
#import "el_PrecResController.h"
#import "el_KinSelController.h"
#import "el_KinResController.h"
#import "el_KinExamController.h"
#import "el_PHPSelController.h"
#import "el_PHPResController.h"
#import "L.h"
#import "el_AppDelegate.h"

@implementation el_Commands

NSString* buffer;

@synthesize	nOutliers;
@synthesize Km, Vmax, cKm, cVmax;

-(void) Restart
{
    NSAlert* alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"No")];
    [alert addButtonWithTitle:L(@"Yes")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:L(@"All the data you have collected will be lost! (Unless you have saved it to disk.) Are you sure?")];
    NSInteger result = [alert runModal];
    
    if (result==1001)          // Yes
    {
        [NSMenu setMenuBarVisible:FALSE];
        [app.expt ClearGraph];
        [app.expt InitExptVar];
        [app.clock InitClockVar];
        [app InitEnzLab];
    }
    
}

-(void) GoHomeNow
{
    NSAlert* alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"No")];
    [alert addButtonWithTitle:L(@"Yes")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:L(@"You go home.\nDo you want to go in to work again? (Saying \"No\" will end the program; saying \"Yes\" will start the next day.)")];
    NSInteger result = [alert runModal];
    if (result==1001)          // Yes
    {
        [app.expt NewDay];
    }
    else
    {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"No")];
        [alert addButtonWithTitle:L(@"Yes")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab")];
        [alert setInformativeText:L(@"Are you sure you want to exit EnzLab?")];
        NSInteger result = [alert runModal];
        if (result==1001)          // Yes
        {
            [NSApp terminate:app];
        }
        else
        {
            [app.expt NewDay];
        }
    }
    
}

- (void) Load
{
    // Create a File Open Dialog class.
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    
    // Set array of file types
    NSArray *fileTypesArray;
    fileTypesArray = [NSArray arrayWithObjects:@"elb_mac", nil];
    
    // Enable options in the dialog.
    [openDlg setCanChooseFiles:YES];
    [openDlg setAllowedFileTypes:fileTypesArray];
    [openDlg setAllowsMultipleSelection:NO];
    
    // Display the dialog box.  If the OK pressed,
    // process the files.
    if ( [openDlg runModal] != NSOKButton ) return;
    
    // Get the list of all files selected
    NSArray *files = [openDlg URLs];
    
    // Get the full path of the file
    NSString* filePath = [[files objectAtIndex:0] path];
    
    NSString* name = [[files objectAtIndex:0] lastPathComponent];
    
    // Load the data from the file.
    
    NSMutableArray* fileArray = [NSKeyedUnarchiver unarchiveObjectWithFile:filePath];
    
    app.EnzChar         =   [fileArray objectAtIndex:0];
    app.Events          =   [fileArray objectAtIndex:1];
    
    app.clock.clkDay    =   [[fileArray objectAtIndex:2] integerValue];
    app.clock.clkHour   =   [[fileArray objectAtIndex:3] integerValue];
    app.clock.clkMin    =   [[fileArray objectAtIndex:4] integerValue];
    app.nSamples        =   [[fileArray objectAtIndex:5] integerValue];
    app.nulEnzyme       =   [[fileArray objectAtIndex:6] integerValue];
    
    app.list            =   [fileArray objectAtIndex:7];
    app.anlist          =   [fileArray objectAtIndex:8];
    
    [app.expt EmptyEnzRunControls];
    [app.clock PaintClock];
    [app.expt UpdateEnzSoln];
    [app.expt UpdateRunNo];
    [app.ec PaintEnzGraph:0.0];
    
    
    NSAlert* alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"OK")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:LF(@"Current data successfully loaded from file %@",name)];
    [alert runModal];
    
}

-(void) Save
{
    NSSavePanel* savePanel = [NSSavePanel savePanel];
    NSArray* allowedTypes = [NSArray arrayWithObject:@"elb_mac"];
    [savePanel setAllowedFileTypes:allowedTypes];
    [savePanel setCanCreateDirectories:YES];
    NSInteger result = [savePanel runModal];
    
    if (result==0) return;
    
    NSError *error;
    Boolean OK;
    NSURL* url = [savePanel URL];
    
    NSMutableArray* fileArray = [[NSMutableArray alloc] init];
    
    // All objects and their sub-objects nust conform to the NSCoding protocol.
    
    [fileArray addObject:app.EnzChar];
    [fileArray addObject:app.Events];
    
    [fileArray addObject:[NSNumber numberWithInteger:app.clock.clkDay]];
    [fileArray addObject:[NSNumber numberWithInteger:app.clock.clkHour]];
    [fileArray addObject:[NSNumber numberWithInteger:app.clock.clkMin]];
    [fileArray addObject:[NSNumber numberWithInteger:app.nSamples]];
    [fileArray addObject:[NSNumber numberWithInteger:app.nulEnzyme]];
    [fileArray addObject:app.list];
    [fileArray addObject:app.anlist];
    
    NSData* data = [NSKeyedArchiver archivedDataWithRootObject:fileArray];
    OK = [data writeToURL: url atomically:YES];
    
    if (!OK)
    {
        // an error occurred
        NSAlert* alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSCriticalAlertStyle];
        [alert setMessageText:[url lastPathComponent]];
        [alert setInformativeText:[error localizedFailureReason]];
        [alert runModal];
        
        return;
    }
    else
    {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab")];
        [alert setInformativeText:LF(@"All current data successfully saved to file %@",[url lastPathComponent])];
        [alert runModal];
    }
}


- (void) ResList
{
    el_ResListController* rlc = [[el_ResListController alloc] initWithNibName:Nil bundle:Nil];
    [rlc showDialog];
}

- (void) AnList
{
    el_AnListController* alc = [[el_AnListController alloc] initWithNibName:Nil bundle:Nil];
    [alc showDialog];
}

- (void) Clipboard
{
    NSPasteboard* pasteBoard = [NSPasteboard generalPasteboard];
    NSString* buffer = @"";
    buffer = [buffer stringByAppendingString:@"No.\tVol\tpH\tS\tI\tVₒ\r\n"];
    for (int i=0; i< app.list.count; i++)
    {
        el_ExptRun* run = [app.list objectAtIndex:i];
        buffer = [buffer stringByAppendingString:[NSString stringWithFormat:@"%ld\t%g\t%.3f\t%g\t%g",
                                         (long)run.n, run.vol, run.pH, run.s, run.i]];
        if (run.v <= 0.0)
            buffer = [buffer stringByAppendingString:[NSString stringWithFormat:@"\t%d\r\n", -1]];
        else
            buffer = [buffer stringByAppendingString:[NSString stringWithFormat:@"\t%.4f\r\n", run.v]];
    }
    
    [pasteBoard clearContents];
    [pasteBoard writeObjects:[NSArray arrayWithObject:buffer]];
}

- (Boolean) Precision
{
    double sum, sumsq, meanV, sdV;
    NSInteger cnt;
    NSAlert* alert;
    
    // Invoke the selection dialog box
    el_PrecSelController* psc = [[el_PrecSelController alloc] initWithNibName:Nil bundle:Nil];
    [psc showDialog];
    
    if (psc.result == FALSE)
        return FALSE;
    
    /*
     Analyse for precision!
     */
    // find selected runs, and keep sums
	sum = sumsq = 0.0;
    cnt = 0;
    
    for (int i = 0; i < app.list.count; i++)
    {
        el_ExptRun* run = [app.list objectAtIndex:i];
        
        if ((run.sel == TRUE) && (run.v > 0.0))
        {
            cnt++;
            sum += run.v;
            sumsq += run.v * run.v;
        }
    }
    
    // did we get enough?
    
    if (cnt == 0)
    {
        alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab: Error")];
        [alert setInformativeText:L(@"You didn't select any successful experimental runs!")];
        [alert runModal];
        
        return FALSE;
    }
    else if (cnt < PREC_MIN)
    {
        if (cnt == 1)
        {
            alert = [[NSAlert alloc] init];
            [alert addButtonWithTitle:L(@"OK")];
            [alert setAlertStyle:NSInformationalAlertStyle];
            [alert setMessageText:(@"EnzLab: Error")];
            [alert setInformativeText:LF(@"You only selected one successful experimental run - you need at least %d for a meaningful precision analysis!",PREC_MIN)];
            [alert runModal];
            
        }
        else
        {
            alert = [[NSAlert alloc] init];
            [alert addButtonWithTitle:L(@"OK")];
            [alert setAlertStyle:NSInformationalAlertStyle];
            [alert setMessageText:L(@"EnzLab: Error")];
            [alert setInformativeText:LF(@"You only selected %ld successful experimental runs - you need at least %d for a meaningful precision analysis!",cnt,PREC_MIN)];
            [alert runModal];
        }
        
        return FALSE;
    }
    
    // display results dialog
    meanV = sum / cnt;
    sdV = sqrt((sumsq - meanV*sum)/ (cnt -1));
    app.nPrecSel = cnt;
    
    
    el_PrecResController* prc = [[el_PrecResController alloc] initWithNibName:Nil bundle:Nil];
    prc.arithMean = meanV;
    prc.StdDev = sdV;
    [prc showDialog];
    
    alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"OK")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:L(@"The precision analysis took you half an hour.")];
    [alert runModal];
    
    [app.clock ClockFwd:CLK_HALFHOUR];
    
    return TRUE;
}

- (void) KinRes
{
    nOutliers = 0;
    el_KinResController* krc = [[el_KinResController alloc] init];
    [krc showDialog];
}

- (void) KinExam
{
    el_ExptRun* run;
    
    app.nOutliers = 0;
    
    for (int i = 0; i < app.list.count; i++)
    {
        run = [app.list objectAtIndex:i];
        if ((run.sel == TRUE) && ((run.s / app.Km) > KM_MAX))
        {
            app.nOutliers++;
        }
    }
    
    if (app.nOutliers == 0)
        return;
    
    el_KinExamController* kec = [[el_KinExamController alloc] init];
    [kec showDialog];
    
    for (int i = 0; i < app.list.count; i++)
    {
        run = [app.list objectAtIndex:i];
        if ((run.sel == TRUE) && ((run.s / app.Km) > KM_MAX))
        {
            run.v = V_NA;
        }
    }
    
    app.nOutliers = 0;
}

- (void) Kinetics
{
    NSAlert* alert;
    //float	Km, Vmax, cKm, cVmax;
    
    // Invoke the selection dialog box
    el_KinSelController* ksc = [[el_KinSelController alloc] initWithNibName:Nil bundle:Nil];
    [ksc showDialog];
    
    if (ksc.result == FALSE)
        return;
    
    // Have they chosen enough?
	if (app.nKinSel < KINETICS_MIN)  // We shouldn't get here because the dialog only enables OK if app.nKinSel >= 5.
    {
		alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab: Error")];
        [alert setInformativeText:L(@"You have not selected enough experiments to perform a kinetic analysis.")];
        [alert runModal];
        
        return;
	}
    
    if ([app.expt MMKinetics: app.nKinSel
                    list: app.list
                      Km: &Km
                    Vmax: &Vmax
                     cKm: &cKm
                   cVmax: &cVmax ] == FALSE)
    {
             
        alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab")];
        [alert setInformativeText:L(@"The (failed) kinetic analysis took you half an hour.")];
        [alert runModal];
        
        [app.clock ClockFwd:CLK_HALFHOUR];
        return;
    
    }
    
    // show the results
    
    // find the first relected run
    
    el_ExptRun* run;
    for (int i = 0; i < app.list.count;  i++)
    {
        run = [app.list objectAtIndex:i];
        if (run.sel)
            break;
    }
    
    app.rpH = run.pH;
    app.Km = Km;
    app.Vmax = Vmax;
    app.cKm = cKm;
    app.cVmax = cVmax;

    [self KinRes];
    
    [self KinExam];
 
    // move on to next kinetic analysis and put the results into the list
	app.Events.nKinetics++;
	[app.lists AddToAnList:app.Km
                       cKm:app.cKm
                      Vmax:app.Vmax
                     cVmax: app.cVmax
                        pH: app.rpH
                         i: run.i
                  nResults: app.nKinSel ];
    
	// and it all took half an hour
    alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"OK")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:L(@"The kinetic analysis took you half an hour.")];
    [alert runModal];
    
    [app.clock ClockFwd:CLK_HALFHOUR];
}

- (void) PHPRes
{
    // Invoke the selection dialog box
    el_PHPResController* prc = [[el_PHPResController alloc] initWithNibName:Nil bundle:Nil];
    [prc showDialog];

}

- (Boolean) PHProfile
{
    NSAlert* alert;
    
    // Invoke the selection dialog box
    el_PHPSelController* psc = [[el_PHPSelController alloc] initWithNibName:Nil bundle:Nil];
    [psc showDialog];
    
    if (psc.result == FALSE)
        return FALSE;
    
    // Have they chosen enough?
	if (app.nPHPSel < PHP_MIN)  // We shouldn't get here because the dialog only enables OK if app.nPHPSel >= 5.
    {
		alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"OK")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab: Error")];
        [alert setInformativeText:L(@"You have not selected enough experiments to draw a pH profile.")];
        [alert runModal];
        
        return FALSE;
	}
    
    // show the result
    [self PHPRes];
    
    // and it all took half an hour
    alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"OK")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:L(@"Plotting the pH profile took you half an hour.")];
    [alert runModal];
    
    [app.clock ClockFwd:CLK_HALFHOUR];
    
    return TRUE;
}

-(void) GoHome
{
    if (app.nulEnzyme > VOL_RESIDUE)
        buffer = L(@"Are you sure you want to go home now? (The remaining thawed enzyme solution will be wasted.)");
    else
        buffer = L(@"Are you sure you want to go home now?");
    
    NSAlert* alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:L(@"No")];
    [alert addButtonWithTitle:L(@"Yes")];
    [alert setAlertStyle:NSInformationalAlertStyle];
    [alert setMessageText:L(@"EnzLab")];
    [alert setInformativeText:buffer];
    NSInteger result = [alert runModal];
    
    if (result==1001)          // Yes
    {
        [self GoHomeNow];
    }
}

- (void) EnzSel
{
    if (app.Events.bEnzSel  == FALSE)
    {
        if ([app.expt EnzSel] == TRUE)
        {
            app.Events.bEnzSel = TRUE;
            
            // Clear the graph
           // [app.expt ClearGraph];
            [app.expt EmptyEnzRunControls];
            [app.expt NewDay];
        }
    }
    else
    {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:L(@"No")];
        [alert addButtonWithTitle:L(@"Yes")];
        [alert setAlertStyle:NSInformationalAlertStyle];
        [alert setMessageText:L(@"EnzLab: Reselect Enzyme")];
        [alert setInformativeText:L(@"This will restart the program, losing all the data you have collected! [Unless you have saved it to disk.]\nAre you sure?")];
        NSInteger result = [alert runModal];
        
        if (result==1001)          // Yes
        {
            [app.expt InitExptVar];
            [app.clock InitClockVar];
            [app InitEnzLab];
        }
    }
}

- (void) WhatNext
{
    NSString *helpName;
    
    if (app.Events.bEnzSel != TRUE)
    	helpName = @"what_next1";
	else if (app.Events.nResults <= ANALYSIS_MIN)
		helpName = @"what_next2";
	else if (app.Events.nKinetics < 1)
		helpName = @"what_next3";
	else if (app.Events.nPrecisions < 1)
		helpName = @"what_next4";
	else if (app.Events.nPHProfiles < 1)
    	helpName = @"what_next5";
	else
		helpName = @"what_next6";
    
    NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:helpName  inBook:locBookName];
    
}

- (void) MoreFrom
{
    NSURL *url = LU(@"more_from");
    [[NSWorkspace sharedWorkspace] openURL:url];
}

- (void) dispatch:(id)sender
{
    if (sender == app.itemSelect)
    {
        [self EnzSel];
    }
    else if (sender == app.itemInformation)
    {
        [app.expt EnzInfo];
    }
    else if (sender== app.itemSave)
    {
        [self Save];
    }
    else if (sender== app.itemLoad)
    {
        [self Load];
    }
    else if (sender == app.itemRestart)
    {
        [self Restart];
    }
    else if (sender == app.itemListExperimental)
    {
        [self ResList];
    }
    else if (sender == app.itemListKinetic)
    {
        [self AnList];
    }
    else if (sender == app.itemCopy)
    {
        [self Clipboard];
    }
    else if (sender == app.itemPrecision)
    {
        if ([self Precision])
            app.Events.nPrecisions++;
    }
    else if (sender == app.itemKinetics)
    {
        [self Kinetics];
    }
    else if (sender == app.itemPHProfile)
    {
        if ([self PHProfile])
            app.Events.nPHProfiles++;
    }
    else if (sender == app.itemGoHome)
    {
        [self GoHome];
    }
    else if (sender == app.itemContents)
    {
        NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
        [[NSHelpManager sharedHelpManager] openHelpAnchor:@"index"  inBook:locBookName];
    }
    else if (sender == app.itemHowWorks)
    {
        NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
        [[NSHelpManager sharedHelpManager] openHelpAnchor:@"how_it_works"  inBook:locBookName];
    }
    else if (sender == app.itemAbout)
    {
        NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
        [[NSHelpManager sharedHelpManager] openHelpAnchor:@"about"  inBook:locBookName];
    }
    else if (sender == app.itemWhatNext)
    {
        [self WhatNext];
    }
    else if (sender == app.itemMoreFrom)
    {
        [self MoreFrom];
    }
    else
        return;
}

@end
