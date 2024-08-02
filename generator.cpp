#include "generator.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// constructor
Generator::Generator(program_node program, string &output_filename) : program(program), current_if_index(0), file(output_filename)
{
    if (!file)
    {
        cout << "Couldn't open output file" << endl;
        exit(0);
    }
};

// generates x86 assembly instructions based on program_node/AST.
void Generator::to_asm()
{
    this->collect_variables();
    this->collect_labels();

    if (!file.is_open())
    {
        cout << "file is closed bruh" << endl;
    }

    // INITIAL SETUP
    file << ".section .text" << endl;
    file << ".globl _start" << endl;
    file << "_start:" << endl;

    // setup stack frame
    file << "   mov %rsp, %rbp" << endl;
    // allocate memory for variables on the stack
    file << "   sub $" << this->variables.size() * 8 << ", %rsp" << endl;
    for (statement_node stmt : this->program.statements)
    {
        this->statement_to_asm(stmt);
    }
    file << ".exit:" << endl;
    file << "   movq $60, %rax" << endl;
    file << "   xor %rdi, %rdi" << endl;
    file << "   syscall" << endl;

    // predefined functions
    file << endl;
    file << "int_to_ascii:" << endl;
    file << "   movq $10, %rbx" << endl;
    file << "   xor %rdx, %rdx" << endl;
    file << "   div %rbx" << endl;
    file << "   add $48, %dl" << endl;
    file << "   movb %dl, (%rcx)" << endl;
    file << "   inc %rsi" << endl;
    file << "   dec %rcx" << endl;
    file << "   test %rax, %rax" << endl;
    file << "   jnz int_to_ascii" << endl;
    file << "   inc %rcx" << endl;
    file << "   mov %rsi, %rdx" << endl;
    file << "   mov %rcx, %rsi" << endl;
    file << "   ret" << endl;

    file << endl;
    file << "ascii_to_int:" << endl;
    file << "   movb (%r9), %al" << endl;
    file << "   movzbq %al, %rax" << endl;
    file << "   sub $48, %rax" << endl;
    file << "   mul %rbx" << endl;
    file << "   add %rax, %r8" << endl;
    file << "   mov %rbx, %rax" << endl;
    file << "   mov $10, %rbx" << endl;
    file << "   mul %rbx" << endl;
    file << "   mov %rax, %rbx" << endl;
    file << "   dec %rcx" << endl;
    file << "   dec %r9" << endl;
    file << "   test %rcx, %rcx" << endl;
    file << "   jnz ascii_to_int" << endl;
    file << "   inc %r9" << endl;
    file << "   mov %r8, %rax" << endl;
    file << "   ret" << endl;

    file << endl;
    file << "exp:" << endl;
    file << "   test %rbx, %rbx" << endl;
    file << "   jz .endexp" << endl;
    file << "   mul %rcx" << endl;
    file << "   dec %rbx" << endl;
    file << "   jmp exp" << endl;
    file << ".endexp:" << endl;
    file << "   ret" << endl;

    file.close();
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
    file << label.label << ":" << endl;
}

// jmp <label>
void Generator::goto_to_asm(goto_node goto_)
{
    if (!this->goto_valid(goto_))
        exit(0);
    file << "   jmp " << goto_.label << endl;
}

void Generator::assign_to_asm(assign_node assign)
{
    // we assume that the result file the expr is in %rax.
    expr_to_asm(assign.expr);

    int index = this->variable_index(assign.identifier);
    // store variable at %rsp + index*8
    file << "   movq %rax, -" << index * 8 + 8 << "(%rbp)" << endl;
}

