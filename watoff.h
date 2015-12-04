#ifndef WATOFF_H
#define WATOFF_H

#include <vector>
#include <deque>

#include "printer.h"
#include "bank.h"
#include "watcard.h"

_Task WATCardOffice {
    class Action {
    public:
        WATCard *card;
    protected:
        unsigned int sid;
        unsigned int amount;
    public:
        Action(WATCard *card, unsigned int sid, unsigned int amount);
        virtual void doAction(Bank &bank, Printer& printer) = 0;
        virtual ~Action() {}
    };
    class CreateAction : public Action {
    public:
        CreateAction(unsigned int sid, unsigned int amount);
        void doAction(Bank &bank, Printer& printer);
    };

    class TransferAction : public Action {
    public:
        TransferAction(WATCard *card, unsigned int sid, unsigned int amount);
        void doAction(Bank &bank, Printer& printer);
    };

    struct Args {
        Action *action;
    };

    struct Job {                            // marshalled arguments and return future
        Args args;                          // call arguments (YOU DEFINE "Args")
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
        int lid;

        void main();

    public:
        Courier(WATCardOffice *office, Bank &bank, Printer& printer, int lid);
    };

    void main();
    std::vector<Courier *> couriers;

    uCondition jobsAvailable;
    std::deque<Job *> jobQ;
    uOwnerLock mutex;

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
