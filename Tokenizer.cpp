
#include "Tokenizer.h"


Token::Token(std::string value = "", TokenType type = TokenType::DEFAULT) :value(value), type(type) {}

std::string Token::get_value()const
{
    return value;
}

TokenType Token::get_type()const
{
    return type;
}

bool Tokenizer::is_symbol(char c)
{
    if (is_direction(c))
        return false;
    else if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c == '_')
        return true;
    else
        return false;
}

bool Tokenizer::is_state(char c)
{
    if (c == '{')
        return true;
    return false;
}

bool Tokenizer::is_direction(char c)
{
    if (c == 'R' || c == 'L' || c == 'S')
        return true;
    return false;
}

bool Tokenizer::is_transition_operator(char c)
{
    if (c == '-')
        return true;
    return false;
}

void Tokenizer::recognize_initial_state(std::stringstream& cur_line_stream)
{
    if (has_initial_state)
        return;

    std::string to_store = "";
    char cur_char = cur_line_stream.peek();
    int counter = 0;
    while (counter < 5)
    {
        to_store += cur_char;
        cur_line_stream.ignore();
        cur_char = cur_line_stream.peek();
        counter++;
    }
    if (to_store != "init:" || !is_state(cur_char))
    {
        std::cerr << "An initial state must be declared!\nSample input:\ninit:{start_state}\nsymbol{state}->symbol{state}direction\n";
        found_error = true;
        return;
    }

    cur_line_stream.ignore();
    cur_char = cur_line_stream.peek();

    std::string token_value = "";
    while (!cur_line_stream.eof() && cur_char != '}')
    {
        token_value += cur_char;
        cur_line_stream.ignore();
        cur_char = cur_line_stream.peek();
    }

    tokenized_rules.push(Token(token_value, TokenType::INITIAL_STATE));
    has_initial_state = true;
    cur_line_stream.ignore();
}


void Tokenizer::recognize_state(std::stringstream& str_stream)
{
    if (str_stream.eof())
        return;

    std::string to_store = "";
    char cur_char = str_stream.peek();

    if (!is_state(cur_char))
        return;

    str_stream.ignore(); // Here we ignore, because if we enter the above if statement we'd erase one char.

    while (cur_char != '}' && !str_stream.eof())
    {
        if (cur_char == '{' || cur_char == '}')
        {
            cur_char = str_stream.peek();
            str_stream.ignore();
            continue;
        }
        to_store += cur_char;
        cur_char = str_stream.peek();
        str_stream.ignore();
    }
    if (to_store == "HALT")
    {
        tokenized_rules.push(Token(to_store, TokenType::HALT));
        return;
    }

    // We can firstly have old state then a new state. 
    if (!has_old_state)
    {
        tokenized_rules.push(Token(to_store, TokenType::OLD_STATE));
        has_old_state = true;
    }
    else
    {
        tokenized_rules.push(Token(to_store, TokenType::NEW_STATE));
        has_old_state = false;
    }
}

void Tokenizer::recognize_symbol(std::stringstream& str_stream)
{
    char cur_char = str_stream.peek();
    if (!is_symbol(cur_char))
        return;
    std::string to_store = "";
    to_store += cur_char;

    str_stream.ignore();
    cur_char = str_stream.peek();

    if (!is_state(cur_char)) // if we don't have a state after the symbol the syntax is wrong.
    {
        found_error = true;
        return;
    }


    if (!has_read_symbol)
    {
        tokenized_rules.push(Token(to_store, TokenType::READ_SYMBOL));
        has_read_symbol = true;
    }
    else
    {
        tokenized_rules.push(Token(to_store, TokenType::WRITE_SYMBOL));
        has_read_symbol = false;
    }
}

void Tokenizer::recognize_direction(std::stringstream& str_stream)
{
    char cur_char = str_stream.peek();
    if (!is_direction(cur_char))
        return;
    else
    {
        std::string to_store = "";
        to_store += cur_char;

        if (cur_char == 'L')
            tokenized_rules.push(Token(to_store, TokenType::L));
        else if (cur_char == 'R')
            tokenized_rules.push(Token(to_store, TokenType::R));
        else
            tokenized_rules.push(Token(to_store, TokenType::S));
    }
    str_stream.ignore();
}

