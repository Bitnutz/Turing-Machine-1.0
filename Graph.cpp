#include "Graph.h"

// Changes the names of the other graph using this states_table
void Graph::change_state_names(Graph& other) 
{
	std::vector<std::vector<Instruction>>::iterator it = other.adj_list.begin();
	for (it; it != other.adj_list.end(); ++it)
	{
		for (Instruction& inst : *it)
		{
			if (has_state(inst.get_from_state()))
			{
				std::string new_name = inst.get_from_state() + "_TM2";
				inst.set_from_state(new_name);
			}
			if (has_state(inst.get_to_state()))
			{
				std::string new_name = inst.get_to_state() + "_TM2";
				inst.set_to_state(new_name);
			}
		}
	}

	// Filling the states_table
	std::list<Cell>::iterator list_it = other.states_table.begin();
	for (list_it; list_it != other.states_table.end(); ++list_it)
	{
		if (!this->has_state(list_it->state))
			this->states_table.emplace_back(states_count++, list_it->state);
		else
		{
			std::string cur_state = list_it->state;
			this->states_table.emplace_back(states_count++, cur_state += "_TM2");
		}
	}
}

int Graph::get_state_index(std::string searched_state)
{
	std::list<Cell>::iterator it = states_table.begin();
	int index = 0;
	for (it; it != states_table.end(); ++it)
	{
		if (it->state == searched_state)
			return it->index;
	}
	return -1;
}

int Graph::get_halting_state_index()
{
	for (int i = 0; i < states_count; i++)
	{
		if (adj_list[i].empty())
			return i;
	}
	return -1;
}

Graph::Graph(InstructionTokenizer& tokenized_instructions) 
{
	this->initial_state = tokenized_instructions.get_initial_state();
	this->states_table = tokenized_instructions.get_states_table();
	std::queue<Instruction> instructions = tokenized_instructions.get_instructions();
	states_count = states_table.size();
	adj_list.resize(states_count);

	while (!instructions.empty())
	{
		Instruction cur_instruction = instructions.front();
		std::string cur_from_state = cur_instruction.get_from_state();
		instructions.pop();
		std::list<Cell>::iterator it = states_table.begin();
	
		for (it; it != states_table.end(); ++it)
		{
			if (it->state == cur_from_state)
			{
				adj_list[it->index].push_back(cur_instruction);
				break;
			}
		}
	}

	int halting_state_index = get_halting_state_index();
	std::list<Cell>::iterator it = states_table.begin();
	int i = 0;
	for (it,i; it != states_table.end(); ++it,++i)
	{
		if (halting_state_index == i)
			halting_state = it->state;
	}
}

Graph::~Graph()
{
}

// This function:
//	1) Changes the state names from the second graph so that there are no clashes. (in the states_table as well)
//	2) Finds the halting state of the first graph and puts the starting state of the second graph on its place.
void Graph::combine_with(Graph& other) //todo: revision this.
{
	change_state_names(other);

	std::vector<std::vector<Instruction>>::iterator vec_it = adj_list.begin();
	int index_to_delete = other.adj_list.size();
	bool found_halt = false;
	for (vec_it; vec_it != adj_list.end(); ++vec_it)
	{
		if (vec_it->empty())
		{
			found_halt = true;
			adj_list.insert(vec_it, other.adj_list.begin(), other.adj_list.end());
		}
		if (found_halt)
		{
			adj_list.erase(adj_list.begin() + index_to_delete);
			break;
		}
		index_to_delete++;
	}
}

std::vector<std::vector<Instruction>> Graph::get_adj_list() const
{
	return adj_list;
}

std::string Graph::get_initial_state() const
{
	return initial_state;
}

size_t Graph::get_states_count() const
{
	return states_count;
}

std::string Graph::get_halting_state() const
{
	return halting_state;
}

std::vector<Instruction> Graph::get_initial_state_vector() const
{
	return adj_list[0];
}

bool Graph::has_state(std::string cur_state)
{
	std::list<Cell>::iterator it = states_table.begin();
	for (it; it != states_table.end(); ++it)
	{
		if (it->state == cur_state)
			return true;
	}
	return false;
}



std::vector<Instruction> Graph::go_to_state(std::string cur_state, char read_symbol)
{
	int cur_state_index = get_state_index(cur_state);
	int new_state_index = -1;
	std::string new_state = "";
	for (Instruction inst : adj_list[cur_state_index])
	{
		if (inst.get_read_symbol() == read_symbol)
			new_state = inst.get_to_state();
	}
	new_state_index = get_state_index(new_state);
	if (new_state_index == -1)
	{
		std::cerr << "Error: Read symbol is not defined in the rules!\n";
	}
	return adj_list[new_state_index];
}

void Graph::test_graph_initialization()
{
	std::cout << "TEST STATES_TABLE#####################\n";
	std::list<Cell>::iterator lit = states_table.begin();
	for (lit; lit != states_table.end(); ++lit)
	{
		std::cout << "Index: " << lit->index << "\t" << lit->state << "\n";
	}

	std::cout << "TEST adj_list#####################\n";
	std::vector<std::vector<Instruction>>::iterator it = adj_list.begin();
	for (it; it != adj_list.end(); ++it)
	{
		for (Instruction inst : *it)
		{
			std::cout << "\t" << inst.get_read_symbol() << "{" << inst.get_from_state() << "} -> " << inst.get_write_symbol() << "{" << inst.get_to_state() << "}" << inst.get_direction() << "\n";
		}
	}
}
