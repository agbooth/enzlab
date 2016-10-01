//
//  el_EnzInfoController.h
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface el_EnzInfoController : NSViewController

@property (strong) IBOutlet NSTextField *enzName;
@property (strong) IBOutlet NSTextField *enzRef;
@property (strong) IBOutlet NSTextField *enzConc;
@property (strong) IBOutlet NSTextField *enzAct;

@property (strong) IBOutlet NSButton *OKButton;


- (IBAction)OKButtonClicked:(id)sender;

- (void) showDialog;
@end
