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
#define MAXN 20000
#define MAXK 1000000
#define NO_ANSWER -1

/*
    "residents" array stores the number of residents in each
    apartment complex.
*/
unsigned int residents[MAXN] = {};
/*
    First element of tuple is element_index in "residents" array.
    Second element of tuple is remainder.
    Third element of tuple is a boolean flag, whether a gap exists or not.

    The unordered_map "solutions" uses states as keys and returnes stored answers.
*/
std::unordered_map<std::tuple<unsigned int, unsigned int, bool>, int > solutions;

int subset_sum(std::tuple<unsigned int, unsigned int, bool> &current_state,
                const int &current_ans, const unsigned int &size)
{   
    if (solutions.find(current_state) != solutions.end())
        /*
            Base case 1:
            - If current state has been previously solved, just return the answer.
        */
        return solutions.at(current_state);
    if (std::get<1>(current_state) == 0)
    {
        /*
            Base case 2:
            - If target sum has been reached, do not add another element.
              Insert answer in "solutions" map and return it.
        */
        solutions.insert({current_state, current_ans});
        return current_ans;
    }
    else if (std::get<1>(current_state) < 0 ||
            (std::get<1>(current_state) > 0 &&
             std::get<0>(current_state) == size))
    {
        /*
            Base case 3:
            - If target sum is unreachable, insert answer in "solutions"
              map and return failure.
        */
        solutions.insert({current_state, NO_ANSWER});
        return NO_ANSWER;
    }
    else if (std::get<2>(current_state))
    {
        /*
            Ordinary case 1:
            - flag == true.
            This means that there is currently only one set and,
            therefore, the option to omit an element is available. 
        */
        auto new_state1 = current_state;
        auto new_state2 = current_state;
        /*
            new_state1 = { current_index + 1, remainder - residents[current_index], true}
            new_state2 = { current_index + 1, remainder, false}
        */
        std::get<1>(new_state1) -= residents[std::get<0>(new_state1)++];
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
            - flag == false.
            This means that there are currently two sets and,
            therefore, the option to omit an element is not available.
        */
        auto new_state = current_state;
        /*
            new_state = { current_index + 1, remainder - citizen[current_index], false}
        */
        std::get<1>(new_state) -= residents[std::get<0>(new_state)++];
        std::get<2>(new_state) = false;
        return subset_sum(new_state, current_ans + 1, size);
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
    for (unsigned int i = 0; i < N; ++i) infile >> residents[i];
    /*
        Compute and print answer.
    */
    auto initial_state = std::make_tuple((unsigned int) 0, (unsigned int) 0, true);
    std::cout << subset_sum(initial_state, 0, N) << std::endl;
    /*
        Close file.
    */
    infile.close();
    return 0;
}