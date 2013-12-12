// main2.cpp
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cassert>
#include <SDL/SDL.h>


#include "utils.h"
#include "CController.h"
#include "CTimer.h"
#include "resource.h"
#include "CParams.h"

using namespace std;

///////////////////////GLOBALS ////////////////////////////////////

//The controller class for this simulation
CController*	g_pController	 = NULL; 

//create an instance of the parameter class.
CParams   g_Params;

//---------------------------- Cleanup ----------------------------------
//
//	simply cleans up any memory issues when the application exits
//-----------------------------------------------------------------------
void Cleanup()
{
	if (g_pController)
		delete g_pController;
}

bool doExit = false;
SDL_Event event;

static int filter(const SDL_Event * event)
{ 
	if (event->type == SDL_QUIT)
		doExit = true;
	return event->type == SDL_QUIT;
}

int main(int argc, char **argv) {
	struct rgbData buffer[HEIGHT][WIDTH];
	bool ok =
		init_app("Sweeper", NULL, SDL_INIT_VIDEO) &&
		SDL_SetVideoMode(WIDTH, HEIGHT, 24, SDL_HWSURFACE);
	assert(ok);

	SDL_Surface * data_sf = SDL_CreateRGBSurfaceFrom((char*)buffer, WIDTH, HEIGHT, 24, WIDTH * 3, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

	SDL_SetEventFilter(filter);

	init_data(buffer);

	//create a timer
	CTimer timer(CParams::iFramesPerSecond);

	//start the timer
	timer.Start();

	// Enter the message loop
	bool bDone = false;

	while (!bDone) {
		while( SDL_PollEvent( &event ) )
        	{
        		filter(&event);
	        	if(doExit) bDone = true;
        	}

		if (timer.ReadyForNextFrame() /*|| g_pController->FastRender()*/)
		{	
			if(!g_pController->Update())
			{
				//we have a problem, end app
				bDone = true;
			}

			//render
			render(data_sf);
		}
		
		//SDL_Delay(1000/30);
	}

	Cleanup();
	return 0;
}
