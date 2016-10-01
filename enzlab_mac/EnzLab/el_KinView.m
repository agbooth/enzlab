//
//  el_KinView.m
//  EnzLab
//
//  Created by Andrew Booth on 24/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_KinView.h"
#import "el_Limits.h"
#import "el_Graph.h"
#import "el_AppDelegate.h"
#import "L.h"
#import <QuartzCore/QuartzCore.h>

#define ALIGN_CENTRE 0
#define ALIGN_LEFT   1
#define ALIGN_RIGHT  2

@implementation el_KinView

static float xChar, yChar, xSpace, ySpace, xLabel, yLabel, xHeight, yHeight, xPos, yPos;
static float xPixels, yPixels, sXPixels, sYPixels;

@synthesize printing;

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        printing = FALSE;
    }
    return self;
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
    NSFont* font = [NSFont fontWithName:@"Helvetica" size:14];
    
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



- (void)drawRect:(NSRect)dirtyRect
{
    float maxs, maxv, s[BUFLEN], v[BUFLEN], x, y;
    NSInteger nPts, crosslen;
    
    [[NSColor whiteColor] setFill];
    NSRectFill(dirtyRect);
    [super drawRect:dirtyRect];
    
    [self setVariables];
    CGContextRef con = [[NSGraphicsContext currentContext] graphicsPort];
    
    CGContextSaveGState(con);
    
    // Flip the context vertically
    CGContextTranslateCTM(con, 0, self.frame.size.height);
    CGContextScaleCTM(con, 1.0, -1.0);
    
    self.layer.backgroundColor = [NSColor whiteColor].CGColor;
  
    if (app.nOutliers == 0)
    {
        [self create_text_at_point:con text: L(@"Results obtained from nonlinear least squares regression:") X: xPixels/2.0 Y: yPixels - (4.0 * yChar) angle: (float) 0.0 alignment: ALIGN_CENTRE];
        
        [self create_text_at_point:con text: L(@"(with 95% confidence limits)") X: xPixels/2.0 Y: yPixels - (3.0 * yChar) angle: (float) 0.0 alignment: ALIGN_CENTRE];
        
        [self create_text_at_point:con text: LF(@"Km = %.3g ± %.3g    Vmax = %.3g ± %.3g  (at pH %g) ", app.Km, app.cKm, app.Vmax, app.cVmax, app.rpH) X: xPixels/2.0 Y: yPixels-(1.5 * yChar) angle: (float) 0.0 alignment: ALIGN_CENTRE];
    }
    else if (app.nOutliers == 1)
    {
        [self create_text_at_point:con text: LF(@"One of the points you used in this analysis was obtained at more than %gKm.",KM_MAX) X: xPixels/2.0 Y: yPixels - (4.0 * yChar) angle: (float) 0.0 alignment: ALIGN_CENTRE];
        [self create_text_at_point:con text: L(@"This point gives little information about your enzyme and will henceforward be labelled 'N/A'.") X: xPixels/2.0 Y: yPixels - (3.0 * yChar) angle: (float) 0.0 alignment: ALIGN_CENTRE];
        
        [self create_text_at_point:con text: L(@"You should now repeat the kinetic analysis.") X: xPixels/2.0 Y: yPixels-(1.5 * yChar) angle: (float) 0.0 alignment: ALIGN_CENTRE];
    }
    else
    {
        [self create_text_at_point:con text: LF(@"%ld of the points you used in this analysis were obtained at more than %gKm.",app.nOutliers, KM_MAX) X: xPixels/2.0 Y: yPixels - (4.0 * yChar) angle: (float) 0.0 alignment: ALIGN_CENTRE];
        [self create_text_at_point:con text: L(@"These points give little information about your enzyme and will henceforward be labelled 'N/A'.") X: xPixels/2.0 Y: yPixels - (3.0 * yChar) angle: (float) 0.0 alignment: ALIGN_CENTRE];
        
        [self create_text_at_point:con text: L(@"You should now repeat the kinetic analysis.") X: xPixels/2.0 Y: yPixels-(1.5 * yChar) angle: (float) 0.0 alignment: ALIGN_CENTRE];
    }
  
  
    // write labels
    [self create_text_at_point:con text: L(@"Initial Rate Against Substrate Concentration") X: xPixels/2.0 Y: 0.0 angle: (float) 0.0 alignment: ALIGN_CENTRE];
    [self create_text_at_point:con text: LF(@"for enzyme %@",app.EnzChar.ref) X: xPixels/2.0 Y: yChar angle: (float) 0.0 alignment: ALIGN_CENTRE];
   
    if (printing)
        [self create_text_at_point:con text: @"Vo  (µmol/min)" X: (XLEFTGAP - 5*XTICLENGTH) * xPixels Y: yPixels/2.0 angle: (float) M_PI/2.0 alignment: ALIGN_CENTRE];  // Replaced with Label - so can use Unicode. See el_KinResController.
    
    [self create_text_at_point:con text: @"S (mM)" X: xPixels/2.0 Y: sYPixels angle: (float) 0.0 alignment: ALIGN_CENTRE];
    
    // axes
    CGContextSetStrokeColorWithColor(con, [NSColor blackColor].CGColor);
    CGContextSetLineWidth(con, 1.0);
    
    CGContextMoveToPoint(con, XLEFTGAP * sXPixels, (1.0 - YBOTTOMGAP)*sYPixels);
    CGContextAddLineToPoint(con, (XLEFTGAP + XMAIN)* sXPixels,(1.0 - YBOTTOMGAP)*sYPixels);
    CGContextMoveToPoint(con, XLEFTGAP * sXPixels, (1.0 - YBOTTOMGAP)*sYPixels);
    CGContextAddLineToPoint(con, XLEFTGAP * sXPixels, YTOPGAP * sYPixels);
    CGContextStrokePath(con);
    
    // scan the data and find biggest values
	maxs = maxv = 0.0;
    nPts = 0;
    
    for (int i = 0; i < app.list.count; i++ )
    {
        el_ExptRun* run = [app.list objectAtIndex:i];
        
        if (run.sel == TRUE)
        {
            s[nPts] = run.s;
            v[nPts] = run.v;
            if (run.s > maxs)
				maxs = run.s;
            if (run.v > maxv)
				maxv = run.v;
            nPts++;
		}
    }
    
    maxv = (float)ceil( maxv);
    
    // tic labels
    
    for (int i = 0; i < XTICS; i++)
    {
		xPos = ((i * XMAIN)/(XTICS - 1) + XLEFTGAP) * sXPixels;
		CGContextMoveToPoint(con, xPos, (1.0 - YBOTTOMGAP) * sYPixels);
        yPos = (1.0 - YBOTTOMGAP + YTICLENGTH) * sYPixels;
		CGContextAddLineToPoint(con, xPos, yPos );
        CGContextStrokePath(con);
        
		[self create_text_at_point:con text: [NSString stringWithFormat:@"%.4g",((float)i / (XTICS - 1)) * maxs] X: xPos Y: yPos angle: (float) 0.0 alignment: ALIGN_CENTRE];
	}
    
    for (int i = 1; i < YTICS; i++)
    {
		yPos = (1.0 - ((i * YMAIN)/(YTICS - 1) + YBOTTOMGAP)) * sYPixels;
		CGContextMoveToPoint(con, XLEFTGAP * sXPixels, yPos);
        xPos = (XLEFTGAP - (XTICLENGTH/2)) * sXPixels;
		CGContextAddLineToPoint(con, xPos, yPos );
        CGContextStrokePath(con);
        
        [self create_text_at_point:con text: [NSString stringWithFormat:@"%.1f",(float)i / (YTICS - 1) * maxv] X: xPos-(1.5 * xChar) Y: yPos-(yChar/1.25) angle: (float) 0.0 alignment: ALIGN_CENTRE];
	}
    
    // the smooth curve with our values of Km and Vmax
	xPos = (int)(XMAIN * sXPixels);
    CGContextMoveToPoint(con, XLEFTGAP * sXPixels, (1.0 - YBOTTOMGAP) * sYPixels);
	for (int i = 0; i < xPos; i += 2)
    {
		x = (i / (XMAIN * sXPixels)) * maxs;
		y = (app.Vmax * x) / (app.Km + x);
		CGContextAddLineToPoint(con, i + (XLEFTGAP * sXPixels), (1.0 - ((y / maxv)*YMAIN + YBOTTOMGAP)) * sYPixels );
	}
    CGContextStrokePath(con);
    
    // and our experimental points upon it
	for (int i = 0; i < nPts; i++) {
		xPos = ((s[i] / maxs) * XMAIN + XLEFTGAP) * sXPixels;
		yPos = (1.0 - ((v[i] / maxv) * YMAIN + YBOTTOMGAP)) * sYPixels;
        crosslen = YTICLENGTH * yPixels / 2;
		CGContextMoveToPoint(con, xPos - crosslen, yPos + crosslen);
		CGContextAddLineToPoint(con, xPos + crosslen, yPos - crosslen );
        CGContextStrokePath(con);
		CGContextMoveToPoint(con, xPos - crosslen, yPos - crosslen );
		CGContextAddLineToPoint(con, xPos + crosslen, yPos + crosslen );
        CGContextStrokePath(con);
	}
    
    // plus a line at 50Km if appropriate
	if (app.nOutliers != 0)
    {
		xPos = (((KM_MAX * app.Km) / maxs) * XMAIN + XLEFTGAP) * sXPixels;
		//hPen = CreatePen( PS_DOT, 1, RGB( 0, 0, 0 ) );
        
        const CGFloat dashPattern[2] = {2,2};
        
        CGContextSetLineDash(con, 0, dashPattern, 2); // set dashed line with 2 pixels on, 2 pixels off
        
		CGContextMoveToPoint(con, xPos, (1.0 - YBOTTOMGAP)*sYPixels);
		CGContextAddLineToPoint(con, xPos, YTOPGAP * sYPixels );
		CGContextStrokePath(con);
		//swprintf( buffer, L"%gKm", KM_MAX );
		//SetTextAlign( hDC, TA_LEFT | TA_BASELINE );
	    //TextOut( hDC, xPos, (YMAIN/2.0 + YBOTTOMGAP) * sYPixels, buffer, lstrlen( buffer ) );
        [self create_text_at_point:con text: [NSString stringWithFormat:@"%gKm",KM_MAX] X: xPos Y: (YMAIN/2.0 + YBOTTOMGAP) * sYPixels angle: (float) 0.0 alignment: ALIGN_CENTRE];
	}
    
    
    
    CGContextRestoreGState(con);
}

- (void) setVariables
{
    // work out the text metrics
    NSString* test = @"X";  // An 'average' character
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
    
    sXPixels = xPixels * 0.8;
    sYPixels = yPixels * 0.8;
    
    xHeight = xPixels - xLabel - xSpace;
    yHeight = yPixels - yLabel - ySpace;
    
}

@end
