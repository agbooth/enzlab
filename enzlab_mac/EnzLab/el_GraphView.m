//
//  el_GraphView.m
//  EnzLab
//
//  Created by Andrew Booth on 15/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_GraphView.h"
#import "el_ExptRun.h"
#import "el_Limits.h"
#import "el_AppDelegate.h"


#define ALIGN_CENTRE 0
#define ALIGN_LEFT   1
#define ALIGN_RIGHT  2

@implementation el_GraphView

@synthesize delay;
@synthesize inc;
@synthesize xHeight;

el_AppDelegate* app;
el_ExptRun* listtail;

static Boolean bPrevFail = FALSE;

float xChar, yChar, xSpace, ySpace, xLabel, yLabel, yHeight, xPos, yPos;
float xPixels, yPixels;


- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        app = (el_AppDelegate *)[[NSApplication sharedApplication] delegate];
    }
    return self;
}


- (Boolean) SameConditions: (el_ExptRun*) testrun
{
    
    return ((testrun.vol == listtail.vol) &&
            (testrun.pH == listtail.pH) &&
            (testrun.s == listtail.s) &&
            (testrun.i == listtail.i));
}


- (void) create_text_at_point:(CGContextRef) con text: (NSString*) text X: (float)X Y:(float) Y angle: (float) angle alignment: (NSInteger) alignment
{
    CGContextSaveGState(con);
    
    // Set text rotation
    CGContextSetTextMatrix (con, CGAffineTransformRotate(CGAffineTransformScale(CGAffineTransformIdentity, 1.0, -1.0 ), angle));
    
    // CGContextShowTextAtPoint only works with C strings encoded with MacOSRoman, not UTF8 or UTF16
    // so the text has to be transcoded.
    NSData *strData = [text dataUsingEncoding:NSMacOSRomanStringEncoding allowLossyConversion:YES];
    char string[[strData length] + 1];
    memcpy(string, [strData bytes], [strData length] + 1);
    string[[strData length]] = '\0';
    
    // set the font
    NSFont* font = [NSFont fontWithName:@"Helvetica" size:16];
    
    const char* fontName = [font.fontName UTF8String];
    
    CGContextSelectFont( con, fontName, 14, kCGEncodingMacRoman);
    
    CGContextSetStrokeColorWithColor(con, [NSColor blackColor].CGColor);
    CGContextSetFillColorWithColor(con, [NSColor blackColor].CGColor);
    
    // work out the size of the rectangle bounding the text
    NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:
                                [NSFont fontWithName:@"Helvetica" size:14], NSFontAttributeName,
                                [NSParagraphStyle defaultParagraphStyle], NSParagraphStyleAttributeName,
                                nil];
    CGSize expectedSize = [text sizeWithAttributes:attributes];
    
    if (angle == 0.0)
    {
        if (alignment == ALIGN_CENTRE)
        {
            X -= expectedSize.width/2.0;
            Y += expectedSize.height;
        }
        if (alignment == ALIGN_RIGHT)
        {
            X -= expectedSize.width;
            Y += expectedSize.height/4.0;
        }
    }
    else
    {
        Y += expectedSize.width/2.0;
        X -= expectedSize.height;
    }
    
    CGContextShowTextAtPoint(con, X, Y, string, strlen(string));
    
    CGContextRestoreGState(con);
}
/*
- (void)drawRect:(NSRect)dirtyRect
{
    

    

    double maxy;
    int i;
    
    double	TimeInt, gradient;
    static double oldmaxy;
    
    NSString* buffer;
    NSInteger multiLine;
    NSInteger index;
    clock_t	clicks;
    
    el_ExptRun* thisrun;
    
    [super drawRect:dirtyRect];
   
 
    if (app.list.count ==0)
        return;
    
    CGContextRef con = [[NSGraphicsContext currentContext] graphicsPort];
    CGContextSaveGState(con);
    
    // Flip the context vertically
    CGContextTranslateCTM(con, 0, self.frame.size.height);
    CGContextScaleCTM(con, 1.0, -1.0);
    
    
    
    
    listtail = [app.list lastObject];

   // if ((delay >= 0.0) || (listtail.v <= 0.0 ) || bPrevFail)
    {
        NSBezierPath* path = [NSBezierPath bezierPathWithRect:self.bounds];
        [[NSColor whiteColor] setFill];
        [path fill];
    }

    // draw graph if current expt was a success
	if (listtail.v > 0.0)
    {
		// select scale
		if (listtail.v < GRAPH_SCALE * GRAPH_A3)
			maxy = GRAPH_A3;
		else if (listtail.v > GRAPH_SCALE * GRAPH_A2)
			maxy = GRAPH_A1;
		else
	    	maxy = GRAPH_A2;
        
		if (oldmaxy == 0.0)
			oldmaxy = maxy;
        
		if (delay > 0.0) //it's under new conditions
		{
			oldmaxy = maxy;
		}
		else
			maxy = oldmaxy; //it's a repeat - must not change the scale

        // get size intervals
        
        
        
        
        
        [self getXHeight];
      
        
        
        // axes
        CGContextSetStrokeColorWithColor(con, [NSColor blackColor].CGColor);
        CGContextSetLineWidth(con, 1.0);
        
        CGContextMoveToPoint(con, xLabel, yPixels - yLabel);
        CGContextAddLineToPoint(con, xLabel + xHeight, yPixels - yLabel);
        CGContextMoveToPoint(con, xLabel, yPixels - yLabel);
        CGContextAddLineToPoint(con, xLabel, ySpace);
        CGContextStrokePath(con);
        
        xPos = 0.0; // Just to keep the compiler happy!
        
        // tic labels : x-axis (time)
        for (i=0; i < GRAPH_XTICS; i++)
        {
            xPos = ((i * xHeight)/(GRAPH_XTICS - 1)) + xLabel;
            CGContextMoveToPoint( con, xPos, yPixels - yLabel);
			yPos = yPixels - yLabel + yChar;
			CGContextAddLineToPoint(con, xPos, yPos );
            CGContextStrokePath(con);
            buffer = [NSString stringWithFormat:@"%.3g min",((float)i * GRAPH_TIME)/ (GRAPH_XTICS - 1)];
            [self create_text_at_point:con text: buffer X: xPos Y:yPos angle:0.0 alignment: ALIGN_CENTRE];
        }
        
         // tic labels : y-axis (rate, v)
        for (i = 0; i < GRAPH_YTICS; i++)
        {
			if (i==2) continue;
			yPos = yPixels - ((i * yHeight)/(GRAPH_YTICS - 1)) - yLabel;
			CGContextMoveToPoint( con, xLabel, yPos );
			xPos = xLabel - xChar;
			CGContextAddLineToPoint(con, xPos, yPos );
            CGContextStrokePath(con);
            buffer = [NSString stringWithFormat:@"%.2g", ((float)i * maxy) / (GRAPH_YTICS - 1)];
            if (i==0)
                buffer = @"0";
            if (i==4)
                buffer = [NSString stringWithFormat:@"%.1f", ((float)i * maxy) / (GRAPH_YTICS - 1)];
            [self create_text_at_point:con text: buffer X: xPos-xChar Y:yPos angle:0.0 alignment: ALIGN_RIGHT];
        }
        
        yPos = yPixels - ((2 * yHeight)/(GRAPH_YTICS - 1)) - yLabel;
        [self create_text_at_point:con text: @"Absorbance" X: xPos Y:yPos angle:M_PI/2.0 alignment: ALIGN_CENTRE];
        
        multiLine = -1;
        
        // Draw the previous runs
        for (index = 0; index < app.list.count-1; index++)
        {
           
            CGContextSetStrokeColorWithColor(con, [NSColor grayColor].CGColor);
            
            thisrun = [app.list objectAtIndex:index];
            
            if ([self SameConditions:thisrun] == FALSE) break;
           
            
            multiLine++;
            
            gradient = ((yHeight * thisrun.v) / (GRAPH_SCALE * maxy)) / xHeight;
    
            if (gradient > 0.0) // don't try to plot a splat
			{
                    
                xPos = xLabel + xHeight;
                yPos = (yPixels - yLabel - (xHeight * gradient));
                CGContextMoveToPoint( con, xLabel, yPixels - yLabel );
                CGContextAddLineToPoint(con, xPos, yPos );
                CGContextStrokePath(con);
   
			}
        }
        
        // Draw this run
        CGContextSetStrokeColorWithColor(con, [NSColor redColor].CGColor);
        thisrun = [app.list lastObject];
        gradient = ((yHeight * thisrun.v) / (GRAPH_SCALE * maxy)) / xHeight;
        if (gradient > 0.0) // don't try to plot a splat
        {
            if (delay == 0)
                inc = xHeight;
            xPos = xLabel + inc;
            yPos = (yPixels - yLabel - (inc * gradient));
            CGContextMoveToPoint( con, xLabel, yPixels - yLabel );
            CGContextAddLineToPoint(con, xPos, yPos );
            CGContextStrokePath(con);
            
        }
        
    }
    
    CGContextRestoreGState(con);
 
}
*/
- (float) getXHeight
{
    // work out the text metrics
    NSString* test = @"X";  // An 'average character
    NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:
                                [NSFont fontWithName:@"Helvetica" size:14], NSFontAttributeName,
                                [NSParagraphStyle defaultParagraphStyle], NSParagraphStyleAttributeName,
                                nil];
    
    CGSize expectedSize = [test sizeWithAttributes:attributes];

    // These are all instance variables
    xChar = expectedSize.width;
    yChar = expectedSize.height;
    
    xSpace = GRAPH_XSPACE * xChar;
    ySpace = GRAPH_YSPACE * yChar;
    
    xLabel = GRAPH_LEFTCHARS * xChar;
    yLabel = GRAPH_BOTTOMCHARS * yChar;
    
    xPixels = self.frame.size.width;
    yPixels = self.frame.size.height;
    
    xHeight = xPixels - xLabel - xSpace;
    yHeight = yPixels - yLabel - ySpace;
    
    return xHeight;
}

@end
