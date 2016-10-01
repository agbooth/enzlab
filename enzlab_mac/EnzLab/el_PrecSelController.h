//
//  el_PrecSelController.h
//  EnzLab
//
//  Created by Andrew Booth on 22/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface el_PrecSelController : NSViewController <NSTableViewDataSource,NSTableViewDelegate>

@property (strong) IBOutlet NSScrollView *scrollView;
@property (strong) IBOutlet NSTableView *tableView;
@property (strong) IBOutlet NSButton *helpButton;
@property (strong) IBOutlet NSButton *cancelButton;
@property (strong) IBOutlet NSButton *OKButton;
@property (strong) IBOutlet NSTableColumn *Column0;
@property (strong) IBOutlet NSTableColumn *Column1;
@property (strong) IBOutlet NSTableColumn *Column2;
@property (strong) IBOutlet NSTableColumn *Column3;
@property (strong) IBOutlet NSTableColumn *Column4;
@property (strong) IBOutlet NSTableColumn *Column5;
@property (strong) IBOutlet NSTextField *Vol_box;
@property (strong) IBOutlet NSTextField *pH_box;
@property (strong) IBOutlet NSTextField *S_box;
@property (strong) IBOutlet NSTextField *I_box;
@property (strong) IBOutlet NSTextField *nMatched;

@property float selVol;
@property float selpH;
@property float selS;
@property float selI;

@property Boolean result;


- (IBAction)TableRowSelected:(id)sender;


- (IBAction)helpButtonPressed:(id)sender;
- (IBAction)cancelButtonPressed:(id)sender;
- (IBAction)OKButtonPressed:(id)sender;

- (void) showDialog;

@end
