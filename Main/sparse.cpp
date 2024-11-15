#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "sparse.h"
#include <cmath>
#include <vector>
#include <cstdlib>
#include <random>
#include <ctime>

using namespace std;

random_device rd;
mt19937 gen(rd());

int uniIntRandom(int a, int b)
{
    uniform_int_distribution<int> distribution(a, b - 1);
    int x = distribution(gen);
    return x;
}

double uniRandom2()
{
    return (uniIntRandom(0, 100) / 100.0);
}

sparse::sparse()
{
}

sparse::sparse(string filename)
{
    cout << "Beginning to read file..." << endl;
    ifstream infile(filename);
    string line, str;

    getline(infile, line);

    istringstream ss(line);

    getline(ss, str, ',');
    num_vertices = stoi(str);

    getline(ss, str, ',');
    num_edges = stoi(str);

    v = new double[num_edges];
    r = new int[num_edges];
    p = new int[num_vertices + 1];

    for (int i = 0; i < num_edges; i++)
    {
        getline(ss, str, ',');
        r[i] = stoi(str);
    }

    for (int i = 0; i < num_vertices + 1; i++)
    {
        getline(ss, str, ',');
        p[i] = stoi(str);
    }

    int k = 0;
    for (int i = 1; i < num_vertices + 1; i++)
    {
        int amount = p[i] - p[i - 1];
        for (int j = 0; j < amount; j++)
        {
            v[k++] = 1.0 / amount;
        }
    }

    // find dangling nodes
    for (int i = 0; i < num_vertices + 1; i++)
    {
        if (p[i] == p[i + 1])
        {
            d.push_back(i);
        }
    }

    // initialize the eigenvector and Monte Carlo approximation
    x = new double[num_vertices];
    x_mc = new double[num_vertices];
    for (int i = 0; i < num_vertices; i++)
    {
        x[i] = 1.0 / num_vertices;
        x_mc[i] = 0.0;
    }

    powerFlag = false;
    mcFlag = false;
}

sparse::~sparse()
{
    // Cleanup code would go here
}


// tol: Tolerance level for the convergence criterion (default: 1e-8): This value determines the threshold for how close successive iterations need to be to consider the algorithm converged.
// maxiters: Maximum number of iterations allowed for the power iteration.
// m: Damping factor, typically around 0.15. It represents the probability of randomly jumping to another page, ensuring convergence and accounting for nodes with no outgoing links.

// n: Total number of vertices in the graph.
// y: An array to store intermediate PageRank values.
// E: Tracks the error between the current and previous iterations.
int sparse::google_power(double tol = 0.00000001, int maxiters = 10000, double m = 0.15)
{
    int n = num_vertices;
    double *y = new double[n];
    double c, E;

    int index, k;

    for (k = 0; k < maxiters; k++)
    {
        // calculate the value of c
        c = m / n;

        for (unsigned j = 0; j < d.size(); j++)
        {
            index = d[j];
            c += (1 - m) / n * x[index];
        }

        // set y to be the vector of length n with all values c
        for (int j = 0; j < n; j++)
        {
            y[j] = c;
        }

        // compute sparse-matrix vector multiplication y = (1 - m)Ax
        for (int j = 0; j < n; j++)
        {
            for (int i = p[j]; i < p[j + 1]; i++)
            {
                y[r[i]] += (1 - m) * v[i] * x[j];
            }
        }

        // check error < tolerance
        E = 0.0;
        for (int j = 0; j < n; j++)
        {
            E += abs(y[j] - x[j]);
            if (E >= tol) // break early to save time
                break;
        }

        // copy y into x
        for (int j = 0; j < n; j++)
        {
            x[j] = y[j];
        }

        // if under tolerance, we’re finished
        if (E < tol && k > 50)
            break;
    }

    powerFlag = true;
    cout << "Power Iteration completed" << endl;

    return (k + 1);
}


