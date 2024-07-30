#include "generator.h"
#include <iostream>
#include <vector>

using namespace std;

// constructor
Generator::Generator(program_node program) : program(program) {};

// generates x86 assembly instructions based on program_node/AST.
void Generator::to_asm()
{
    for (statement_node stmt : this->program.statements)
    {
        this->statement_to_asm(stmt);
    }
}

// generates x86 assmebly for a statement
void Generator::statement_to_asm(statement_node stmt)
{
    switch (stmt.kind)
    {
    case STMT_ASSIGN:
        assign_to_asm(get<assign_node>(stmt.statement));
        break;
    case STMT_GOTO:
        goto_to_asm(get<goto_node>(stmt.statement));
        break;
    case STMT_IF_THEN:
        break;
    case STMT_LABEL:
        label_to_asm(get<label_node>(stmt.statement));
        break;
    case STMT_PRINT:
        break;
    default:
        break;
    }
}

// label:
void Generator::label_to_asm(label_node label)
{
    cout << label.label << ":" << endl;
}

// jmp <label>
void Generator::goto_to_asm(goto_node goto_)
{
    cout << "   jmp " << goto_.label << endl;
}

void Generator::assign_to_asm(assign_node assign)
{
    if (!expr_valid(assign.expr))
    {
        // can't compile because of a sem error.
        exit(0);
    }
    // if it's a valid assignment, make sure we store that
    // this variable is now defined.
    this->variables.push_back(assign.identifier);
}

// checks if an expr has undefined variables (semantic errors)
bool Generator::expr_valid(expr_node expr)
{
    if (expr.kind == UNARY_EXPR)
    {
        return this->term_valid(get<term_node>(expr.expr));
    }
    else if (expr.kind == BINARY_EXPR_PLUS)
    {
        return this->term_valid(get<term_binary_node>(expr.expr).lhs) && this->term_valid(get<term_binary_node>(expr.expr).rhs);
    }
    else
    {
        cout << "possible error checking if expr is valid" << endl;
        return false;
    }
}

// checks if a term has undefined variables (semantic errors)
bool Generator::term_valid(term_node term)
{
    switch (term.kind)
    {
    case TERM_INPUT:
        return true;
    case TERM_INT_LITERAL:
        return true;
    case TERM_IDENTIFIER:
        return this->is_defined(term.value);
    default:
        cout << "possible error checking if term is valid" << endl;
        return false;
    }
}

// check if a variable is defined
bool Generator::is_defined(string var)
{
    for (auto v : this->variables)
    {
        if (v == var)
            return true;
    }
    cout << "SEMANTIC ERROR: variable \"" << var << "\" is not defined." << endl;
    return false;
}