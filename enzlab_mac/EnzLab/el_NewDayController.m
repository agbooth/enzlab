//
//  el_NewDayController.m
//  EnzLab
//
//  Created by Andrew Booth on 13/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_NewDayController.h"
#import "el_AppDelegate.h"
#import "el_Limits.h"
#import "L.h"

@interface el_NewDayController ()

@end

@implementation el_NewDayController
@synthesize stepper, day, noSamples, thawSamples, helpButton, OKButton;

NSInteger nToThaw;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_NewDayController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_NewDayController"
                         owner:self];
    }
    
    
    day.stringValue = LF(@"It's day %ld.", (long)app.clock.clkDay);
    noSamples.stringValue = LF(@"(You have %ld.)", (long)app.nSamples);

    [self.view.window makeFirstResponder:stepper];
    
    if (app.nSamples > 0)
    {
        [stepper setMaxValue: (double)app.nSamples];
        [stepper setMinValue:1.0];
        [stepper setIncrement:1.0];
        [stepper setIntegerValue:1];
        [thawSamples setIntegerValue:1];
        nToThaw = 1;
    }
    else
    {
        noSamples.stringValue = L(@"(You have none left!)");
        
        [thawSamples setIntegerValue:0];
        [stepper setEnabled:FALSE];
        [OKButton setEnabled:TRUE];
        nToThaw = 0;
        
    }
    
    [OKButton setTitle:L(@"OK")];
    
    [NSApp runModalForWindow: self.view.window];
    
    
}
#pragma GCC diagnostic pop





- (IBAction)helpButtonPressed:(id)sender {
    [self.view.window makeFirstResponder:helpButton];
    NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"newday"  inBook:locBookName];
}

- (IBAction)OKButtonPressed:(id)sender {
    
    app.nSamples -= nToThaw;
    app.nulEnzyme = nToThaw * SAMPLE_SIZE;
    [app.expt UpdateEnzSoln];
    
    [self.view.window orderOut:nil];
    [self.view.window close];
    [NSApp stopModal];

}


- (IBAction)stepperDidChange:(id)sender {
    
    nToThaw = [sender integerValue];
    [thawSamples setIntegerValue:nToThaw];
    
}
@end
