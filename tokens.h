#include <string>
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
};

string token_repr(token_type t);