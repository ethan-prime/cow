#include <string>

#pragma once

using namespace std;

enum token_type
{
    IDENTIFIER,
    STR_LITERAL,
    INT_LITERAL,
    INPUT,
    PRINT,
    GOTO,
    IF,
    THEN,
    OP_LT,
    OP_PLUS,
    OP_MINUS,
    OP_MULT,
    OP_DIV,
    LABEL,
    EQUAL,
    INVALID,
    END_OF_FILE,
};

class Token
{
public:
    token_type kind;
    string value;
    unsigned int line_number;

    Token(token_type kind, string value, unsigned int line_number);

    string repr();
};
