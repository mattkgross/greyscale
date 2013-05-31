//----------------------------------------------------------------------------
// Documentation
//----------------------------------------------------------------------------
// Filename: grayscale.cxx
// Last Edited: 12/25/2011
// Author: Matthew Gross
// Email: mattkgross@gmail.com
// Description: Reads an image file and then for every pixel on the screen,
// if it is X shades (or more) of grey darker than any of the four pixels
// surrounding it, make it black, otherwise make it white. It then creates a
// new image file with the modified data.
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Directives
//----------------------------------------------------------------------------

#include <iostream>
#include <cstdlib>
#include <graphics.h>
#include <string>
#include <cmath>
#include <vector>
using namespace std;

//----------------------------------------------------------------------------
// Classes
//----------------------------------------------------------------------------
struct coloring
{
    int red, green, blue;
};

//----------------------------------------------------------------------------
// Named Constants
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Displays an image in the graphics window. Parameter name is the name of the
// image file (extension included). Parameters width and height are the
// image's dimensions.
//----------------------------------------------------------------------------
void callimage(char* name, size_t width, size_t height);

//----------------------------------------------------------------------------
// Checks a point against a "change" color base. If the grayscale is greater
// than x, then a value of true will be returned.
//----------------------------------------------------------------------------
bool checkgrey(size_t i, size_t j, coloring change, int x);

//----------------------------------------------------------------------------
// Checks a pixel point. If it's black, returns true. Else, returns false.
//----------------------------------------------------------------------------
bool checkpixel(size_t i, size_t j);

//----------------------------------------------------------------------------
// For every pixel on the screen, if it is X shades of grey darker than any
// of the four pixels surrounding it, make it black, otherwise make it white.
//----------------------------------------------------------------------------
void findgrey(size_t width, size_t height, vector<bool>& newimg, int x);

//----------------------------------------------------------------------------
// Analyze each pixel. If surrounding 8 neighbors are all white, turn that
// pixel white.
//----------------------------------------------------------------------------
void killstray(size_t width, size_t height);

//----------------------------------------------------------------------------
// Writes the altered image to a new file.
//----------------------------------------------------------------------------
void setimage(size_t width, size_t height);


