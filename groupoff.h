#ifndef GROUPOFF_H
#define GROUPOFF_H

#include "MPRNG.h"
#include "printer.h"
#include "watcard.h"

_Task Groupoff {
    void main();

    uCondition bench;					// condition lock for waiting students
    std::vector<WATCard::FWATCard> tempCards;

    Printer &printer;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;

  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard();
};

#endif
