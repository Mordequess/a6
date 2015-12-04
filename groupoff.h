#ifndef GROUPOFF_H
#define GROUPOFF_H

#include <list>

#include "MPRNG.h"
#include "printer.h"
#include "watcard.h"

_Task Groupoff {
    void main();

    uCondition bench;                    // condition lock for waiting students
    std::list<WATCard::FWATCard> cards;

    Printer &printer;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;

  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard();
};

#endif
