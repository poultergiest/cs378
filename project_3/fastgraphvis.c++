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
		x = 0;
		y = 0;
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

// implement quad tree
class QTree {
public:
	QTree* q1;
	QTree* q2;
	QTree* q3;
	QTree* q4;

	int level;

	COORD pos; // top left corner
	int width;
	int height;

	COORD force;

	vector<Node*> points;  // points bounded by this tree

	Node approximatedNode;

	QTree(int x, int y, int w, int h, int l, vector<Node*> p) {
		q1 = q2 = q3 = q4 = NULL;
		pos.x = x;
		pos.y = y;
		height = h;
		width = w;
		level = l;
		for(int i = 0; i < (int) p.size(); ++i) {
			if(p[i]->pos.x >= x && p[i]->pos.y >= y && p[i]->pos.x < x + w && p[i]->pos.y < y + h) {
				points.push_back(p[i]);
			}
		}

		if(points.size() > 1) {
			int half_width = w / 2;
			int half_height = h / 2;
			q1 = new QTree(pos.x + half_width, pos.y, half_width, half_height, l+1, points);
			if (q1->getSize() == 0) {
				delete q1;
				q1 = NULL;
			}
			q2 = new QTree(pos.x, pos.y, half_width, half_height, l+1, points);
			if (q2->getSize() == 0) {
				delete q2;
				q2 = NULL;
			}
			q3 = new QTree(pos.x, pos.y + half_height, half_width, half_height, l+1, points);
			if (q3->getSize() == 0) {
				delete q3;
				q3 = NULL;
			}
			q4 = new QTree(pos.x+ half_width, pos.y + half_height, half_width, half_height, l+1, points);
			if (q4->getSize() == 0) {
				delete q4;
				q4 = NULL;
			}
		}
	}

	~QTree() {
		if(q1 != NULL) {
			delete q1;
		}
		if(q2 != NULL) {
			delete q2;
		}
		if(q3 != NULL) {
			delete q3;
		}
		if(q4 != NULL) {
			delete q4;
		}
	}

	// approximatedNode computeApproximatedNode() {
	// 	node n1; 
	// 	if(q1 != null);
	// 	getApproximatedNode(q1);
	// 	getApproximatedNode(q2);
	// 	getApproximatedNode(q3);
	// 	getApproximatedNode(q4);

	// 	approximatedNode = averageNode(q1,q2, q3, q4);
	// 	return approximatedNode;
	// }

	// Node GetNode() {
	// 	return computeApproximatedNode();
	// }

	// void computeCuolombs(vector<forces> forces) { // vector of size = total number of nodes

	// }

	// void computeCuolombsForce(node n1, node n2) {

	// }

	void DrawQTree(struct rgbData data[][WIDTH]) {
		rgbData green = {0,255,0};
		//draw self.
		drawline(data, pos.x, pos.y, pos.x+width, pos.y, green);
		drawline(data, pos.x, pos.y, pos.x, pos.y+height, green);
		drawline(data, pos.x+width, pos.y, pos.x+width, pos.y+height, green);
		drawline(data, pos.x, pos.y+height, pos.x+width, pos.y+height, green);

		//draw children;
		if(q1 != NULL) {
			q1->DrawQTree(data);
		}

		if(q2 != NULL) {
			q2->DrawQTree(data);
		}

		if(q3 != NULL) {
			q3->DrawQTree(data);
		}

		if(q4 != NULL) {
			q4->DrawQTree(data);
		}
	}

	int getSize() {
		return (int) points.size();
	}

	void computeApproximatedNode() {
		if(points.size() == 1) {
			approximatedNode = Node(*points[0]);
			return;
		}
		for(int i = 0; i < (int) points.size(); ++i) {
			//approximatedNode.pos.x += 
		}

	}

};


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
		if(gdistance(zero, force) > 2) {
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

static bool renderGraph(struct rgbData data[][WIDTH], unsigned width, unsigned height, AdjGraph& g, QTree& qt, int which, bool forces)
{
	for(SDL_Event event; SDL_PollEvent(&event);)
		if(event.type == SDL_QUIT) return 0;

	rgbData black = {0,0,0};
	rgbData white = {255,255,255};
	rgbData red = {255,0,0};
	rgbData green = {0,255,0};
	rgbData blue = {0,0,255};
	rgbData color = {200,70,110};

	colorBG(data, black); 
	// DRAW EDGES
	for(int i = 0; i < g.getSize(); ++i) {
		COORD src = g.getCoord(i);
		vector<int> neighbors = g.getNeighbors(i);
		for(int j = 0; j < (int) neighbors.size(); ++j) {
			COORD dest = g.getCoord(neighbors[j]);
			drawline(data, src.x, src.y, dest.x, dest.y, blue);
		}
	}

	// DRAW NODES
	for(int i = 0; i < g.getSize(); ++i) {
		COORD node = g.getCoord(i);
		drawcircle(data, node.x, node.y, 10, red);
		char buffer[4];
		snprintf(buffer, sizeof(buffer), "%d", i);
		drawstring(data, node.x-2, node.y-2, buffer, white);
	}


	qt.DrawQTree(data);
	return 1;
}

AdjGraph setupHalfConnectedGraph(int s) {
	AdjGraph g(s);
	int size = s;
	int half = (size / 2) + 1;
	for(int i = 0; i < size; ++i) {
		g.setCoord(i, rand()%WIDTH, rand()%HEIGHT);
		for(int j = 0; j < half; ++j) {
			g.setEdge(i, rand() % size, 1);
		}
		g.setEdge(i,i,0);
	}
	return g;
}

int main(int argc, char **argv) {

	//Initialize visualization
	struct rgbData buffer[HEIGHT][WIDTH];

	bool ok =
	init_app("SDL example", NULL, SDL_INIT_VIDEO) &&
	SDL_SetVideoMode(WIDTH, HEIGHT, 24, SDL_HWSURFACE);

	assert(ok);

	SDL_Surface * data_sf = 
	SDL_CreateRGBSurfaceFrom((char*)buffer, WIDTH, HEIGHT, 24, WIDTH * 3,
		0x000000FF, 0x0000FF00, 0x00FF0000, 0);

	SDL_SetEventFilter(filter);

	init_data(buffer);

	int rs = 20;
	AdjGraph ring(rs);
	ring = setupHalfConnectedGraph(rs);

	QTree* qt = new QTree(0,0, 1024, 1024, 1, ring.GetNodePointers());


	bool equalized = false;
	while (!doExit) {
		//apply laws
		if(!equalized) {
			equalized = apply_forces(ring);
			delete qt;
			qt = new QTree(0,0, 1024, 1024, 1, ring.GetNodePointers());
		}
		//render
		renderGraph(buffer, WIDTH, HEIGHT, ring, *qt, 0, false);
		render(data_sf);
		SDL_Delay(1000/30);
	}
	delete qt;
	return 0;
}
