#pragma once
#include "Tokenizer.h"


// Tape class is used in the Machine class (which is our main engine)
// It works with the input.
class Tape
{
private:
	std::stack<char> lhs, rhs;
	char head;
	size_t tape_length;
public:

	Tape();

	bool is_empty()const;

	void fill_tape(std::string&);
	void move_head_left(char);
	void move_head_right(char);

	char get_head_symbol()const;
	std::string get_tape();
};