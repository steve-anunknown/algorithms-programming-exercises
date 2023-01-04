#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <tuple>
#define FILE_NUMBER 25
#define MAXN 200000
#define MAXK 200000

/*
    The "STOCK" array stores the number of cereal stock per store.
*/
unsigned int STOCK[MAXN] = {};
int PREFIX[MAXN] = {};

/*
    Check whether a median larger than "med" is possible for a sub-array
    of size at least k. O(n)
*/
bool possible_median(const unsigned int &k, const unsigned int &size, const unsigned int &med)
{
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

int main (void)
{
    std::ifstream infile;
	std::ifstream answers;
	std::string input = "./input-output/cereal/input";
	std::string output = "./input-output/cereal/output";
	std::string suffix = ".txt";
	std::string number = "";
	std::string message = "";
    unsigned int N = 0;
    unsigned int K = 0;
    unsigned int answer = 0;
    unsigned int correct = 0;
    for(unsigned int i = 1; i < FILE_NUMBER; ++i)
	{
		number = std::to_string(i);
		infile.open(input+number+suffix);
		if (!infile) exit(1);
		infile >> N >> K;
		for (unsigned int j = 0; j < N; ++j) infile >> STOCK[j];
		infile.close();
        answer = maximum_median(N,K);
		answers.open(output+number+suffix);
		answers >> correct ;
		message = (answer==correct) ? "SUCCESS" : "FAIL" ;
		std::cout << "\ntest " << number << ' ' << answer << ' ' << correct << ' ' << message;
		answers.close();
	}
    for (unsigned int i = 0; i < N; ++i) std::cin >> STOCK[i];
	std::cout << maximum_median(N,K) << std::endl;

    return 0;
}