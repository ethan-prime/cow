#include <string>
using namespace std;

class Lexer
{
private:
    string buffer;
    unsigned int pos;
    unsigned int read_pos;
    char current_char;

public:
    // constructor
    Lexer(const string &buffer);

    // peeks at char in buffer
    char peek();

    // reads char, moves to next in buffer
    char read();

    // skips until current char isnt space
    void skip_whitespace();
};