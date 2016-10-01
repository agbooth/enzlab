//
//  el_EnzSelController.m
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_EnzSelController.h"
#import "el_AppDelegate.h"
#import "L.h"

@interface el_EnzSelController ()

@end

@implementation el_EnzSelController

@synthesize enzymeGroup, protease, dehydrogenase, hydratase, decarboxylase, phosphatase;
@synthesize helpButton, cancelButton, OKButton;

@synthesize result;

NSInteger nChoice;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        nChoice = app.EnzChar.ID;
        if (nChoice == IDD_UNKNOWN)
            nChoice = IDD_PROTEASE;
        result = FALSE;
    }
    
    return self;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_EnzSelController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_EnzCellController"
                         owner:self];
    }
    
    [self.view.window makeFirstResponder:enzymeGroup];
    
    app.Events.bEnzSel = FALSE;
    
    [enzymeGroup selectCellAtRow: nChoice - IDD_PROTEASE column:0];
    
    [NSApp runModalForWindow: self.view.window];
}
#pragma GCC diagnostic pop

- (IBAction)enzymeGroupClicked:(id)sender
{
    nChoice = IDD_PROTEASE + [enzymeGroup selectedRow];
}

- (IBAction)helpButtonClicked:(id)sender {
    [self.view.window makeFirstResponder:helpButton];
    NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"enz_sel"  inBook:locBookName];
    
}

- (IBAction)cancelButtonClicked:(id)sender {
    result = FALSE;
    [self.view.window orderOut:nil];
    [self.view.window close];
    [NSApp stopModal];
}

- (IBAction)OKButtonClicked:(id)sender {
    result = TRUE;
    app.EnzChar.ID = nChoice;
    [self.view.window orderOut:nil];
    [self.view.window close];
    [NSApp stopModal];
}

- (Boolean) getResult
{
    return result;
}
@end
