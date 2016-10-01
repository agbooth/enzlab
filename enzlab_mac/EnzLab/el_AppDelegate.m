//
//  el_AppDelegate.m
//  EnzLab
//
//  Created by Andrew Booth on 08/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_AppDelegate.h"
#import "el_EnzController.h"
#import "el_IntroController.h"
#import "el_WelcomeController.h"
#import "el_Expt.h"
#import "el_Clock.h"
#import "el_Limits.h"
#import "el_GraphView.h"
#import "L.h"

@implementation el_AppDelegate

@synthesize Vol_remaining_label;
@synthesize Clock_Label;
@synthesize Run_label;
@synthesize Vol_box;
@synthesize pH_box;
@synthesize Substrate_box;
@synthesize Inhibitor_box;
@synthesize Rate_label;
@synthesize Repeat_button;
@synthesize Go_button;

@synthesize nSamples, nPrecSel, nKinSel, nPHPSel;
@synthesize nulEnzyme;
@synthesize Events;
@synthesize ec, expt, clock;

@synthesize EnzChar, list, anlist, commands, lists;

@synthesize graphView;

@synthesize itemAbout, itemSave, itemLoad, itemRestart, itemQuit;
@synthesize itemInformation, itemSelect;
@synthesize itemExperiment;
@synthesize itemListExperimental, itemListKinetic, itemCopy;
@synthesize itemPrecision, itemKinetics, itemPHProfile;
@synthesize itemGoHome;
@synthesize itemContents, itemHowWorks, itemWhatNext, itemMoreFrom;
@synthesize	nOutliers;
@synthesize Km, Vmax, cKm, cVmax, rpH;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [NSMenu setMenuBarVisible:FALSE];
    
    el_IntroController* ic = [[el_IntroController alloc] initWithNibName:Nil bundle:Nil];
    [ic showDialog];
    
    el_WelcomeController* wc = [[el_WelcomeController alloc] initWithNibName:Nil bundle:Nil];
    [wc showDialog];
    
    
    
    ec = [[el_EnzController alloc] initWithNibName:Nil bundle:Nil];
    
    [[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:ec];
    
    Vol_box.delegate = pH_box.delegate = Substrate_box.delegate = Inhibitor_box.delegate = ec;
    
    [Go_button setTarget:ec];
    [Go_button setAction:@selector(GoButtonClicked)];
    
    [Repeat_button setTarget:ec];
    [Repeat_button setAction:@selector(RepeatButtonClicked)];
    
    commands = [[el_Commands alloc] init];
    
    expt = [[el_Expt alloc] init];
    [expt InitExptVar];
    [expt InitExptWin];
    
    clock = [[el_Clock alloc] init];
    [clock InitClockWin];
    [clock InitClockVar];
    
    [self.window makeKeyAndOrderFront:self];
    [self.window makeFirstResponder:Vol_box];
    self.window.delegate = ec;
    
    [self InitEnzLab];
}

- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification
{
    return YES;
}

- (void) InitEnzLab
{
    // Initialise EVENTS
    Events = [[el_SFlags alloc] init];
	Events.bEnzSel = FALSE;
	Events.bHelp = FALSE;
	Events.nResults = 1;
	Events.nPrecisions = 0;
	Events.nKinetics = 0;
    Events.nPHProfiles = 0;
	Events.todNow = TOD_UNKNOWN;
    
    lists = [[el_Reslist alloc] init];
    [lists InitResList];
    [lists InitAnList];

    [expt InitEnz];
    
    [expt EmptyEnzRunControls];
    
    // Get 'em to choose the enzyme and start the first day if they do
	if ([expt EnzSel] == TRUE)
    {
		Events.bEnzSel = TRUE;
		[expt EmptyEnzRunControls];
		[expt NewDay];
    }
    
    [NSMenu setMenuBarVisible:TRUE];
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return YES;
}

// Handle menu action messages - pass them to the el_Commands object.
- (IBAction)dispatch:(id)sender
{
    [commands dispatch:sender];
}

- (void)menuNeedsUpdate:(NSMenu *)menu
{
    NSMenu* topMenu = [menu supermenu];
    NSString *title = [menu title];
    
    NSInteger index = [topMenu indexOfItemWithTitle:title];
    
    switch (index)
    {
        case 0: // EnzLab menu
            break;
            
        case 1: // File menu
            [[menu itemAtIndex:0] setEnabled:Events.bEnzSel]; // Save
            [[menu itemAtIndex:3] setEnabled:Events.bEnzSel]; // Restart
            break;
            
        case 2: // Enzyme menu
            [[menu itemAtIndex:0] setEnabled:Events.bEnzSel]; // Information
            //[[menu itemAtIndex:1] setEnabled:TRUE];
            [[menu itemAtIndex:1] setTitle:(Events.bEnzSel) ? L(@"Reselect...") : L(@"Select...")];
            break;
            
      //  case 3: // Experiment menu
      //      [[menu itemAtIndex:0] setEnabled:Events.bEnzSel]; // Experiment
      //      break;
            
        case 3: // Results menu
            [[menu itemAtIndex:0] setEnabled:(Events.nResults > 1)];
            [[menu itemAtIndex:1] setEnabled:(Events.nKinetics > 0)];
            [[menu itemAtIndex:2] setEnabled:(Events.nResults > 1)];
            break;
            
        case 4: // Analysis menu
            [[menu itemAtIndex:0] setEnabled:(Events.nResults > ANALYSIS_MIN)];
            [[menu itemAtIndex:1] setEnabled:(Events.nResults > ANALYSIS_MIN)];
            [[menu itemAtIndex:2] setEnabled:(Events.nResults > ANALYSIS_MIN)];
            break;
            
        case 5: // Go home menu
            [[menu itemAtIndex:0] setEnabled:Events.bEnzSel]; // Go home
            break;
            
        case 6: // Help menu
            break;
            
        
    }
    
    
}


@end
