//
//  el_IntroController.m
//  EnzLab
//
//  Created by Andrew Booth on 08/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_IntroController.h"
#import "el_WelcomeController.h"
#import "el_AppDelegate.h"
#import "L.h"

@interface el_IntroController ()

@end

@implementation el_IntroController

@synthesize image;
@synthesize licenseLabel;
@synthesize OKButton;
@synthesize HelpButton;

NSModalSession session;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_IntroController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_IntroController"
                         owner:self];
    }
    
    
    [self.view.window makeFirstResponder:OKButton];
    
    [image setImage:[NSApp applicationIconImage]];

    NSString* Licensee = L(@"Registration");
    licenseLabel.stringValue = Licensee;
    
    [OKButton setTitle:L(@"OK")];
    
    [NSApp runModalForWindow: self.view.window];
    //session = [NSApp beginModalSessionForWindow: self.view.window];
    //[NSApp runModalSession:session];
    
}
#pragma GCC diagnostic pop

- (IBAction)OKButtonClicked:(id)sender
{
    
 //   [NSApp endModalSession:session];
    [self.view.window orderOut:nil];
    [self.view.window close];
    [NSApp stopModal];

}

- (IBAction)HelpButtonClicked:(id)sender
{
    [self.view.window makeFirstResponder:HelpButton];
    NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"intro"  inBook:locBookName];
}

- (Boolean) windowShouldClose:(id)sender
{
    [NSApp terminate:app];
    return YES;
}




@end