void Tokenizer::recognize_transition_operator(std::stringstream& str_stream)
{
    std::string to_store = "";
    char cur_char = str_stream.peek();

    if (!is_transition_operator(cur_char))
        return;

    while (!is_symbol(cur_char) && !is_state(cur_char) && !is_direction(cur_char))
    {
        to_store += cur_char;
        str_stream.ignore();
        cur_char = str_stream.peek();
    }

    tokenized_rules.push(Token(to_store, TokenType::TRANSITION_OPERATOR));
}

Tokenizer::Tokenizer() : found_error(false), has_initial_state(false), has_old_state(false), has_read_symbol(false) {}

void Tokenizer::tokenize_rules(std::string rules_file_name)
{
    std::ifstream rules_file(rules_file_name);
    assert(rules_file && "Could not open the file with the rules!\n");
    std::string cur_line = "";


    while (std::getline(rules_file, cur_line))
    {
        std::stringstream cur_line_stream(cur_line);

        while (!cur_line_stream.eof()) // while we exhaust the current line we'll keep recognizing stuff.
        {
            if (is_state(cur_line_stream.peek()) || !has_initial_state)
            {
                recognize_initial_state(cur_line_stream);
                recognize_state(cur_line_stream);
            }
            else if (is_symbol(cur_line_stream.peek()))
            {
                recognize_symbol(cur_line_stream);
            }
            else if (is_direction(cur_line_stream.peek()))
            {
                recognize_direction(cur_line_stream);
                break; // when we reach a direction we can simply break, because the directions are written at the end of the line. 
            }
            else if (is_transition_operator(cur_line_stream.peek()))
            {
                recognize_transition_operator(cur_line_stream);
            }
            else
            {
                tokenized_rules.push(Token("", TokenType::INVALID_TOKEN));
                found_error = true;
                std::cerr << "Invalid token detected!\n";
                return;
            }
            if (found_error)
                return;
        }
    }
}

void Tokenizer::tokenize_input(std::string input_file_name)
{
    if (input_file_name == "none") // in case we don't want to create a machine with input.
    {
        input_tape = "";
        return;
    }
    std::ifstream input_file(input_file_name);
    assert(input_file && "Could not open the file with the input tape!\n");
    char cur_symbol = input_file.peek();
    while (is_symbol(cur_symbol))
    {
        input_tape += cur_symbol;
        input_file.ignore();
        cur_symbol = input_file.peek();
    }

    if (!input_file.eof())
    {
        std::cerr << "Invalid tape symbol found!\n\t--> '" << (char)input_file.peek() << "'\n";
        found_error = true;
        return;
    }
}

bool Tokenizer::check_for_errors()const
{
    return found_error;
}

std::queue<Token> Tokenizer::get_tokenized_rules()const
{
    return tokenized_rules;
}

std::string Tokenizer::get_input_tape()const
{
    return input_tape;
}

void Tokenizer::test_tokenizer()
{
    while (!tokenized_rules.empty())
    {
        Token cur_token = tokenized_rules.front();
        tokenized_rules.pop();
        std::cout << "  " << cur_token.get_value() << "\n";
    }
}

Instruction::Instruction(char read_symbol = ' ', std::string from_state = "", char write_symbol = ' ', std::string to_state = "", char direction = ' ')
    :read_symbol(read_symbol), from_state(from_state), write_symbol(write_symbol), to_state(to_state), direction(direction) {}

bool Instruction::is_fully_filled()
{
    if (read_symbol != ' ' && from_state != "" && write_symbol != ' ' && to_state != "" && direction != ' ')
        return true;
    return false;
}

void Instruction::set_read_symbol(const char& c)
{
    read_symbol = c;
}
void Instruction::set_from_state(const std::string& str)
{
    from_state = str;
}
void Instruction::set_write_symbol(const char& c)
{
    write_symbol = c;
}
void Instruction::set_to_state(const std::string& str)
{
    to_state = str;
}
void Instruction::set_direction(const char& c)
{
    direction = c;
}

char Instruction::get_read_symbol()const
{
    return read_symbol;
}
std::string Instruction::get_from_state()const
{
    return from_state;
}
char Instruction::get_write_symbol()const
{
    return write_symbol;
}
std::string Instruction::get_to_state()const
{
    return to_state;
}
char Instruction::get_direction()const
{
    return direction;
}


