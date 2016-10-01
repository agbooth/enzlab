//
//  el_WelcomeController.m
//  EnzLab
//
//  Created by Andrew Booth on 08/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_AppDelegate.h"
#import "el_WelcomeController.h"
#import "L.h"

@interface el_WelcomeController ()

@end

@implementation el_WelcomeController

@synthesize image;
@synthesize OKButton;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_WelcomeController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_WelcomeController"
                         owner:self];
    }
    
    
    [self.view.window makeFirstResponder:OKButton];
    
    [image setImage:[NSApp applicationIconImage]];
    
    
    [OKButton setTitle:L(@"OK")];
    
    [NSApp runModalForWindow: self.view.window];
}
#pragma GCC diagnostic pop

- (IBAction)OKButtonClicked:(id)sender
{
    [self.view.window orderOut:nil];
    [self.view.window close];
    [NSApp stopModal];
}



- (Boolean) windowShouldClose:(id)sender
{
    [NSApp terminate:app];
    return YES;
}




@end

