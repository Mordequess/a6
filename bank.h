#ifndef BANK_H
#define BANK_H

#include <vector>

_Monitor Bank {

    uCondition insufficientFunds;
    unsigned int waiters;
    std::vector<unsigned int> studentAccounts;

public:
    Bank( unsigned int numStudents );
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif
