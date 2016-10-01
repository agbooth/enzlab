//
//  el_EnzInfoController.m
//  EnzLab
//
//  Created by Andrew Booth on 12/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_EnzInfoController.h"
#import "el_AppDelegate.h"
#import "L.h"

@implementation el_EnzInfoController
@synthesize enzName, enzRef, enzConc, enzAct, OKButton;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_EnzInfoController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_EnzInfoController"
                         owner:self];
    }
    
    
    [self.view.window makeFirstResponder:OKButton];
    
    
    
    enzName.stringValue = LF(@"Your enzyme is a %@.", app.EnzChar.name);
    enzRef.stringValue = LF(@"Ref. no.  %@", app.EnzChar.ref);
    enzConc.stringValue = LF(@"%.1f mg/mL", 2.5 / app.EnzChar.Vn);
    enzAct.stringValue = LF(@"%.1f µmol product per min per mg enzyme", 100.0*app.EnzChar.Vn);
    
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

@end
