#ifndef WATOFF_H
#define WATOFF_H

#include <vector>
#include <deque>

#include "printer.h"
#include "bank.h"
#include "watcard.h"

_Task WATCardOffice {

    struct Args {
        WATCard *card;
        unsigned int sid;
        unsigned int amount;
        Bank &bank;
        Printer &printer;
        void doArg();
    };

    struct Job {                            // marshalled arguments and return future
        Args args;                          // call arguments
        WATCard::FWATCard result;           // return future
        Job( Args args ) : args( args ) {}
    };

    _Task Courier {                         // communicates with bank
        WATCardOffice *office;
        Bank &bank;
        Printer &printer;
        int lid;                            // courier id (for printing)

        void main();

    public:
        Courier(WATCardOffice *office, Bank &bank, Printer& printer, int lid);
    };

    void main();
    std::vector<Courier *> couriers;        // list of couriers

    uCondition jobsAvailable;               // bench for waiting couriers
    std::deque<Job *> jobQ;                 // deck of available jobs

    Printer &printer;
    Bank &bank;
    int numCouriers;
    bool dying;

  public:
    _Event Lost {};                         // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
    static std::vector<Courier *> toDelete;
};

#endif
