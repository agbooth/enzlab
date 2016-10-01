//
//  el_PHPView.m
//  EnzLab
//
//  Created by Andrew Booth on 27/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_PHPView.h"
#import "el_Limits.h"
#import "el_Graph.h"
#import "el_AppDelegate.h"
#import "L.h"
#import <QuartzCore/QuartzCore.h>

#define ALIGN_CENTRE 0
#define ALIGN_LEFT   1
#define ALIGN_RIGHT  2

@implementation el_PHPView

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
    float maxv, pH[BUFLEN], v[BUFLEN];
    NSInteger nPts, crosslen;
    
    float offset;
    
    [[NSColor whiteColor] setFill];
    NSRectFill(dirtyRect);
    [super drawRect:dirtyRect];
    
    [self setVariables];
    offset = sXPixels/10.0;
    CGContextRef con = [[NSGraphicsContext currentContext] graphicsPort];
    
    CGContextSaveGState(con);
    
    // Flip the context vertically
    CGContextTranslateCTM(con, 0, self.frame.size.height);
    CGContextScaleCTM(con, 1.0, -1.0);
    
    self.layer.backgroundColor = [NSColor whiteColor].CGColor;

    // write title
    [self create_text_at_point:con text: LF(@"pH profile of enzyme %@",app.EnzChar.ref) X: xPixels/2.0 Y: 0.0 + offset angle: (float) 0.0 alignment: ALIGN_CENTRE];
    
    if (printing)
        [self create_text_at_point:con text: @"Vo  (µmol/min)" X: (XLEFTGAP - 5*XTICLENGTH) * xPixels Y: yPixels/2.0 + offset angle: (float) M_PI/2.0 alignment: ALIGN_CENTRE];  // Replaced with Label - so can use Unicode. See el_KinPHPController.
    
    [self create_text_at_point:con text: @"pH" X: xPixels/2.0 Y: sYPixels + offset angle: (float) 0.0 alignment: ALIGN_CENTRE];
    
    // scan the data and find biggest value
	maxv = 0.0;
    nPts = 0;
    
    for (int i = 0; i < app.list.count; i++ )
    {
        el_ExptRun* run = [app.list objectAtIndex:i];
        
        if (run.sel == TRUE)
        {
            pH[nPts] = run.pH;
            v[nPts] = run.v;
            
            if (run.v > maxv)
				maxv = run.v;
            nPts++;
		}
    }
    
    maxv = (float)ceil( maxv);
    
    // axes
    CGContextSetStrokeColorWithColor(con, [NSColor blackColor].CGColor);
    CGContextSetLineWidth(con, 1.0);
    
    CGContextMoveToPoint(con, XLEFTGAP * sXPixels, (1.0 - YBOTTOMGAP)*sYPixels + offset);
    CGContextAddLineToPoint(con, (XLEFTGAP + XMAIN)* sXPixels,(1.0 - YBOTTOMGAP)*sYPixels + offset);
    CGContextMoveToPoint(con, XLEFTGAP * sXPixels, (1.0 - YBOTTOMGAP)*sYPixels + offset);
    CGContextAddLineToPoint(con, XLEFTGAP * sXPixels, YTOPGAP * sYPixels + offset);
    CGContextStrokePath(con);
    
    
    // tic labels : x-axis (pH)
    for (int i = 0; i < XTICS_PH; i++)
    {
		xPos = ((i * XMAIN)/(XTICS_PH - 1) + XLEFTGAP) * sXPixels;
		CGContextMoveToPoint(con, xPos, (1.0 - YBOTTOMGAP) * sYPixels + offset);
        yPos = (1.0 - YBOTTOMGAP + YTICLENGTH) * sYPixels + offset;
		CGContextAddLineToPoint(con, xPos, yPos);
        CGContextStrokePath(con);
        
		[self create_text_at_point:con text: [NSString stringWithFormat:@"%.4g",(((float)i / (XTICS_PH - 1)) * (PH_MAX-PH_MIN)) + PH_MIN] X: xPos Y: yPos angle: (float) 0.0 alignment: ALIGN_CENTRE];
	}
    
    // tic labels : y-axis (rate,v)
    for (int i = 1; i < YTICS; i++)
    {
		yPos = (1.0 - ((i * YMAIN)/(YTICS - 1) + YBOTTOMGAP)) * sYPixels + offset;
		CGContextMoveToPoint(con, XLEFTGAP * sXPixels, yPos);
        xPos = (XLEFTGAP - (XTICLENGTH/2)) * sXPixels;
		CGContextAddLineToPoint(con, xPos, yPos );
        CGContextStrokePath(con);
        
        [self create_text_at_point:con text: [NSString stringWithFormat:@"%.1f",(float)i / (YTICS - 1) * maxv] X: xPos-(1.5 * xChar) Y: yPos-(yChar/1.25) angle: (float) 0.0 alignment: ALIGN_CENTRE];
	}

    // and our experimental points
	for (int i = 0; i < nPts; i++) {
		xPos = (((pH[i] - PH_MIN) / (PH_MAX - PH_MIN)) * XMAIN + XLEFTGAP) * sXPixels;
		yPos = (1.0 - ((v[i] / maxv) * YMAIN + YBOTTOMGAP)) * sYPixels + offset;
        crosslen = YTICLENGTH * yPixels / 2;
		CGContextMoveToPoint(con, xPos - crosslen, yPos + crosslen);
		CGContextAddLineToPoint(con, xPos + crosslen, yPos - crosslen );
        CGContextStrokePath(con);
		CGContextMoveToPoint(con, xPos - crosslen, yPos - crosslen );
		CGContextAddLineToPoint(con, xPos + crosslen, yPos + crosslen );
        CGContextStrokePath(con);
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
