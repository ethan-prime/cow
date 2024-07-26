#include "tokens.h"
using namespace std;

string token_repr(token_type t)
{
    switch (t)
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
        return "<";
    case OP_PLUS:
        return "+";
    case LABEL:
        return "label";
    case EQUAL:
        return "=";
    case INVALID:
        return "invalid";
    }
}