//
//  el_KinExamController.m
//  EnzLab
//
//  Created by Andrew Booth on 26/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_KinExamController.h"
#import "el_AppDelegate.h"
#import "L.h"

@interface el_KinExamController ()

@end

@implementation el_KinExamController
@synthesize helpButton, OKButton, kinView, yAxisLabel;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_KinExamController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_KinExamController"
                         owner:self];
    }
    
    [OKButton setTitle:L(@"OK")];
    
    yAxisLabel.wantsLayer = YES;
    yAxisLabel.layer.transform = CATransform3DMakeRotation(M_PI/2.0, 0, 0, 1);
    
    [kinView setNeedsDisplay:TRUE];
    
    [NSApp runModalForWindow: self.view.window];
    
    
}
#pragma GCC diagnostic pop

- (IBAction)OKButtonPressed:(id)sender
{
    
    [self.view.window orderOut:nil];
    [self.view.window close];
    [NSApp stopModal];
    
}

- (IBAction)helpButtonPressed:(id)sender {
    [self.view.window makeFirstResponder:helpButton];
    NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"kin_exam"  inBook:locBookName];
}

@end
