//
//  el_PrecResController.m
//  EnzLab
//
//  Created by Andrew Booth on 22/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_PrecResController.h"
#import "el_Limits.h"
#import "el_AppDelegate.h"
#import "L.h"

@interface el_PrecResController ()

@end

@implementation el_PrecResController

@synthesize scrollView, tableView, helpButton, OKButton;
@synthesize Column0, Column1, Column2, Column3, Column4, Column5;
@synthesize arithMean, StdDev;
@synthesize selection, mean, stddev;
@synthesize selArray;


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {        
        selArray = [[NSMutableArray alloc] init];
        
        for (int i = 0; i < app.list.count; i++)
        {
            el_ExptRun* run = [app.list objectAtIndex:i];
            
            if (run.sel == TRUE)
                [selArray addObject:run];
            
        }
    }
    
    return self;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_PrecResController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_PrecResController"
                         owner:self];
    }
    
    [selection setStringValue:LF(@"(%ld experiments selected)", app.nPrecSel)];
    [mean setStringValue:[NSString stringWithFormat:@"%.3f µmol/min", arithMean]];
    [stddev setStringValue:[NSString stringWithFormat:@"%.3f µmol/min", StdDev]];
    
    [self.view.window makeFirstResponder:OKButton];
    
    [OKButton setTitle:L(@"OK")];
    
    [NSApp runModalForWindow: self.view.window];
    
    
}
#pragma GCC diagnostic pop

- (IBAction)helpButtonPressed:(id)sender {
    [self.view.window makeFirstResponder:helpButton];
    NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"prec_res"  inBook:locBookName];
}


- (IBAction)OKButtonPressed:(id)sender
{
    
    [self.view.window orderOut:nil];
    [self.view.window close];
    [NSApp stopModal];
}

#pragma NSTableViewDataSource methods

// Provides the NSStrings for the rows
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    
    el_ExptRun* run = [selArray objectAtIndex:rowIndex];
    
    if (run.sel == TRUE)
        
    {
        
        if (aTableColumn == Column0)
            return [NSString stringWithFormat:@"%ld",run.n];
        else if (aTableColumn == Column1)
            return [NSString stringWithFormat:@"%.3g",run.vol];
        else if (aTableColumn == Column2)
            return [NSString stringWithFormat:@"%.1f",run.pH];
        else if (aTableColumn == Column3)
            return [NSString stringWithFormat:@"%.3g",run.s];
        else if (aTableColumn == Column4)
            return [NSString stringWithFormat:@"%.3g",run.i];
        else if (aTableColumn == Column5)
        {
            if (run.v <=0.0)
            {
                if (run.v == V_TOOFAST)
                    return L(@"[TOO FAST]");
                else if (run.v == V_TOOSLOW)
                    return L(@"[TOO SLOW]");
                else if (run.v == V_SPLAT)
                    return L(@"[SPLAT!]");
                else if (run.v == V_NA)
                    return L(@"[N/A]");
                else
                    return L(@"[ ? ]");
            }
            else
                return [NSString stringWithFormat:@"%.2f",run.v];
        }
        else
            return Nil;
    }
    else
        return Nil;
}

// Number of rows in the NSTable (The number of columns is set when the NSTable is created).
- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return app.nPrecSel;
    
}

#pragma NSTableViewDelegate method

- (BOOL)tableView:(NSTableView *)tableView shouldEditTableColumn:(NSTableColumn *)tableColumn
              row:(NSInteger)row
{
    // Don't allow any cells to be editable
    return NO;
}

- (BOOL)tableView:(NSTableView *)aTableView shouldSelectRow:(NSInteger)rowIndex
{
    return NO;
}

@end
