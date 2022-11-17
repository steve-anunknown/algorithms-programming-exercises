/*  
    National Technical University of Athens
    School of Electrical and Computer Engineering
    7th Semester 2022-2023
    Algorithms: First Set of Programming Exercises
    Author: Anastasios Stefanos Anagnostou
*/

#include <iostream>
#include <fstream>
#include <map>
#include <tuple>
#define MAXN 20000
#define MAXK 1000000
#define NO_ANSWER MAXN+1

/*
    "RESIDENTS" array stores the number of residents in each
    apartment complex.
*/
unsigned int RESIDENTS[MAXN] = {};

/*
    The state is represented by a quadruplet.
    First field is the array's element's index.
    Second field is the remainder.
    Third field is whether an element was added before or not.
    Fourth field is whether the second set has been created or not.
*/
typedef std::tuple<unsigned int, int, bool, bool> state;
/*
    The key that corresponds to a stored solution is basically the state
    but the third field is disregarded because it actually doesn't 
    change the solution. It is only needed in order to know when
    the change from the first set to the second set happens.
*/
typedef std::tuple<unsigned int, int, bool> key;

/*
    The "SOLUTIONS" map stores the computed solutions using the keys
    as defined above.
*/
std::map<key, int > SOLUTIONS;

int subset_sum(state &current_state, const int &current_ans, const unsigned int &size)
{  
    key current_key = std::make_tuple(std::get<0>(current_state), // index
                                      std::get<1>(current_state), // remainder
                                      std::get<3>(current_state));// second set
    if (std::get<1>(current_state) == 0)
    {
        /*
            Base case 1:
            - If remainder == 0 (target sum has been reached), do not add another element.
              Insert answer in "SOLUTIONS" map and return it.
        */
        if (SOLUTIONS.find(current_key) != SOLUTIONS.end() &&
			SOLUTIONS.at(current_key) > current_ans)
			SOLUTIONS.insert({current_key, current_ans});
        return current_ans;
    }
    else if (std::get<1>(current_state) < 0 ||
            (std::get<1>(current_state) > 0 && std::get<0>(current_state) == size) ||
            (std::get<1>(current_state) > 0 &&
            !std::get<2>(current_state) && std::get<3>(current_state)))
    {
        /*
            Base case 2: Target sum is unreachable
            - Remainder is negative.
            - Remainder is positive but no more elements.
            - Remainder is positive but the second set just closed.
                - flag (added before or not) == false.
                - flag (second set or not) == true;
            Store the state and return "no answer". 
        */
		
        SOLUTIONS.insert({current_key, NO_ANSWER});
        return NO_ANSWER;
    }
    else if (SOLUTIONS.find(current_key) != SOLUTIONS.end())
	{
        /*
            Base case 3:
            - If current state has been previously solved, just return it.
        */
		
        return SOLUTIONS.at(current_key);
	}
    else if (std::get<2>(current_state) || current_ans == 0)
    {
        /*
            Ordinary case 1:
            - flag (added before or not) == true.
            - flag (second set or not) == true or false;
            Currently creating the second set. 
			
			The "current_ans == 0" conditions ensures that a solution
			can start without picking any elements. Without this condition,
			if a solution started by omitting 2 elements, it would trigger
			the "second set" flag and produce wrong results.
        */
        auto new_state1 = current_state;
        auto new_state2 = current_state;
        /*
            new_state1 = { current_index + 1, remainder - RESIDENTS[current_index], true, X}
            new_state2 = { current_index + 1, remainder, false, X}
        */
        std::get<1>(new_state1) -= RESIDENTS[std::get<0>(new_state1)++];
        std::get<2>(new_state1) = true;
        std::get<0>(new_state2)++;
        std::get<2>(new_state2) = false;
        return std::min(subset_sum(new_state1, current_ans + 1, size),
                        subset_sum(new_state2, current_ans, size));
    }
    else
    {
        /*  
            Ordinary case 2:
            - flag (added before or not) == false.
            - flag (second set or not) == false;
            This means that an element has been omitted. So, the
            only choice is to either keep omitting elements or
            to include elements. After an element is included,
            the choice to omit is not available.
        */
        auto new_state1 = current_state;
        auto new_state2 = current_state;
        /*
            new_state1 = { current_index + 1, remainder - RESIDENTS[current_index], true, true}
            new_state2 = { current_index + 1, remainder, false, false}
        */
        std::get<1>(new_state1) -= RESIDENTS[std::get<0>(new_state1)++];
        std::get<2>(new_state1) = true;
        std::get<3>(new_state1) = true;
        std::get<0>(new_state2)++;
        std::get<2>(new_state2) = false;
        return std::min(subset_sum(new_state1, current_ans + 1, size),
                        subset_sum(new_state2, current_ans, size));
    }
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
    unsigned int N = 0 ;
    unsigned int K = 0 ;
    infile >> N >> K;
    for (unsigned int i = 0; i < N; ++i) infile >> RESIDENTS[i];
    /*
        Compute and print answer.
    */
    auto initial_state = std::make_tuple((unsigned int) 0, (int) K, true, false);
    int answer = subset_sum(initial_state, 0, N);
	if (answer == NO_ANSWER) answer = -1;
    std::cout << answer << std::endl;
    /*
        Close file.
    */
    infile.close();
    return 0;
}