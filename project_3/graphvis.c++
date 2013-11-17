#include <iostream>
#include <vector>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <cassert>
#include <SDL/SDL.h>
#include "drawing.h"

bool doExit = false;

using namespace std;

#define HOOKES_K  2
#define COULOMBS_K 15


class COORD {
public:
	COORD() {
		x=0; 
		y=0;
	}
	COORD(int i, int j) {
		x = i;
		y = j;
	}
	int x;
	int y;
};
typedef COORD FORCE;

class Node {
public:
	Node() {
		label = 0;
	}

	Node(int l, int x, int y, int fx, int fy) {
		label = l;
		pos.x = x;
		pos.y = y;
		force.x = fx;
		force.y = fy;
	}

	Node(const Node& cpy) {
		label = -1;
		pos.x = cpy.pos.x;
		pos.y = cpy.pos.y;
		force.x = cpy.force.x;
		force.y = cpy.force.y;
	}
	int label;
	COORD pos;
	FORCE force;
};

// insert graph
class AdjGraph {
	int _size;
	vector<vector<bool> > _g;
	vector<Node> _nodes;
public:
	AdjGraph(int n) {
		_size = n;
		_nodes.resize(_size);
		_g.resize(_size);

		for(int i = 0; i < _size; ++i) {
			_g[i].resize(_size);
			_nodes[i].label = i;
		}
	}

	Node& getNode(int node) {
		return _nodes[node];
	}

	vector<Node*> GetNodePointers() {
		vector<Node*> ret;

		for(int i =0; i < _size; ++i) {
			ret.push_back(&(_nodes[i]));
		}

		return ret;
	}

	bool getEdge(int node1, int node2) {
		return _g[node1][node2];
	}

	void setEdge(int node1, int node2, bool value) {  // makes undirected graphs
		_g[node1][node2] = value;
		_g[node2][node1] = value;
	}

	COORD getCoord(int node) {
		return _nodes[node].pos;
	}

	void setCoord(int node, int i, int j) {
		_nodes[node].pos.x = i;
		_nodes[node].pos.y = j;
	}

	vector<COORD> GetNodePos() {
		vector<COORD> v;

		for(int i = 0; i < _size; ++i) {
			v.push_back(_nodes[i].pos);
		}

		return v;
	}

	void addNode(int x, int y) {
		_size++;
		_g.resize(_size);

		_nodes.push_back(Node(_size-1, x, y, 0, 0));

		for(int i = 0; i < _size; ++i) {
			_g[i].resize(_size);
		}
	}

	vector<int> getNeighbors(int node) {
		vector<int> n;
		for(int i = 0; i < _size; ++i) {
			if(_g[node][i]) {
				n.push_back(i);
		}
		}
		return n;
	}

	int getSize() { return _size; }

	void print() {
		for(int x = 0; x < _size; ++x) {
			for(int y = 0; y < _size; ++y) {
				cout << getEdge(x,y) << " ";
			}
			cout << endl;
		}
	}

	void printNodeCoords(int node) {
		cout << node << ": (" << _nodes[node].pos.x << ", " << _nodes[node].pos.y << ")" << endl;
	}

	void printCoords() {
		for(int i = 0; i < _size; ++i) {
			printNodeCoords(i);
		}
	}
};



// implement forces
int gdistance(COORD n1, COORD n2) {
	int x = (n1.x - n2.x) * (n1.x - n2.x);
	int y = (n1.y - n2.y) * (n1.y - n2.y);
	return (int) sqrt(x+y);
}

// implement forces
void nodes_hookes_force(Node& src, Node& dest) {
	int d = gdistance(src.pos, dest.pos);

	if(d > 100) {
		src.force.x += (HOOKES_K * (dest.pos.x - src.pos.x)) / d;
		src.force.y += (HOOKES_K * (dest.pos.y - src.pos.y)) / d;
	}
}

