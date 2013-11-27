#include <algorithm>
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

#define MAX_THREADS 61
#define SIZE 20

#define INF (INT_MAX-50000)

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
                        _nodes[i].dist = INF;
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

        void reset() {
            for(int i = 0; i < _nodes.size(); ++i) {
                _nodes[i].dist = INF;
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
                                        cout << "1 ";
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
thread_safe_node_queue per_thread_work[MAX_THREADS];

bool still_working[MAX_THREADS];

timespec runtimes[MAX_THREADS];

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
                        if(counter > 200) {
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

void initRTS() {
    for(int i = 0; i < MAX_THREADS; ++i) {
        runtimes[i].tv_nsec = 0;
        runtimes[i].tv_sec = 0;
        still_working[i] = true;
        pthread_mutex_init(&per_thread_work[i].lock, NULL);
    }
}

void destroyLocks() {
    for(int i = 0; i < MAX_THREADS; ++i) {
        pthread_mutex_destroy(&per_thread_work[i].lock);
    }
}


CrsGraph graph(0);

void *ThreadProc(void *threadid)
{
	long tid;
	tid = (long)threadid;
	timespec start, end, res;

    thread_safe_node_queue local;

	CrsGraph& g = graph;

	int counter = 0;

	clock_gettime(CLOCK_MONOTONIC, &start);
	while(true) {
		//cout << "hello from thread: " << tid << endl;
		pthread_mutex_lock(&work.lock);
		if (work._Q2.empty()) {
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

        g.getNodeLock(cur_node.label);
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
        g.releaseNodeLock(cur_node.label);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);

	res = diff(start, end);
    runtimes[tid].tv_sec = res.tv_sec;
    runtimes[tid].tv_nsec = res.tv_nsec;
	pthread_exit(NULL);
}

int CUR_THREADS = 0;

void *ThreadProc2(void *threadid)
{
    long tid;
    tid = (long)threadid;
    timespec start, end, res;

    CrsGraph& g = graph;

    clock_gettime(CLOCK_MONOTONIC, &start);
    while(true) {
        //cout << "hello from thread: " << tid << endl;
        pthread_mutex_lock(&(per_thread_work[tid].lock));
        
        if (per_thread_work[tid]._Q2.empty()) {
            if(CUR_THREADS == 1) {
                pthread_mutex_unlock(&(per_thread_work[tid].lock));
                break;
            }
            //steal
            bool stole = false;
            
            if(tid > 0 && still_working[tid-1]) {
                // guy to left has work, steal
                pthread_mutex_lock(&per_thread_work[tid-1].lock);
                if(per_thread_work[tid-1]._Q2.size() > 20) {
                    Node cur_node = per_thread_work[tid-1]._Q2.top();
                    Node& n = g.getNode(cur_node.label);
                    per_thread_work[tid-1]._Q2.pop();
                    per_thread_work[tid]._Q2.push(n);
                    stole = true;
                }
                pthread_mutex_unlock(&per_thread_work[tid-1].lock);
            }

            if(tid < CUR_THREADS && still_working[tid+1]) {
                // guy to left has work, steal
                pthread_mutex_lock(&per_thread_work[tid+1].lock);
                if(per_thread_work[tid+1]._Q2.size() > 20) {
                    Node cur_node = per_thread_work[tid+1]._Q2.top();
                    Node& n = g.getNode(cur_node.label);
                    per_thread_work[tid+1]._Q2.pop();
                    per_thread_work[tid]._Q2.push(n);
                    stole = true;
                }
                pthread_mutex_unlock(&per_thread_work[tid+1].lock);
            }

            if(stole) {
                still_working[tid] = true;
            } else {
                still_working[tid] = false;
                bool work_to_do = false;
                for(int i = 0; i < CUR_THREADS; ++i) {
                    if(still_working[i]) {
                        work_to_do = true;
                        pthread_mutex_unlock(&per_thread_work[tid].lock);
                        break;
                    }
                }
                pthread_mutex_unlock(&per_thread_work[tid].lock);
                if(work_to_do) {
                    usleep(5000);
                    continue;
                } else {
                    break;
                }
            }
            // end steal
            pthread_mutex_unlock(&per_thread_work[tid].lock);\
        }

        still_working[tid] = true;
        Node cur_node = per_thread_work[tid]._Q2.top();
        per_thread_work[tid]._Q2.pop();
        pthread_mutex_unlock(&per_thread_work[tid].lock);

        vector<int> nbors = g.getNeighbors(cur_node.label);

        g.getNodeLock(cur_node.label);

        for (int i = 0; i < (int) nbors.size(); ++i)
        {
                int n_ind = nbors[i];
                int new_dist = g.getDist(cur_node.label) + g.getEdge(cur_node.label, n_ind);

                if(new_dist < g.getDist(n_ind)) {
                        g.setDist(n_ind, new_dist);

                        Node& n = g.getNode(n_ind);
                        pthread_mutex_lock(&per_thread_work[tid].lock);
                        per_thread_work[tid]._Q2.push(n);
                        pthread_mutex_unlock(&per_thread_work[tid].lock);
                }
        }
        g.releaseNodeLock(cur_node.label);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    res = diff(start, end);
    runtimes[tid].tv_sec = res.tv_sec;
    runtimes[tid].tv_nsec = res.tv_nsec;
    pthread_exit(NULL);
}

void *ThreadProc3(void *threadid)
{
    long tid;
    tid = (long)threadid;
    timespec start, end, res;

    CrsGraph& g = graph;

    clock_gettime(CLOCK_MONOTONIC, &start);
    while(true) {
        //cout << "hello from thread: " << tid << endl;
        pthread_mutex_lock(&(per_thread_work[tid].lock));
        
        if (per_thread_work[tid]._Q2.empty()) {
            if(CUR_THREADS == 1) {
                pthread_mutex_unlock(&(per_thread_work[tid].lock));
                break;
            }

            still_working[tid] = false;
            bool work_to_do = false;
            for(int i = 0; i < CUR_THREADS; ++i) {
                if(still_working[i]) {
                    work_to_do = true;
                    pthread_mutex_unlock(&per_thread_work[tid].lock);
                    break;
                }
            }
            pthread_mutex_unlock(&per_thread_work[tid].lock);
            if(work_to_do) {
                usleep(500);
                continue;
            } else {
                break;
            }
            
            pthread_mutex_unlock(&per_thread_work[tid].lock);\
        }

        still_working[tid] = true;
        Node cur_node = per_thread_work[tid]._Q2.top();
        per_thread_work[tid]._Q2.pop();
        pthread_mutex_unlock(&per_thread_work[tid].lock);

        vector<int> nbors = g.getNeighbors(cur_node.label);

        g.getNodeLock(cur_node.label);

        for (int i = 0; i < (int) nbors.size(); ++i)
        {
                int n_ind = nbors[i];
                int new_dist = g.getDist(cur_node.label) + g.getEdge(cur_node.label, n_ind);

                if(new_dist < g.getDist(n_ind)) {
                        g.setDist(n_ind, new_dist);

                        Node& n = g.getNode(n_ind);
                        pthread_mutex_lock(&per_thread_work[n_ind%CUR_THREADS].lock);
                        still_working[n_ind%CUR_THREADS] = true;
                        per_thread_work[n_ind%CUR_THREADS]._Q2.push(n);
                        pthread_mutex_unlock(&per_thread_work[n_ind%CUR_THREADS].lock);
                }
        }
        g.releaseNodeLock(cur_node.label);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    res = diff(start, end);
    runtimes[tid].tv_sec = res.tv_sec;
    runtimes[tid].tv_nsec = res.tv_nsec;
    pthread_exit(NULL);
}

CrsGraph setupRingGraph(int ring_size) {
	CrsGraph graph(ring_size);
	for(int i = 0; i < ring_size-1; ++i) {
		graph.addEdge(i, i+1, 5, false);
	}
	graph.addEdge(ring_size-1, 0, 5, true);
	return graph;
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

int main(int argc, char * argv[]) {
	pthread_t threads[MAX_THREADS ];

	ifstream map_file("map.gr");
	graph = setupGraphFromFile(map_file);
	map_file.close();

	int ret = 0;
	int size = SIZE;

	pthread_mutex_init(&work.lock, NULL);
    
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int t;
    timespec start, end, res;

    ofstream file;
    file.open("datastuffthing.txt", ios::out | ios::app);

    for(int NUM_THREADS = 1; NUM_THREADS < MAX_THREADS; NUM_THREADS++) {
        //cout << "threads: " << NUM_THREADS << endl;
        graph.reset();
        initRTS();
    	CUR_THREADS = NUM_THREADS;
        graph.setDist(0, 0);
        Node& s = graph.getNode(0);
        work._Q2.push(s);
        per_thread_work[0]._Q2.push(s);
        still_working[0] = true;

        clock_gettime(CLOCK_MONOTONIC, &start);
    	for(t = 0; t < NUM_THREADS; ++t) {
    		// ret = pthread_create(&threads[t], &attr, ThreadProc1, (void*)t);
            // ret = pthread_create(&threads[t], &attr, ThreadProc2, (void*)t);
            ret = pthread_create(&threads[t], &attr, ThreadProc3, (void*)t);
    		usleep(50);
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
        clock_gettime(CLOCK_MONOTONIC, &end);
        res = diff(start, end);

        //cout << "dist: " << graph.getDist(1) << endl;
        
        file << (res.tv_sec * 1000000000) + res.tv_nsec << endl;
        cout << (res.tv_sec * 1000000000) + res.tv_nsec << endl;
    }

	file.close();

	pthread_mutex_destroy(&work.lock);
    destroyLocks();

	//return 0;
	pthread_exit(NULL);
}
