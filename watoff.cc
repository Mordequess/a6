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

WATCardOffice::Action::Action(WATCard *card, unsigned int sid, unsigned int amount) :
    card(card), sid(sid), amount(amount) {
}

WATCardOffice::CreateAction::CreateAction(unsigned int sid, unsigned int amount) :
    Action(NULL, sid, amount) {
}

void WATCardOffice::CreateAction::doAction(Bank &bank, Printer& printer) {
    card = new WATCard();
    bank.withdraw(sid, amount);
    card->deposit(amount);
    printer.print(Printer::Kind::WATCardOffice, 'C', sid, amount);
}

WATCardOffice::TransferAction::TransferAction(WATCard *card, unsigned int sid, unsigned int amount) :
    Action(card, sid, amount) {
}

void WATCardOffice::TransferAction::doAction(Bank &bank, Printer& printer) {
    bank.withdraw(sid, amount);
    card->deposit(amount);
    printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
}

WATCardOffice::Courier::Courier(WATCardOffice *office, Bank &bank, Printer &printer, int lid) :
        office(office), bank(bank), printer(printer), lid(lid) {
    printer.print(Printer::Kind::Courier, lid, 'S');
}

void WATCardOffice::Courier::main() {
    for (;;) {
        Job *job = office->requestWork(); // should block
        Action *action = job->args.action;
        action->doAction(bank, printer);
        job->result.delivery(action->card);
        delete job;
    }
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
        couriers.push_back(new Courier(this, bank, printer, i));
    }

    for (;;) {

    }
    printer.print(Printer::Kind::WATCardOffice, 'F');

}

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
        printer(prt), bank(bank), numCouriers(numCouriers) {
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    Args args = {
        new CreateAction(
            sid,
            amount
        )
    };

    Job* job = new Job(args);
    jobQ.push_back(job);
    jobsAvailable.signal();
    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    Args args = {
        new TransferAction(
            card,
            sid,
            amount
        )
    };

    Job* job = new Job(args);
    jobQ.push_back(job);
    jobsAvailable.signal();
    return job->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    if (!jobQ.size()) {
        jobsAvailable.wait();
    }
    mutex.acquire();
    Job *j = jobQ.front();
    jobQ.pop_front();
    printer.print(Printer::Kind::WATCardOffice, 'W');
    mutex.release();
    return j;
}

