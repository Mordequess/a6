#ifndef GROUPOFF_H
#define GROUPOFF_H

#include <list>

#include "MPRNG.h"
#include "printer.h"
#include "watcard.h"

_Task Groupoff {
    void main();

    std::list<WATCard::FWATCard> cards;         // list of students to give cards to

    Printer &printer;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;

  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard();
};

#endif
