#pragma once

#include "Tokenizer.h"


// The Graph class takes the instructions and:
// 1) Creates a graph out of them.
// 2) Creates states table with the unique states.
// 3) Almost fully static class ( only combine_with() function is a modifier)
class Graph
{
private:
	std::vector<std::vector<Instruction>> adj_list;
	std::list<Cell> states_table;
	std::string initial_state;
	std::string halting_state;
	size_t states_count;

	// Changes the state names in the adj_list so that they don't 
	// clash with the state names from the second graph passed in the arguements.
	// Used only if we want to combine some graph with other.
	void change_state_names(Graph& other);

	// Given a name of a state, return the index from the states table.
	// Used for finding the index in the adj_list of a certain state.
	int get_state_index(std::string);
	int get_halting_state_index();
public:
	Graph(InstructionTokenizer&); 

	void combine_with(Graph& other);

	//getters
	std::vector<std::vector<Instruction>> get_adj_list()const;
	std::string get_initial_state()const;
	size_t get_states_count()const;
	std::string get_halting_state()const;
	std::vector<Instruction>get_initial_state_vector()const;
	
	bool has_state(std::string);

	// Given a current state and read symbol, return a state we can go to (the index would be the state) 
	// and a vector of possible further instructions that can be executed from that state
	std::vector<Instruction> go_to_state(std::string,char);

	void test_graph_initialization();
};

