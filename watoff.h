#ifndef WATOFF_H
#define WATOFF_H

#include "printer.h"
#include "bank.h"
#include "watcard.h"

_Task WATCardOffice {
    struct Job {                            // marshalled arguments and return future
        Args args;                          // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;           // return future
        Job( Args args ) : args( args ) {}
    };

    _Task Courier {                         // communicates with bank
        void main() {
            Job j = WATCardOffice::requestWork(); //should block
            j.args();                             //do the thing
        }

    public:
        Courier();
    };

    void main();
    std::vector<Courier*> couriers;

  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
