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
    
    for (unsigned int i = 0; i < numStudents; i += 1) {     // prepare a giftcard for each student
        cards.push_back(WATCard::FWATCard());
    }
    for (unsigned int i = 0; i < numStudents; i += 1) {     // students will request a card each, we give them a future
        _Accept(giftCard) {}
    }

    for (;;) {
        _Accept(~Groupoff) {                                // be prepared to die
            break;
        } _Else {
            if (cards.size() == 0) {
                break;
            }

            uThisTask().yield(groupoffDelay);               // yield non random number of times

            int index = mprng(cards.size() - 1);            // select random student
            auto it = cards.begin();
            for (int j = 0; j < index; j += 1) {
                ++it;
            }
            auto card = *it;
            cards.erase(it);
            WATCard *realCard = new WATCard;                // replace their future gift card with this gift card
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
    static int idx = 0;
    
    auto it = cards.begin();
    for (int i = 0; i < idx; i += 1) {
        ++it;
    }
    idx += 1;
    return *it;
}
