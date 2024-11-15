// montecarlo_test.cpp
// driver file for approximating monte carlo solutions

#include <iostream>
#include "sparse.h"
#include <string>
#include <ctime>
#include <chrono>
#include <fstream>

using namespace std;

#define NO_OF_GRAPHS 5


int main(int argc, char *argv[])
{
    int vert_size;
    if (argc > 1)
        vert_size = atoi(argv[1]);

    // Higher values of mult lead to more random walks, 
    // which improves the accuracy of the Monte Carlo approximation 
    // but increases computational cost.
    int mult = atoi(argv[2]);   // A multiplier determining how many random walks to perform per vertex.

    double avg[4], time[4];
    for (int i = 0; i < 4; i++)
    {
        avg[i] = 0.0;
        time[i] = 0.0;
    }

    // number of simulations performed for each graph
    // By performing multiple simulations, you can compute average error across simulations.
    int sim_per = 10;

    for (int i = 0; i < NO_OF_GRAPHS; i++)
    {
        cout << "Reading file #" << i << " ... " << endl;
        sparse A("vert_" + to_string(vert_size) + "/sparse_" + to_string(i) + ".csv");

        A.google_power(1e-8, 1e2, 0.15);

        for (int j = 0; j < sim_per; j++)
        {
            // acquire the error and set time markers
            auto tic0 = chrono::high_resolution_clock::now();
            A.mc1(vert_size * mult, 0.15);
            avg[0] += A.computeMCerror();

            auto tic1 = chrono::high_resolution_clock::now();
            A.mc2(mult, 0.15);
            avg[1] += A.computeMCerror();

            auto tic2 = chrono::high_resolution_clock::now();
            A.mc3(mult, 0.15);
            avg[2] += A.computeMCerror();

            auto tic3 = chrono::high_resolution_clock::now();
            A.mc4(mult, 0.15);
            avg[3] += A.computeMCerror();
            auto tic4 = chrono::high_resolution_clock::now();

            // update the timing
            time[0] += (double)(chrono::duration_cast<chrono::microseconds>(tic1 - tic0).count()) / 1000000;
            time[1] += (double)(chrono::duration_cast<chrono::microseconds>(tic2 - tic1).count()) / 1000000;
            time[2] += (double)(chrono::duration_cast<chrono::microseconds>(tic3 - tic2).count()) / 1000000;
            time[3] += (double)(chrono::duration_cast<chrono::microseconds>(tic4 - tic3).count()) / 1000000;
        }
    }

    ofstream file;
    file.open("vert_" + to_string(vert_size) + "/mc_results_" + to_string(vert_size) + ".txt", ofstream::out | ofstream::app);
    
    int divisor = sim_per * NO_OF_GRAPHS;
    for (int i = 0; i < 4; i++)
    {
        avg[i] /= divisor;
        file << "Average Error for Method " << i + 1 << ": " << avg[i] << endl;

        time[i] /= divisor;
        file << "Average Time for Method " << i + 1 << ": " << time[i] << endl
             << endl;
    }
    file.close();

    return 0;
}