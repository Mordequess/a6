#include "vending.h"

#include <stdio.h>

/**
 * A vending machine’s function is to sell soda to students at some cost.
 *
 * Each vending machine is passed an id in the range [0, NumVendingMachines) for identification,
 * MSRP price for a bottle of soda, and the maximum number of bottles of each flavour in a vending
 * machine. A new vending machine is empty (no stock) and begins by registering with the name server.
 *
 * A student calls buy to obtain one of their favourite sodas. If the student has insufficient funds
 * to purchase the soda or the specified soda is unavailable, exceptions Funds or Stock are raised,
 * respectively; otherwise, the student’s WATCard is debited by the cost of a soda. (A flag variable
 * is necessary to know when to raise Funds or Stock on the correct task stack.)
 *
 * Periodically, the truck comes by to restock the vending machines with new soda from the bottling
 * plant. Restocking is performed in two steps. The truck calls inventory to return a pointer to an
 * array containing the amount of each kind of soda currently in the vending machine. The truck uses
 * this information to transfer into each machine as much of its stock of new soda as fits; for each
 * kind of soda, no more than MaxStockPerFlavour per flavour can be added to a machine. If the truck
 * cannot top-up a particular flavour, it transfers as many bottles as it has (which could be 0).
 *
 * After transferring new soda into the machine by directly modifying the array passed from inventory,
 * the truck calls restocked to indicate the operation is complete.
 *
 * The vending machine cannot accept buy calls during restocking. The cost member returns the cost of
 * purchasing a soda for this machine.
 *
 * The getId member returns the identification number of the vending machine.
 *
 * You define the public type Flavours to represent the different flavours of soda.
*/

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                unsigned int maxStockPerFlavour ) :
        printer(prt),
        nameServer(nameServer),
        id(id),
        sodaCost(sodaCost),
        maxStockPerFlavour(maxStockPerFlavour),
        _inventory(Flavours::FLAVOUR_COUNT) {
    nameServer.VMregister(this);
    printer.print(Printer::Kind::Vending, id, 'S', sodaCost);
}

void VendingMachine::main() {
    for (;;) {
        try {
            _Enable {
                _Accept(~VendingMachine) {
                    break;
                }
                or _Accept(inventory);
                or _Accept(restocked);
                or _Accept(buy);
            }
        } catch(...) {
        }
    }

    printer.print(Printer::Kind::Vending, id, 'F');
}

void VendingMachine::buy( Flavours flavour, WATCard &card ) {
    if (card.getBalance() < sodaCost) {
        _Throw Funds();
    }

    if (!_inventory[flavour]) {
        _Throw Stock();
    }

    card.withdraw(sodaCost);
    --_inventory[flavour];

    printer.print(Printer::Kind::Vending, id, 'B', flavour, _inventory[flavour]);
}

unsigned int *VendingMachine::inventory() {
    printer.print(Printer::Kind::Vending, id, 'r');
    return _inventory.data();
}

void VendingMachine::restocked() {
    printer.print(Printer::Kind::Vending, id, 'R');
}

_Nomutex unsigned int VendingMachine::cost() {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
    return id;
}

