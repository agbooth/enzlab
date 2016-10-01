//
//  el_AppDelegate.h
//  EnzLab
//
//  Created by Andrew Booth on 08/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "el_EnzController.h"
#import "el_SFlags.h"
#import "el_Expt.h"
#import "el_Clock.h"
#import "el_EnzChar.h"
#import "el_Reslist.h"
#import "el_Commands.h"
#import "el_GraphView.h"

// EnzSel  [ENZ]
#define IDD_UNKNOWN			340
#define IDD_PROTEASE		341
#define IDD_DEHYDROGENASE	342
#define IDD_HYDRATASE		343
#define IDD_DECARBOXYLASE	344
#define IDD_PHOSPHATASE		345

#define app ((el_AppDelegate *)[[NSApplication sharedApplication] delegate])

@interface el_AppDelegate : NSObject <NSApplicationDelegate, NSMenuDelegate, NSUserNotificationCenterDelegate>

@property (assign) IBOutlet NSWindow *window;

@property (weak) IBOutlet NSTextField *Vol_remaining_label;
@property (weak) IBOutlet NSTextField *Clock_Label;
@property (weak) IBOutlet NSTextField *Run_label;
@property (weak) IBOutlet NSTextField *Vol_box;
@property (weak) IBOutlet NSTextField *pH_box;
@property (weak) IBOutlet NSTextField *Substrate_box;
@property (weak) IBOutlet NSTextField *Inhibitor_box;
@property (weak) IBOutlet NSTextField *Rate_label;
@property (weak) IBOutlet NSButton *Repeat_button;
@property (weak) IBOutlet NSButton *Go_button;

@property NSInteger nSamples;
@property NSInteger nPrecSel;
@property NSInteger nKinSel;
@property NSInteger nPHPSel;
@property NSUInteger nulEnzyme;
@property (strong) el_SFlags *Events;
@property (strong) el_EnzController* ec;
@property (strong) el_Expt *expt;
@property (strong) el_Clock *clock;
@property (strong) el_EnzChar *EnzChar;
@property (strong) el_Commands *commands;
@property (strong) el_Reslist *lists;

@property (strong) IBOutlet NSView *graphView;
@property NSMutableArray *anlist;
@property NSMutableArray *list;

@property NSInteger	nOutliers;
@property float Km, Vmax, cKm, cVmax, rpH;

@property (weak) IBOutlet NSMenuItem *itemAbout;
@property (weak) IBOutlet NSMenuItem *itemSave;
@property (weak) IBOutlet NSMenuItem *itemLoad;
@property (weak) IBOutlet NSMenuItem *itemRestart;
@property (weak) IBOutlet NSMenuItem *itemQuit;
@property (weak) IBOutlet NSMenuItem *itemInformation;
@property (weak) IBOutlet NSMenuItem *itemSelect;
@property (weak) IBOutlet NSMenuItem *itemExperiment;
@property (weak) IBOutlet NSMenuItem *itemListExperimental;
@property (weak) IBOutlet NSMenuItem *itemListKinetic;
@property (weak) IBOutlet NSMenuItem *itemCopy;
@property (weak) IBOutlet NSMenuItem *itemPrecision;
@property (weak) IBOutlet NSMenuItem *itemKinetics;
@property (weak) IBOutlet NSMenuItem *itemPHProfile;
@property (weak) IBOutlet NSMenuItem *itemGoHome;
@property (weak) IBOutlet NSMenuItem *itemContents;
@property (weak) IBOutlet NSMenuItem *itemHowWorks;
@property (weak) IBOutlet NSMenuItem *itemWhatNext;
@property (weak) IBOutlet NSMenuItem *itemMoreFrom;

- (IBAction)dispatch:(id)sender;


- (void) InitEnzLab;

@end