void Generator::if_then_to_asm(if_then_node if_then)
{
    if (!this->comparison_valid(if_then.comparison) || !this->statement_valid(*if_then.statement))
    {
        // can't compile because file a sem error.
        exit(0);
    }
    // we assume that the result file the comparison is in %rax.
    comparison_to_asm(if_then.comparison);

    file << "   test %rax, %rax" << endl; // see if %rax is 0
    // if %rax is 0, the condition is true, so we should skip to after
    // the then statement.
    file << "   jz .ENDIF" << this->current_if_index << endl;
    int endif = this->current_if_index;
    this->current_if_index++;

    // this code will run if the jz instruction doesn't execute
    // (if the comparison evaluates to true)
    statement_to_asm(*if_then.statement);
    file << ".ENDIF" << endif << ":" << endl;
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
    if (comp.kind == COMP_LESS_THAN || comp.kind == COMP_EQUAL || comp.kind == COMP_GREATER_THAN)
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
    else if (expr.kind == BINARY_EXPR_PLUS || expr.kind == BINARY_EXPR_MINUS || expr.kind == BINARY_EXPR_MULT || expr.kind == BINARY_EXPR_DIV || expr.kind == BINARY_EXPR_MOD || expr.kind == BINARY_EXPR_EXP)
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
                // can't compile because file a sem error.
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
        file << "   mov %rax, %rcx" << endl;
        term_to_asm(binary_expr.rhs);
        // now we have to add them
        file << "   addq %rcx, %rax" << endl;
        // notice result is in %rax.
    }
    else if (expr.kind == BINARY_EXPR_MINUS)
    {
        term_binary_node binary_expr = get<term_binary_node>(expr.expr);
        term_to_asm(binary_expr.rhs);
        // assume rhs result in %rax. let's store in %rcx for now.
        file << "   mov %rax, %rcx" << endl;
        term_to_asm(binary_expr.lhs);
        // now we have to subtract them
        file << "   subq %rcx, %rax" << endl;
        // notice result is in %rax.
    }
    else if (expr.kind == BINARY_EXPR_MULT)
    {
        term_binary_node binary_expr = get<term_binary_node>(expr.expr);
        term_to_asm(binary_expr.lhs);
        // assume lhs result in %rax. let's store in %rcx for now.
        file << "   mov %rax, %rcx" << endl;
        term_to_asm(binary_expr.rhs);
        // now we have to multiply them
        file << "   mul %rcx" << endl;
    }
    else if (expr.kind == BINARY_EXPR_DIV)
    {
        term_binary_node binary_expr = get<term_binary_node>(expr.expr);
        term_to_asm(binary_expr.rhs);
        // assume rhs result (divisor) in %rax. let's store in %rcx for now.
        file << "   mov %rax, %rcx" << endl;
        term_to_asm(binary_expr.lhs);
        // now we have to divide them
        file << "   xor %rdx, %rdx" << endl; // clear for div instr
        file << "   div %rcx" << endl;
    }
    else if (expr.kind == BINARY_EXPR_MOD)
    {
        term_binary_node binary_expr = get<term_binary_node>(expr.expr);
        term_to_asm(binary_expr.rhs);
        // assume rhs result (divisor) in %rax. let's store in %rcx for now.
        file << "   mov %rax, %rcx" << endl;
        term_to_asm(binary_expr.lhs);
        // now we have to divide them
        file << "   xor %rdx, %rdx" << endl; // clear for div instr
        file << "   div %rcx" << endl;
        // the remainder is in %rdx, move to %rax.
        file << "   mov %rdx, %rax" << endl;
    }
    else if (expr.kind == BINARY_EXPR_EXP)
    {
        term_binary_node binary_expr = get<term_binary_node>(expr.expr);
        term_to_asm(binary_expr.lhs);
        // lhs is base, store in %rcx for now
        file << "   mov %rax, %rcx" << endl;
        term_to_asm(binary_expr.rhs);
        // rhs is exponent, store in %rbx
        file << "   mov %rax, %rbx" << endl;
        // perform exponentiation with result in %rax
        file << "   movq $1, %rax" << endl;
        file << "   call exp" << endl;
    }
}

