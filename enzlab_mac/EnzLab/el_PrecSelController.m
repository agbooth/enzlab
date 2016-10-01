//
//  el_PrecSelController.m
//  EnzLab
//
//  Created by Andrew Booth on 22/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_PrecSelController.h"
#import "el_AppDelegate.h"
#import "el_ExptRun.h"
#import "el_Limits.h"
#import "L.h"

@interface el_PrecSelController ()

@end

@implementation el_PrecSelController

@synthesize selVol, selpH, selS, selI,  result;
@synthesize Vol_box, pH_box, S_box, I_box, nMatched;
@synthesize scrollView, tableView, helpButton, cancelButton, OKButton;
@synthesize Column0, Column1, Column2, Column3, Column4, Column5;

- (void) PrecSelFillBoxes: (el_ExptRun*) run
{
    selVol = run.vol;
    [self.Vol_box setStringValue:[NSString stringWithFormat:@"%g",selVol]];
    selpH = run.pH;
    [self.pH_box setStringValue:[NSString stringWithFormat:@"%.1f",selpH]];
    selS = run.s;
    [self.S_box setStringValue:[NSString stringWithFormat:@"%g",selS]];
    selI = run.i;
    [self.I_box setStringValue:[NSString stringWithFormat:@"%g",selI]];
    
    [self PrecSelHighLighting];
}

- (void) PrecSelHighLighting
{
    app.nPrecSel = 0;
    NSMutableIndexSet* indexSet = [[NSMutableIndexSet alloc] init];
    
    for (int i = 0; i < app.list.count; i++)
    {
        [tableView deselectRow:i];
        el_ExptRun* run = [app.list objectAtIndex:i];
        run.sel = FALSE;
        if ((run.vol == selVol) &&
            (run.pH == selpH) &&
            (run.s == selS) &&
            (run.i == selI) &&
            (run.v > 0.0))
        {
            run.sel = TRUE;
            [indexSet addIndex:i];
            app.nPrecSel++;
        }
    }
    [tableView selectRowIndexes:indexSet byExtendingSelection:TRUE];
    [nMatched setIntegerValue:app.nPrecSel];
    
    [OKButton setEnabled: (app.nPrecSel >= PREC_MIN)];
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_PrecSelController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_PrecSelController"
                         owner:self];
    }
    
    [self PrecSelFillBoxes: [app.list lastObject]];
    [self.view.window makeFirstResponder:tableView];
    
    [OKButton setTitle:L(@"OK")];
    
    [NSApp runModalForWindow: self.view.window];
    
    
}
#pragma GCC diagnostic pop

// NSTextFieldDelegate method
- (void)controlTextDidChange:(NSNotification *)aNotification
{
 //   NSString* text = [aNotification.object stringValue];
    
    selVol = [Vol_box floatValue];
    selpH = [pH_box floatValue];
    selS = [S_box floatValue];
    selI = [I_box floatValue];
    
    [self PrecSelHighLighting];

}


- (IBAction)TableRowSelected:(id)sender {
    
    NSInteger rowIndex = [tableView selectedRow];
    
    if (rowIndex >= app.list.count)
        return;   // avoid range exception
    
    el_ExptRun* run = [app.list objectAtIndex:rowIndex];
    [self PrecSelFillBoxes:run];
    
}

- (IBAction)helpButtonPressed:(id)sender {
    [self.view.window makeFirstResponder:helpButton];
    NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"prec_sel"  inBook:locBookName];
}

- (IBAction)cancelButtonPressed:(id)sender
{
    el_ExptRun* run;
    
    for (int i = 0; i <app.list.count; i++)
    {
        run = [app.list objectAtIndex:i];
        run.sel = FALSE;
    }
    
    [self.view.window orderOut:nil];
    [self.view.window close];
    [NSApp stopModal];
    
    result = FALSE;
    
}

- (IBAction)OKButtonPressed:(id)sender
{
    el_ExptRun* run;
    
    app.nPrecSel = 0;
    
    for (int i = 0; i <app.list.count; i++)
    {
        run = [app.list objectAtIndex:i];
    //    run.sel = [tableView isRowSelected:i];
        if (run.sel)
            app.nPrecSel++;
    }
    
    [self.view.window orderOut:nil];
    [self.view.window close];
    [NSApp stopModal];
    
    result = TRUE;
}


#pragma NSTableViewDataSource methods

// Provides the NSStrings for the rows
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    
    el_ExptRun* run = [app.list objectAtIndex:rowIndex];
    
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

// Number of rows in the NSTable (The number of columns is set when the NSTable is created).
- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return app.list.count;
    
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
    el_ExptRun* run = [app.list objectAtIndex:rowIndex];
    return (run.v > 0.0);
}

@end
