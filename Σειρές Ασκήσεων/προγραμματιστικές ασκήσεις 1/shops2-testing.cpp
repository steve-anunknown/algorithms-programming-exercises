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

unsigned int line = 0;
void print_state(const state &s)
{
	/*
		Debugging help. Print state.
	*/
	std::cout << '\n' << line ++ << ".) element index " << std::get<0>(s) <<'\n';
	std::cout << line ++ <<".) remainder " << std::get<1>(s) <<'\n';
	std::cout << line ++ <<".) flag (added or not) " << std::get<2>(s) <<'\n';
	std::cout << line ++ <<".) flag (second set) " << std::get<3>(s) <<'\n';
}

/*
    The "SOLUTIONS" map stores the computed solutions using the keys
    as defined above.
*/
std::map<key, int> SOLUTIONS;

int subset_sum(state &current_state, const int &current_ans, const unsigned int &size)
{  
    /*
        std::get<> returns references, so any change to the
        variables below, results in an actual change to the
        fields of the tuple.
    */
    unsigned int index = std::get<0>(current_state);
    int remainder = std::get<1>(current_state);
    bool added = std::get<2>(current_state);
    bool second_set = std::get<3>(current_state);
    key current_key = std::make_tuple(index, remainder, second_set);
    if (remainder == 0)
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
    else if (remainder < 0 || (remainder > 0 && index == size) ||
            (remainder > 0 && !added && second_set))
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
    else if (added || current_ans == 0)
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

int main (void)
{
    std::ifstream infile;
    std::ifstream answers;
    std::string input = "./input-output/shops2/input";
    std::string output = "./input-output/shops2/output";
    std::string suffix = ".txt";
    std::string number = "";
    std::string message = "";
    unsigned int N = 0 ;
    unsigned int K = 0 ;
    auto initial_state = std::make_tuple((unsigned int) 0, (int) K, true, false);
    int answer = 0;
    int correct_answer = 0;
    std::cout << " ============= BEGINNING TESTING ============= \n"; 
    for(unsigned int i = 1; i < FILE_NUMBER; ++i)
    {
	number = std::to_string(i);
	infile.open(input+number+suffix);
	if (!infile) exit(1);
	infile >> N >> K;
	for (unsigned int j = 0; j < N; ++j) infile >> RESIDENTS[j];
	infile.close();
	initial_state = std::make_tuple((unsigned int) 0, (int) K, true, false);
	answer = subset_sum(initial_state, 0, N);
	if (answer == NO_ANSWER) answer = -1;
	answers.open(output+number+suffix);
	answers >> correct_answer ;
	message = (answer==correct_answer) ? "SUCCESS" : "FAIL" ;
	std::cout << "\ntest " << number << ' ' << answer << ' ' << correct_answer << ' ' << message;
	answers.close();
	SOLUTIONS.clear();
    }
    std::cout << std::endl;
    return 0;
}
