#include "parent.h"

/*

The Parent task periodically gives a random amount of money [$1, $3] to a random student. 

Before each gift is transferred, the parent yields for parentalDelay times (not random). 

The parent must check for a call to its destructor to know when to terminate. Since it must not block on this call, 
it is necessary to use a terminating _Else on the accept statement. (Hence, the parent is busy waiting for the call to its destructor.)

*/

#include <iostream>

extern MPRNG mprng;

void Parent::main() {
    printer.print(Printer::Kind::Parent, 'S');
    
    for (;;) {
        _Accept(~Parent) {                              // be prepared to die
            printer.print(Printer::Kind::Parent, 'F');
            return;
        }

        _Else {
            uThisTask().yield(mprng(parentalDelay - 1));                    // yield non random number of times
            int student = mprng(numStudents - 1);                           // deposit to random student
            int amount = mprng(1,3);
            bank.deposit(student, amount);
            printer.print(Printer::Kind::Parent, 'D', student, amount);
        }
    }
}

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) : 
    printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {

}
