#include "parser.h"

#pragma once

class Generator
{
private:
    program_node program;
    // stores all the variable names used in the program.
    // helps check for any semantic errors.
    vector<string> variables;

public:
    // constructor
    Generator(program_node program);

    // CODE GENERATORS

    // generates x86 assembly instructions based on program_node/AST.
    void to_asm();

    // generates x86 assmebly for a statement
    void statement_to_asm(statement_node stmt);

    void label_to_asm(label_node label);
    void goto_to_asm(goto_node goto_);
    void assign_to_asm(assign_node assign);

    // HELPERS

    // checks if an expr has undefined variables (semantic errors)
    bool expr_valid(expr_node expr);
    // checks if a term has undefined variables (semantic errors)
    bool term_valid(term_node term);
    bool is_defined(string var);
};