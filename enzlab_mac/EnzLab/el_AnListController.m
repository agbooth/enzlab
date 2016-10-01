//
//  el_AnListController.m
//  EnzLab
//
//  Created by Andrew Booth on 27/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_AnListController.h"
#import "el_AnList.h"
#import "el_Limits.h"
#import "el_AppDelegate.h"
#import "L.h"
#import <WebKit/WebKit.h>
@interface el_AnListController ()

@end

@implementation el_AnListController

@synthesize scrollView, tableView, helpButton, printButton, OKButton;
@synthesize Column0, Column1, Column2, Column3, Column4, Column5;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
- (void) showDialog
{
    if ([[NSBundle mainBundle] respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]) {
        // We're running on Mountain Lion or higher
        [[NSBundle mainBundle] loadNibNamed:@"el_AnListController"
                                      owner:self
                            topLevelObjects:nil];
    } else {
        // We're running on Lion
        [NSBundle loadNibNamed:@"el_AnListController"
                         owner:self];
    }
    
    [self.view.window makeFirstResponder:OKButton];
    
    [OKButton setTitle:L(@"OK")];
    
    [NSApp runModalForWindow: self.view.window];
    
    
}
#pragma GCC diagnostic pop

- (NSString*) HTMLContents
{
    NSMutableString* contents = [NSMutableString stringWithFormat:@"%@",@"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">"];
    [contents appendString:@"<html>"];
    [contents appendString:@"<head>"];
    [contents appendString:@"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"];
    [contents appendString:@"<title>Untitled Document</title>"];
    [contents appendString:@"</head>"];
    
    [contents appendString:@"<body>"];
    [contents appendString:@"<font face=\"Arial, Helvetica, sans-serif\" size=\"2\">"];
    [contents appendString:@"<table width=\"450\" border=\"0\">"];
    [contents appendString:@"<tr>"];
    [contents appendString:@"<th width=\"15%\" scope=\"col\">No</th>"];
    [contents appendString:@"<th width=\"15%\" scope=\"col\">Pts</th>"];
    [contents appendString:@"<th width=\"15%\" scope=\"col\">pH</th>"];
    [contents appendString:@"<th width=\"15%\" scope=\"col\">I&nbsp;(mM)</th>"];
    [contents appendString:@"<th width=\"15%\" scope=\"col\">Km&nbsp;(mM)</th>"];
    [contents appendString:@"<th width=\"15%\" scope=\"col\">Vmax;&nbsp;(µmol/min)</th>"];
    [contents appendString:@"</tr>"];
    
    for (int i = 0; i< app.anlist.count; i++)
    {
        el_AnList* an = [app.anlist objectAtIndex:i];
        [contents appendString:@"<tr>"];
        
        [contents appendString:@"<td><div align=\"center\">"];
        [contents appendString:[NSString stringWithFormat:@"%ld",an.n]];
        [contents appendString:@"</div></td>"];
        
        [contents appendString:@"<td><div align=\"center\">"];
        [contents appendString:[NSString stringWithFormat:@"%ld",an.nResults]];
        [contents appendString:@"</div></td>"];
        
        [contents appendString:@"<td><div align=\"center\">"];
        [contents appendString:[NSString stringWithFormat:@"%.1f",an.pH]];
        [contents appendString:@"</div></td>"];
        
        [contents appendString:@"<td><div align=\"center\">"];
        [contents appendString:[NSString stringWithFormat:@"%.3g",an.i]];
        [contents appendString:@"</div></td>"];
        
        [contents appendString:@"<td><div align=\"center\">"];
        [contents appendString:[NSString stringWithFormat:@"%.3g&#xb1;%.3g",an.Km,an.cKm]];
        [contents appendString:@"</div></td>"];
        
        [contents appendString:@"<td><div align=\"center\">"];
        [contents appendString:[NSString stringWithFormat:@"%.3g&#xb1;%.3g",an.Vmax,an.cVmax]];
        [contents appendString:@"</div></td>"];
        
        [contents appendString:@"</tr>"];
    }
    [contents appendString:@"</table>"];
    [contents appendString:@"</font>"];
    [contents appendString:@"</body>"];
    [contents appendString:@"</html>"];
    
    return contents;
}

- (void) print
{
    // Get Print Info
    NSPrintInfo *printInfo = [NSPrintInfo sharedPrintInfo];
    
    // Printing via a WebView
    NSRect textRect = NSMakeRect(0,0,500,600);
    WebView *webView = [[WebView alloc] initWithFrame:textRect];
    [[webView mainFrame] loadHTMLString: [self HTMLContents] baseURL:Nil];
    NSPrintOperation *textPrint = [NSPrintOperation printOperationWithView:webView printInfo:printInfo];
    [textPrint setCanSpawnSeparateThread:YES];
    [textPrint runOperation];
}




- (IBAction)helpButtonPressed:(id)sender {
    [self.view.window makeFirstResponder:helpButton];
    NSString *locBookName = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleHelpBookName"];
    [[NSHelpManager sharedHelpManager] openHelpAnchor:@"anlist"  inBook:locBookName];

}

- (IBAction)printButtonPressed:(id)sender {
    
    [self print];
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
    
    el_AnList* an = [app.anlist objectAtIndex:rowIndex];
    //return [self MakeExptLine:[app.list objectAtIndex:rowIndex]];
    if (aTableColumn == Column0)
        return [NSString stringWithFormat:@"%ld",an.n];
    else if (aTableColumn == Column1)
        return [NSString stringWithFormat:@"%ld",an.nResults];
    else if (aTableColumn == Column2)
        return [NSString stringWithFormat:@"%.1f",an.pH];
    else if (aTableColumn == Column3)
        return [NSString stringWithFormat:@"%.3g",an.i];
    else if (aTableColumn == Column4)
        return [NSString stringWithFormat:@"%.3g±%.3g",an.Km,an.cKm];
    else if (aTableColumn == Column5)
        return [NSString stringWithFormat:@"%.3g±%.3g",an.Vmax,an.cVmax];
    else
        return Nil;
}

// Number of rows in the NSTable (The number of columns is set when the NSTable is created).
- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return app.anlist.count;
    
}

#pragma NSTableViewDelegate method

- (BOOL)tableView:(NSTableView *)tableView shouldEditTableColumn:(NSTableColumn *)tableColumn
              row:(NSInteger)row
{
    // Don't allow any cells to be editable
    return NO;
}
@end
