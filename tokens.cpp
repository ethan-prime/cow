#include "tokens.h"
using namespace std;

Token::Token(token_type kind, string value, unsigned int line_number) : kind(kind), value(value), line_number(line_number) {};

string Token::repr()
{
    switch (this->kind)
    {
    case IDENTIFIER:
        return "idenitifer";
    case STR_LITERAL:
        return "string";
    case INT_LITERAL:
        return "int";
    case INPUT:
        return "input";
    case PRINT:
        return "print";
    case GOTO:
        return "goto";
    case IF:
        return "if";
    case THEN:
        return "then";
    case OP_LT:
        return "op_lt";
    case OP_PLUS:
        return "op_plus";
    case LABEL:
        return "label";
    case EQUAL:
        return "equal";
    case INVALID:
        return "invalid";
    case END_OF_FILE:
        return "eof";
    case OP_MINUS:
        return "op_minus";
    case OP_MULT:
        return "op_mult";
    case OP_DIV:
        return "op_div";
    case EQUALEQUAL:
        return "equalequal";
    default:
        return "moo! something went wrong when tokenizing...";
    }
}