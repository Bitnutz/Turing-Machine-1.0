#pragma once

#include "Graph.h"
#include "Tape.h"


// The main engine.
// Takes the rules and the input and prints out in a file the output tape(if it halts).
class Machine
{
private:
	Graph rules;
	Tape tape;
	std::string current_state;
	char tape_head;
public:
	Machine(Graph&, std::string&);
	void compute();
	std::string get_current_tape();

	// Same as combine_with() in graph
	void combine_with(Graph&); // not working properly.
};
