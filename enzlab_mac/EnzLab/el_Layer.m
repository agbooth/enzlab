//
//  el_Layer.m
//  EnzLab
//
//  Created by Andrew Booth on 17/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_Layer.h"
#import "el_Limits.h"
#import "el_AppDelegate.h"
#import "L.h"
#import <QuartzCore/QuartzCore.h>

#define ALIGN_CENTRE 0
#define ALIGN_LEFT   1
#define ALIGN_RIGHT  2




@implementation el_Layer

@dynamic strokeEnd;
@synthesize delay;
@synthesize xHeight;

el_ExptRun* listtail;


static float xChar, yChar, xSpace, ySpace, xLabel, yLabel, yHeight, xPos, yPos;
static float xPixels, yPixels;

static Boolean bPrevFail = FALSE;

double	gradient;
double maxy;
static double oldmaxy;

- initWithLayer:(id)layer
{
    self = [super initWithLayer:layer];
    if (self) {
        if ([layer isKindOfClass:[el_Layer class]])
        {
            self.strokeEnd = [(el_Layer *)layer strokeEnd];
            self.delay = [(el_Layer *)layer delay];
            
        }
    }
    return self;
}

+ (BOOL)needsDisplayForKey:(NSString*)key {
    if ([key isEqualToString:@"strokeEnd"]) {
        return YES;
    } else {
        return [super needsDisplayForKey:key];
    }
}


