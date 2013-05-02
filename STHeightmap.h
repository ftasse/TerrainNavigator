// STHeightmap.h
#ifndef __STHEIGHTMAP_H__
#define __STHEIGHTMAP_H__

#include <string>

typedef float Pixel ;

/**
* The STHeightmap class encapsulates heightmap data, stored as an array
* of float values. The image data is stored in a
* format that is consistent with OpenGL; row-major with the bottom row
* of the image being first in the array of pixels.
* This means that the bottom-left pixel in the image is the first pixel
* in the internal array.
*
* The easiest way to create a STHeightmap is to construct one with the file
* name of a terrain:
*
*   STHeightmap* terrain = new STHeightmap("./terrain.ter");
*
* Alternatively, you can construct a new heightmap filled with a solid
* color of your choosing:
*
*   STHeightmap* red = new STHeightmap(640, 480, 255.);
*
* The pixels of an STHeightmap can be read and written using GetPixel()
* and SetPixel(), respectively:
*
*   STColor4ub p = frog->GetPixel(10, 25);
*   red->SetPixel(10, 25, p);
*
* Any image can be written to a file by using Save():
*
*   red->Save("./output.ter");
*/

class STHeightmap
{
public:
    //
    // Type of pixels in an STImage.
    //
    typedef float Pixel;

    //
    // Load a new image from an image file (PPM, JPEG
    // and PNG formats are supported).
    // Returns NULL on failure.
    //
    STHeightmap(const std::string& filename);

    //
    // Construct a new image of the specified width and height,
    // filled completely with the specified pixel color.
    //
    STHeightmap(int width, int height, Pixel color = 0.);

    //
    // Delete and clean up an existing image.
    //
    ~STHeightmap();

    //
    // Save the image to a file (TER and PGM
    // formats are supported).
    // Returns a non-zero value on error.
    //
    bool Save(const std::string& filename) const;

    //
    // Get the width (in pixels) of the image.
    //
    int GetWidth() const { return mWidth; }

    //
    // Get the height (in pixels) of the image.
    //
    int GetHeight() const { return mHeight; }

    //
    // Read a pixel value given its (x,y) location.
    //
    Pixel GetPixel(int x, int y) const;

    //
    // Write a pixel value given its (x,y) location.
    //
    void SetPixel(int x, int y, Pixel value);

    float GetScale() const {	return mScale; };

    void SetScale(float s) {	mScale = s; }

    //
    // Get read-only access to the "raw" array of pixel data.
    // The STImage object owns this data, and it is not valid
    // to use it after the image is deleted.
    //
    const Pixel* GetPixels() const { return mPixels; }

    //
    // Get read-write access to the "raw" array of pixel data.
    // The STImage object owns this data, and it is not valid
    // to use it after the image is deleted.
    //
    Pixel* GetPixels() { return mPixels; }

private:
    // Image height, in pixels.
    int mHeight;

    // Image width, in pixels.
    int mWidth;

    float mScale;
    int mBaseHeight, mHeightScale;

    // An array of mWidth*mHeight pixels, stored in row-major
    // left-to-right, bottom-to-top order.
    Pixel* mPixels;

    //
    void Initialize(int width, int height);

    //
    // Format-specific routines for loading/saving
    // particular image file formats.
    //
    void LoadPGM(const std::string& filename);
    bool  SavePGM(const std::string& filename) const;

    void LoadTER(const std::string& filename);
    bool  SaveTER(const std::string& filename) const;
};

#endif // __STHEIGHTMAP_H__
