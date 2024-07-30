#include "parser.h"

#pragma once

class Generator
{
private:
    program_node program;

public:
    // constructor
    Generator(program_node program);

    // generates x86 assembly instructions based on program_node/AST.
    void to_asm();

    // generates x86 assmebly for a statement
    void statement_to_asm(statement_node stmt);

    void label_to_asm(label_node label);
    void goto_to_asm(goto_node goto_);
};