// topdiff and botdiff: Track differences between successive iterations for calculating the convergence rate.
double sparse::powerConRate(int iters, double m = 0.15)
{
    int n = num_vertices;
    double *y = new double[n];
    double c, conRate, topdiff, botdiff;

    // create a new vector to start from beginning; x is reserved
    double *z = new double[n];
    for (int i = 0; i < n; i++)
        z[i] = 1.0 / n;

    ofstream file;
    file.open("vert_" + to_string(n) + "/conrates_" + to_string(n) + ".txt", ofstream::out | ofstream::app);

    int index, k;
    for (k = 0; k < iters - 1; k++)
    {
        // calculate the value of c for pagerank eigenvector
        c = m / n;
        for (unsigned j = 0; j < d.size(); j++)
        {
            index = d[j];
            c += (1 - m) / n * z[index];
        }

        for (int j = 0; j < n; j++)
            y[j] = c;

        // compute sparse-matrix vector multiplication; add to c
        for (int j = 0; j < n; j++)
        {
            for (int i = p[j]; i < p[j + 1]; i++)
            {
                y[r[i]] += (1 - m) * v[i] * z[j];
            }
        }

        topdiff = 0.0;
        botdiff = 0.0;
        // compute the rate of convergence
        for (int j = 0; j < n; j++)
        {
            topdiff += abs(x[j] - y[j]);
            botdiff += abs(x[j] - z[j]);
        }
        conRate = topdiff / botdiff;
        // file << conRate << endl;

        // copy y into z
        for (int j = 0; j < n; j++)
            z[j] = y[j];
    }
    file << conRate << endl;
    // file << endl;
    file.close();

    ofstream file2;
    file2.open("vert_" + to_string(n) + "/pagerank_" + to_string(n) + ".txt", ofstream::out | ofstream::app);
    for (int j = 0; j < n; j++)
    {
        file2 << "PageRank of node " << j << " = " << z[j] << endl;
    }
    file2.close();

    return conRate;
}

int sparse::getNumVertices()
{
    return num_vertices;
}

void sparse::printAll()
{
    cout << "Printing array v:" << endl;
    for (int i = 0; i < num_edges; i++)
        cout << v[i] << ",";
    cout << endl;

    cout << endl;
    cout << "Printing array r:" << endl;
    for (int i = 0; i < num_edges; i++)
        cout << r[i] << ",";
    cout << endl;

    cout << endl;
    cout << "Printing array p:" << endl;
    for (int i = 0; i < num_vertices + 1; i++)
        cout << p[i] << ",";
    cout << endl;
}

void sparse::mc1(int N, double m = 0.15)
{
    double counter = 1.0 / N;
    // restart the monte carlo approximation
    for (int i = 0; i < num_vertices; i++)
        x_mc[i] = 0.0;

    double bored;
    int w, numNeighbors, offset;

    for (int i = 0; i < N; i++)
    {
        w = uniIntRandom(0, num_vertices);
        while (true)
        {
            bored = uniRandom2();
            if (bored < m)
                break; // end walk; surfer got bored

            numNeighbors = p[w + 1] - p[w];
            if (numNeighbors == 0)
                // dangling node; randomly jump anywhere
                w = uniIntRandom(0, num_vertices);
            else
            {
                offset = uniIntRandom(0, numNeighbors);
                w = r[p[w] + offset];
            }
        }
        // count number of times it ends at a particular vertex
        x_mc[w] += counter;
    }


    // Normalize the x_mc array to get the PageRank vector
    double sum = 0.0;
    for (int i = 0; i < num_vertices; i++)
        sum += x_mc[i];  // Sum up all elements in x_mc

    if (sum != 0.0)  // To avoid division by zero
    {
        for (int i = 0; i < num_vertices; i++)
            x_mc[i] /= sum;  // Normalize to get probabilities (PageRank)
    }

    ofstream file;
    file.open("vert_" + to_string(num_vertices) + "/mc1_pagerank_" + to_string(num_vertices) + ".txt", ofstream::out | ofstream::app);
    // Now x_mc contains the PageRank vector.
    for (int j = 0; j < num_vertices; j++)
    {
        file << "PageRank of node " << j << " = " << x_mc[j] << endl;
    }
    file.close();
}

void sparse::mc2(int q, double m = 0.15)
{
    double counter = 1.0 / (q * num_vertices); // Calculate contribution per visit

    // Restart the Monte Carlo approximation
    for (int i = 0; i < num_vertices; i++)
    {
        x_mc[i] = 0.0; // Reset Monte Carlo results
    }

    double bored;
    int w, numNeighbors, offset;

    // Iterate over each vertex
    for (int i = 0; i < num_vertices; i++)
    {
        // Perform q random walks starting from vertex i
        for (int dummy = 0; dummy < q; dummy++)
        {
            w = i; // Start at vertex i
            while (true)
            {
                bored = uniRandom2(); // Check if the surfer gets bored
                if (bored < m)
                {
                    break; // End walk if bored
                }

                // Get the number of neighbors of the current vertex
                numNeighbors = p[w + 1] - p[w];

                if (numNeighbors == 0)
                {
                    // If current vertex is a dangling node, randomly jump anywhere
                    w = uniIntRandom(0, num_vertices);
                }
                else
                {
                    // Randomly select a neighbor
                    offset = uniIntRandom(0, numNeighbors);
                    w = r[p[w] + offset];
                }
            }
            // Increment the count for the ending vertex
            x_mc[w] += counter;
        }
    }


    // Normalize the x_mc array to get the PageRank vector
    double sum = 0.0;
    for (int i = 0; i < num_vertices; i++)
        sum += x_mc[i];  // Sum up all elements in x_mc

    if (sum != 0.0)  // To avoid division by zero
    {
        for (int i = 0; i < num_vertices; i++)
            x_mc[i] /= sum;  // Normalize to get probabilities (PageRank)
    }

    ofstream file;
    file.open("vert_" + to_string(num_vertices) + "/mc2_pagerank_" + to_string(num_vertices) + ".txt", ofstream::out | ofstream::app);
    // Now x_mc contains the PageRank vector.
    for (int j = 0; j < num_vertices; j++)
    {
        file << "PageRank of node " << j << " = " << x_mc[j] << endl;
    }
    file.close();
}

