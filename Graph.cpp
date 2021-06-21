/* Sapir Nauguker 206542375
   Yonatan Avizov 318432101*/

#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <random>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <map>
#include <list>
#include <iostream>
#include <queue>

#define e 2.718

using namespace std;

template <typename T>

/* the object Graph */
class Graph {
private:
	map<T, list<T>> adj;

	bool connect = true;// true == graph  is conected , false = graph is not conected 
	bool sol = false;// true = there is a isolated node , false these is no isolated node 
	int diam = 0; // -1 if infint

public:
	/* function that add the node to the lists for the 2 nodes */
	void addEdge(int x, int y) {
		adj[x].push_back(y);
		adj[y].push_back(x);
	};

	map<T, list<T>> getMap() { return adj; };

	int BFS(T src);

	bool getConnect() { return connect; };
	bool getSol() { return sol; };
	int getDiam() { return diam; };
	void setConnect(bool c) { connect = c; };
	void setSol(bool s) { sol = s; };
	void setDiam(int d) { diam = d; };

};

Graph<int> build_random_graph(int V, float p);
float rand_pro(float low, float high);
float Threshold1(int V);
float Threshold2(int V);
int diameter(Graph<int>);
bool connectivity(Graph<int> g);
bool Is_Isolated(Graph<int> g);

void add_csv_connect(int V, ofstream* filename, float p);
void add_csv_solated(int V, ofstream* filename, float p);
void add_csv_diameter(int V, ofstream* filename, float p);

const int VR_len = 1000; // the numbers of nodes in the graph
const int Run_graph = 500; // how many graphs to create

void main() {

	float* Pro_connect = new float[10](); // array for the connection probability
	float* Pro_solated = new float[10](); // array for the solated probability
	float* Pro_diam = new float[10](); // array for the diam probability

	float th_connect_and_solated = Threshold1(VR_len);
	float th_diam = Threshold2(VR_len);

	srand(time(0));
	for (int i = 0; i < 10; i++)
	{
		if (i < 5) // probability that low from the Threshold1
		{
			Pro_connect[i] = rand_pro(0.005, th_connect_and_solated);
			Pro_solated[i] = rand_pro(0.005, th_connect_and_solated);
			Pro_diam[i] = rand_pro(0.11, th_diam);
		}
		else // probability that high fron the Threshold1
		{
			Pro_connect[i] = rand_pro(th_connect_and_solated, 0.008);
			Pro_solated[i] = rand_pro(th_connect_and_solated, 0.008);
			Pro_diam[i] = rand_pro(th_diam, 0.12);
		}
	}
	ofstream foutcsv;
	foutcsv.open("Graphs_connect.csv"); // create new csv Graphs_connect
	foutcsv << "p" << ","
		<< "connect = 1" << "," << "connect = 0" << endl;
	for (int i = 0; i < 10; i++)
	{
		add_csv_connect(VR_len, &foutcsv, Pro_connect[i]);
	}
	foutcsv.close();

	foutcsv.open("Graphs_solated.csv"); // create new csv Graphs_solated
	foutcsv << "p" << ","
		<< "is solated = 1" << "," << "is solated = 0" << endl;
	for (int i = 0; i < 10; i++)
	{
		add_csv_solated(VR_len, &foutcsv, Pro_solated[i]);
	}
	foutcsv.close();

	foutcsv.open("Graphs_diam.csv"); // create new csv Graphs_diam
	foutcsv << "p" << ","
		<< "diam = 2" << "," << "diam > 2" << endl;
	for (int i = 0; i < 10; i++)
	{	
		add_csv_diameter(VR_len, &foutcsv, Pro_diam[i]);
	}
	foutcsv.close();
}

/* function that return random probability */
float rand_pro(float low, float high)
{
	return low + static_cast <float> (rand()) / (static_cast<float> (RAND_MAX / (high - low)));
}

/* add the graph to the csv */
void add_csv_connect(int V, ofstream* filename, float p)
{
	Graph<int> graph;
	float sum_connect = 0;

	*filename << p << ",";
	srand(time(0));
	for (int i = 0; i < Run_graph; i++) {
		graph = build_random_graph(V, p); // create new graph with the nodes array and the probability
		if (connectivity(graph) == true)
			sum_connect++;
	}

	sum_connect = sum_connect / Run_graph;
	*filename << sum_connect << "," << (float)(1 - sum_connect) << endl;
}

