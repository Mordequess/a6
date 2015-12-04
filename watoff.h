#ifndef WATOFF_H
#define WATOFF_H

#include "printer.h"
#include "bank.h"
#include "watcard.h"

_Task WATCardOffice {
    struct Args {
    };

    struct Job {                            // marshalled arguments and return future
        Args args;                          // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;           // return future
        Job( Args args ) : args( args ) {}
    };

    _Task Courier {                         // communicates with bank
        WATCardOffice *office;
        void main();

    public:
        Courier(WATCardOffice *office);
    };

    void main();
    std::vector<Courier*> couriers;
    Printer &printer;
    Bank &bank;
    int numCouriers;

  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
