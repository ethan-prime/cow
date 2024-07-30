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