// Driver file for generating web graphs of specified size

#include <iostream>
#include "llist.h"
#include "graph.h"
#include "sparse.h"

#include <cstdlib>
#include <random>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <fstream>

using namespace std;

#define MAX_SIZE 1600000
#define NO_OF_GRAPHS 5

// Random number generator
random_device rd01;
mt19937 gen01(rd01());

// Uniform random number generator
double uniRandom() {
    uniform_real_distribution<double> distribution(0, 1);
    return distribution(gen01);
}

// Function to create an initial graph
graph initial_graph() {
    graph G;
    // Create the same initial graph every time
    for (int i = 0; i < 5; i++) {
        G.addVertex();
    }

    // Adjacency array for edges
    int adj_arr[5][2] = {
        {1, 3}, // edges from vertex 0
        {3, 3}, // edges from vertex 1
        {1, 3}, // edges from vertex 2
        {1, 2}, // edges from vertex 3
        {0, 3}  // edges from vertex 4
    };

    // Add edges to the graph
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 2; j++) {
            G.addEdge(i, adj_arr[i][j]);
        }
    }
    return G;
}

int chooseVertex(graph G, bool z, double delta_z) {
    // z true means in case; false means out case
    double lo = 0.0, hi = 0.0; // Initialize the lower and upper bounds
    double P, x = uniRandom(); // Generate a random number in [0, 1)

    int n = G.getNumVertices(); // Number of vertices
    int t = G.getNumEdges();    // Total number of edges

    // Iterate through each vertex to calculate probabilities
    for (int i = 0; i < n; i++) {
        if (z) {
            // Calculate probability based on in-degree
            P = (G.getD_in(i) + delta_z) / (t + delta_z * n);
        } else {
            // Calculate probability based on out-degree
            P = static_cast<double>(G.getD_out(i)) / t;
        }

        lo = hi; // Set the lower bound to the previous upper bound
        hi += P; // Increment the upper bound by the current probability

        // Check if the random number falls within the current range
        if (lo <= x && x < hi) {
            return i; // Return the selected vertex
        }
    }
    return n - 1; // Default return in case no vertex was selected
}

// G: The starting graph (possibly empty).
// N: The desired number of vertices in the graph (default: 100,000).
// alpha, beta, gamma: Probabilities dictating how new edges and vertices are added:
// alpha: Add a new vertex with an outgoing edge.
// beta: Add an edge between two existing vertices.
// gamma: Add a new vertex with an incoming edge.
// delta_in, delta_out: Constants for preferential attachment, avoiding issues with zero in/out-degrees.
graph bollobas(graph G, int N = 100000, double alpha = 0.41,
                double beta = 0.54, double gamma = 0.05,
                double delta_in = 0.2, double delta_out = 0) {
    double x;
    int n = G.getNumVertices();
    int v, w;
    auto tic = chrono::high_resolution_clock::now(); // Start timer

    // Keep adding vertices until we reach the desired count
    while (n < N) {
        x = uniRandom(); // Generate a random number

        if (0 <= x && x <= alpha) {
            w = chooseVertex(G, true, delta_in); // Choose vertex based on in-degree
            v = G.addVertex(); // Add new vertex
            n++; // Increment vertex count
        } else if (alpha < x && x <= alpha + beta) {
            v = chooseVertex(G, false, delta_out); // Choose vertex based on out-degree
            w = chooseVertex(G, true, delta_in); // Choose another based on in-degree
        } else {
            v = chooseVertex(G, false, delta_out); // Choose based on out-degree
            w = G.addVertex(); // Add new vertex
            n++; // Increment vertex count
        }

        G.addEdge(v, w); // Add edge between chosen vertices

        // Print progress every 1000 vertices
        // if (n % 1000 == 0) {
        //     auto toc = chrono::high_resolution_clock::now();
        //     cout << "Vertices: " << G.getNumVertices()
        //          << "; Time Elapsed: "
        //          << static_cast<double>(chrono::duration_cast<chrono::milliseconds>(toc - tic).count()) / 1000
        //          << " seconds" << endl;
        // }
    }
    return G;
}

int main(int argc, char *argv[]) {
    clock_t start;
    double duration;
    ofstream file;
    int size;

    // Check for command-line arguments and set the size
    if (argc > 1) {
        size = atoi(argv[1]);
    } else {
        cerr << "Usage: " << argv[0] << " <size>" << endl;
        return 1; // Exit if no size is provided
    }

    // Open output file for runtime data
    file.open("vert_" + to_string(size) + "/runtimes_" + to_string(size) + ".txt");

    // Run multiple iterations
    for (int i = 0; i < NO_OF_GRAPHS; i++) {
        start = clock(); // Start timer
        cout << "Beginning #" << i << " ... " << endl;

        graph G = initial_graph(); // Initialize graph
        G = bollobas(G, size); // Generate the Bollobas graph
        // G.saveInDistribution(i); // Save in-degree distribution
        // G.saveOutDistribution(i); // Save out-degree distribution

        // CLOCKS_PER_SEC = 1,000,000
        duration = (clock() - start) / static_cast<double>(CLOCKS_PER_SEC); // Calculate duration
        file << duration << endl; // Write duration to file (duration in seconds)3

        G.saveSparse(i); // Save sparse representation
    }

    file.close(); // Close the output file
    return 0; // Exit the program
}
