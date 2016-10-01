//
//  el_WelcomeController.m
//  EnzLab
//
//  Created by Andrew Booth on 08/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_WelcomController.h"
#import "el_AppDelegate.h"

@interface el_WelcomeController ()

@end

@implementation el_WelcomeController

@synthesize image;
@synthesize OKButton;

el_AppDelegate* app;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        app = (el_AppDelegate *)[[NSApplication sharedApplication] delegate];
    }
    return self;
}

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

    
    [OKButton setTitle:NSLocalizedString(@"OK",@"")];
    
    [NSApp runModalForWindow: self.view.window];
}

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
