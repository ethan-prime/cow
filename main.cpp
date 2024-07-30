#include "lexer.h"
#include "parser.h"
#include "generator.h"
#include <iostream>
using namespace std;

int main()
{
    string buffer = read_file("example.milk");
    Lexer lexer(buffer);

    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    program_node p = parser.parse_program();

    Generator generator(p);
    generator.to_asm();

    return 0;
}