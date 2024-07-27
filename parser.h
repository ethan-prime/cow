#include <string>
#include "tokens.h"
#include <vector>

#pragma once

using namespace std;

enum statement_type
{
    STMT_ASSIGN,
    STMT_IF_THEN,
    STMT_GOTO,
    STMT_PRINT, // make this a function in later versions?
    STMT_LABEL
};

enum expr_type
{
    EXPR_PLUS
};

enum term_kind
{
    TERM_INPUT,
    TERM_INT_LITERAL,
    TERM_IDENTIFIER,
};

struct term_node
{
    term_kind kind;
    string value; // may need union for this later, rn we are just storing identifiers and ints.
};

struct term_binary_node
{
    term_node lhs;
    term_node rhs;
};

struct expr_node
{
    expr_type kind;
    union
    {
        term_binary_node binary_expr;
    };
};

struct assign_node
{
    string identifier;
    expr_node expr;
};

enum comparison_type
{
    LESS_THAN,
};

struct comparison_node
{
    comparison_type kind;
    union
    {
        term_binary_node binary_expr;
    };
};

struct statement_node;
struct if_then_node
{
    comparison_node comparison;
    statement_node *statement;
};

struct goto_node
{
    string label;
};

struct print_node
{
    term_node term;
};

struct label_node
{
    string label;
};

struct statement_node
{
    statement_type kind;
    union
    {
        assign_node assign;
        if_then_node if_then;
        goto_node goto_;
        print_node print;
        label_node label;
    };
};

struct program_node
{
    vector<statement_node> statements;
};