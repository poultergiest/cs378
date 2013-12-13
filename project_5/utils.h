#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <SDL/SDL.h>

const int WIDTH = 512;
const int HEIGHT = 512;


using namespace std;

typedef int OutCode;

//----------------------------------------------------------------------------
//	some random number functions.
//----------------------------------------------------------------------------

//returns a random integer between x and y
inline int	  RandInt(int x,int y) {return rand()%(y-x+1)+x;}

//returns a random float between zero and 1
inline double RandFloat()		   {return (rand())/(RAND_MAX+1.0);}

//returns a random bool
inline bool   RandBool()
{
	if (RandInt(0,1)) return true;

	else return false;
}

//returns a random float in the range -1 < n < 1
inline double RandomClamped()	   {return RandFloat() - RandFloat();}


//-----------------------------------------------------------------------
//	
//	some handy little functions
//-----------------------------------------------------------------------

//converts an integer to a std::string
string itos(int arg);

//converts an float to a std::string
string ftos (float arg);

//	clamps the first argument between the second two
void Clamp(double &arg, double min, double max);


// SDL HELPERS
bool init_app(const char * name, SDL_Surface * icon, uint32_t flags);
void init_data(struct rgbData data[][WIDTH]);
void drawcircle(struct rgbData data[][WIDTH], int x0, int y0, int radius, rgbData color);
void setPixelUnsafe(struct rgbData data[][WIDTH], int x, int y, struct rgbData color);
void drawchar(struct rgbData data[][WIDTH], int x0, int y0, char d, struct rgbData color);
void drawstring(struct rgbData data[][WIDTH], int x0, int y0, const char* d, struct rgbData color);
void drawlineUnsafe(rgbData data[][WIDTH], int x1, int y1, int x2, int y2, struct rgbData color);
OutCode ComputeOutCode(int x, int y);
void drawline(rgbData data[][WIDTH], int x0, int y0, int x1, int y1, struct rgbData color);


/////////////////////////////////////////////////////////////////////
//
//	Point structure
//
/////////////////////////////////////////////////////////////////////
struct SPoint
{
	float x, y;
	
	SPoint(){}
	SPoint(float a, float b):x(a),y(b){}
};

struct rgbData {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

#endif