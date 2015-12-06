#ifndef BANK_H
#define BANK_H

#include <vector>

_Monitor Bank {

    uCondition insufficientFunds; 								// waiting on deposits
    unsigned int waiters;										// number of people waiting
    std::vector<unsigned int> studentAccounts; 					// the student accounts

public:
    Bank( unsigned int numStudents );
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif
