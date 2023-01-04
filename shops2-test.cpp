/*  
    National Technical University of Athens
    School of Electrical and Computer Engineering
    7th Semester 2022-2023
    Algorithms: First Set of Programming Exercises
    Author: Anastasios Stefanos Anagnostou
*/

#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <chrono>
#define MAXN 20000
#define MAXK 1000000
#define NO_ANSWER MAXN+1
#define FILE_NUMBER 25

/// @brief Stores the number of residents in each apartment complex.
unsigned int RESIDENTS[MAXN] = {};
unsigned int PARTIAL_SUMS[MAXN] = {};

/*
    The state is represented by a quintuplet.
    First field is the array's element's index.
    Second field is the remainder.
    Third field is whether an element was added before or not.
    Fourth field is whether the second set has been created or not.
    Fifth field is whether we are at the beginning or in a gap.
*/
typedef std::tuple<unsigned int, int, bool> state;
/*
    The key that corresponds to a stored solution is basically the state
    but the third field is disregarded because it actually doesn't 
    change the solution. It is only needed in order to know when
    the change from the first set to the second set happens.
*/
typedef std::tuple<unsigned int, int> key;

/// @brief Search for the smallest contiguous set of numbers that add to target.
/// @param start The index which to search from.
/// @param end The index which to end to.
/// @param target The target which to sum to.
/// @return The minimum size of the set the elements of which add to the target.
int linear(const unsigned int &start, const unsigned int &end, const unsigned int &target)
{
    int answer = 0;
    unsigned int from = start;
    unsigned int to = start;
    unsigned int minimum = NO_ANSWER;
    while(to <= end || answer >= target)
    {
        if (answer < target)
            answer += RESIDENTS[to++];
        else if (answer == target)
        {
            minimum = std::min(to - from, minimum);
            answer -= RESIDENTS[from++];
        }
        else answer -= RESIDENTS[from++];
    }
    return minimum;
}

#define INDEX (std::get<0>(current_state))
#define REMAINDER (std::get<1>(current_state))
#define ADDED (std::get<2>(current_state))
#define VICTORY (REMAINDER == 0)
#define DEFEAT ((REMAINDER < minimum) || (current_ans >= minimum_sofar) || (INDEX!=size && REMAINDER > PARTIAL_SUMS[size-1] - PARTIAL_SUMS[INDEX] + RESIDENTS[INDEX]) || (REMAINDER > 0 && INDEX == size))
#define NOADD (subset_sum(new_state2, current_ans, size))
#define ADD (subset_sum(new_state1, current_ans + 1, size))

/// @brief minimum of the RESIDENTS array.
int minimum = NO_ANSWER;
int minimum_sofar = NO_ANSWER;
/// @brief 
/// @param current_state The state which to solve for.
/// @param current_ans The answer up to the state.
/// @param size The size which to search up to.
/// @return The minimum amount of elements, split at most between two contiguous sets, that solve the state.
int subset_sum(state &current_state, const int &current_ans, const unsigned int &size)
{
    if (VICTORY) return current_ans;
    else if (DEFEAT) return NO_ANSWER;
    auto new_state1 = current_state;
    std::get<1>(new_state1) -= RESIDENTS[std::get<0>(new_state1)++];
    std::get<2>(new_state1) = true;
    int ans = 0;
    if (!ADDED)
    {
        auto new_state2 = current_state;
        std::get<0>(new_state2)++;
        std::get<2>(new_state2) = false;
        ans = std::min(NOADD, ADD);
    }
    else
    {
        /*
            If control reaches this point, then we are building the first set.
            Explore both adding and not adding the current index, altering
            only the "added" flag.
        */
        ans = std::min(current_ans + linear(INDEX, size, REMAINDER), ADD);
    }
    minimum_sofar = std::min(minimum_sofar, ans);
    return ans;
}

void solve(const unsigned &N, const unsigned &K, int &answer)
{
    auto initial_state = std::make_tuple((unsigned int) 0, (int) K, false);
    answer = subset_sum(initial_state, 0, N);
	answer = (answer == NO_ANSWER) ? -1 : answer;
    minimum = NO_ANSWER;
    minimum_sofar = NO_ANSWER;
}

int main (void)
{
	std::ifstream infile;
	std::ifstream outfile;
	std::string input = "./input-output/shops2/input";
	std::string output = "./input-output/shops2/output";
	std::string suffix = ".txt";
	std::string number = "";
	std::string message = "";
    unsigned int N = 0 ;
    unsigned int K = 0 ;
	int computed_output = 0;
	int correct_output = 0;
	std::cout << " ======= BEGINNING TESTING ======= \n"; 
	for(unsigned int i = 1; i <= FILE_NUMBER; ++i)
	{
        number = std::to_string(i);
		infile.open(input+number+suffix);
		if (!infile)
        {
            std::cerr << "failed to open" << std::endl;
            exit(1);
        }
		infile >> N >> K;
        infile >> RESIDENTS[0];
        PARTIAL_SUMS[0] = RESIDENTS[0];
        minimum = RESIDENTS[0] < minimum ? RESIDENTS[0] : minimum;
		for (unsigned int j = 1; j < N; ++j) 
        {
            infile >> RESIDENTS[j]; 
            PARTIAL_SUMS[j] = PARTIAL_SUMS[j-1] + RESIDENTS[j];
            minimum = RESIDENTS[j] < minimum ? RESIDENTS[j] : minimum;
        }
		infile.close();

        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        solve(N, K, computed_output);
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        
        outfile.open(output+number+suffix);
        if (!outfile)
        {
            std::cerr << "failed to open" << std::endl;
            exit(1);
        }
		outfile >> correct_output ;
		outfile.close();

		message = (computed_output==correct_output) ? "SUCCESS" : "FAIL" ;
		std::cout << "\ntest " << number << ": " << computed_output << ' ' << correct_output << ' ' << message << ' '<< time_span.count();
	}
	std::cout << std::endl;
    return 0;
}