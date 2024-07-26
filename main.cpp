#include "lexer.h"
#include <iostream>
using namespace std;

int main()
{
    cout << "welcome to the cow language" << endl;

    // read file here into buffer
    string buffer = read_file("example.milk");

    cout << "INPUT FILE:" << endl;
    cout << buffer << endl
         << endl;

    Lexer lexer(buffer);

    vector<Token> tokens = lexer.tokenize();
    // print tokens
    cout << "LEXED FILE:" << endl;
    for (auto token : tokens)
    {
        if (token.value != "")
        {
            cout << token.repr() << " (" << token.value << ")" << endl;
        }
        else
        {
            cout << token.repr() << endl;
        }
    }
    return 0;
}