void add_csv_solated(int V, ofstream* filename, float p)
{
	Graph<int> graph;
	float sum_solated = 0;

	*filename << p << ",";
	srand(time(0));
	for (int i = 0; i < Run_graph; i++) {

		graph = build_random_graph(V, p); // create new graph with the nodes array and the probability
		if (Is_Isolated(graph) == true)
			sum_solated++;
	}

	sum_solated = sum_solated / Run_graph;
	*filename << sum_solated << "," << (float)(1 - sum_solated) << endl;
}

/* add the graph to the csv */
void add_csv_diameter(int V, ofstream* filename, float p)
{
	Graph<int> graph;
	float sum_diam_equal_2 = 0;

	*filename << p << ",";
	srand(time(0));
	for (int i = 0; i < Run_graph; i++) 
	{
		graph = build_random_graph(V, p); // create new graph with the nodes array and the probability
		int tempd = diameter(graph);
		if (tempd == 2)
			sum_diam_equal_2++;

	}
	sum_diam_equal_2 = sum_diam_equal_2 / Run_graph;
	*filename << sum_diam_equal_2 << "," << 1 - sum_diam_equal_2 << endl;
}

/* function that that create a new graph */
Graph<int> build_random_graph(int V, float p)
{
	Graph<int> g;
	float tempProb = 0;

	for (int start = 0; start < V; start++) // run on all the nodes in the graph
	{
		for (int end = start + 1; end < V; end++) // chack on all the nodes from the current 
		{
			tempProb = 0 + static_cast <float> (rand()) / (static_cast<float> (RAND_MAX / (1 - 0)));
			if (tempProb <= p) // check if probability is low that p
			{
				g.addEdge(start, end); // add the edge
			}
		}
	}
	return g;
}

/* BFS function */
template<typename T>
int Graph<T>::BFS(T src)
{

	int* diams = new int[VR_len + 1](); // create array for the dists from every node to src
	queue<T> q; // queue for running the nodes chack

	int diam = 0; // sum the current diam
	int vertexes = 1; // sum for chack if all the nodes visite
	bool* visited = new bool[VR_len] {false}; // create bool-array for the visite nodes

	q.push(src); // add to running queue the corrent node
	visited[src] = true; // 1 = grey , has been visit
	diams[src] = 0; // the dist from the corrent node to himself is 0
	while (!q.empty()) // run on the queue
	{
		T node = q.front(); // take the first node in queue
		q.pop(); // delete the node from the queue

		for (int nbr : adj[node]) // run on all the negihbors of the corrent node
		{
			if (visited[nbr] == false) // chack if this neighbor has been visit
			{
				q.push(nbr);// list of neighbor
				visited[nbr] = true; // to cave that we visited the node
				diams[nbr] = diams[node] + 1; // the dist is the far from the corrent node+1
				diam = diams[nbr] > diam ? diams[nbr] : diam; // save the high diam
				vertexes++;
			}
		}
	}

	delete[]diams;
	delete[]visited;
	return vertexes == VR_len ? diam : -1; // if they visite all nodes return the diam if not -1
}

/* function that chack and return the diameter */
int diameter(Graph<int> g)
{
	int demy, diamter = 0;
	if (g.BFS(0) == -1) // if it return -1 
	{
		return -1; // there is not diam
	}
	else // if there is diam
	{
		for (int i = 1; i < VR_len; i++) // run on all the nodes
		{
			demy = g.BFS(i); // save temp diam
			if (demy > diamter) // is the temp diam is bigger that the currnet diam
				diamter = demy; // the temp diam is the currnet diam
		}
	}
	return diamter; // return the currnet diam
}

/* function that chack and return the connect */
bool connectivity(Graph<int> g)
{
	return g.BFS(0) > 0 ? true : false; // 1 = connect, 0 = not connect
}

/* function that chack and return the is solated */
bool Is_Isolated(Graph<int> g)
{
	map<int, list<int>> adj = g.getMap(); // run on the map
	for (int i = 0; i < VR_len; i++) // chack all the nodes
	{
		if (adj[i].size() == 0) // if the node size is 0 the nodes is solated
			return true; // 1 = solated
	}
	return false; // 0 = not solated
}

/* function to sum the Threshold1 */
float Threshold1(int V)
{
	return log(V) / V;
}

/* function to sum the Threshold2 */
float Threshold2(int V)
{
	return sqrt((2 * log(V)) / V);
}