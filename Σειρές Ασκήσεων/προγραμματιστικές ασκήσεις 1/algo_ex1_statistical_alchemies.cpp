/*  
    National Technical University of Athens
    School of Electrical and Computer Engineering
    7th Semester 2022-2023
    Algorithms: First Set of Programming Exercises
    Author: Anastasios Stefanos Anagnostou
*/

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <tuple>
#define MAXN 200000
#define MAXK 200000

unsigned int stock[MAXN] = {};

unsigned int maximum_median()
{
    
}

int main (int argc, char *argv[])
{
    /*
        Assert that two arguments have been passed:
        1) The executable's name.
        2) The name of the input data file.
    */
    if (argc != 2)
    {
        std::cerr << "\n Usage: ./exefile.exe inputdatafile.txt" << std::endl;
        exit(1);
    }
    /*
        Try to open the file with the given filename.
    */
    std::ifstream infile;
    infile.open(argv[1]);
    if (!infile)
    {
        /*
            If input file could not be opened, exit with error.
        */
        std::cerr << "unable to open input file \n";
        exit(1);
    }
    /*
        Read input.
    */
    unsigned int N = 0;
    unsigned int K = 0;
    infile >> N >> K;
    for (unsigned int i = 0; i < N; ++i) infile >> stock[i];
    const unsigned int median_index = (K+1)/2;

    return 0;
}