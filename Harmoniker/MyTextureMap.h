//
//  MyTextureMap.h
//  Harmoniker
//
//  Created by David Gavilan on 7/1/12.
//  Copyright 2012 vid Games. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface MyTextureMap : NSPersistentDocument {
    // IBOutlet: Identifier used to identify a property so Interface Builder can synchronize the display and connection of outlets with Xcode. Insert this identifier immediately before the type in any declarations.
    IBOutlet NSImageView *imageViewFront;
    IBOutlet NSImageView *imageViewBack;
    IBOutlet NSImageView *imageViewIrradiance;
    IBOutlet NSColorWell *colorWell;
    IBOutlet NSTextField *tfNumBands;
    IBOutlet NSTextField *tfNumSamples;
    IBOutlet NSTableView *shTable;
    CGImageRef           imgIrradiance;
}

@property (retain) NSImageView *imageViewFront;
@property (retain) NSImageView *imageViewBack;
@property (retain) NSImageView *imageViewIrradiance;
@property (retain) NSColorWell *colorWell;

- (IBAction)computeHarmonics:(id)sender;
- (IBAction)validateNumBands:(id)sender;
- (IBAction)validateNumSamples:(id)sender;
- (IBAction)saveIrradiance:(id)sender;

@end
