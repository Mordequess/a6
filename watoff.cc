#include "watoff.h"

#include <stdio.h>

/*

The WATCardOffice is an administrator task used by a student to transfer funds from their bank account to their
WATCard to buy a soda. 

Initially, the WATCard office creates a fixed-sized courier pool with numCouriers
courier tasks to communicate with the bank. (Additional couriers may not be created after the WATCardOffice
begins.) 

A student performs an asynchronous call to create to create a “real” WATCard with an initial balance.
A future WATCard is returned and sufficient funds are subsequently obtained from the bank (see Parent task) via
a courier to satisfy the create request. 

A student performs an asynchronous call to transfer when its WATCard
indicates there is insufficient funds to buy a soda. A future WATCard is returned and sufficient funds are
subsequently obtained from the bank (see Parent task) via a courier to satisfy the transfer request. 

The WATCard office is empowered to transfer funds from a student’s bank-account to its WATCard by sending a request through
a courier to the bank. 

Each courier task calls requestWork, blocks until a Job request is ready, and then receives
the next Job request as the result of the call. As soon as the request is satisfied (i.e., money is obtained from the
bank), the courier updates the student’s WATCard. 

There is a 1 in 6 chance a courier loses a student’s WATCard
after the update. When the card is lost, the exception WATCardOffice::Lost is inserted into the future, rather than
making the future available, and the current WATCard is deleted

*/

WATCardOffice::Courier::Courier(WATCardOffice *office) : office(office) {
}

void WATCardOffice::Courier::main() {
    Job *job = office->requestWork(); //should block
    fprintf(stderr, "Courier::main not implemented %p\n", job);
}


/*
    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        Job( Args args ) : args( args ) {}
    };

    _Event Lost {};                        // lost WATCard
    _Task Courier { ... };                 // communicates with bank
*/

void WATCardOffice::main() {
    printer.print(Printer::Kind::WATCardOffice, 'S');

    for (int i = 0; i < numCouriers; i += 1) {
        couriers.push_back(new Courier(this));
    }

    for (;;) {

    }
    printer.print(Printer::Kind::WATCardOffice, 'F');

}

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
    printer(prt), bank(bank), numCouriers(numCouriers) {
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    printer.print(Printer::Kind::WATCardOffice, 'C', sid, amount);
    WATCard::FWATCard future;
    WATCard *card = new WATCard();
    card->deposit(amount);
    future.delivery(card);
    return future;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    bank.withdraw(sid, amount);
    card->deposit(amount);
    printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
    WATCard::FWATCard future;
    fprintf(stderr, "WATCardOffice::transfer TODO\n");
    return future;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    //block courier until job is ready
    fprintf(stderr, "WATCardOffice::requestWork TODO\n");
    Args args;
    Job *j = new Job(args);
    printer.print(Printer::Kind::WATCardOffice, 'W');

    return j;
}

