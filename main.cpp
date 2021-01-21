#include "Machine.h"

// our input must end on new line (\n) to avoid reading the /0.
int main()
{
	Tokenizer tokenizer1;
	tokenizer1.tokenize_rules("rules.txt");
	tokenizer1.tokenize_input("input.txt");

	InstructionTokenizer instructions1;
	instructions1.parse_tokens(tokenizer1.get_tokenized_rules());
	std::string input_tape1 = tokenizer1.get_input_tape();
	std::string initial_state = instructions1.get_initial_state();

	Graph graph1(instructions1);
	graph1.test_graph_initialization();



	/*Tokenizer tokenizer2;
	tokenizer2.tokenize_rules("rules2.txt");
	tokenizer2.tokenize_input("none");
	InstructionTokenizer instructions2;
	instructions2.parse_tokens(tokenizer2.get_tokenized_rules());
	std::string input_tape2 = tokenizer2.get_input_tape();
	Graph graph2(instructions2.get_instructions(), instructions2.get_states_table());
	graph2.test_graph_initialization();
	graph1.combine_with(graph2);*/

	//instructions1.test_instruction_tokenizer();
	graph1.test_graph_initialization();

	Machine machine(graph1, input_tape1);
	std::cout << "Input:\n" << machine.get_current_tape() << "\nOutput:\n";
	machine.compute();

	return 0;
}

/*
X{start}->X{start}R
0{start}->X{B}R
1{start}->X{C}R
_{start}->_{accept}R
1{B}->X{D}L
1{D}->1{D}L
0{D}->0{D}L
X{D}->X{D}L
_{D}->_{start}R
0{C}->X{D}L
X{C}->X{C}R
1{C}->1{C}R
*/
