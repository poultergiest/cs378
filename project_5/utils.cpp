#include "utils.h"
#include <math.h>



//--------------------------itos------------------------------------
//	converts an integer to a string
//------------------------------------------------------------------		
string itos(int arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}


//--------------------------ftos------------------------------------
//	converts a float to a string
//------------------------------------------------------------------		
string ftos(float arg)
{
    ostringstream buffer;
	
	//send the int to the ostringstream
    buffer << arg;	
	
	//capture the string
    return buffer.str();		
}
//-------------------------------------Clamp()-----------------------------------------
//
//	clamps the first argument between the second two
//
//-------------------------------------------------------------------------------------
void Clamp(double &arg, double min, double max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
}


bool init_app(const char * name, SDL_Surface * icon, uint32_t flags)
{
  atexit(SDL_Quit);
  if(SDL_Init(flags) < 0)
    return 0;

  SDL_WM_SetCaption(name, name);
  SDL_WM_SetIcon(icon, NULL);

  return 1;
}

void init_data(struct rgbData data[][WIDTH])
{
  memset(data, 255, WIDTH*HEIGHT*sizeof(rgbData));
}

void render(SDL_Surface * sf)
{
  SDL_Surface * screen = SDL_GetVideoSurface();
  if(SDL_BlitSurface(sf, NULL, screen, NULL) == 0)
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void setPixel(struct rgbData data[][WIDTH], int x, int y, struct rgbData color) {
  if (x < 0 || x >= WIDTH) return;
  if (y < 0 || y >= HEIGHT) return;
  data[y][x] = color;
}

void colorBG(struct rgbData data[][WIDTH], struct rgbData color) {
  for(int x = 0; x < WIDTH; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      setPixel(data, x, y, color);
    }
  }
}

void drawcircle(struct rgbData data[][WIDTH], int x0, int y0, int radius, rgbData color) {
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
 
  setPixel(data,x0, y0 + radius, color);
  setPixel(data,x0, y0 - radius, color);
  setPixel(data,x0 + radius, y0, color);
  setPixel(data,x0 - radius, y0, color);
 
  while(x < y) {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    setPixel(data,x0 + x, y0 + y,color);
    setPixel(data,x0 - x, y0 + y,color);
    setPixel(data,x0 + x, y0 - y,color);
    setPixel(data,x0 - x, y0 - y,color);
    setPixel(data,x0 + y, y0 + x,color);
    setPixel(data,x0 - y, y0 + x,color);
    setPixel(data,x0 + y, y0 - x,color);
    setPixel(data,x0 - y, y0 - x,color);
  }
}
