#include "watcard.h"

/*

The WATCard manages the money associated with a card. When a WATCard is created it has a $0 balance.

The courier calls deposit after a funds transfer. 

A vending machine calls withdraw when a soda is purchased.

A student and a vending machine call getBalance to determine the balance. 

FWATCard is a future pointer to a
student’s WATCard for synchronizing access to the WATCard between the student and the courier.

*/

WATCard::WATCard() : balance(0) {

}

void WATCard::deposit( unsigned int amount ) {
    mutex.acquire();
    
    balance += amount;
    synch.broadcast();     //should this be signal to avoid starvation on "large" orders?

    mutex.release();
}

void WATCard::withdraw( unsigned int amount ) {
    mutex.acquire();

    while (balance < amount) {
        synch.wait(mutex);
    }

    balance += amount;
    mutex.release();
}

unsigned int WATCard::getBalance() {
    return balance;
}

