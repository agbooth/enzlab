//
//  el_EnzController.m
//  EnzLab
//
//  Created by Andrew Booth on 09/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_EnzController.h"
#import "el_Layer.h"
#import "el_AppDelegate.h"
#import "L.h"
#import <QuartzCore/QuartzCore.h>

@interface el_EnzController ()

@end

@implementation el_EnzController

@synthesize lineLayer, velocityString;

el_Layer* graphLayer;
CAShapeLayer* lineLayer;
CAKeyframeAnimation *animation;

Boolean swapped = FALSE;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.view = app.graphView;
        
        graphLayer = [[el_Layer alloc] init];
        
        // set the layer's frame
        NSRect frame = self.view.frame;
        frame.origin.x = 0.0;
        graphLayer.frame = frame;
        [graphLayer setNeedsDisplayOnBoundsChange:TRUE];
        
        [self.view setLayer:graphLayer];
        //[self.view setWantsLayer:TRUE];
    
        
        lineLayer = [[CAShapeLayer alloc] init];
        lineLayer.frame = frame;
        lineLayer.strokeColor = [NSColor redColor].CGColor;
        lineLayer.geometryFlipped = TRUE;
        lineLayer.hidden = TRUE;
        
        //[graphLayer addSublayer: lineLayer];
        [self.view.layer addSublayer: lineLayer];
        
        
        
     }
    
    return self;
}

- (void)userNotificationCenter:(NSUserNotificationCenter *)center didDeliverNotification:(NSUserNotification *)notification
{
    
    if ([notification.title isEqualToString:@"finishedDrawingAxes"])
    {
        
        if (graphLayer.delay != 0.0)
        {
            // Reveal the line-drawing layer
            lineLayer.hidden =  FALSE;
            
            // Its path has alredy been calculated, so just copy it over.
            lineLayer.path = graphLayer.path;
            
            // for some reason there is a 1 pixel misalignment between the two layers, so apply a correction.
            
            NSRect frame = graphLayer.frame;
            frame.origin.y += 1.0;
            frame.origin.x = 0.0;
            lineLayer.frame = frame;
            
            // Set up the animation.
            // The end of the stroke will increase from 0 to 100% of its final value over 2 seconds.
            [lineLayer removeAllAnimations];
            CABasicAnimation *pathAnimation = [CABasicAnimation animationWithKeyPath:@"strokeEnd"];
            pathAnimation.duration = 2.0;
            pathAnimation.delegate = self;
            pathAnimation.fromValue = [NSNumber numberWithFloat:0.0f];
            pathAnimation.toValue = [NSNumber numberWithFloat:1.0f];
        
            // The animation is started by adding it to the layer.
            [lineLayer addAnimation:pathAnimation forKey:@"strokeEnd"];
            
        }
    }
}

- (void) PaintEnzGraph: (float) delay
{
    
    graphLayer.delay = delay;
    
    // Draw the frame of the graph.
    // This will send a notification when it has been drawn.
    // In response we can animate the line if necessary.
    
    [graphLayer setNeedsDisplay];
    
}

// CABasicAnimation delegate method
- (void)animationDidStart:(CAAnimation *)theAnimation
{
    //NSLog(@"Animation started");
}

// CABasicAnimation delegate method
- (void)animationDidStop:(CAAnimation *)theAnimation finished:(BOOL)flag
{
    //NSLog(@"Animation finished");
    lineLayer.hidden = TRUE;
    graphLayer.delay = 0;
    [graphLayer setNeedsDisplay];
    
    [app.Rate_label setStringValue:velocityString];
    
    if ([app.clock ClockFwd: CLK_HALFHOUR])
    {
        [app.Repeat_button setEnabled:TRUE];
        [app.Go_button setEnabled:FALSE];
    }
    
}

// NSWindowDelegate method
- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
    
    NSSize size = frameSize;
    
    
    
    if (frameSize.width < 700.0)   // Stop the frame getting so small everything overlaps
        size.width = 700.0;
    
    if (frameSize.height < 450.0)
        size.height = 450.0;
    return size;
}

// NSTextFieldDelegate method
- (void)controlTextDidChange:(NSNotification *)aNotification
{
    if (app.Events.bEnzSel == TRUE)
    {
        [app.Repeat_button setEnabled:FALSE];
        [app.Go_button setEnabled:TRUE];
        
        [app.Rate_label setStringValue:@""];
        [app.expt UpdateRunNo];
    }
}

- (void) GoButtonClicked
{
    
    [app.expt DoEnzRun];
    
}

- (void) RepeatButtonClicked
{
    [app.expt UpdateRunNo];
    [app.expt DoEnzRun];
    
}


@end
