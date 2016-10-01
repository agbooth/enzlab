//
//  el_NewDayController.h
//  EnzLab
//
//  Created by Andrew Booth on 13/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface el_NewDayController : NSViewController

@property (strong) IBOutlet NSTextField *day;
@property (strong) IBOutlet NSTextField *noSamples;
@property (strong) IBOutlet NSTextField *thawSamples;
@property (strong) IBOutlet NSStepper *stepper;
@property (strong) IBOutlet NSButton *helpButton;
@property (strong) IBOutlet NSButton *OKButton;
- (IBAction)helpButtonPressed:(id)sender;
- (IBAction)OKButtonPressed:(id)sender;
- (IBAction)stepperDidChange:(id)sender;

- (void) showDialog;
@end
