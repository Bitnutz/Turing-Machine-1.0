#include "Tape.h"

Tape::Tape()
{
	for (int i = 0; i < 5; i++)
	{
		lhs.push('_');
		rhs.push('_');
	}
	head = rhs.top();
	tape_length = 0;

	// Overall, we put five _ in each stack to pinpoint the boundaries of the beginning and the end.
	// However, the tape in the TM doesn't have begin and end, but it has to be represented in some way.
}

bool Tape::is_empty() const
{
	return lhs.size() == 1 && rhs.size() == 1;
}

void Tape::fill_tape(std::string& input)
{
	while(!input.empty())
	{
		rhs.push(input.back());
		input.pop_back();
		tape_length++;
	}
	head = rhs.top();
}

void Tape::move_head_left(char write_symbol)
{
	rhs.pop();
	rhs.push(write_symbol);
	if (lhs.top() == '_' && lhs.size() <= 5) // means we reached the end of the beginning
	{

		rhs.push('_');
		head = '_';
	}
	else
	{
		rhs.push(lhs.top());
		head = rhs.top();
		lhs.pop();
	}
}

void Tape::move_head_right(char write_symbol)
{
	rhs.pop();
	rhs.push(write_symbol);
	if (rhs.size() <= 6) // means the head is the only non-empty cell
	{
		lhs.push(head);
		head = '_';
		rhs.pop();
	}
	else if (head == '_' && rhs.size() <= 5) // means we reached the end of the end.
	{
		lhs.push('_');
	}
	else
	{
		lhs.push(rhs.top());
		rhs.pop();
		head = rhs.top();
	}
}

char Tape::get_head_symbol() const
{
	return head;
}

std::string Tape::get_tape()
{
	std::string result = "";
	std::stack<char> temp;
	while (!lhs.empty())
	{
		temp.push(lhs.top());
		lhs.pop();
	}
	while (!temp.empty())
	{
		lhs.push(temp.top());
		result = result + temp.top() + " ";
		temp.pop();
	}

	while (!rhs.empty())
	{
		temp.push(rhs.top());
		result = result + rhs.top() + " ";
		rhs.pop();
	}
	while (!temp.empty())
	{
		rhs.push(temp.top());
		temp.pop();
	}
	return result;
}


