//
//  el_KinResController.m
//  EnzLab
//
//  Created by Andrew Booth on 24/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_KinResController.h"
#import "el_AppDelegate.h"
#import "L.h"


@interface el_KinResController ()

@end

@implementation el_KinResController

@synthesize helpButton, printButton, OKButton, kinView, yAxisLabel;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_KinResController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_KinResController"
                         owner:self];
    }
    
    [OKButton setTitle:L(@"OK")];
    
    yAxisLabel.wantsLayer = YES;
    yAxisLabel.layer.transform = CATransform3DMakeRotation(M_PI/2.0, 0, 0, 1);
    
    [kinView setNeedsDisplay:TRUE];
    
    [NSApp runModalForWindow: self.view.window];
    
    
}
#pragma GCC diagnostic pop

- (void) print
{
    // Get Print Info
    NSPrintInfo *printInfo = [NSPrintInfo sharedPrintInfo];
    
    NSRect textRect = NSMakeRect(0,0,400,600);
    el_KinView *printView = [[el_KinView alloc] initWithFrame:textRect];
    printView.printing = TRUE;
    
    // Print the graph view
    NSPrintOperation *viewPrint = [NSPrintOperation printOperationWithView:printView printInfo:printInfo];
    [viewPrint setCanSpawnSeparateThread:YES];
    [viewPrint runOperation];
}

- (IBAction)OKButtonPressed:(id)sender
{
    
    [self.view.window orderOut:nil];
    [self.view.window close];
    [NSApp stopModal];
    
}

- (IBAction)printButtonPressed:(id)sender {
    
    [self print];
}

- (IBAction)helpButtonPressed:(id)sender {
    [self.view.window makeFirstResponder:helpButton];
    NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"kin_res"  inBook:locBookName];
}
@end
