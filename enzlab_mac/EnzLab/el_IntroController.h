//
//  el_IntroController.h
//  EnzLab
//
//  Created by Andrew Booth on 08/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface el_IntroController : NSViewController 

@property (strong) IBOutlet NSPanel *thisPanel;
@property (weak) IBOutlet NSImageView *image;
@property (weak) IBOutlet NSTextField *licenseLabel;
@property (weak) IBOutlet NSButton *OKButton;
@property (weak) IBOutlet NSButton *HelpButton;

- (IBAction)HelpButtonClicked:(id)sender;
- (IBAction)OKButtonClicked:(id)sender;

- (void) showDialog;
@end
