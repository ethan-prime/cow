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
    // print_tokens(tokens);

    Parser parser(tokens);
    program_node p = parser.parse_program();
    // print_program(p);

    Generator generator(p);
    generator.to_asm();

    return 0;
}