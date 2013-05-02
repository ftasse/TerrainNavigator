// STHeightmap_pgm.cpp
#include "STHeightmap.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

// The PPM format guarantees no line in the file is longer
// than 70 characters. We are a little conservative here.
static const int MAX_PGM_LINE = 80;


//
// Creates an STImage from the contents of a PPM file
//
void STHeightmap::LoadPGM(const std::string& filename)
{
   ifstream ifp;
   ifp.open(filename.c_str(), ios::in | ios_base::binary);

    if (!ifp)
     {
        fprintf(stderr, "STHeightmap::LoadPGM() - Could not open '%s'.\n",
                filename.c_str());
    }

    char header [100], *ptr;

    ifp.getline(header,100,'\n');        //magic number
    if ( (header[0]!=80) || (header[1]!=53) )      //if not P5
   {
        fprintf(stderr, "Invalid PGM file format.\n");
    }

    ifp.getline(header,100,'\n');
    while(header[0]=='#')        //file name line in file starts with #
        ifp.getline(header,100,'\n');

    mWidth=strtol(header,&ptr,0);    //number of colums
    mHeight=atoi(ptr);            //number of rows

    ifp.getline(header,100,'\n');
    strtol(header,&ptr,0);    //max gray value

   unsigned char * charImage = (unsigned char *) new unsigned char [mWidth*mHeight];    //creates 2D array

    ifp.read( reinterpret_cast<char *>(charImage), (mWidth*mHeight)*sizeof(unsigned char));  //reads in 2D ar


    ifp.close();

    Initialize(mWidth, mHeight);

    int val;

    //cout<<mWidth<<" "<<mHeight<<endl;

    for(int j=0; j<mHeight; j++)
          for(int i=0; i<mWidth; i++){
            val = (int)charImage[i+j*mWidth];
            SetPixel(i, j, val);  //just to make sure the pixel value is never 0
        }
    delete [] charImage;

    mScale=1.f;
    mBaseHeight=mHeightScale=1;
}

//
// Create a PPM file from the pixel contents of the STImage
//
bool
STHeightmap::SavePGM(const std::string& filename) const
{
   ofstream ofp;
   ofp.open(filename.c_str(), ios::out | ios::trunc | ios_base::binary);

    if (!ofp)
    {
        fprintf(stderr, "STHeightmap::SavePGM() - Could not open '%s'.\n",
                filename.c_str());
        return 1;
    }

    ofp << "P5" << std::endl;
    ofp << mWidth << " " << mHeight << std::endl;
    ofp << 255 << std::endl;

    char val;
    for(int j = 0; j <mHeight; j++)
	for(int i = 0; i < mWidth; i++)
	{
		val = (char) ( (float) GetPixel(i,j));
		ofp.write((char *) &val, sizeof(val));
	}


    if (ofp.fail())
    {
        fprintf(stderr, "STHeightmap::SavePGM() - Could not open '%s'.\n",
                filename.c_str());
        return 1;
    }

    ofp.close();
    return 0;
}



