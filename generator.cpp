#include "generator.h"
#include <iostream>
#include <vector>

using namespace std;

// constructor
Generator::Generator(program_node program) : program(program), current_if_index(0) {};

// generates x86 assembly instructions based on program_node/AST.
void Generator::to_asm()
{
    this->collect_variables();
    this->collect_labels();

    // INITIAL SETUP
    cout << ".section .text" << endl;
    cout << ".globl _start" << endl;
    cout << "_start:" << endl;

    // setup stack frame
    cout << "   mov %rsp, %rbp" << endl;
    // allocate memory for variables on the stack
    cout << "   sub $" << this->variables.size() * 8 << ", %rsp" << endl;
    for (statement_node stmt : this->program.statements)
    {
        this->statement_to_asm(stmt);
    }
    cout << ".exit:" << endl;
    cout << "   movq $60, %rax" << endl;
    cout << "   xor %rdi, %rdi" << endl;
    cout << "   syscall" << endl;

    // predefined functions
    cout << endl;
    cout << "int_to_ascii:" << endl;
    cout << "   movq $10, %rbx" << endl;
    cout << "   xor %rdx, %rdx" << endl;
    cout << "   div %rbx" << endl;
    cout << "   add $48, %dl" << endl;
    cout << "   movb %dl, (%rcx)" << endl;
    cout << "   inc %rsi" << endl;
    cout << "   dec %rcx" << endl;
    cout << "   test %rax, %rax" << endl;
    cout << "   jnz int_to_ascii" << endl;
    cout << "   inc %rcx" << endl;
    cout << "   mov %rsi, %rdx" << endl;
    cout << "   mov %rcx, %rsi" << endl;
    cout << "   ret" << endl;

    cout << endl;
    cout << "ascii_to_int:" << endl;
    cout << "   movb (%r9), %al" << endl;
    cout << "   movzbq %al, %rax" << endl;
    cout << "   sub $48, %rax" << endl;
    cout << "   mul %rbx" << endl;
    cout << "   add %rax, %r8" << endl;
    cout << "   mov %rbx, %rax" << endl;
    cout << "   mov $10, %rbx" << endl;
    cout << "   mul %rbx" << endl;
    cout << "   mov %rax, %rbx" << endl;
    cout << "   dec %rcx" << endl;
    cout << "   dec %r9" << endl;
    cout << "   test %rcx, %rcx" << endl;
    cout << "   jnz ascii_to_int" << endl;
    cout << "   inc %r9" << endl;
    cout << "   mov %r8, %rax" << endl;
    cout << "   ret" << endl;
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
        print_to_asm(get<print_node>(stmt.statement));
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
    // we assume that the result of the expr is in %rax.
    expr_to_asm(assign.expr);

    int index = this->variable_index(assign.identifier);
    // store variable at %rsp + index*8
    cout << "   movq %rax, -" << index * 8 + 8 << "(%rbp)" << endl;
}

