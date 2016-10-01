//
//  el_WelcomeController.h
//  EnzLab
//
//  Created by Andrew Booth on 08/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface el_WelcomeController : NSViewController

@property (strong) IBOutlet NSPanel *thisPanel;
@property (weak) IBOutlet NSImageView *image;
@property (weak) IBOutlet NSButton *OKButton;

- (IBAction)OKButtonClicked:(id)sender;

- (void) showDialog;
@end
