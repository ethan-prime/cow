#include "generator.h"
#include <iostream>
#include <vector>

using namespace std;

// constructor
Generator::Generator(program_node program) : program(program) {};

// generates x86 assembly instructions based on program_node/AST.
void Generator::to_asm()
{
    this->collect_variables();
    this->collect_labels();
    // allocate memory for variables on the stack
    cout << "   sub $" << this->variables.size() * 8 << ", %rsp" << endl;
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
        if_then_to_asm(get<if_then_node>(stmt.statement));
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
    if (!this->goto_valid(goto_))
        exit(0);
    cout << "   jmp " << goto_.label << endl;
}

void Generator::assign_to_asm(assign_node assign)
{
}

void Generator::if_then_to_asm(if_then_node if_then)
{
    if (!this->comparison_valid(if_then.comparison) || !this->statement_valid(*if_then.statement))
    {
        // can't compile because of a sem error.
        exit(0);
    }
}

// make sure we never access any undeclared vars,
// and add any valid variables to our variables vector.
bool Generator::statement_valid(statement_node stmt)
{
    switch (stmt.kind)
    {
    case STMT_ASSIGN:
        return this->expr_valid(get<assign_node>(stmt.statement).expr);
        break;
    case STMT_GOTO:
        return this->goto_valid(get<goto_node>(stmt.statement));
        return true;
        break;
    case STMT_IF_THEN:
        return this->comparison_valid(get<if_then_node>(stmt.statement).comparison) && this->statement_valid(*get<if_then_node>(stmt.statement).statement);
        break;
    case STMT_LABEL:
        return true;
        break;
    case STMT_PRINT:
        return this->term_valid(get<print_node>(stmt.statement).term);
        break;
    default:
        cout << "possible error checking if stmt is valid" << endl;
        return false;
        break;
    }
}

bool Generator::comparison_valid(comparison_node comp)
{
    if (comp.kind == COMP_LESS_THAN)
    {
        return this->term_valid(get<term_binary_node>(comp.binary_expr).lhs) && this->term_valid(get<term_binary_node>(comp.binary_expr).rhs);
    }
    else
    {
        cout << "possible error checking if comparison is valid" << endl;
        return false;
    }
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

bool Generator::goto_valid(goto_node goto_)
{
    for (auto l : this->labels)
    {
        if (l == goto_.label)
            return true;
    }
    cout << "SEMANTIC ERROR: label \"" << goto_.label << "\" is not defined." << endl;
    return false;
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

void Generator::collect_variables()
{
    for (auto stmt : this->program.statements)
    {
        if (stmt.kind == STMT_ASSIGN)
        {
            assign_node assign = get<assign_node>(stmt.statement);
            if (!this->expr_valid(assign.expr))
            {
                // can't compile because of a sem error.
                exit(0);
            }
            // if it's a valid ssignment, make sure we store that
            // this variable isa now defined.
            if (!in_vector(this->variables, assign.identifier))
                this->variables.push_back(assign.identifier);
        }
    }
}

void Generator::collect_labels()
{
    for (auto stmt : this->program.statements)
    {
        if (stmt.kind == STMT_LABEL)
        {
            label_node label = get<label_node>(stmt.statement);
            // if it's a valid ssignment, make sure we store that
            // this variable isa now defined.
            if (!in_vector(this->labels, label.label))
                this->labels.push_back(label.label);
        }
    }
}

template <typename T>
bool in_vector(vector<T> vec, T value)
{
    for (auto v : vec)
    {
        if (v == value)
            return true;
    }
    return false;
}