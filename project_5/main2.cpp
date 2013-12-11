// main2.cpp
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cassert>
#include <SDL/SDL.h>


#include "utils.h"
//#include "CController.h"
//#include "CTimer.h"
#include "resource.h"
#include "CParams.h"

using namespace std;

bool doExit = false;

static int filter(const SDL_Event * event)
{ 
	if (event->type == SDL_QUIT)
		doExit = true;
	return event->type == SDL_QUIT;
}

int main(int argc, char **argv) {
	struct rgbData buffer[HEIGHT][WIDTH];
	bool ok =
		init_app("SDL example", NULL, SDL_INIT_VIDEO) &&
		SDL_SetVideoMode(WIDTH, HEIGHT, 24, SDL_HWSURFACE);
	assert(ok);

	SDL_Surface * data_sf = SDL_CreateRGBSurfaceFrom((char*)buffer, WIDTH, HEIGHT, 24, WIDTH * 3, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

	SDL_SetEventFilter(filter);

	init_data(buffer);

	while (!doExit) {
		//render
		render(data_sf);
		SDL_Delay(1000/30);
	}

	return 0;
}