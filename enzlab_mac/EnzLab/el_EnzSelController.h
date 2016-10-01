//
//  el_EnzSelController.h
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface el_EnzSelController : NSViewController
@property (strong) IBOutlet NSMatrix *enzymeGroup;
@property (strong) IBOutlet NSButtonCell *protease;
@property (strong) IBOutlet NSButtonCell *dehydrogenase;
@property (strong) IBOutlet NSButtonCell *hydratase;
@property (strong) IBOutlet NSButtonCell *decarboxylase;
@property (strong) IBOutlet NSButtonCell *phosphatase;

@property (strong) IBOutlet NSButton *helpButton;
@property (strong) IBOutlet NSButton *cancelButton;
@property (strong) IBOutlet NSButton *OKButton;

@property Boolean result;

- (IBAction)enzymeGroupClicked:(id)sender;
- (IBAction)helpButtonClicked:(id)sender;
- (IBAction)cancelButtonClicked:(id)sender;
- (IBAction)OKButtonClicked:(id)sender;

- (void) showDialog;
- (Boolean) getResult;

@end