void nodes_coulombs_force(Node& src, Node& dest) {
	int d = gdistance(src.pos, dest.pos);
	int rr = d*d;

	if(rr != 0 && d < 200) {
		src.force.x += -1 * (COULOMBS_K * COULOMBS_K * (dest.pos.x - src.pos.x) / (rr));
		src.force.y += -1 * (COULOMBS_K * COULOMBS_K * (dest.pos.y - src.pos.y) / (rr));
	}
}

bool apply_forces(AdjGraph& g) {
	for(int i = 0; i < g.getSize(); ++i) {
		Node& tsrc = g.getNode(i);
		for(int j = 0; j < g.getSize(); ++j) {
			if(i == j) continue;
			Node& tdest = g.getNode(j);
			// if connected, apply hookes.
			if(g.getEdge(i, j)) {
				nodes_hookes_force(tsrc, tdest);
			}
			// always apply coulombs
			nodes_coulombs_force(tsrc, tdest);
		}
	}

	bool changed = false;

	for(int n = 0; n < g.getSize(); ++n) {
		Node& tsrc = g.getNode(n);
		COORD zero(0,0);
		COORD force(tsrc.force.x, tsrc.force.y);
		if(gdistance(zero, force) > 3) {
			changed = true;
			tsrc.pos.x += tsrc.force.x;
			tsrc.pos.y += tsrc.force.y;
		}
		tsrc.force.x = 0;
		tsrc.force.y = 0;
	}
	return changed;
}

//Perform graph layout, Daniel Tunkelang style
static bool init_app(const char * name, SDL_Surface * icon, uint32_t flags)
{
  atexit(SDL_Quit);
  if(SDL_Init(flags) < 0)
    return 0;

  SDL_WM_SetCaption(name, name);
  SDL_WM_SetIcon(icon, NULL);

  return 1;
}

static void init_data(struct rgbData data[][WIDTH])
{
  memset(data, 255, WIDTH*HEIGHT*sizeof(rgbData));
}

static void render(SDL_Surface * sf)
{
  SDL_Surface * screen = SDL_GetVideoSurface();
  if(SDL_BlitSurface(sf, NULL, screen, NULL) == 0)
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

static int filter(const SDL_Event * event)
{ 
	if (event->type == SDL_QUIT)
		doExit = true;
	return event->type == SDL_QUIT;
}

static bool renderGraph(struct rgbData data[][WIDTH], unsigned width, unsigned height, AdjGraph& g, int which, bool forces)
{
	for(SDL_Event event; SDL_PollEvent(&event);)
		if(event.type == SDL_QUIT) return 0;

	rgbData black = {0,0,0};
	rgbData white = {255,255,255};
	rgbData red = {255,0,0};
	rgbData green = {0,255,0};
	rgbData blue = {0,0,255};
	rgbData color = {200,70,110};

	colorBG(data, white); 

	// DRAW NODES
	for(int i = 0; i < g.getSize(); ++i) {
		COORD node = g.getCoord(i);
		drawcircle(data, node.x, node.y, 10, red);
		char buffer[4];
		snprintf(buffer, sizeof(buffer), "%d", i);
		drawstring(data, node.x-2, node.y-2, buffer, black);
	}

	// DRAW EDGES
	for(int i = 0; i < g.getSize(); ++i) {
		COORD src = g.getCoord(i);
		vector<int> neighbors = g.getNeighbors(i);
		for(int j = 0; j < (int) neighbors.size(); ++j) {
			COORD dest = g.getCoord(neighbors[j]);
			drawline(data, src.x, src.y, dest.x, dest.y, black);
		}
	}
	return 1;
}

AdjGraph generateCircle() {
	AdjGraph circle(5);
	circle.setCoord(0, 450, 450);
	circle.setCoord(1, 470, 450);
	circle.setEdge(0, 1, 1);
	circle.setCoord(2, 470, 475);
	circle.setEdge(1, 2, 1);
	circle.setCoord(3, 450, 475);
	circle.setEdge(2, 3, 1);
	circle.setCoord(4, 430, 465);
	circle.setEdge(3, 4, 1);
	circle.setEdge(4, 0, 1);

	return circle;
}

AdjGraph generateCube() {
	AdjGraph cube(8);
	//bottom diamond
	cube.setCoord(0, 450, 450);
	cube.setCoord(1, 470, 470);
	cube.setEdge(0, 1, 1);
	cube.setCoord(2, 450, 490);
	cube.setEdge(1, 2, 1);
	cube.setCoord(3, 430, 470);
	cube.setEdge(2, 3, 1);
	cube.setEdge(3, 0, 1);
	//top diamond
	cube.setCoord(4, 450, 400);
	cube.setCoord(5, 470, 420);
	cube.setEdge(4, 5, 1);
	cube.setCoord(6, 450, 440);
	cube.setEdge(5, 6, 1);
	cube.setCoord(7, 430, 420);
	cube.setEdge(6, 7, 1);
	cube.setEdge(7, 4, 1);
	//connect top to bottom
	cube.setEdge(0, 4, 1);
	cube.setEdge(1, 5, 1);
	cube.setEdge(2, 6, 1);
	cube.setEdge(3, 7, 1);
	return cube;
}

AdjGraph generateGrid() {
	AdjGraph grid(64);
	int r = 8, c = 8, count = 0;
	int start_x = 400, start_y = 300; 
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {		
			grid.setCoord(i*r+j, start_x + j*20, start_y + i*20);
			if(j < c-1) {
				grid.setEdge(i*r+j, i*r+j+1, 1);
			}
			if(i > 0) {
				grid.setEdge(i*r+j, (i-1)*r+j, 1);
			}
			
			count++;
		}
			start_y += 20;
			start_x = 400;
		
	}
	return grid;
}

