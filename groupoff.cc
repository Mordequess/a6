#include "groupoff.h"

/*

The Groupoff task begins by accepting a call from each student to obtain a future gift-card. 

Then groupoff periodically puts a real WATCard with value SodaCost into a random future gift-card. 

A future gift-card is assigned only once. Before each future gift-card is assigned a real WATCard, groupoff yields for groupoffDelay times (not random). 

The groupoff loops until all the real WATCard’s are assigned to a future gift-card or a call to its destructor occurs. Since it must not block on the destructor call, it is necessary to use a terminating Else
on the accept statement.

*/

extern MPRNG mprng;

void Groupoff::main(){
    printer.print(Printer::Kind::Groupoff, 'S');
    
    for (unsigned int i = 0; i < numStudents; i += 1) {
        _Accept(giftCard) {
            cards.push_back(WATCard::FWATCard());
        }
    }

    for (;;) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            if (cards.size() == 0) {
                break;
            }

            uThisTask().yield(groupoffDelay);

            int index = mprng(cards.size());
            auto it = cards.begin();
            for (int j = 0; j < index; ++j) {
                ++it;
            }
            auto card = *it;
            cards.erase(it);
            WATCard *realCard = new WATCard;
            realCard->deposit(sodaCost);
            card.delivery(realCard);

            printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
        }
    }

    printer.print(Printer::Kind::Groupoff, 'F');
}

Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) :
    printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {

}

WATCard::FWATCard Groupoff::giftCard() {
    return cards.back();
}
