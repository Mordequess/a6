#ifndef WATOFF_H
#define WATOFF_H

#include <vector>
#include <deque>

#include "printer.h"
#include "bank.h"
#include "watcard.h"

_Task WATCardOffice {
    class Action {                          // class for job instructions
    public:
        WATCard *card;
        unsigned int sid;
        unsigned int amount;

        Action(WATCard *card, unsigned int sid, unsigned int amount) :
            card(card), sid(sid), amount(amount) {}
        virtual void doAction(Bank &bank, Printer& printer) = 0;
        virtual ~Action() {}
    };

    class CreateAction : public Action {        // inheriting class: create
    public:
        CreateAction(unsigned int sid, unsigned int amount):
            Action(NULL, sid, amount) {}
        void doAction(Bank &bank, Printer& printer);
    };

    class TransferAction : public Action {      // inheriting class: transfer
    public:
        TransferAction(WATCard *card, unsigned int sid, unsigned int amount):
            Action(card, sid, amount){}
        void doAction(Bank &bank, Printer& printer);
    };

    struct Args {
        Action *action;
    };

    struct Job {                            // marshalled arguments and return future
        Args args;                          // call arguments
        WATCard::FWATCard result;           // return future
        Job( Args args ) : args( args ) {}
        ~Job() {
            delete args.action;
        }
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
