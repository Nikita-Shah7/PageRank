#pragma once  // Prevents multiple inclusions

// directed graph header

// #include "llist.cpp"
#include "llist.h"


class graph {
private:
    llist *adj_list;
    int num_vertices, num_edges;
    int *d_in, *d_out;

public:
    graph();
    graph(llist *, int, int, int *, int *);
    ~graph();

    int addVertex();
    void addEdge(int, int);

    int getNumVertices();
    int getNumEdges();
    int getD_in(int);
    int getD_out(int);

    void printInDegrees();
    void printOutDegrees();
    void printNeighbors(int);
    void printAdjList();

    void saveInDegrees(int);
    void saveOutDegrees(int);
    void saveInDistribution(int);
    void saveOutDistribution(int);
    
    void saveSparse(int);
};