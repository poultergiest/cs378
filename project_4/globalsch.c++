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

#define NUM_THREADS 5
#define SIZE 20

#define INFINITY (INT_MAX-50000)

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

class Node {
public:
	Node() {
		pthread_mutex_init(&mutex, NULL);
		label = 0;
	}

	Node(int l, int x, int y) {
		pthread_mutex_init(&mutex, NULL);
		label = l;
	}

	Node(const Node& cpy) {
		label = cpy.label;
		dist = cpy.dist;
		pthread_mutex_init(&mutex, NULL);
	}

	~Node() {
		pthread_mutex_destroy(&mutex);
	}

	void getLock() {
		pthread_mutex_lock(&mutex);
	}

	void releaseLock() {
		pthread_mutex_unlock(&mutex);
	}

	int dist;
	int label;
	pthread_mutex_t mutex;
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
			_nodes[i].dist = INFINITY;
			_nodes[i].label = i;
		}
	}

	Node& getNode(int node) {
		return _nodes[node];
	}

	int getEdge(int node1, int node2) {
		return _g[node1][node2];
	}

	void setEdge(int node1, int node2, int value) {
		_g[node1][node2] = value;
	}

	int getDist(int node1) {
		return _nodes[node1].dist;
	}

	void setDist(int node1, int value) {  // makes undirected graphs
		_nodes[node1].dist = value;
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
			if(i == node) continue;
			if(_g[node][i] > 0) {
				n.push_back(i);
			}
		}
		return n;
	}

	void getNodeLock(int node) {
		_nodes[node].getLock();
	}

	void releaseNodeLock(int node) {
		_nodes[node].releaseLock();
	}

	void getNeighborLocks(vector<int> nbors) {
		for(int i = 0; i < (int) nbors.size(); ++i) {
			assert(nbors[i] >= 0 && nbors[i] < _size);
			_nodes[nbors[i]].getLock();
		}
	}

	void releaseNeighborLocks(vector<int> nbors) {
		for(int i = (int)nbors.size()-1; i >= 0; --i) {
			/*cout << "index " << nbors[i] << "size " << (int)nbors.size() << endl;*/
			assert(nbors[i] >= 0 && nbors[i] < _size);
			_nodes[nbors[i]].releaseLock();
		}
	}

	void print() {
		for(int x = 0; x < _size; ++x) {
			for(int y = 0; y < _size; ++y) {
				cout << getEdge(x,y) << " ";
			}
			cout << endl;
		}
	}
};

class nodeComparison {
public:
	bool operator() (const Node& lhs, const Node& rhs) const {
		return lhs.dist > rhs.dist;
	}
};

typedef priority_queue<Node, vector<Node>, nodeComparison> node_priority_queue;

class thread_safe_node_queue {
public:
	node_priority_queue _Q2;
	pthread_mutex_t lock;

};
thread_safe_node_queue work;

int sssp(AdjGraph& g, int source, int target) {
	g.setDist(source, 0);
	pthread_mutex_lock(&work.lock);
	Node& s = g.getNode(source);
	work._Q2.push(s);
	pthread_mutex_unlock(&work.lock);

	int counter = 0;

	while(true) {
		pthread_mutex_lock(&work.lock);
		if (work._Q2.empty()) {
			break;
			counter++;
			pthread_mutex_unlock(&work.lock);
			if(counter > 100) {
				break;
			}
			usleep(5000);
			continue;
		}

		counter = 0;
		Node cur_node = work._Q2.top();
		work._Q2.pop();
		pthread_mutex_unlock(&work.lock);
		cout << "here" << endl;
		
		vector<int> nbors = g.getNeighbors(cur_node.label);

		//get locks for all neighbors
		g.getNodeLock(cur_node.label);
		g.getNeighborLocks(nbors);
		cout << "here" << endl;
		for (int i = 0; i < (int) nbors.size(); ++i)
		{
			int n_ind = nbors[i];
			int new_dist = g.getDist(cur_node.label) + g.getEdge(cur_node.label, n_ind);

			if(new_dist < g.getDist(n_ind)) {
				g.setDist(n_ind, new_dist);

				Node& n = g.getNode(n_ind);
				pthread_mutex_lock(&work.lock);
				work._Q2.push(n);
				pthread_mutex_unlock(&work.lock);
			}
		}

		//release locks for all neighbors
		g.releaseNeighborLocks(nbors);
		cout << "here20" << endl;
		g.releaseNodeLock(cur_node.label);
	}
	return g.getNode(target).dist;
}

AdjGraph graph(0);

