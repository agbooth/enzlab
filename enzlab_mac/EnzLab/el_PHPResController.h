//
//  el_PHPResController.h
//  EnzLab
//
//  Created by Andrew Booth on 27/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "el_PHPView.h"

@interface el_PHPResController : NSViewController
@property (strong) IBOutlet NSButton *OKButton;
@property (strong) IBOutlet NSButton *printButton;
@property (strong) IBOutlet NSButton *helpButton;
@property (strong) IBOutlet el_PHPView *phpView;
@property (strong) IBOutlet NSTextField *yAxisLabel;

- (IBAction)OKButtonPressed:(id)sender;
- (IBAction)printButtonPressed:(id)sender;
- (IBAction)helpButtonPressed:(id)sender;

- (void) showDialog;
@end
