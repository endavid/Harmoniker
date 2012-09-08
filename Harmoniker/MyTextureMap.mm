//
//  MyTextureMap.m
//  Harmoniker
//
//  Created by David Gavilan on 7/1/12.
//  Copyright 2012 vid Games. All rights reserved.
//

#import "MyTextureMap.h"
#include "math/SphericalHarmonics.h"


@implementation MyTextureMap

@synthesize imageViewFront;
@synthesize imageViewBack;
@synthesize colorWell;

namespace {
    size_t g_frontWidth = 0;
    size_t g_frontHeight = 0;
    size_t g_frontStride = 4;
    size_t g_backWidth = 0;
    size_t g_backHeight = 0;
    size_t g_backStride = 4;
    const UInt8* g_frontBytes = NULL;
    const UInt8* g_backBytes = NULL;
    
    /**
     *  This sampler function uses 2 images as if they were projections on the front
     *  and back hemispheres of a light probe.
     *  @param theta: the inclination (0-π)
     *  @param phi: the azimuth (0-2π)
     *  @return normalized RGB value at the given spherical coordinate
     *  TODO: HDR image support and linear RGB (now it's sRGB)
     */
    vd::math::Vector3 polarSampler(double theta, double phi) {
        // spherical to UV
        float u = phi * vd::math::PI_INV;   // 0..2
        float v = theta * vd::math::PI_INV; // 0..1
        // for color normalization
        float s = 1.f/256.f;
        
        // choose hemisphere
        if (u < 1.f) { // front
            // UV to pixel coordinates
            int y = (int)vd::math::Min(g_frontHeight-1, floorf(g_frontHeight*v));
            int x = (int)vd::math::Min(g_frontWidth-1, floorf(g_frontWidth*u));
            int i = g_frontStride*(g_frontWidth*y + x);
            float r = g_frontBytes[i];
            float g = g_frontBytes[i+1];
            float b = g_frontBytes[i+2];
            return vd::math::Vector3(s*r, s*g, s*b);
        } else { // back
            // UV to pixel coordinates
            u = u - 1.f;
            int y = (int)vd::math::Min(g_backHeight-1, floorf(g_backHeight*v));
            int x = (int)vd::math::Min(g_backWidth-1, floorf(g_backWidth*u));
            int i = g_backStride*(g_backWidth*y + x);
            float r = g_backBytes[i];
            float g = g_backBytes[i+1];
            float b = g_backBytes[i+2];
            return vd::math::Vector3(s*r, s*g, s*b);
        }
    }
    
} // anonymous namespace

- (id)init
{
    self = [super init];
    if (self) {
        // Add your subclass-specific initialization here.
        // If an error occurs here, send a [self release] message and return nil.
    }
    return self;
}

- (NSString *)windowNibName
{
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"MyTextureMap";
}

- (void)windowControllerDidLoadNib:(NSWindowController *)aController
{
    [super windowControllerDidLoadNib:aController];
    // Add any code here that needs to be executed once the windowController has loaded the document's window.
    //if (self.imageFront != nil) {
    //    [imageViewFront setImage:self.imageFront];
    //}
}

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError {
    NSData *data;
    data = [NSArchiver archivedDataWithRootObject:[self.imageViewFront image]];
    return data;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError {
    NSImage *tmpImage = [NSUnarchiver unarchiveObjectWithData: data];
    [imageViewFront setImage:tmpImage];
    return YES;
}

+ (BOOL)autosavesInPlace
{
    return YES;
}

// TODO: add a progress bar...
-(IBAction)computeHarmonics:(id)sender {
    CGImageRef imageFront = [[imageViewFront image] CGImageForProposedRect: NULL context:nil hints:nil];
    CGImageRef imageBack = [[imageViewBack image] CGImageForProposedRect: NULL context:nil hints:nil];
    if (imageFront == NULL || imageBack == NULL) {
        NSLog(@"No image!");
        return;
    }
    
    g_frontWidth = CGImageGetWidth(imageFront);
    g_frontHeight = CGImageGetHeight(imageFront);
    g_backWidth = CGImageGetWidth(imageBack);
    g_backHeight = CGImageGetHeight(imageBack);
    
    NSLog(@"%zu x %zu; %zu x %zu", g_frontWidth, g_frontHeight, g_backWidth, g_backHeight);
    CFDataRef dataFront = CGDataProviderCopyData(CGImageGetDataProvider(imageFront));
    CFDataRef dataBack = CGDataProviderCopyData(CGImageGetDataProvider(imageBack));

    NSLog(@"bytesPerRow: %lu", CGImageGetBytesPerRow(imageFront));
    NSLog(@"bitsPerPixel: %lu", CGImageGetBitsPerPixel(imageFront));
    NSLog(@"bitsPerComponent: %lu", CGImageGetBitsPerComponent(imageFront));
    
    CFIndex dfSize = CFDataGetLength(dataFront);
    CFIndex dbSize = CFDataGetLength(dataBack);
    
    if (dfSize == g_frontWidth*g_frontHeight*4) { // RGBA
        g_frontStride = 4;
    } else if (dfSize == g_frontWidth*g_frontHeight*3) { // RGB
        g_frontStride = 3;
    } else {
        NSLog(@"Bad length! front: %ld",dfSize);
        return;
    }
    if (dbSize == g_backWidth*g_backHeight*4) { // RGBA
        g_backStride = 4;
    } else if (dbSize == g_backWidth*g_backHeight*3) { // RGB
        g_backStride = 3;
    } else {
        NSLog(@"Bad length! back: %ld",dbSize);
        return;
    }
    
    g_frontBytes = CFDataGetBytePtr(dataFront);
    g_backBytes = CFDataGetBytePtr(dataBack);
    
#if false
    for (int y=0; y<g_frontHeight;y++) {
        for (int x=0; x<g_frontWidth;++x) {
            int i = g_frontStride*(g_frontWidth*y+x);
            int r = g_frontBytes[i];
            int g = g_frontBytes[i+1];
            int b = g_frontBytes[i+2];
            NSLog(@"%d,%d: (%d,%d,%d)", x,y, r,g,b);
        }
    }
#endif
    
#if true
    // generate samples
    vd::math::SphericalHarmonics* sh = new vd::math::SphericalHarmonics();
    // compute spherical harmonics
    sh->ProjectPolarFn(&polarSampler);
    
    
    // Output coefficients
    const vd::math::Vector3* coeff = sh->GetCoeffs();
    for (int i=0; i<sh->GetNumCoeffs(); ++i) {
        NSLog(@"%d: (%3.4f,%3.4f,%3.4f)", i, coeff[i].GetX(), coeff[i].GetY(), coeff[i].GetZ());
    }
    
    // debug band 0 coefficients
    float r = coeff[0].GetX() * vd::math::PI_INV;
    float g = coeff[0].GetY() * vd::math::PI_INV;
    float b = coeff[0].GetZ() * vd::math::PI_INV;
    [colorWell setColor:[NSColor colorWithSRGBRed:r green:g blue:b alpha:1.0f]];
    
#endif

    g_frontBytes = NULL;
    g_backBytes = NULL;
}

//CFDataRef CopyImagePixels(CGImageRef inImage) {     return CGDataProviderCopyData(CGImageGetDataProvider(inImage)); }

@end
