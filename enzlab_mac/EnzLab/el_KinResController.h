//
//  el_KinResController.h
//  EnzLab
//
//  Created by Andrew Booth on 24/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "el_KinView.h"

@interface el_KinResController : NSViewController

@property (strong) IBOutlet NSButton *OKButton;
@property (strong) IBOutlet NSButton *printButton;
@property (strong) IBOutlet NSButton *helpButton;
@property (strong) IBOutlet el_KinView *kinView;
@property (strong) IBOutlet NSTextField *yAxisLabel;

- (IBAction)OKButtonPressed:(id)sender;
- (IBAction)printButtonPressed:(id)sender;
- (IBAction)helpButtonPressed:(id)sender;

- (void) showDialog;

@end