//----------------------------------------------------------------------------
// Function Definitions
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
int main( )
{
    cout << "This program will change the grayscale of a specified image " <<
    "located within the same directory as this .exe file. " <<
    "Make sure the image you have has a name no longer than 95 characters" <<
    endl;

    char name[100];
    size_t width, height;
    int x;
    cout << "Please enter the name of the image: ";
    cin >> name;
    cout << endl << "Please enter image width: ";
    cin >> width;
    cout << endl << "Please enter image height: ";
    cin >> height;
    cout << endl << "Please enter the shades of gray you would like augmented: ";
    cin >> x;

    initwindow(width, height, "Grayscale Changer", 0, 0, false);

    callimage(name, width, height);
  
    vector<bool> newimg(width*height);
    findgrey(width, height, newimg, x);
    killstray(width, height);
    

    setimage(width, height);
    
    cout << "Programmed by Matthew Gross" << endl <<
    "mattkgross@gmail.com" << endl;
    
    return EXIT_SUCCESS;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void callimage(char* name, size_t width, size_t height)
{
    readimagefile(name, 0, 0, width, height);
    unsigned int storage_size = imagesize(0, 0, width, height);
    getimage(0, 0, width-1, height-1, malloc(storage_size));
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
bool checkgrey(size_t i, size_t j, coloring change, int x)
{
    int color, red, green, blue;
    color = getpixel(i, j);
    red = RED_VALUE(color); 
    green = GREEN_VALUE(color); 
    blue = BLUE_VALUE(color);
    
    if((red - change.red >= x) && (green - change.green >= x) &&
       (blue - change.blue >= x))
		return true;
    return false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
bool checkpixel(size_t i, size_t j)
{
    int color = getpixel(i,j);
    if(color == 0)
		return true;
    return false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void findgrey(size_t width, size_t height, vector<bool>& newimg, int x)
{
    vector<coloring> colorgrid(width*height);

    // Gets and stores the rgb of each pixel of the image.
    size_t i, j;
    int color;
    int count = 0;
    bool a, b, c, d;
    for(i = 0; i < width; ++i)
    {
		for(j = 0; j < height; ++j)
		{
			color = getpixel(i, j);
			colorgrid.at(count).red = RED_VALUE(color);
			colorgrid.at(count).green = GREEN_VALUE(color);
			colorgrid.at(count).blue = BLUE_VALUE(color);
			++count;
		}
    }

    // Checks gray scale and decides which pixels to change black/white.
    count = 0;
    for(i = 0; i < width; ++i)
    {
		for(j = 0; j < height; ++j)
		{
			// Avoid them damn side and corner errors.
			if(i > 0)
				a = checkgrey(i-1, j, colorgrid.at(count), x);
			if(i < width-1)
				b = checkgrey(i+1, j, colorgrid.at(count), x);
			if(j > 0)
				c = checkgrey(i, j-1, colorgrid.at(count), x);
			if(j < height-1)
				d = checkgrey(i, j+1, colorgrid.at(count), x);

			if(a || b || c || d)
				newimg.at(count) = true;
			else
				newimg.at(count) = false;
			++count;
		}
    }

    // Changes image to black (true) and white (false) pixels.
    count = 0;
    for(i = 0; i < width; ++i)
    {
		for(j = 0; j < height; ++j)
		{
			if(newimg.at(count))
			putpixel(i, j, BLACK);
			else
			putpixel(i, j, WHITE);
			++count;
		}
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void killstray(size_t width, size_t height)
{
    vector<bool> bow(width*height), newimg(width*height);
    size_t i, j;
    int count = 0;

    // Create an array with the value of each pixel (true = balck)
    // (false = white) in it.
    for(i = 0; i < width; ++i)
    {
	for(j = 0; j < height; ++j)
	{
	    bow.at(count) = checkpixel(i,j);
	    ++count;
	}
    }

    // Analyze each pixel
    bool a, b, c, d, e, f, g, h;
    count = 0;
    for(i = 0; i < width; ++i)
    {
	for(j = 0; j < height; ++j)
	{
	    // If pixel is black, check it's neighbors.
	    if(bow.at(count))
	    {
		// Avoid them damn side and corner errors.
		if(i > 0 && j > 0)
		    a = checkpixel(i-1, j-1);
		if(i > 0 && j < height-1)
		    b = checkpixel(i-1, j+1);
		if(i < width - 1 && j > 0)
		    c = checkpixel(i+1, j-1);
		if(i < width - 1 && j < height-1)
		    d = checkpixel(i+1, j+1);
		if(i > 0)
		    e = checkpixel(i-1, j);
		if(i < width-1)
		    f = checkpixel(i+1, j);
		if(j > 0)
		    g = checkpixel(i, j-1);
		if(j < height-1)
		    h = checkpixel(i, j+1);
		
		// If all of it's neighbors are white, it is now white.
		if(!a && !b && !c && !d && !e && !f && !g && !h)
		    bow.at(count) = false;
	    }
	    ++count;
	}
    }

    // Changes image to black (true) and white (false) pixels.
    count = 0;
    for(i = 0; i < width; ++i)
    {
	for(j = 0; j < height; ++j)
	{
	    if(bow.at(count))
		putpixel(i, j, BLACK);
	    else
		putpixel(i, j, WHITE);
	    ++count;
	}
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void setimage(size_t width, size_t height)
{
    unsigned int storage_size = imagesize(0, 0, width, height);
    getimage(0, 0, width-1, height-1, malloc(storage_size));
    writeimagefile(NULL, 0, 0, width, height);
}
//----------------------------------------------------------------------------