void sparse::mc3(int q, double m = 0.15)
{
    double counter = 1.0; // Contribution for each visit
    int total_visits = 0; // Count of total visits

    // Restart the Monte Carlo approximation
    for (int i = 0; i < num_vertices; i++)
    {
        x_mc[i] = 0.0; // Reset Monte Carlo results
    }

    double bored; // Variable to check if the surfer gets bored
    int w, numNeighbors, offset;

    // Perform random walks starting from each vertex
    for (int i = 0; i < num_vertices; i++)
    {
        for (int dummy = 0; dummy < q; dummy++)
        {
            w = i; // Start at vertex i
            while (true)
            {
                x_mc[w] += counter;   // Increment visit count for vertex w
                total_visits++;       // Increase total visits
                bored = uniRandom2(); // Check if surfer gets bored
                if (bored < m)
                {
                    break; // End walk if bored
                }

                // Get the number of neighbors
                numNeighbors = p[w + 1] - p[w];
                if (numNeighbors == 0)
                {
                    // If dangling node, randomly jump anywhere
                    w = uniIntRandom(0, num_vertices);
                }
                else
                {
                    // Randomly select a neighbor
                    offset = uniIntRandom(0, numNeighbors);
                    w = r[p[w] + offset];
                }
            } // End while
        } // End for dummy
    } // End for i

    // Compute the average for each vertex
    for (int i = 0; i < num_vertices; i++)
    {
        x_mc[i] /= total_visits;
    }

    ofstream file;
    file.open("vert_" + to_string(num_vertices) + "/mc3_pagerank_" + to_string(num_vertices) + ".txt", ofstream::out | ofstream::app);
    // Now x_mc contains the PageRank vector.
    for (int j = 0; j < num_vertices; j++)
    {
        file << "PageRank of node " << j << " = " << x_mc[j] << endl;
    }
    file.close();
}

void sparse::mc4(int q, double m = 0.15)
{
    double counter = 1.0; // Contribution for each visit
    int total_visits = 0; // Count of total visits

    // Restart the Monte Carlo approximation
    for (int i = 0; i < num_vertices; i++)
    {
        x_mc[i] = 0.0; // Reset Monte Carlo results
    }

    double bored; // Variable to check if the surfer gets bored
    int w, numNeighbors, offset;

    // Perform random walks starting from each vertex
    for (int i = 0; i < num_vertices; i++)
    {
        for (int dummy = 0; dummy < q; dummy++)
        {
            w = i; // Start at vertex i
            while (true)
            {
                x_mc[w] += counter;   // Increment visit count for vertex w
                total_visits++;       // Increase total visits
                bored = uniRandom2(); // Check if surfer gets bored
                if (bored < m)
                {
                    break; // End walk if bored
                }

                // Get the number of neighbors
                numNeighbors = p[w + 1] - p[w];
                if (numNeighbors == 0)
                {
                    // End walk at dangling nodes
                    break;
                }
                else
                {
                    // Randomly select a neighbor
                    offset = uniIntRandom(0, numNeighbors);
                    w = r[p[w] + offset];
                }
            } // End while
        } // End for dummy
    } // End for i

    // Compute the average for each vertex
    for (int i = 0; i < num_vertices; i++)
    {
        x_mc[i] /= total_visits;
    }

    ofstream file;
    file.open("vert_" + to_string(num_vertices) + "/mc4_pagerank_" + to_string(num_vertices) + ".txt", ofstream::out | ofstream::app);
    // Now x_mc contains the PageRank vector.
    for (int j = 0; j < num_vertices; j++)
    {
        file << "PageRank of node " << j << " = " << x_mc[j] << endl;
    }
    file.close();
}

double sparse ::computeMCerror()
{
    double error = 0.0;
    for (int i = 0; i < num_vertices; i++)
        error += abs(x[i] - x_mc[i]);

    return (error);
}