void *ThreadProc(void *threadid)
{
	long tid;
	tid = (long)threadid;

	cout << "hello from thread: " << tid << endl;

	timespec start, end, res;

	AdjGraph& g = graph;

	int counter = 0;

	clock_gettime(CLOCK_MONOTONIC, &start);
	while(true) {
		//cout << "hello from thread: " << tid << endl;
		pthread_mutex_lock(&work.lock);
		if (work._Q2.empty()) {
			counter++;
			pthread_mutex_unlock(&work.lock);
			if(counter > 500) {
				break;
			}
			usleep(5000);
			continue;
		}

		counter = 0;
		Node cur_node = work._Q2.top();
		work._Q2.pop();
		pthread_mutex_unlock(&work.lock);
		
		vector<int> nbors = g.getNeighbors(cur_node.label);

		//get locks for all neighbors
		g.getNodeLock(cur_node.label);
		/*g.getNeighborLocks(nbors);*/

		for (int i = 0; i < (int) nbors.size(); ++i)
		{
			int n_ind = nbors[i];
			g.getNodeLock(n_ind);
			int new_dist = g.getDist(cur_node.label) + g.getEdge(cur_node.label, n_ind);

			if(new_dist < g.getDist(n_ind)) {
				g.setDist(n_ind, new_dist);

				Node& n = g.getNode(n_ind);
				pthread_mutex_lock(&work.lock);
				work._Q2.push(n);
				pthread_mutex_unlock(&work.lock);
			}
			g.releaseNodeLock(n_ind);
		}

		//release locks for all neighbors
		/*g.releaseNeighborLocks(nbors);*/
		g.releaseNodeLock(cur_node.label);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);

	res = diff(start, end);
	pthread_exit(NULL);
}


AdjGraph setupHalfConnectedGraph(int s) {
	AdjGraph g(s);
	int size = s;
	int half = (size / 2) + 1;
	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < half; ++j) {
			g.setEdge(i, rand() % size, 5);
		}
		g.setEdge(i,i,0);
	}
	g.setEdge(0, 4, 0);
	return g;
}

AdjGraph circleGraph(int s) {
	AdjGraph ring(s);
	for(int n = 0; n < s-1; ++n) {
		ring.setEdge(n, n+1, 1);
	}
	ring.setEdge(s-1, 0, 1);
	return ring;
}

AdjGraph setupGraphFromFile(ifstream& file) {
	string line;
	
	while(getline(file, line))
	{
	    if(line[0] == 'p') break;
	}

	// Parses the number of nodes we need
	line = line.substr(5);
	unsigned pos = line.find(" ");
	line = line.substr(0, pos);

	int size = atoi(line.c_str());
	getline(file, line);getline(file, line);

	cout << "Graph node size is: " << size << endl;

	// TODO: REMOVE THIS
	size = 20;

	AdjGraph graph(size);

	int node1 = 0;
	int node2 = 0;
	int length_between_nodes = 0;
	int edges = 0;
	while(getline(file, line))
	{
		if(line[0] == 'a') {
			line = line.substr(2);
			node1 = atoi(line.c_str());
			line = line.substr(line.find(" ")+1);
			node2 = atoi(line.c_str());
			if(node1 < 1 || node2 < 1 || node1 >= size || node2 >= size) {
				continue;
			}
			line = line.substr(line.find(" ")+1);
			length_between_nodes = atoi(line.c_str());
			graph.setEdge(node1-1, node2-1, length_between_nodes);
		} else {
			cout << "wtf: " << line << endl;
			break;
		}
		edges++;
	}
	cout << "edges: " << edges << endl;
	return graph;
}

int main(int argc, char * argv[]) {
	pthread_t threads[NUM_THREADS];

	ifstream map_file("map.gr");
	graph = setupGraphFromFile(map_file);
	graph.print();
return 0;
	int ret = 0;
	int size = SIZE;
	//graph = circleGraph(size);
	graph = setupHalfConnectedGraph(size);

	graph.print();
	pthread_mutex_init(&work.lock, NULL);
	/*int v = sssp(graph, 0, 9);
	cout << "res: " << v << endl;*/
	/*return 0;*/

	Node& s = graph.getNode(0);
	s.dist = 0;
	work._Q2.push(s);
	//cout << "Result " << v << endl;

	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


	int t;
	

	for(t = 0; t < NUM_THREADS; ++t) {
	//printf("In main: creating thread %d\n", t);
		ret = pthread_create(&threads[t], &attr, ThreadProc, (void*)t);
		usleep(1000);
		if(ret) {
			printf("Error: %d\n", ret);

			return -1;
		}
	}
	pthread_attr_destroy(&attr);
	
	for(t=0; t < NUM_THREADS; ++t) {
		ret = pthread_join(threads[t], NULL);
		if(ret) {
			printf("JOIN ERROR: %d\n", ret);
			return -1;
		}
	}

	printf("Threads completed: %d\n\nWriting runtimes\n", NUM_THREADS);

	int dist = graph.getDist(size-1);

	cout << "Distance to target: " << dist << endl;

	// ofstream file;
	// file.open("datastuffthing.txt", ios::out | ios::app);
	// file << "hey" << endl;
	// file.close();

	pthread_mutex_destroy(&work.lock);
	
	//return 0;
	pthread_exit(NULL);
}
