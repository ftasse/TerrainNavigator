// STHeightmap_ter.cpp
#include "STHeightmap.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sys/stat.h>
using namespace std;

#define DEFAULT_DIMX 3000
#define DEFAULT_DIMY 3000

// The PPM format guarantees no line in the file is longer
// than 70 characters. We are a little conservative here.
static const int MAX_PGM_LINE = 80;

//
// Creates an STImage from the contents of a PPM file
//
void STHeightmap::LoadTER(const std::string& filename)
{
	ifstream infile;
	char * inbuffer = NULL;
        char * tmpstr;
	int pos=16, iter=0;
	int tersize, w=0,h=0, truncdim,mindim;
	bool alts= false;
	string frombuffer;

	infile.open(filename.c_str(), ios_base::in | ios_base::binary);
	if(!infile.is_open()){
		fprintf(stderr, "STHeightmap::LoadTER() - Could not open '%s'.\n", filename.c_str());
	}

        struct stat results;
	stat((char *) filename.c_str(), &results);
	int size = results.st_size;

	// put file contents in buffer
	inbuffer = new char[size];
	infile.read(inbuffer, size);

	tmpstr = (char *) &inbuffer[0];
	frombuffer = string((char *) tmpstr, 16);

	if (frombuffer.compare("TERRAGENTERRAIN") == 0)
	{
		delete tmpstr;
		fprintf(stderr, "STHeightmap::LoadTER() - Invalid TER file format.\n");
	 }


        //printf("Error: this is not a Terragen file!\n");
        mScale=30.0f;

        while(!alts && iter < 7){
                tmpstr = (char *) &inbuffer[pos];
                frombuffer = string((char *) tmpstr, 4);
                if(frombuffer.compare("SIZE") == 0) // num data points on shortest side - 1
                {
                    pos += 4;
                    tersize = (int) (* ((short *) &inbuffer[pos]));
                    w = tersize+1; h = tersize+1;  // defaults assuming no XPTS, YPTS
                    pos += 4;
                }
                if(frombuffer.compare("XPTS") == 0) // num data points in x dimension
                {
                    pos += 4;
                    w = (int) (* ((short *) &inbuffer[pos]));
                    pos += 4;
                    printf("XPTS = %d found\n", w );
                }
                if(frombuffer.compare("YPTS") == 0) // num data points in y dimension
                {
                    pos += 4;
                    h = (int) (* ((short *) &inbuffer[pos]));
                    pos += 4;
                    printf("YPTS = %d found\n", h );
                }
                if(frombuffer.compare("SCAL") == 0) // scale of terrain element in metres
                {
                    pos += 4;
                    mScale = (* ((float *) &inbuffer[pos]));
                    pos += 12;
                }
                if(frombuffer.compare("CRAD") == 0) // planet radius - skip
                    pos += 8;
                if(frombuffer.compare("CRVM") == 0) // draping state - skip
                    pos += 8;
                if(frombuffer.compare("ALTW") == 0) // marks the beginning of the altitude data
                {
                    pos += 4;
                    mHeightScale = (int) (* ((short *) &inbuffer[pos]));
                    pos += 2;
                    mBaseHeight = (int) (* ((short *) &inbuffer[pos]));
                    pos += 2;
                    alts = true; // finish processing header block
                }
                iter++;
            }

		if (!alts) {
			delete tmpstr;
			fprintf(stderr, "STHeightmap::LoadTER() - Could not find elevation values.\n");
		}

		if(h != w)
		{
			mindim = min(h, w);
			printf("Terrain is not square. Truncating from (%dx%d) to (%dx%d)\n",w,h,mindim, mindim );
		}
		else
			mindim = h;


		// terrain must be power of 2
		/*
		float p = log((double) mindim) / log(2.0f);
		truncdim = (int) pow(2.0f, floor(p));
		if(truncdim != mindim){
			cout<<"Terrain is not a power of 2. Truncating from (" << mindim << " x " << mindim <<") to (" << truncdim << " x " << truncdim << ")\n";
		}*/ truncdim = mindim;

		if(truncdim > DEFAULT_DIMX){
                     //printf("Terrain is larger than system can handle ... truncating to (%dx%d)\n",DEFAULT_DIMX,DEFAULT_DIMX);
		     truncdim = DEFAULT_DIMX;
		}

                //printf("Terrain dim = %dx%d\n", truncdim,truncdim);

		mWidth=truncdim;
		mHeight=truncdim;

		Initialize(mWidth,mHeight);

                for(int j = mindim-1; j >=0; j--)
                        for(int i = 0; i < mindim; i++)
			{
                            int elev = (* ((short *) &inbuffer[pos])) ;
                            if (i<mWidth && j<mHeight){
                                //printf("elevation: %d\n",c);
                                float alt = mBaseHeight+(elev*mHeightScale/65536.0);
                                SetPixel(i,j, alt );

                            }
                            pos += 2;

			}

        //printf("mScale:%f mBaseHeight:%d, mHeightScale:%d\n",mScale,mBaseHeight,mHeightScale);

        delete [] inbuffer;
	infile.close();

}

//
// Create a PPM file from the pixel contents of the STImage
//
bool
STHeightmap::SaveTER(const std::string& filename) const
{
    FILE* imgFile = fopen(filename.c_str(), "w");
    if (!imgFile) {
        fprintf(stderr, "STHeightmap::SavePGM() - Could not open '%s'.\n",
                filename.c_str());
        return 1;
    }

    fprintf(imgFile, "P3\n");
    fprintf(imgFile, "%d %d\n", mWidth, mHeight);
    fprintf(imgFile, "255\n");

    int numPixels = mWidth * mHeight;
    for (int ii = 0; ii < numPixels; ++ii) {
        unsigned char pixel = mPixels[ii];
        fprintf(imgFile,"%d\n",pixel);
    }
    fclose(imgFile);

    return 0;
}
