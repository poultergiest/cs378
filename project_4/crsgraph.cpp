#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <pthread.h>

using namespace std;

#define INFINITY (INT_MAX-50000)

struct Edge {
	int src;
	int dest;
	int length;
};

bool EdgeComparator(Edge e1, Edge e2) {
	if(e1.src == e2.src) return e1.dest < e2.dest;
	return e1.src < e2.src;
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

class CrsGraph {
	int _size;
	vector<int> label;
	vector<Node> _nodes;
	vector<int> ptr;
	vector<Edge> edges;
public:
	CrsGraph(int n) {
		_size = n;
		label.resize(_size);
		_nodes.resize(_size);
		ptr.resize(_size);
		edges.reserve(733846);

		for(int i = 0; i < _size; ++i) {
			label[i] = i;
			_nodes[i].dist = INFINITY;
			_nodes[i].label = i;
		}
	}

	void sortEdges() {
		sort(edges.begin(), edges.end(), EdgeComparator);
		int index = 0;
		for(int i = 0; i < _size; ++i) {
			while(index < _size && edges[index].src <= i) index++;
			ptr[i] = index;
		}
	}

	void printEdges() {
		for(int i = 0; i < (int) edges.size(); ++i) {
			cout << i << " edge src: " << edges[i].src << " dest: " << edges[i].dest << endl;
		}
	}

	int getDist(int node1) {
		return _nodes[node1].dist;
	}

	void setDist(int node1, int value) {  // makes undirected graphs
		_nodes[node1].dist = value;
	}

	bool hasEdge(int node1, int node2) {
		for(int i = 0; i < (int) edges.size(); ++i) {
			if(edges[i].src == node1 && edges[i].dest == node2) return true;
		}
		return false;
	}

	int getEdge(int node1, int node2) {
		for(int i = 0; i < (int) edges.size(); ++i) {
			if(edges[i].src == node1 && edges[i].dest == node2) return edges[i].length;
		}
		assert(false);
	}

	void setLabel(int node, int value) { label[node] = value; }
	int getLabel(int node) { return label[node]; }

	vector<int> getNeighbors(int node) {
		int start = 0;
		if(node > 0) {
			start = ptr[node-1];
		}
		int end = ptr[node];
		vector<int> n(end-start);
		for(int i = 0; i < end-start; ++i) n[i] = edges[i+start].dest;
		return n;
	}

	int getSize() { return _size; }

	void addNode() {
		_size++;
		label.push_back(INT_MAX);
		ptr.resize(_size);
	}

	Node& getNode(int node) {
                return _nodes[node];
        }

	void getNodeLock(int node) {
                _nodes[node].getLock();
        }

        void releaseNodeLock(int node) {
                _nodes[node].releaseLock();
        }

	void addEdge(int node1, int node2, int len, bool sort = true) {
		//if(hasEdge(node1, node2)) return;
		Edge temp;
		temp.src = node1;
		temp.dest = node2;
		temp.length = len;
		edges.push_back(temp);
		if(sort)
			sortEdges();
	}

	void printGraph() {
		int index = 0;
		for(int i = 0; i < _size; i++) {
			for(int j = 0; j < _size; j++) {
				if(index >= (int) edges.size()) {
					cout << "0 ";
					continue;
				}
				if(edges[index].src == i && edges[index].dest == j) {
					cout << edges[index].length << " ";
					index++;
					continue;
				}
				cout << "0 ";
			}
			cout << endl;
		}
	}

	void printPtr() {
		for(int i = 0; i < (int) ptr.size(); i++) {
			cout << i << ": " << ptr[i] << endl;
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

int sssp(CrsGraph& g, int source, int target) {
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

		vector<int> nbors = g.getNeighbors(cur_node.label);

		//get locks for all neighbors
		g.getNodeLock(cur_node.label);
		//g.getNeighborLocks(nbors);
		//cout << "here" << endl;
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
		//g.releaseNeighborLocks(nbors);
		g.releaseNodeLock(cur_node.label);
	}
	return g.getNode(target).dist;
}

CrsGraph setupGraphFromFile(ifstream& file) {
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

	CrsGraph graph(size);

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
			graph.addEdge(node1-1, node2-1, length_between_nodes, false);
		} else {
			cout << "wtf: " << line << endl;
			break;
		}
		edges++;
	}
	cout << "edges: " << edges << endl;
	graph.sortEdges();
	return graph;
}

CrsGraph setupRingGraph(int ring_size) {
	CrsGraph graph(ring_size);
	for(int i = 0; i < ring_size-1; ++i) {
		graph.addEdge(i, i+1, 5, false);
	}
	graph.addEdge(ring_size-1, 0, 5, true);
	return graph;
}

int main() {
	ifstream map_file("map.gr");
	CrsGraph graph = setupGraphFromFile(map_file);
	//CrsGraph graph = setupRingGraph(10);
	//graph.printGraph();
	int x = sssp(graph, 1, 2);
	cout << "sssp: " << x << endl;
	return 0;
}
