#include "student.h"
#include "vending.h"
#include "MPRNG.h"

extern MPRNG mprng;

/**
 * A Student’s function is to periodically buy some of their favourite soda from a vending machine
 * (a bottle costs whatever the vending machine is charging). Each student is passed an id in the
 * range [0, NumStudents) for identification.
 *
 * A student begins by selecting a random number of bottles to purchase [1, MaxPurchases], a
 * random favourite flavour [0, VendingMachine::Flavours::FLAVOUR_COUNT), creates a WATCard via the
 * WATCardOffice with a $5 balance, creates a gift card via the Groupoff with a value of SodaCost, and
 * obtains the location of a vending machine from the name server.
 *
 * A student terminates after purchasing all the soda initially selected.
 *
 * Before each attempt to buy a soda, a student yields a random number of times in the range [1, 10].
 * A student then blocks waiting for money to be transferred from the WATCardOffice to their WATCard
 * or from Groupoff to their gift card.
 *
 * Once money is available on either card, the student attempts to buy a bottle of soda from their current
 * vending machine. Note, a giftcard future can only be used once, so it must be reset after use to prevent
 * any further usage.
 *
 * If a courier has lost a student’s WATCard during a transfer (see WATCardOffice::Courier), the exception
 * WATCardOffice::Lost is raised when the future value is accessed. In this case, the student must create
 * a new WATCard via the WATCardOffice with a $5 balance, and re-attempt to buy a soda but without
 * yielding as no call to buy has occurred. Note, a courier can lose a student’s WATCard during the
 * transfer for the new create so this issue can occur repeatedly.
 *
 * If the vending machine delivers a bottle of soda, the student drinks it and attempts another purchase.
 * If the vending machine indicates insufficient funds, a student transfers the current vending-machine
 * soda-cost plus $5 to their WATCard via the WATCard office. If the vending machine is out of the
 * student’s favourite flavour, the student must obtain a new vending machine from the name server.
 *
 * (Hence, a student may busy wait among vending machines until its specific soda appears from the
 * bottling plant.)
 *
 * <big-red-text>
 *  Watch out for the case of a student who only buys one soda using the gift card.
 * </big-red-text>
*/

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
         unsigned int id, unsigned int maxPurchases ) :
        printer(prt),
        nameServer(nameServer),
        cardOffice(cardOffice),
        groupoff(groupoff),
        id(id),
        maxPurchases(maxPurchases) {
}

void Student::main() {
    int bottlesToPurchase = mprng(1, maxPurchases);
    auto favouriteFlavour = (VendingMachine::Flavours) mprng(0, VendingMachine::Flavours::FLAVOUR_COUNT - 1);
    auto card = cardOffice.create(id, 5);
    auto giftCard = groupoff.giftCard();
    bool giftCardUsed = false;
    printer.print(Printer::Kind::Student, id, 'S', favouriteFlavour, bottlesToPurchase);

    auto vendingMachine = nameServer.getMachine(id);
    int sodaCost = vendingMachine->cost();
    printer.print(Printer::Kind::Student, id, 'V', vendingMachine->getId());

    auto tryBuy = [this, &card, &bottlesToPurchase, &vendingMachine, favouriteFlavour, sodaCost] (WATCard &pcard) {
        try {
            vendingMachine->buy(favouriteFlavour, pcard);
        } catch(VendingMachine::Funds) {
            // Assumption: gift cards have enough money.
            card = cardOffice.transfer(id, 5 + sodaCost, &pcard);
        } catch(VendingMachine::Stock) {
            vendingMachine = nameServer.getMachine(id);
            printer.print(Printer::Kind::Student, id, 'V', vendingMachine->getId());
            return;
        }
        --bottlesToPurchase;
    };

    while(bottlesToPurchase) {
        yield(mprng(1, 10));
        if (giftCardUsed) {
            _Select(card) {
                tryBuy(*card);
                printer.print(Printer::Kind::Student, id, 'B', (*card).getBalance());
            }
        } else {
            _Select(card) {
                tryBuy(*card);
                printer.print(Printer::Kind::Student, id, 'B', (*card).getBalance());
            } or _Select(giftCard) {
                tryBuy(*giftCard);
                printer.print(Printer::Kind::Student, id, 'G', (*giftCard).getBalance());
                giftCardUsed = true;
            }
        }
    }

    printer.print(Printer::Kind::Student, id, 'F');
    //delete card;
    //delete giftCard;
}
