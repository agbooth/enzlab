//
//  el_EnzController.h
//  EnzLab
//
//  Created by Andrew Booth on 09/05/2014.
//  Copyright (c) 2014 Andrew Booth. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

@interface el_EnzController : NSViewController <NSWindowDelegate, NSTextFieldDelegate, NSUserNotificationCenterDelegate>

@property (strong) CAShapeLayer* lineLayer;
@property (strong) NSString* velocityString;

- (void) GoButtonClicked;
- (void) RepeatButtonClicked;
- (void) PaintEnzGraph: (float) delay;
@end