Cell::Cell(int index = 0, std::string state = "") :index(index), state(state) {}


bool InstructionTokenizer::is_state(TokenType type)
{
    if (type == TokenType::INITIAL_STATE || type == TokenType::HALT || type == TokenType::OLD_STATE || type == TokenType::NEW_STATE)
        return true;
    return false;
}

bool InstructionTokenizer::add_unique_state(const Token& cur_state)
{
    if (!is_state(cur_state.get_type()))
        return false;
    else if (states_table.empty())
    {
        states_table.emplace_back(Cell(states_count++, cur_state.get_value()));
        return true;
    }
    else
    {
        std::string str_state = cur_state.get_value();
        std::list<Cell>::iterator it = states_table.begin();
        for (it; it != states_table.end(); ++it)
        {
            if (it->state == str_state) // If we find a state that is the same as the passed state, we return false, else we add it to the list.
                return false;
        }
        states_table.emplace_back(Cell(states_count++, str_state));
        return true;
    }
}

InstructionTokenizer::InstructionTokenizer() :states_count(0), initial_state("") {}

void InstructionTokenizer::parse_tokens(std::queue<Token> tokens)
{
    if (tokens.empty())
    {
        std::cerr << "Empty queue of tokens passed to the token parser!\n";
        return;
    }
    Token cur_token;
    Instruction single_instruction;
    while (!tokens.empty())
    {
        cur_token = tokens.front();
        while (!single_instruction.is_fully_filled())
        {
            cur_token = tokens.front();
            if (cur_token.get_type() == TokenType::INITIAL_STATE)
            {
                add_unique_state(cur_token);
                initial_state = cur_token.get_value();
            }
            else if (cur_token.get_type() == TokenType::READ_SYMBOL)
            {
                single_instruction.set_read_symbol(cur_token.get_value()[0]);
            }
            else if (cur_token.get_type() == TokenType::WRITE_SYMBOL)
            {
                single_instruction.set_write_symbol(cur_token.get_value()[0]);
            }
            else if (cur_token.get_type() == TokenType::OLD_STATE)
            {
                add_unique_state(cur_token);
                single_instruction.set_from_state(cur_token.get_value());
            }
            else if (cur_token.get_type() == TokenType::NEW_STATE || cur_token.get_type() == TokenType::HALT) // halt can be encountered only after the transition operator.(we can't move from halt to something else)
            {
                add_unique_state(cur_token);
                single_instruction.set_to_state(cur_token.get_value());
            }
            else if (cur_token.get_type() == TokenType::L || cur_token.get_type() == TokenType::R || cur_token.get_type() == TokenType::S)
            {
                single_instruction.set_direction(cur_token.get_value()[0]);
            }
            else if (cur_token.get_type() == TokenType::TRANSITION_OPERATOR || cur_token.get_type() == TokenType::DEFAULT)
            {
                tokens.pop();
                cur_token = tokens.front();
                continue;
            }
            else
            {
                std::cerr << "Invalid token detected in the token parser!\n";
                return;
            }
            tokens.pop();
        }
        instructions.push(single_instruction);
        single_instruction = Instruction();
    }
}

std::list<Cell> InstructionTokenizer::get_states_table() const
{
    return states_table;
}

std::queue<Instruction> InstructionTokenizer::get_instructions() const
{
    return instructions;
}

std::string InstructionTokenizer::get_initial_state() const
{
    return initial_state;
}

int InstructionTokenizer::get_states_count() const
{
    return states_count;
}

void InstructionTokenizer::test_instruction_tokenizer() // just for debugging.
{
    Instruction cur_instruction;
    while (!instructions.empty())
    {
        cur_instruction = instructions.front();
        std::cout << cur_instruction.get_read_symbol() << " " << cur_instruction.get_from_state() << " -> "
            << cur_instruction.get_write_symbol() << " " << cur_instruction.get_to_state() << " " << cur_instruction.get_direction() << "\n";
        instructions.pop();
    }

    std::list<Cell>::iterator it = states_table.begin();
    for (it; it != states_table.end(); ++it)
    {
        std::cout << "index: " << it->index << "\tstate: " << it->state << "\n";
    }
}