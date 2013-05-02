// STHeightmap.cpp
#include "STHeightmap.h"

#include <assert.h>
#include <stdio.h>
#include <string>

//
// Load a new image from an image file (PPM, JPEG
// and PNG formats are supported).
// Returns NULL on failure.
//
STHeightmap::STHeightmap(const std::string& filename)
    : mWidth(-1)
    , mHeight(-1)
    , mPixels(NULL)
{

    // Determine the right routine based on the file's extension.
    // The format-specific subroutines are each implemented in
    // a different file.
    std::string ext = (filename.substr(filename.find_last_of(".") + 1));
    if (ext.compare("pgm") == 0) {
        LoadPGM(filename);
    }
    else if (ext.compare("ter") == 0) {
        LoadTER(filename);
    }
    else {
        fprintf(stderr,
                "STHeightmap::STHeightmap() - Unknown image file type \"%s\".\n",
                filename.c_str());
    }
}

//
// Construct a new image of the specified width and height,
// filled completely with the specified pixel color.
//
STHeightmap::STHeightmap(int width, int height, Pixel color)
{
    Initialize(width, height);

    int numPixels = mWidth * mHeight;
    for (int ii = 0; ii < numPixels; ++ii) {
        mPixels[ii] = color;
    }
}

// Common initialization logic shared by all construcotrs.
void STHeightmap::Initialize(int width, int height)
{
    if (width <= 0)
        printf("STImage width must be positive");
    if (height <= 0)
        printf("STImage height must be positive");

    mWidth = width;
    mHeight = height;

    int numPixels = mWidth * mHeight;
    mPixels = new Pixel[numPixels];
}

//
// Delete and clean up an existing image.
//
STHeightmap::~STHeightmap()
{
    if (mPixels != NULL) {
        delete [] mPixels;
    }
}

//
// Save the image to a file (PGM and TER
// formats are supported).
// Returns a non-zero value on error.
//
bool STHeightmap::Save(const std::string& filename) const
{
    // Determine the right routine based on the file's extension.
    // The format-specific subroutines are each implemented in
    // a different file.
    std::string ext = filename.substr(filename.find_last_of(".") + 1);

    if (ext.compare("PGM") == 0 ) {
        return SavePGM(filename);
    }
    else if (ext.compare("TER") == 0) {
        return SaveTER(filename);
    }
    else {
        fprintf(stderr,
                "STHeightmap::Save() - Unknown image file type \"%s\".\n",
                filename.c_str());
        assert(false);
        return 1;
    }
}

//
// Read a pixel value given its (x,y) location.
//
STHeightmap::Pixel STHeightmap::GetPixel(int x, int y) const
{
    // Check that (x,y) is in range.
    assert(x >= 0 && x < mWidth);
    assert(y >= 0 && y < mHeight);

    return mPixels[y*mWidth + x];
}

//
// Write a pixel value given its (x,y) location.
//
void STHeightmap::SetPixel(int x, int y, Pixel value)
{
    // Check that (x,y) is in range.
    assert(x >= 0 && x < mWidth);
    assert(y >= 0 && y < mHeight);

    mPixels[y*mWidth + x] = value;
}
