#include "bank.h"

/*

The Bank is a monitor, which behaves like a server, that manages student-account information for all students.

Each student’s account initially starts with a balance of $0. 

The parent calls deposit to endow gifts to a specific student. 

A courier calls withdraw to transfer money on behalf of the WATCard office for a specific student. The
courier waits until enough money has been deposited, which may require multiple deposits.

*/

Bank::Bank( unsigned int numStudents ) : waiters(0) {
    for (unsigned int i = 0; i < numStudents; i += 1) {
        studentAccounts.push_back(0);
    }
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    studentAccounts[id] += amount;

    for (unsigned int i = 0; i < waiters; i += 1) {
        insufficientFunds.signal();
    }
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    waiters += 1;
    while (studentAccounts[id] < amount) {
        insufficientFunds.wait();
    }
    waiters -= 1;

    studentAccounts[id] -= amount;
}
