#include <iostream>
// #include "llist.h"
#include "graph.h"
#include <fstream>
using namespace std;

#define MAX_SIZE 1600000

graph::graph()
{
    adj_list = new llist[MAX_SIZE];
    d_in = new int[MAX_SIZE];
    d_out = new int[MAX_SIZE];
    num_vertices = 0;
    num_edges = 0;
}

graph::graph(llist *old_adj, int old_vert, int old_edges, int *old_in, int *old_out)
{
    num_vertices = old_vert;
    num_edges = old_edges;
    adj_list = new llist[MAX_SIZE];
    d_in = new int[MAX_SIZE];
    d_out = new int[MAX_SIZE];
    for (int i = 0; i < num_vertices; i++)
    {
        adj_list[i] = old_adj[i];
        d_in[i] = old_in[i];
        d_out[i] = old_out[i];
    }
}

graph::~graph()
{
}

int graph::addVertex()
{
    int newVertex = num_vertices;
    num_vertices++;
    d_in[newVertex] = 0;
    d_out[newVertex] = 0;
    return newVertex;
}

void graph::addEdge(int v, int w)
{
    if (v != w)
    {
        if (adj_list[v].add(w))
        {
            num_edges++;
            d_in[w]++;
            d_out[v]++;
        }
    }
}

int graph::getNumVertices()
{
    return num_vertices;
}

int graph::getNumEdges()
{
    return num_edges;
}

int graph::getD_in(int index)
{
    return d_in[index];
}

int graph::getD_out(int index)
{
    return d_out[index];
}

void graph::printInDegrees()
{
    cout << "Printing the In Degrees..." << endl;
    for (int i = 0; i < num_vertices; i++)
        cout << "d_in(" << i << ") = " << d_in[i] << endl;
}

void graph::printOutDegrees()
{
    cout << "Printing the Out Degrees..." << endl;
    for (int i = 0; i < num_vertices; i++)
        cout << "d_out(" << i << ") = " << d_out[i] << endl;
}

void graph::printNeighbors(int index)
{
    cout << "Printing the neighbors of vertex " << index << endl;
    adj_list[index].displayAll();
}

void graph::printAdjList()
{
    cout << "Printing the Full Adjacency List..." << endl;
    for (int i = 0; i < num_vertices; i++)
        printNeighbors(i);
}

void graph::saveInDegrees(int num)
{
    ofstream file;
    file.open("indegree_" + to_string(num) + ".txt");
    for (int i = 0; i < num_vertices; i++)
        file << d_in[i] << endl;
    file.close();
}

void graph::saveOutDegrees(int num)
{
    ofstream file;
    file.open("outdegree_" + to_string(num) + ".txt");
    for (int i = 0; i < num_vertices; i++)
        file << d_out[i] << endl;
    file.close();
}

void graph::saveInDistribution(int num)
{
    int dist[num_vertices];
    for (int i = 0; i < num_vertices; i++)
        dist[i] = 0;
    ofstream file;
    file.open("vert_" + to_string(num_vertices) + "/in_dist_" + to_string(num) + ".txt");
    for (int i = 0; i < num_vertices; i++)
        dist[d_in[i]]++;
    for (int i = 0; i < num_vertices; i++)
        file << dist[i] << endl;
    file.close();
}

void graph::saveOutDistribution(int num)
{
    int dist[num_vertices];
    for (int i = 0; i < num_vertices; i++)
        dist[i] = 0;
    ofstream file;
    file.open("vert_" + to_string(num_vertices) + "/out_dist_" + to_string(num) + ".txt");
    for (int i = 0; i < num_vertices; i++)
        dist[d_out[i]]++;
    for (int i = 0; i < num_vertices; i++)
        file << dist[i] << endl;
    file.close();
}

void graph::saveSparse(int num)
{
    ofstream file;
    file.open("vert_" + to_string(num_vertices) + "/sparse_" + to_string(num) + ".csv");
    // print the number of vertices and edges for reference
    file << num_vertices << "," << num_edges << ",";
    // skip printing the v array; this can be obtained from the data
    // print the r array; separated by commas
    for (int i = 0; i < num_vertices; i++)
    {
        if (!adj_list[i].isEmpty())
            file << adj_list[i].csvPrint(); // << " ,";
    }
    // print the p array; separated by commas
    int subtotal = 0;
    for (int i = 0; i < num_vertices; i++)
    {
        file << subtotal << ",";
        subtotal += adj_list[i].getSize();
    }
    file << subtotal;
    file.close();
}
