#include <iostream>
#include <vector>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <cassert>
#include <pthread.h>
#include <fstream>
#include <queue>

using namespace std;

#define NUM_THREADS 40
#define NUM_OPS 345



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

class state {
public:
	int data;
	bool start;
};
state global;


void *ThreadProc(void *threadid)
{
	long tid;
	tid = (long)threadid;
	timespec start, end, res;

	while(!global.start) {
		asm("nop"); //dummy work that can't be compiled away
	}

	int cur_ops = 0;

	int strong_work = NUM_OPS / NUM_THREADS;
	int weak_work = NUM_OPS;

	clock_gettime(CLOCK_MONOTONIC, &start);
	while(cur_ops < strong_work) {
		if(global.data < NUM_OPS) {
			cur_ops++;
		}	
	}
	clock_gettime(CLOCK_MONOTONIC, &end);

	res = diff(start, end);
	pthread_exit(NULL);
}

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


class Node {
public:
	Node() {
		label = 0;
	}

	Node(int l, int x, int y) {
		label = l;
		pos.x = x;
		pos.y = y;
	}

	Node(const Node& cpy) {
		label = -1;
		pos.x = cpy.pos.x;
		pos.y = cpy.pos.y;
	}

	int dist;
	int label;
	int prev;
	COORD pos;
};

class AdjGraph {
	int _size;
	vector<vector<int> > _g;
	vector<Node> _nodes;
public:
	AdjGraph(int n) {
		_size = n;
		_nodes.resize(_size);
		_g.resize(_size);

		for(int i = 0; i < _size; ++i) {
			_g[i].resize(_size);
			_nodes[i].dist = INT_MAX;
			_nodes[i].label = i;
			_nodes[i].prev = -1;
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

	int getEdge(int node1, int node2) {
		return _g[node1][node2];
	}

	void setEdge(int node1, int node2, int value) {  // makes undirected graphs
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

		_nodes.push_back(Node(_size-1, x, y));

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

queue<int> Q;

int sssp(AdjGraph g, int source, int target) {
	g.getNode(source).dist = 0;
	Q.push(source);

	while(!Q.empty()) {
		int cur_ind = Q.front();
		Q.pop();
		vector<int> nbors = g.getNeighbors(cur_ind);

		for (int i = 0; i < (int)nbors.size(); ++i)
		{
			int n_ind = nbors[i];
			int new_dist = g.getNode(n_ind).dist + g.getEdge(cur_ind, n_ind);

			if(new_dist < g.getNode(n_ind).dist) {
				g.getNode(n_ind).dist = new_dist;
				Q.push(n_ind);
			}
		}
	}
	return g.getNode(target).dist;
}


int main(int argc, char * argv[]) {
	pthread_t threads[NUM_THREADS];
	int ret = 0;

	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int t;
	global.start = false;

	for(t = 0; t < NUM_THREADS; ++t) {
	//printf("In main: creating thread %d\n", t);
		ret = pthread_create(&threads[t], &attr, ThreadProc, (void*)t);
		if(ret) {
			printf("Error: %d\n", ret);

			return -1;
		}
	}
	pthread_attr_destroy(&attr);
	global.start = true;
	for(t=0; t < NUM_THREADS; ++t) {
		ret = pthread_join(threads[t], NULL);
		if(ret) {
			printf("JOIN ERROR: %d\n", ret);
			return -1;
		}
	}

	printf("Threads completed: %d\n\nWriting runtimes\n", NUM_THREADS);

	ofstream file;
	file.open("datastuffthing.txt", ios::out | ios::app);
	file << "hey" << endl;
	file.close();

	pthread_exit(NULL);
}
