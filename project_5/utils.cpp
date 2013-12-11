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