timespec diff(timespec start, timespec end)
{
  timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}


int main(int argc, char **argv) {

	//Initialize visualization
	struct rgbData buffer[HEIGHT][WIDTH];
	struct timespec start, stop;
	timespec res;

	bool ok =
	init_app("SDL example", NULL, SDL_INIT_VIDEO) &&
	SDL_SetVideoMode(WIDTH, HEIGHT, 24, SDL_HWSURFACE);

	assert(ok);

	SDL_Surface * data_sf = 
	SDL_CreateRGBSurfaceFrom((char*)buffer, WIDTH, HEIGHT, 24, WIDTH * 3,
	                     0x000000FF, 0x0000FF00, 0x00FF0000, 0);

	SDL_SetEventFilter(filter);

	init_data(buffer);
	
	/* Generate Graphs */
	//AdjGraph circle = generateCircle();
	AdjGraph cube = generateCube();
	//AdjGraph grid = generateGrid();

	bool changed = true;

	clock_gettime(CLOCK_MONOTONIC, &start);

	while (!doExit) {
		//apply laws
		if(changed) {
			changed = apply_forces(cube);
		}

		else {

			clock_gettime(CLOCK_MONOTONIC, &stop);
			res = diff(start,stop);
    		//break;

			/*Node n1 = circle.getNode(0);
			Node n2 = circle.getNode(1);
			int eq_dis = gdistance(n1.pos, n2.pos);
			cout << "Equalized Distance" << eq_dis << endl;*/

		}
		//apply_forces(cube);
		//apply_forces(grid);
		//cout << endl;
		//ring.printCoords();
		//render
		renderGraph(buffer, WIDTH, HEIGHT, cube, 0, false);
		render(data_sf);
		SDL_Delay(1000/30);
	}

	

	cout << "Total Run Time" <<endl;
	printf("Time: %zu %zu\n", res.tv_sec, res.tv_nsec);
	return 0;
}


/*
//for a ring
for(int n = 0; n < rs-1; ++n) {
		ring.setEdge(n, n+1, 1);
	}
	ring.setEdge(rs-1, 0, 1);

*/