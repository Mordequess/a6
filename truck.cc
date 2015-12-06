#include "truck.h"

#include "vending.h"
#include "MPRNG.h"

extern MPRNG mprng;

/**
 * The truck moves soda from the bottling plant to the vending machines. The truck is passed the number of
 * vending machines, numVendingMachines, and the maximum number of bottles of each flavour in a vending
 * machine maxStockPerFlavour.
 *
 * The truck begins by obtaining the location of each vending machine from the name server.
 *
 * Before each shipment from the bottling plant, the truck yields a random number of times [1, 10]
 * to get a coffee from Tom Hortons.
 *
 * The truck then calls BottlingPlant::getShipment to obtain a new shipment of soda; any soda still on the
 * truck is thrown away as it is past its due date.
 *
 * If the bottling plant is closing down, the truck terminates.
 *
 * To ensure fairness, the vending machines are restocked in cyclic order starting at the vending machine
 * after the last machine the truck restocked, until there is no more soda on the truck or the truck has
 * made a complete cycle of all the vending machines; so there is no guarantee each vending machine
 * is completely restocked or the entire complement of vending machines is restocked or all the soda on
 * the truck is used.
 *
 * The truck can only restock up to MaxStockPerFlavour for each flavour in each vending machine (see
 * VendingMachine task).
 */

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour) : 
        printer(prt),
        nameServer(nameServer),
        plant(plant),
        numVendingMachines(numVendingMachines),
        maxStockPerFlavour(maxStockPerFlavour) {
    printer.print(Printer::Kind::Truck, 'S');
}

unsigned int Truck::cargoSum(unsigned int *cargo) {          // helper method to calculate cargo sizes
    unsigned int sumOfCargo = 0;
    for (int i = 0; i < VendingMachine::Flavours::FLAVOUR_COUNT; i += 1) {
        sumOfCargo += cargo[i];
    }
    return sumOfCargo;
}

void Truck::main() {
    VendingMachine **machines = nameServer.getMachineList();            // setup: number of machines, size of truck
    unsigned int cargo[VendingMachine::Flavours::FLAVOUR_COUNT];
    unsigned int lastStocked = -1;
    unsigned int firstStocked;

    for (;;) {
        _Accept(~Truck) {
            break;
        } _Else {
            uThisTask().yield(mprng(1, 10));                            // wait random number of times

            plant.getShipment(cargo);                                   // getShipment will ignore and overwrite current cargo
            printer.print(Printer::Kind::Truck, 'P', cargoSum(cargo));
            firstStocked = lastStocked;

            for (;;) {                                                  // delivery loop
                _Accept(~Truck) {
                    goto pickupLoop;
                } _Else {
                    unsigned int next = (lastStocked + 1) % numVendingMachines; //find next machine

                    // We want to break if:
                    //  - there is no stock
                    //  - the truck has made a complete cycle
                    
                    if (cargoSum(cargo) == 0) {
                        break;
                    }
                    
                    if (next == firstStocked) {
                        break;
                    }

                    printer.print(Printer::Kind::Truck, 'd', next, cargoSum(cargo));    // begin delivery to this machine

                    VendingMachine *vm = machines[next];
                    unsigned int *inventory = vm->inventory();
                    int missing = 0;
                    
                    for (int i = 0; i < VendingMachine::Flavours::FLAVOUR_COUNT; i += 1) { // stock each flavor
                        unsigned int needed = maxStockPerFlavour - inventory[i];
                        if (needed > cargo[i]) {
                            missing += needed - cargo[i];
                            inventory[i] += cargo[i];
                            cargo[i] = 0;
                        } else {
                            inventory[i] += needed;
                            cargo[i] -= needed;
                        }
                    }

                    if (missing) {                                                  // check if we were unsuccessful
                        printer.print(Printer::Kind::Truck, 'U', next, missing);
                    }
                    vm->restocked();

                    printer.print(Printer::Kind::Truck, 'D', next, cargoSum(cargo)); // finished delivery
                    lastStocked = next;
                }
            }  // end delivery loop
        }
    } pickupLoop:

    printer.print(Printer::Kind::Truck, 'F');
    
}
