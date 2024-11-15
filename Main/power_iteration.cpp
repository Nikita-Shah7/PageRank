// Power iteration algorithm for Google matrix in sparse form

#include <iostream>
#include "sparse.h"
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <fstream>

using namespace std;

#define NO_OF_GRAPHS 5

// Setup the initial probability vector
void setup_initial(double *x, int size)
{
    for (int i = 0; i < size; i++)
        x[i] = 1.0 / size; // Each element initialized to 1/size
}

// Print vector contents
void printVector(double *x, int size)
{
    for (int i = 0; i < size; i++)
        cout << x[i] << endl;
}

int main(int argc, char *argv[])
{
    int vert_size, iters;

    // Check command-line arguments for vertex size
    if (argc > 1)
    {
        vert_size = atoi(argv[1]);
    }
    else
    {
        cerr << "Usage: " << argv[0] << " <vertex_size>" << endl;
        return 1; // Exit if no size is provided
    }

    ofstream file;
    file.open("vert_" + to_string(vert_size) + "/powertimes_" + to_string(vert_size) + ".txt");

    // Iterate to read multiple files and perform power iterations
    for (int i = 0; i < NO_OF_GRAPHS; i++)
    {
        cout << endl;
        cout << "Reading file #" << i << " ... " << endl;

        sparse A("vert_" + to_string(vert_size) + "/sparse_" + to_string(i) + ".csv");

        cout << "Beginning the power iteration ... " << endl;
        auto tic = chrono::high_resolution_clock::now();

        iters = A.google_power(1e-8, 1e5, 0.15); // Run power iteration

        auto toc = chrono::high_resolution_clock::now();
        cout << "Time Elapsed: "
             << static_cast<double>(chrono::duration_cast<chrono::microseconds>(toc - tic).count()) / 1000000
             << " seconds, Iterations: " << iters << endl;

        A.powerConRate(iters, 0.15); // Calculate convergence rate

        file << static_cast<double>(chrono::duration_cast<chrono::microseconds>(toc - tic).count()) / 1000000
             << ", " << iters << endl; // Log results to file
    }

    file.close(); // Close the output file
    return 0;     // Exit the program
}