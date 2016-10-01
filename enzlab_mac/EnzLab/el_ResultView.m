//
//  el_ResultView.m
//  EnzLab
//
// Just a subclass to allow interception of keyDown:
//  Created by Andrew Booth on 18/06/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import "el_ResultView.h"

@implementation el_ResultView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}
/*
-(void)keyDown:(NSEvent*)event
{
    // I added these based on the addition to your question :)
    switch( [event keyCode] ) {
    	case 126:	// up arrow
    	case 125:	// down arrow
    	case 124:	// right arrow
    	case 123:	// left arrow
    		NSLog(@"Arrow key pressed!");
    		break;
    	default:
    		NSLog(@"Key pressed: %@", event);
    		break;
    }
}
*/
@end