- (void) SetVariables
{
    // select scale
    if (listtail.v < GRAPH_SCALE * GRAPH_A4)
        maxy = GRAPH_A4;
    else if (listtail.v < GRAPH_SCALE * GRAPH_A3)
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
    
    [self getXHeight];
    
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


-(void)drawInContext:(CGContextRef)ctx {
    
    
    int i;
    

    
    NSString* buffer;
    NSInteger multiLine;
    NSInteger index;

    
    el_ExptRun* thisrun;
    
    
    if (app.list.count ==0)
        return;
    
    CGContextSaveGState(ctx);
    
    // Flip the context vertically
    CGContextTranslateCTM(ctx, 0, self.frame.size.height);
    CGContextScaleCTM(ctx, 1.0, -1.0);
    
    listtail = [app.list lastObject];
    
    self.backgroundColor = [NSColor whiteColor].CGColor;
    
    if ((delay >= 0.0) || (listtail.v <= 0.0 ) || bPrevFail)
    {
        // Clear the drawing area
        CGRect rect = CGContextGetClipBoundingBox(ctx);
        CGContextSetFillColorWithColor(ctx, [NSColor whiteColor].CGColor);
        CGContextFillRect(ctx, rect);
    }
    
    // draw graph if current expt was a success
	if (listtail.v > 0.0)
    {
		[self SetVariables];
        
        // axes
        CGContextSetStrokeColorWithColor(ctx, [NSColor blackColor].CGColor);
        CGContextSetLineWidth(ctx, 1.0);
        
        CGContextMoveToPoint(ctx, xLabel, yPixels - yLabel);
        CGContextAddLineToPoint(ctx, xLabel + xHeight, yPixels - yLabel);
        CGContextMoveToPoint(ctx, xLabel, yPixels - yLabel);
        CGContextAddLineToPoint(ctx, xLabel, ySpace);
        CGContextStrokePath(ctx);
        
        xPos = 0.0; // Just to keep the compiler happy!
        
        // tic labels : x-axis (time)
        for (i=0; i < GRAPH_XTICS; i++)
        {
            xPos = ((i * xHeight)/(GRAPH_XTICS - 1)) + xLabel;
            CGContextMoveToPoint( ctx, xPos, yPixels - yLabel);
			yPos = yPixels - yLabel + yChar;
			CGContextAddLineToPoint(ctx, xPos, yPos );
            CGContextStrokePath(ctx);
            buffer = [NSString stringWithFormat:@"%.3g min",((float)i * GRAPH_TIME)/ (GRAPH_XTICS - 1)];
            [self create_text_at_point:ctx text: buffer X: xPos Y:yPos angle:0.0 alignment: ALIGN_CENTRE];
        }
        
        // tic labels : y-axis (rate, v)
        for (i = 0; i < GRAPH_YTICS; i++)
        {
			if (i==2) continue;
			yPos = yPixels - ((i * yHeight)/(GRAPH_YTICS - 1)) - yLabel;
			CGContextMoveToPoint( ctx, xLabel, yPos );
			xPos = xLabel - xChar;
			CGContextAddLineToPoint(ctx, xPos, yPos );
            CGContextStrokePath(ctx);
            buffer = [NSString stringWithFormat:@"%.2g", ((float)i * maxy) / (GRAPH_YTICS - 1)];
            if (i==0)
                buffer = @"0";
            if (i==4)
                buffer = [NSString stringWithFormat:@"%.1f", ((float)i * maxy) / (GRAPH_YTICS - 1)];
            [self create_text_at_point:ctx text: buffer X: xPos-xChar Y:yPos angle:0.0 alignment: ALIGN_RIGHT];
        }
        
        yPos = yPixels - ((2 * yHeight)/(GRAPH_YTICS - 1)) - yLabel;
        [self create_text_at_point:ctx text: L(@"Absorbance") X: xPos Y:yPos angle:M_PI/2.0 alignment: ALIGN_CENTRE];
        
        multiLine = -1;
        
        // Draw the previous runs
        
        if (app.list.count > 1)
        {
            for (index = app.list.count-2; index >= 0; index--)
            {
                
                CGContextSetStrokeColorWithColor(ctx, [NSColor blackColor].CGColor);
                
                thisrun = [app.list objectAtIndex:index];
                
                if (thisrun.v <= 0) continue; // skip splats etc.
                
                if ([self SameConditions:thisrun] == FALSE) break;
                
                
                multiLine++;
                
                gradient = ((yHeight * thisrun.v) / (GRAPH_SCALE * maxy)) / xHeight;
                
                if (gradient > 0.0) // don't try to plot a splat
                {
                    
                    xPos = xLabel + xHeight;
                    yPos = (yPixels - yLabel - (xHeight * gradient));
                    CGContextMoveToPoint( ctx, xLabel, yPixels - yLabel );
                    CGContextAddLineToPoint(ctx, xPos, yPos );
                    CGContextStrokePath(ctx);
                    
                }
            }
        }
  
        // Now deal with the current run
        thisrun = [app.list lastObject];
        
        // Work out the path for the animation
        CGMutablePathRef cpath = CGPathCreateMutable();
        gradient = ((yHeight * thisrun.v) / (GRAPH_SCALE * maxy)) / xHeight;
        xPos = xLabel + (xHeight * self.strokeEnd);
        yPos = (yPixels - yLabel - (xHeight * gradient * self.strokeEnd));
        
        CGPathMoveToPoint(cpath, NULL, xLabel, yPixels - yLabel);
        CGPathAddLineToPoint(cpath, NULL, xPos, yPos);
        
        // Store the path in CAShapeLAyer's path property
        self.path = cpath;
        
        if (gradient > 0.0) // don't try to plot a splat
        {
            if (delay == 0.0) // draw the path here as there won't be animated drawing
            {
                CGContextAddPath(ctx, cpath);
                CGContextSetStrokeColorWithColor(ctx, [NSColor blackColor].CGColor);
                CGContextStrokePath(ctx);
                CGPathRelease(cpath);
            }
                   
            
        }
        
        if (multiLine > -1)
            [self create_text_at_point:ctx text: L(@"Latest Experimental Plots") X: xPixels/2.0 Y:yChar angle:0.0 alignment: ALIGN_CENTRE];
        else
            [self create_text_at_point:ctx text: L(@"Latest Experimental Plot") X: xPixels/2.0 Y:yChar angle:0.0 alignment: ALIGN_CENTRE];
        
        bPrevFail = FALSE;
        
    
        // Signal to start animation if needed.
        NSUserNotification *notification = [[NSUserNotification alloc] init];
        notification.title = @"finishedDrawingAxes";
        
        [[NSUserNotificationCenter defaultUserNotificationCenter] deliverNotification:notification];
    }
    else
    {
        [self SetVariables];
        
        if (listtail.v == V_SPLAT)
            [self create_text_at_point:ctx text: L(@"SPLAT!") X: xPixels/2.0 Y:yPixels/2.0 angle:0.0 alignment: ALIGN_CENTRE];
        else if (listtail.v == V_TOOSLOW)
            [self create_text_at_point:ctx text: L(@"TOO SLOW!") X: xPixels/2.0 Y:yPixels/2.0 angle:0.0 alignment: ALIGN_CENTRE];
        else if (listtail.v == V_TOOFAST)
            [self create_text_at_point:ctx text: L(@"TOO FAST!") X: xPixels/2.0 Y:yPixels/2.0 angle:0.0 alignment: ALIGN_CENTRE];
        else if (listtail.v == V_CLEAR)
            [self create_text_at_point:ctx text: @"                     " X: xPixels/2.0 Y:yPixels/2.0 angle:0.0 alignment: ALIGN_CENTRE];
        else
            [self create_text_at_point:ctx text: L(@"[GRAPH UNAVAILABLE]") X: xPixels/2.0 Y:yPixels/2.0 angle:0.0 alignment: ALIGN_CENTRE];
        
        bPrevFail = TRUE;
    }
   
    CGContextRestoreGState(ctx);
    
    
}




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
