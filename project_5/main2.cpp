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

int main(int argc, char **argv) {
	bool ok =
	init_app("SDL example", NULL, SDL_INIT_VIDEO) &&
	SDL_SetVideoMode(WIDTH, HEIGHT, 24, SDL_HWSURFACE);

	assert(ok);

	int i = 0;

	while(i < 100000) {
		i++;
	}

	return 0;
}