//
//  el_PrecResController.h
//  EnzLab
//
//  Created by Andrew Booth on 22/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface el_PrecResController : NSViewController <NSTableViewDataSource,NSTableViewDelegate, NSTextFieldDelegate>

@property (strong) IBOutlet NSScrollView *scrollView;
@property (strong) IBOutlet NSTableView *tableView;
@property (strong) IBOutlet NSButton *helpButton;
@property (strong) IBOutlet NSButton *OKButton;
@property (strong) IBOutlet NSTableColumn *Column0;
@property (strong) IBOutlet NSTableColumn *Column1;
@property (strong) IBOutlet NSTableColumn *Column2;
@property (strong) IBOutlet NSTableColumn *Column3;
@property (strong) IBOutlet NSTableColumn *Column4;
@property (strong) IBOutlet NSTableColumn *Column5;

@property (strong) IBOutlet NSTextField *selection;
@property (strong) IBOutlet NSTextField *mean;
@property (strong) IBOutlet NSTextField *stddev;

@property (strong) NSMutableArray* selArray;

@property double arithMean;
@property double StdDev;

- (IBAction)helpButtonPressed:(id)sender;
- (IBAction)OKButtonPressed:(id)sender;


- (void) showDialog;

@end
