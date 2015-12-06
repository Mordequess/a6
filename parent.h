#ifndef PARENT_H
#define PARENT_H

#include "MPRNG.h"
#include "printer.h"
#include "bank.h"

_Task Parent {
    void main();

    Printer &printer;
    Bank &bank;
    unsigned int numStudents;
    unsigned int parentalDelay;

  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};


#endif
