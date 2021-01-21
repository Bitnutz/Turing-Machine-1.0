#include "Machine.h"

Machine::Machine(Graph& rules, std::string &input_tape): rules(rules)
{
	tape.fill_tape(input_tape);
	current_state = rules.get_initial_state();
	tape_head = tape.get_head_symbol();
}

void Machine::compute()
{
	std::ofstream output_file("output.txt");
	assert(output_file && "Could not create/open the output file!\n");
	std::string result = "";
	std::string halting_state = rules.get_halting_state();

	// state_edges are the possible paths we can go
	std::vector<Instruction> cur_state_edges = rules.get_initial_state_vector();
	char direction = ' ';
	while (current_state != halting_state)
	{
		for (Instruction inst : cur_state_edges)
		{
			if (inst.get_read_symbol() == tape_head)
			{

				/*std::cout << "current instruction: " << inst.get_read_symbol() << " " << inst.get_from_state() << " -> "
					<< inst.get_write_symbol() << " " << inst.get_to_state() << " " << inst.get_direction() << "\n";*/

				cur_state_edges = rules.go_to_state(current_state, inst.get_read_symbol());

				std::cout << tape.get_tape() << "\n";

				direction = inst.get_direction();
				tape_head = inst.get_write_symbol();
				break;
			}
		}

		if (cur_state_edges.size() == 0)
		{
			result = tape.get_tape();
			std::cout << "Halting state reached!\n";
			break;
		}
		current_state = cur_state_edges[0].get_from_state();
		
		// Updating the tape internally
		if (direction == 'R')
			tape.move_head_right(tape_head);
		else if (direction == 'L')
			tape.move_head_left(tape_head);
		else
			continue;
		tape_head = tape.get_head_symbol();
	}
	output_file << result;
	std::cout << result << "\n";
}

std::string Machine::get_current_tape()
{
	return tape.get_tape();
}

void Machine::combine_with(Graph& other_rules)
{
	rules.combine_with(other_rules);
}

