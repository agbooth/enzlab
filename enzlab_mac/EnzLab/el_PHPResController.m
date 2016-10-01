//
//  el_PHPResController.m
//  EnzLab
//
//  Created by Andrew Booth on 27/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_PHPResController.h"
#import "el_AppDelegate.h"
#import "L.h"

@interface el_PHPResController ()

@end

@implementation el_PHPResController

@synthesize helpButton, printButton, OKButton, phpView, yAxisLabel;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_PHPResController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_PHPResController"
                         owner:self];
    }
    
    [OKButton setTitle:L(@"OK")];
    
    yAxisLabel.wantsLayer = YES;
    yAxisLabel.layer.transform = CATransform3DMakeRotation(M_PI/2.0, 0, 0, 1);
    
    [phpView setNeedsDisplay:TRUE];
    
    [NSApp runModalForWindow: self.view.window];
    
    
}
#pragma GCC diagnostic pop

- (void) print
{
    // Get Print Info
    NSPrintInfo *printInfo = [NSPrintInfo sharedPrintInfo];
    
    NSRect textRect = NSMakeRect(0,0,400,600);
    el_PHPView *printView = [[el_PHPView alloc] initWithFrame:textRect];
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
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"php_res"  inBook:locBookName];
}

@end
