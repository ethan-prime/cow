#include "lexer.h"
#include <sstream>
#include <fstream>

// constructor
Lexer::Lexer(const string &buffer) : buffer(buffer), pos(0), read_pos(0), line_number(1), current_char(0)
{
    this->read();
};

// peeks at next char in buffer (doesnt move pos)
char Lexer::peek()
{
    if (this->read_pos >= this->buffer.size())
    {
        return EOF;
    }

    return this->buffer[this->read_pos];
}

// reads and moves pos of next char in buffer
char Lexer::read()
{
    this->current_char = this->peek();

    this->pos = this->read_pos;
    this->read_pos += 1;

    return this->current_char;
}

// skips whitespace in buffer
void Lexer::skip_whitespace()
{
    while (isspace(this->current_char))
    {
        if (this->current_char == '\n')
            line_number++;
        this->read();
    }
}

// returns true if its a keyword, false otherwise. stores token_type in &kind.
bool is_keyword(const string &buf, token_type &kind)
{
    if (buf == "input")
    {
        kind = INPUT;
        return true;
    }
    else if (buf == "print")
    {
        kind = PRINT;
        return true;
    }
    else if (buf == "goto")
    {
        kind = GOTO;
        return true;
    }
    else if (buf == "if")
    {
        kind = IF;
        return true;
    }
    else if (buf == "then")
    {
        kind = THEN;
        return true;
    }
    else
    {
        return false;
    }
}

// returns next token in buffer
Token Lexer::next_token()
{
    this->skip_whitespace();

    // check current character
    if (this->current_char == EOF)
    {
        this->read();
        return Token(END_OF_FILE, "", this->line_number);
    }
    else if (this->current_char == '<')
    {
        this->read();
        return Token(OP_LT, "", this->line_number);
    }
    else if (this->current_char == '+')
    {
        this->read();
        return Token(OP_PLUS, "", this->line_number);
    }
    // add support for == soon !
    else if (this->current_char == '=')
    {
        this->read();
        return Token(EQUAL, "", this->line_number);
    }
    // label !?
    else if (this->current_char == ':')
    {
        string buf = "";
        this->read(); // advance from :

        // get label name
        while (isalpha(this->current_char))
        {
            buf.push_back(this->current_char);
            this->read();
        }

        return Token(LABEL, buf, this->line_number);
    }
    // int
    else if (isdigit(this->current_char))
    {
        string buf = "";

        // collect int
        while (isdigit(this->current_char))
        {
            buf.push_back(this->current_char);
            this->read();
        }

        return Token(INT_LITERAL, buf, this->line_number);
    }
    // idenitifer or other alpha keyword!
    else if (isalnum(this->current_char))
    {
        string buf = "";

        // collect buffer
        while (isalnum(this->current_char))
        {
            buf.push_back(this->current_char);
            this->read();
        }

        // check to see if its a keyword or not
        token_type kind;
        if (is_keyword(buf, kind))
        {
            // if its a keyword, return that.
            return Token(kind, "", this->line_number);
        }
        // else, it should be an identifier.
        return Token(IDENTIFIER, buf, this->line_number);
    }
    // INVALID TOKEN!!!
    else
    {
        this->read();
        return Token(INVALID, string(1, this->current_char), this->line_number);
    }
}

// returns a vector of tokens tokenized from a buffer string.
vector<Token> Lexer::tokenize()
{
    vector<Token> tokens;

    // lex until end of file
    Token current_token = this->next_token();
    while (current_token.kind != END_OF_FILE)
    {
        tokens.push_back(current_token);
        current_token = this->next_token();
    }

    tokens.push_back(current_token);

    return tokens;
}

// reads contents of file into &buf.
string read_file(const string &filename)
{
    ifstream file(filename); // Open the file
    if (!file)
    {
        throw std::runtime_error("Could not open file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // Read the file content into the stringstream
    return buffer.str();
}