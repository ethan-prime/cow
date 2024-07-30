#include "lexer.h"
#include "parser.h"
#include "generator.h"
#include <iostream>
using namespace std;

int main()
{
    cout << "welcome to the cow language" << endl;

    // read file here into buffer
    string buffer = read_file("example.milk");

    /*cout << "INPUT FILE:" << endl;
    cout << buffer << endl
         << endl;
    */

    Lexer lexer(buffer);

    vector<Token> tokens = lexer.tokenize();
    /* print tokens
    cout << "LEXED FILE:" << endl;
    for (auto token : tokens)
    {
        if (token.value != "")
        {
            cout << token.repr() << " (" << token.value << ") @ line " << token.line_number << endl;
        }
        else
        {
            cout << token.repr() << " @ line " << token.line_number << endl;
        }
    }

    cout << endl;
    cout << "PARSING FILE..." << endl;
    */
    Parser parser(tokens);
    program_node p = parser.parse_program();
    // cout << endl;
    // cout << "AST:" << endl;
    // print_program(p);

    Generator generator(p);
    generator.to_asm();
    return 0;
}