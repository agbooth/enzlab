//
//  el_ResListController.h
//  EnzLab
//
//  Created by Andrew Booth on 21/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface el_ResListController : NSViewController <NSTableViewDataSource,NSTableViewDelegate>

@property (strong) IBOutlet NSScrollView *scrollView;
@property (strong) IBOutlet NSTableView *tableView;
@property (strong) IBOutlet NSButton *helpButton;
@property (strong) IBOutlet NSButton *printButton;
@property (strong) IBOutlet NSButton *OKButton;
@property (strong) IBOutlet NSTableColumn *Column0;
@property (strong) IBOutlet NSTableColumn *Column1;
@property (strong) IBOutlet NSTableColumn *Column2;
@property (strong) IBOutlet NSTableColumn *Column3;
@property (strong) IBOutlet NSTableColumn *Column4;
@property (strong) IBOutlet NSTableColumn *Column5;


- (IBAction)helpButtonPressed:(id)sender;
- (IBAction)printButtonPressed:(id)sender;
- (IBAction)OKButtonPressed:(id)sender;

- (void) showDialog;

@end