void Generator::input_to_asm(term_node term)
{
    // we need to allocate space for the input
    file << "   sub $24, %rsp" << endl;
    file << "   mov %rbp, %rsi" << endl;
    file << "   addq $" << this->buffer_ptr << ", %rsi" << endl;
    file << "   movq $0, %rax" << endl;
    file << "   movq $0, %rdi" << endl;
    file << "   movq $24, %rdx" << endl;
    file << "   syscall" << endl;

    // now, the number file bytes read in should be in %rax.
    // + setup for call to ascii_to_int
    file << "   mov %rax, %rcx" << endl;
    file << "   subq $1, %rcx" << endl;
    file << "   mov %rsi, %r9" << endl;
    file << "   addq %rcx, %r9" << endl;
    file << "   subq $1, %r9" << endl;

    file << "   movq $1, %rbx" << endl;
    file << "   xor %r8, %r8" << endl;
    file << "   call ascii_to_int" << endl;
    // input value is now in %rax.
}

void Generator::term_to_asm(term_node term)
{
    // we need to store the result in %rax.
    if (term.kind == TERM_IDENTIFIER)
    {
        // get its value, load from its index!
        unsigned int index = this->variable_index(term.value);
        file << "   movq -" << index * 8 + 8 << "(%rbp)" << ", %rax" << endl;
    }
    else if (term.kind == TERM_INT_LITERAL)
    {
        file << "   movq $" << term.value << ", %rax" << endl;
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
        file << "   mov %rax, %rcx" << endl;
        term_node rhs = binary_expr.rhs;
        term_to_asm(rhs);
        // rhs is in %rax, so lets compare now.
        file << "   cmpq %rax, %rcx" << endl;
        // if res is less than 0, we know lhs < rhs,
        // so put a 1 in %rax or else %rax should be 0.
        file << "   setl %al" << endl;
        // put it in %rax with zero extend
        file << "   movzbq %al, %rax" << endl;
    }
    else if (comp.kind == COMP_EQUAL)
    {
        term_binary_node binary_expr = get<term_binary_node>(comp.binary_expr);
        term_node lhs = binary_expr.lhs;
        term_to_asm(lhs);
        file << "   mov %rax, %rcx" << endl;
        term_node rhs = binary_expr.rhs;
        term_to_asm(rhs);
        file << "   cmpq %rcx, %rax" << endl;
        file << "   sete %al" << endl;
        file << "   movzbq %al, %rax" << endl;
    }
    else if (comp.kind == COMP_GREATER_THAN)
    {
        term_binary_node binary_expr = get<term_binary_node>(comp.binary_expr);
        term_node lhs = binary_expr.lhs;
        term_to_asm(lhs);
        // lhs is in %rax -> store temp in %rcx.
        file << "   mov %rax, %rcx" << endl;
        term_node rhs = binary_expr.rhs;
        term_to_asm(rhs);
        // rhs is in %rax, so lets compare now.
        file << "   cmpq %rax, %rcx" << endl;
        // if res is less than 0, we know lhs < rhs,
        // so put a 1 in %rax or else %rax should be 0.
        file << "   setg %al" << endl;
        // put it in %rax with zero extend
        file << "   movzbq %al, %rax" << endl;
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
    file << "   movq $1, %rsi" << endl; // we have to print at least a \n
    file << "   sub $24, %rsp" << endl;
    file << "   mov %rbp, %rcx" << endl;
    file << "   addq $" << this->buffer_ptr + 23 << ", %rcx" << endl;
    file << "   movq $0x0A, (%rcx)" << endl; // newline character at end file str
    file << "   dec %rcx" << endl;
    file << "   call int_to_ascii" << endl;

    this->buffer_ptr += 24;
    // sys_write: %rax = 1; %rdi = unsigned int fd; %rsi = const char *buf; %rdx = size_t count;

    // now we assume that our buffer ptr is in %rsi and the length is in %rdx.
    // time for a syscall

    file << "   movq $1, %rax" << endl;
    file << "   movq $1, %rdi" << endl;
    file << "   syscall" << endl;
}