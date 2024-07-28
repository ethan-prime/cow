#include "parser.h"
#include <iostream>
using namespace std;

// constructor
Parser::Parser(vector<Token> tokens) : tokens(tokens) {};

// returns current token in tokenfeed
Token Parser::current_token()
{
    return this->tokens[this->pos];
}

// advances to next token in the tokenfeed
void Parser::advance()
{
    this->pos++;
}

// called when invalid syntax/parsing error
void Parser::error(const string &expected)
{
    cout << "Expected " << expected << endl;
    cout << "Found token: ";
    Token token = this->current_token();
    if (token.value != "")
    {
        cout << token.repr() << " (" << token.value << ") @ line " << token.line_number << endl;
    }
    else
    {
        cout << token.repr() << " @ line " << token.line_number << endl;
    }
    exit(0);
};

// parses a list of tokens into a program. returns a program node (AST representation).
// <program> ::= <statement> <statement>* ... <statement>*
program_node Parser::parse_program()
{
    // we will add statements to this and return when done parsing.

    program_node program;

    do
    {
        program.statements.push_back(this->parse_statement());
    } while (this->current_token().kind != END_OF_FILE);

    return program;
};

statement_node Parser::parse_statement()
{
    // <statement> ::= <assignment> | 'if' <comparison> 'then' <statement> | 'goto' <label> | 'print' <term> | <label>
    if (this->current_token().kind == IDENTIFIER)
    {
        // this has to be an assignment!
        return this->parse_assignment();
    }
    else if (this->current_token().kind == IF)
    {
        // this is an if then statement
        return this->parse_if_then();
    }
    else if (this->current_token().kind == GOTO)
    {
        // this is a goto...
        return this->parse_goto();
    }
    else if (this->current_token().kind == PRINT)
    {
        return this->parse_print();
    }
    else if (this->current_token().kind == LABEL)
    {
        return this->parse_label();
    }
    else
    {
        error("a statement");
    }
};

// <assignment> ::= <identifier> = <expr>
statement_node Parser::parse_assignment()
{
    statement_node stmt;
    string identifier;
    expr_node expr;

    if (this->current_token().kind == IDENTIFIER)
    {
        identifier = this->current_token().value;
        this->advance();
    }
    else
    {
        error("identifier");
    }
    if (this->current_token().kind == EQUAL)
    {
        this->advance();
    }
    else
    {
        error("=");
    }
    expr = this->parse_expr();
    stmt.kind = STMT_ASSIGN;
    stmt.statement = assign_node{identifier, expr};
    return stmt;
};

// <expr> ::= <term> | <term> <arithmetic_op> <term>
expr_node Parser::parse_expr()
{
    expr_node expr;
    term_node lhs = this->parse_term();
    term_node rhs;

    if (this->current_token().kind == OP_PLUS)
    {
        this->advance();
        rhs = this->parse_term();

        expr.expr = term_binary_node{lhs, rhs};
        expr.kind = BINARY_EXPR_PLUS;
    }
    else
    {
        // unary expr
        expr.expr = lhs;
        expr.kind = UNARY_EXPR;
    }
    return expr;
};

// <term>:: = <identifier> | <int_literal> | 'input'
term_node Parser::parse_term()
{
    term_node term;
    if (this->current_token().kind == INPUT)
    {
        term.kind = TERM_INPUT;
    }
    else if (this->current_token().kind == INT_LITERAL)
    {
        term.kind = TERM_INT_LITERAL;
        term.value = this->current_token().value;
    }
    else if (this->current_token().kind == IDENTIFIER)
    {
        term.kind = TERM_IDENTIFIER;
        term.value = this->current_token().value;
    }
    else
    {
        error("term");
    }
    this->advance();
    return term;
};

// 'if' <comparison> 'then' <statement>
statement_node Parser::parse_if_then()
{
    statement_node stmt;
    if_then_node if_then;

    if (this->current_token().kind == IF)
    {
        this->advance();
    }
    else
    {
        error("if");
    }
    if_then.comparison = this->parse_comparison();
    if (this->current_token().kind == THEN)
    {
        this->advance();
    }
    else
    {
        error("then");
    }
    statement_node then_stmt = this->parse_statement();
    if_then.statement = &then_stmt;
    stmt.kind = STMT_IF_THEN;
    return stmt;
};

// <comparison> ::= <term> <comparison_op> <term>
comparison_node Parser::parse_comparison()
{
    comparison_node comp;

    term_node lhs, rhs;

    lhs = parse_term();

    // TODO: add more comparison ops later
    if (this->current_token().kind == OP_LT)
    {
        this->advance();
    }
    else
    {
        error("<");
    }

    rhs = parse_term();
    comp.binary_expr = term_binary_node{lhs, rhs};
    comp.kind = COMP_LESS_THAN;

    return comp;
};

// 'goto' <label>
statement_node Parser::parse_goto()
{
    statement_node stmt;
    if (this->current_token().kind == GOTO)
    {
        this->advance();
    }
    else
    {
        error("goto");
    }
    if (this->current_token().kind == LABEL)
    {
        stmt.statement = goto_node{this->current_token().value};
    }
    else
    {
        error("label");
    }
    this->advance();
    stmt.kind = STMT_GOTO;
    return stmt;
};

// <label> ::= ':' <str_literal>
statement_node Parser::parse_label()
{
    statement_node stmt;
    if (this->current_token().kind == LABEL)
    {
        stmt.statement = label_node{this->current_token().value};
    }
    else
    {
        error("label");
    }
    this->advance();
    stmt.kind = STMT_LABEL;
    return stmt;
}

statement_node Parser::parse_print()
{
    statement_node stmt;
    if (this->current_token().kind == PRINT)
    {
        this->advance();
    }
    else
    {
        error("print");
    }
    stmt.statement = print_node{this->parse_term()};
    stmt.kind = STMT_PRINT;
    return stmt;
};