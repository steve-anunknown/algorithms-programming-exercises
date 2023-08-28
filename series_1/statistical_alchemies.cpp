/*  
    National Technical University of Athens
    School of Electrical and Computer Engineering
    7th Semester 2022-2023
    Algorithms: First Set of Programming Exercises
    Author: Anastasios Stefanos Anagnostou
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <tuple>
#define MAXN 200000
#define MAXK 200000

/*
    The "STOCK" array stores the number of cereal stock per store.
*/
unsigned int STOCK[MAXN] = {};
int PREFIX[MAXN] = {};

bool possible_median(const unsigned int &k, const unsigned int &size, const unsigned int &med)
{
    /*
        Check whether a median larger than "med" is possible for a sub-array
        of size at least k. O(n)
    */
    PREFIX[0] = STOCK[0] >= med ? 1 : -1;
    for (unsigned int i = 1 ; i < size ; ++i)
    {
        /*
            Count how many elements are larger than "med".
        */
        PREFIX[i] = ( STOCK[i] >= med ) ? 1 + PREFIX[i-1] : -1 + PREFIX[i-1];
    }
    int maximum = PREFIX[k-1];
    int minimum = 0;
    for (unsigned int i = k; i < size; ++i)
    {
        /*
            Count how many elements in range within K are larger than "med".
        */
        minimum = std::min(minimum, PREFIX[i-k]);
        maximum = std::max(maximum, PREFIX[i] - minimum);
    }
    /*
        If maximum > 0 it means that more than half of the elements of a subarray
        of size k are larger than "med". So, if they get sorted, the median will be
        equal to "med" or larger.

        If maximum <= 0, it means that more than half of the elements are smaller than
        "med". So, if they get sorted, the median will be smaller than "med".
    */
    return maximum > 0;    
}

unsigned int maximum_median(const unsigned int &N, const unsigned int &K)
{
    unsigned int left = *std::min_element(STOCK, STOCK+N);
    unsigned int right = *std::max_element(STOCK, STOCK+N);
    unsigned int mid = (left + right)/2;
    unsigned int maximum = 0;
    while (left <= right)
    {
        /*
            Perform binary search in the solutions space. O(log(n))
        */
        if (possible_median(K, N, mid))
        {
            maximum = mid;
            left = mid + 1;
        }
        else
            right = mid - 1;
        mid = (left + right)/2;
    }
    return maximum;
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
    for (unsigned int i = 0; i < N; ++i) infile >> STOCK[i];
    /*
        Compute and print answer.
    */
    std::cout << maximum_median(N, K) << std::endl;
    /*
        Close file.
    */
    infile.close();
    return 0;
}
