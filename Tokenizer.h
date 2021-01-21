#pragma once

#include <list>
#include <stack>
#include <cassert>
#include <queue>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>

enum class TokenType
{
    INITIAL_STATE, OLD_STATE, NEW_STATE, HALT, READ_SYMBOL, WRITE_SYMBOL, TRANSITION_OPERATOR, L, R, S, DEFAULT, INVALID_TOKEN
};

class Token
{
private:
    std::string value;
    TokenType type;
public:
    Token(std::string, TokenType);
    std::string get_value()const;
    TokenType get_type()const;
};

    // The purpose of this class is to tokenize the input and 
    // check if there are any invalid tokens used in the files.
    // If so, we'll break as soon as we find INVALID_TOKEN
class Tokenizer
{
private:
    std::queue<Token> tokenized_rules;
    std::string input_tape;
    bool found_error;

    bool has_initial_state;
    bool has_old_state;
    bool has_read_symbol;

    bool is_symbol(char c);
    bool is_state(char c);
    bool is_direction(char c);
    bool is_transition_operator(char c);

    void recognize_initial_state(std::stringstream&);
    void recognize_state(std::stringstream&);
    void recognize_symbol(std::stringstream&);
    void recognize_direction(std::stringstream&);
    void recognize_transition_operator(std::stringstream&);
public:
    Tokenizer();

    void tokenize_rules(std::string rules_file_name);
    void tokenize_input(std::string input_file_name);

    bool check_for_errors()const;

    std::queue<Token> get_tokenized_rules()const;
    std::string get_input_tape()const;

    void test_tokenizer(); // For debugging
};

class Instruction
{
private:
    char read_symbol;
    std::string from_state;
    char write_symbol;
    std::string to_state;
    char direction;
public:
    Instruction(char, std::string, char, std::string, char);

    bool is_fully_filled();

    void set_read_symbol(const char&);
    void set_from_state(const std::string&);
    void set_write_symbol(const char&);
    void set_to_state(const std::string&);
    void set_direction(const char&);

    char get_read_symbol()const;
    std::string get_from_state()const;
    char get_write_symbol()const;
    std::string get_to_state()const;
    char get_direction()const;
};

struct Cell
{
    int index;
    std::string state;

    Cell(int, std::string);
};

// This class is meant to create instructions out of the tokens. These instructions will represent edges in a directed cyclic weighted graph.
// In this class we must also fill the states_table, which will keep track of the unique states so that we can represent them in a graph.
class InstructionTokenizer
{
private:
    std::list<Cell> states_table;
    int states_count;
    std::queue<Instruction> instructions;
    std::string initial_state;

    bool is_state(TokenType);               // Given a type, return true if the type is any of the states.
    bool add_unique_state(const Token&);    // Adds unique states in the states_table.
public:
    InstructionTokenizer();
    void parse_tokens(std::queue<Token>);  // here we find the initial state as well.

    std::list<Cell> get_states_table()const;
    std::queue<Instruction> get_instructions()const;
    std::string get_initial_state()const;
    int get_states_count()const;

    void test_instruction_tokenizer(); // actually a modifying function so be careful
};