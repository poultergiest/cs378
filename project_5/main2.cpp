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


//------------------------- EventFilter --------------------------------
//
//	The SDL event handler for keystrokes and mouse clicks
//-----------------------------------------------------------------------
static void EventFilter(const SDL_Event * event, SDL_Surface * data_sf)
{ 
	if (event->type == SDL_QUIT)
		doExit = true;
	if (event->type == SDL_KEYUP) {
		switch( event->key.keysym.sym )
                {
                    case SDLK_ESCAPE:
                    case SDLK_q:
                    	doExit = true;
                    	break;
                    case SDLK_f:
                    	g_pController->FastRenderToggle();
                    	break;
                    case SDLK_r:
                    	if (g_pController) {
                    		delete g_pController;
                    	}
		        g_pController = new CController(data_sf);
                    	break;
                }
	}
}


// Used to initilize our controller
void init(SDL_Surface * data_sf) {
	//seed the random number generator
	srand(time(NULL));

	//setup the controller
	g_pController = new CController(data_sf);
}

int main(int argc, char **argv) {
	struct rgbData buffer[HEIGHT][WIDTH];
	bool ok =
		init_app("Mine Sweepers", NULL, SDL_INIT_VIDEO) &&
		SDL_SetVideoMode(WIDTH, HEIGHT, 24, SDL_HWSURFACE);
	assert(ok);

	SDL_Surface * data_sf = SDL_CreateRGBSurfaceFrom((char*)buffer, WIDTH, HEIGHT, 24, WIDTH * 3, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

	init_data(buffer);
	init(data_sf); // for the controller and other buffers

	//create a timer
	CTimer timer(CParams::iFramesPerSecond);

	//start the timer
	timer.Start();

	// Enter the message loop
	bool bDone = false;
	rgbData white = {255,255,255};

	while (!bDone) {
		while( SDL_PollEvent( &event ) )
        	{
        		EventFilter(&event, data_sf);
	        	if(doExit) bDone = true;
        	}

		if (timer.ReadyForNextFrame() || g_pController->FastRender())
		{	
			if(!g_pController->Update())
			{
				//we have a problem, end app
				bDone = true;
			}

			//render
			//colorBG(buffer, white);
			// g_pController->Render(buffer);
			// render(data_sf);
		}
	}
	SDL_FreeSurface( data_sf );
	Cleanup();
	return 0;
}
