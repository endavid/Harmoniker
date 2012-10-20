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
@synthesize imageViewIrradiance;
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
    unsigned char* g_imgBuffer = NULL;
    
    const int IRRADIANCE_W = 32;
    const int IRRADIANCE_H = 32;
    const int IRRADIANCE_BANDS = 3;     ///< R,G,B
    
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
        float s = 1.f/255.f;
        
        // choose hemisphere
        if (u < 1.f) { // front
            // UV to pixel coordinates
            int y = (int)vd::math::Min(g_frontHeight-1, floorf(g_frontHeight*v));
            int x = (int)vd::math::Min(g_frontWidth-1, floorf(g_frontWidth*u));
            int i = (int)(g_frontStride*(g_frontWidth*y + x));
            float r = g_frontBytes[i];
            float g = g_frontBytes[i+1];
            float b = g_frontBytes[i+2];
            return vd::math::Vector3(s*r, s*g, s*b);
        } else { // back
            // UV to pixel coordinates
            u = u - 1.f;
            int y = (int)vd::math::Min(g_backHeight-1, floorf(g_backHeight*v));
            int x = (int)vd::math::Min(g_backWidth-1, floorf(g_backWidth*u));
            int i = (int)(g_backStride*(g_backWidth*y + x));
            float r = g_backBytes[i];
            float g = g_backBytes[i+1];
            float b = g_backBytes[i+2];
            return vd::math::Vector3(s*r, s*g, s*b);
        }
    } // polarSampler
    
    /** Saves image to disk
     *  @see http://stackoverflow.com/questions/1320988/saving-cgimageref-to-a-png-file
     */
    void CGImageWriteToFile(CGImageRef image, NSURL *filename) {
        CFURLRef url = (CFURLRef)filename;
        CGImageDestinationRef destination = CGImageDestinationCreateWithURL(url, kUTTypePNG, 1, NULL);
        CGImageDestinationAddImage(destination, image, nil);
        
        if (!CGImageDestinationFinalize(destination)) {
            NSLog(@"Failed to write image to %@", [filename absoluteString]);
        }
        
        CFRelease(destination);
    }
    
    /** 
     * @brief Creates a map of sphere coordinates on 2D
     * Y axis is defined with respect to the center of the image, being 1 at the center, 0 at radius = 0.7, and -1 if radius >= 1.
     * TODO: convert linear color to sRGB
     */
    void initSphereMap(unsigned char* buffer, int width, int height, int bytesPerPixel, vd::math::SphericalHarmonics* sh = NULL) {
        float hInv = 1.0f/(float)height;
        float wInv = 1.0f/(float)width;
        for (int j = 0; j<height; ++j) {
            const float v = 2.f * (j+0.5f) * hInv - 1.f;
            for (int i = 0; i<width; ++i) {
                const float u = 2.f * (i+0.5f) * wInv - 1.f;
                float x = u;
                float z = v;
                float radius = sqrtf(x * x + z * z);
                float y = 1.0f - 2.f * radius;
                if (y < -1.f) y = -1.f;
                // normalize x, z (no div by zero, since never in the center for even sizes)
                float rInv = 1.0f/radius;
                radius = sqrtf(1.0f-y*y);
                x = radius * x * rInv;
                z = radius * z * rInv;
                
                if ( sh!= NULL ) {
                    // get irradiance for given direction
                    vd::math::Vector3 n(x,z,y);
                    vd::math::Vector3 irradiance = sh->GetIrradianceApproximation(n);
                    x = 255.f * vd::math::Clamp(irradiance.GetX() * vd::math::PI_INV, 0.f, 1.f);
                    y = 255.f * vd::math::Clamp(irradiance.GetY() * vd::math::PI_INV, 0.f, 1.f);
                    z = 255.f * vd::math::Clamp(irradiance.GetZ() * vd::math::PI_INV, 0.f, 1.f);
                } else {
                    // for debugging
                    x = 127.5f * x + 127.5f;
                    y = 127.5f * y + 127.5f;
                    z = 127.5f * z + 127.5f;
                }
                buffer[i*bytesPerPixel+bytesPerPixel*width*j] = (unsigned char)x;
                buffer[i*bytesPerPixel+bytesPerPixel*width*j+1] = (unsigned char)y;
                buffer[i*bytesPerPixel+bytesPerPixel*width*j+2] = (unsigned char)z;
            }
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
}

/// Initialization code that needs the instantiated IBOutlets (before this function is called, they are still nil!)
-(void)awakeFromNib {
    if (self.imageViewIrradiance != nil) {
        size_t bufferLength = IRRADIANCE_W * IRRADIANCE_H * IRRADIANCE_BANDS;
        g_imgBuffer = (unsigned char*)malloc(bufferLength);
        memset(g_imgBuffer, 0, bufferLength);
        
        initSphereMap(g_imgBuffer, IRRADIANCE_W, IRRADIANCE_H, IRRADIANCE_BANDS);
        [self updateImgIrradiance];
    }
}

-(void)updateImgIrradiance {
    CGColorSpaceRef rgb = CGColorSpaceCreateDeviceRGB();
    size_t bufferLength = IRRADIANCE_W * IRRADIANCE_H * IRRADIANCE_BANDS;
    
    CGDataProviderRef provider =
    CGDataProviderCreateWithData(NULL, g_imgBuffer, bufferLength, NULL);//freeBitmapBuffer);
    
    imgIrradiance = CGImageCreate(IRRADIANCE_W, IRRADIANCE_H, 8, 8 * IRRADIANCE_BANDS, IRRADIANCE_W * IRRADIANCE_BANDS, rgb, kCGBitmapByteOrderDefault, provider, NULL /*decode*/, false /*shouldInterpolate*/, kCGRenderingIntentDefault);
    
    NSImage *img = [[NSImage alloc] initWithCGImage: imgIrradiance size:NSZeroSize];
    [imageViewIrradiance setImage:img];
    
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(rgb);
    
    //CGImageRelease(imageRef);
    
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
    int numBands = [tfNumBands intValue];
    int numSamplesSqr = (int)sqrtf([tfNumSamples intValue]);
    
    // generate samples
    vd::math::SphericalHarmonics* sh = new vd::math::SphericalHarmonics(numBands, numSamplesSqr);
    // compute spherical harmonics
    sh->ProjectPolarFn(&polarSampler);
    
    //[shTable insertValue:[NSString stringWithFormat:@"test"] inPropertyWithKey:@"Index"];
    
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
    
    // update sphere map
    initSphereMap(g_imgBuffer, IRRADIANCE_W, IRRADIANCE_H, IRRADIANCE_BANDS, sh);
    [self updateImgIrradiance];

#endif

    g_frontBytes = NULL;
    g_backBytes = NULL;
}

-(IBAction)validateNumBands:(id)sender{
    int value = [tfNumBands intValue];
    if (value <= 0) {
        value = 1;
    }
    [tfNumBands setStringValue:[NSString stringWithFormat:@"%d",value]];
}
-(IBAction)validateNumSamples:(id)sender{
    int value = [tfNumSamples intValue];
    if (value <= 0) {
        value = 1;
    }
    value = (int)sqrtf(value);
    value *= value; 
    [tfNumSamples setStringValue:[NSString stringWithFormat:@"%d",value]];
}
-(IBAction)saveIrradiance:(id)sender{
    // Create the File Save Dialog class.
    NSSavePanel* saveDlg = [NSSavePanel savePanel];
        
    // Display the dialog.  If the OK button was pressed, save
    if ( [saveDlg runModal] == NSOKButton ) {
        NSURL* file = [saveDlg URL];
        CGImageWriteToFile(imgIrradiance, file);
    }
}

- (void)dealloc {
    free(g_imgBuffer);
    g_imgBuffer = NULL;
    [super dealloc];
}

//CFDataRef CopyImagePixels(CGImageRef inImage) {     return CGDataProviderCopyData(CGImageGetDataProvider(inImage)); }

@end
