#include "lexer.h"

Lexer::Lexer(const string &buffer) : buffer(buffer), pos(0), read_pos(0), current_char(0)
{
    this->read();
};

char Lexer::peek()
{
    if (this->read_pos >= this->buffer.size())
    {
        return EOF;
    }

    return this->buffer[this->read_pos];
}

char Lexer::read()
{
    this->current_char = this->peek();

    this->pos = this->read_pos;
    this->read_pos += 1;

    return this->current_char;
}

void Lexer::skip_whitespace()
{
    while (isspace(this->current_char))
        this->read();
}

Token Lexer::next_token()
{
    Token token(INVALID, "invalid");
    return token;
}