void Generator::if_then_to_asm(if_then_node if_then)
{
    if (!this->comparison_valid(if_then.comparison) || !this->statement_valid(*if_then.statement))
    {
        // can't compile because of a sem error.
        exit(0);
    }
    // we assume that the result of the comparison is in %rax.
    comparison_to_asm(if_then.comparison);

    cout << "   test %rax, %rax" << endl; // see if %rax is 0
    // if %rax is 0, the condition is true, so we should skip to after
    // the then statement.
    cout << "   jz .ENDIF" << this->current_if_index << endl;
    int endif = this->current_if_index;
    this->current_if_index++;

    // this code will run if the jz instruction doesn't execute
    // (if the comparison evaluates to true)
    statement_to_asm(*if_then.statement);
    cout << ".ENDIF" << endif << ":" << endl;
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
    else if (expr.kind == BINARY_EXPR_PLUS || expr.kind == BINARY_EXPR_MINUS || expr.kind == BINARY_EXPR_MULT || expr.kind == BINARY_EXPR_DIV)
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
    this->buffer_ptr = this->variables.size() * 8;
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

int Generator::variable_index(string var)
{
    for (unsigned int i = 0; i < this->variables.size(); i++)
    {
        if (this->variables[i] == var)
            return i;
    }
    // not found
    return -1;
}

void Generator::expr_to_asm(expr_node expr)
{
    if (expr.kind == UNARY_EXPR)
    {
        term_to_asm(get<term_node>(expr.expr));
        // result is already in %rax.
    }
    else if (expr.kind == BINARY_EXPR_PLUS)
    {
        term_binary_node binary_expr = get<term_binary_node>(expr.expr);
        term_to_asm(binary_expr.lhs);
        // assume lhs result in %rax. let's store in %rcx for now.
        cout << "   mov %rax, %rcx" << endl;
        term_to_asm(binary_expr.rhs);
        // now we have to add them
        cout << "   addq %rcx, %rax" << endl;
        // notice result is in %rax.
    }
    else if (expr.kind == BINARY_EXPR_MINUS)
    {
        term_binary_node binary_expr = get<term_binary_node>(expr.expr);
        term_to_asm(binary_expr.rhs);
        // assume rhs result in %rax. let's store in %rcx for now.
        cout << "   mov %rax, %rcx" << endl;
        term_to_asm(binary_expr.lhs);
        // now we have to subtract them
        cout << "   subq %rcx, %rax" << endl;
        // notice result is in %rax.
    }
    else if (expr.kind == BINARY_EXPR_MULT)
    {
        term_binary_node binary_expr = get<term_binary_node>(expr.expr);
        term_to_asm(binary_expr.lhs);
        // assume lhs result in %rax. let's store in %rcx for now.
        cout << "   mov %rax, %rcx" << endl;
        term_to_asm(binary_expr.rhs);
        // now we have to multiply them
        cout << "   mul %rcx" << endl;
    }
    else if (expr.kind == BINARY_EXPR_DIV)
    {
        term_binary_node binary_expr = get<term_binary_node>(expr.expr);
        term_to_asm(binary_expr.rhs);
        // assume rhs result (divisor) in %rax. let's store in %rcx for now.
        cout << "   mov %rax, %rcx" << endl;
        term_to_asm(binary_expr.lhs);
        // now we have to multiply them
        cout << "   xor %rdx, %rdx" << endl; // clear for div instr
        cout << "   div %rcx" << endl;
    }
}

void Generator::input_to_asm(term_node term)
{
    // we need to allocate space for the input
    cout << "   sub $24, %rsp" << endl;
    cout << "   mov %rbp, %rsi" << endl;
    cout << "   addq $" << this->buffer_ptr << ", %rsi" << endl;
    cout << "   movq $0, %rax" << endl;
    cout << "   movq $0, %rdi" << endl;
    cout << "   movq $24, %rdx" << endl;
    cout << "   syscall" << endl;

    // now, the number of bytes read in should be in %rax.
    // + setup for call to ascii_to_int
    cout << "   mov %rax, %rcx" << endl;
    cout << "   subq $1, %rcx" << endl;
    cout << "   mov %rsi, %r9" << endl;
    cout << "   addq %rcx, %r9" << endl;
    cout << "   subq $1, %r9" << endl;

    cout << "   movq $1, %rbx" << endl;
    cout << "   xor %r8, %r8" << endl;
    cout << "   call ascii_to_int" << endl;
    // input value is now in %rax.
}

void Generator::term_to_asm(term_node term)
{
    // we need to store the result in %rax.
    if (term.kind == TERM_IDENTIFIER)
    {
        // get its value, load from its index!
        unsigned int index = this->variable_index(term.value);
        cout << "   movq -" << index * 8 + 8 << "(%rbp)" << ", %rax" << endl;
    }
    else if (term.kind == TERM_INT_LITERAL)
    {
        cout << "   movq $" << term.value << ", %rax" << endl;
        return;
    }
    else if (term.kind == TERM_INPUT)
    {
        input_to_asm(term);
    }
    else
    {
        return;
    }
}

void Generator::comparison_to_asm(comparison_node comp)
{
    // we need to store in %rax.
    if (comp.kind == COMP_LESS_THAN)
    {
        term_binary_node binary_expr = get<term_binary_node>(comp.binary_expr);
        term_node lhs = binary_expr.lhs;
        term_to_asm(lhs);
        // lhs is in %rax -> store temp in %rcx.
        cout << "   mov %rax, %rcx" << endl;
        term_node rhs = binary_expr.rhs;
        term_to_asm(rhs);
        // rhs is in %rax, so lets compare now.
        cout << "   cmpq %rax, %rcx" << endl;
        // if res is less than 0, we know lhs < rhs,
        // so put a 1 in %rax or else %rax should be 0.
        cout << "   setl %al" << endl;
        // put it in %rax with zero extend
        cout << "   movzbq %al, %rax" << endl;
    }
}

void Generator::print_to_asm(print_node print)
{
    // get term that must be printed
    term_to_asm(print.term);

    // assume result is in %rax.
    // however, it is an actual integer but we
    // need to print ASCII codes.

    // now, we have to allocate memory on the stack
    // to store our buffer

    // preparation for call to int_to_ascii
    cout << "   movq $1, %rsi" << endl; // we have to print at least a \n
    cout << "   sub $24, %rsp" << endl;
    cout << "   mov %rbp, %rcx" << endl;
    cout << "   addq $" << this->buffer_ptr + 23 << ", %rcx" << endl;
    cout << "   movq $0x0A, (%rcx)" << endl; // newline character at end of str
    cout << "   dec %rcx" << endl;
    cout << "   call int_to_ascii" << endl;

    this->buffer_ptr += 24;
    // sys_write: %rax = 1; %rdi = unsigned int fd; %rsi = const char *buf; %rdx = size_t count;

    // now we assume that our buffer ptr is in %rsi and the length is in %rdx.
    // time for a syscall

    cout << "   movq $1, %rax" << endl;
    cout << "   movq $1, %rdi" << endl;
    cout << "   syscall" << endl;
}