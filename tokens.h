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
    LABEL,
    EQUAL,
    INVALID,
    END_OF_FILE,
};

class Token
// maybe we could store row, col for error printing?
{
public:
    token_type kind;
    string value;

    Token(token_type kind, string value);

    string repr();
};
