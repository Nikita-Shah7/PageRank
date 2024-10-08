#pragma once  // Prevents multiple inclusions

// sparse matrix header

#include<iostream>
#include <vector>
using namespace std;

class sparse
{
private:
    int *p, *r;
    double *v;
    int num_vertices, num_edges;
    std::vector<int> d;     // keep track of dangling nodes
    double *x;              // let sparse track its PageRank eigenvector
    double *x_mc;           // and the monte carlo approximation
    bool powerFlag, mcFlag; // true if finished computing

public:
    sparse();
    sparse(std::string); // create from file
    ~sparse();
    // does the power iteration on google matrix;
    // outputs # iterations required
    int google_power(double tol, int maxiters, double m);
    int getNumVertices();

    double powerConRate(int, double); // approximate convergence rate after final power vector found
    void printAll();

    void mc1(int, double);
    void mc2(int, double);
    void mc3(int, double);
    void mc4(int, double);
    double computeMCerror();
};
