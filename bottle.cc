#include "bottle.h"
#include "truck.h"
#include "vending.h"
#include "MPRNG.h"
#include <stdlib.h>

extern MPRNG mprng;

/**
 * The bottling plant periodically produces random new quantities of each flavour of soda,
 * [0, MaxShippedPerFlavour] per flavour. The bottling plant is passed the number of vending machines,
 * NumVendingMachines, the maximum number of bottles of each flavour generated during a production run
 * and subsequently shipped, MaxShippedPerFlavour, the maximum number of bottles of each flavour in a
 * vending machine MaxStockPerFlavour, and the length of time between shipment pickups by the truck,
 * TimeBetweenShipments.
 *
 * It begins by creating a truck, performing a production run, and waiting for the truck to
 * pickup the production run. The truck then distributes these bottles to initialize the registered
 * vending machines.
 *
 * To simulate a production run of soda, the bottling plant yields for TimeBetweenShipments times
 * (not random). The truck calls getShipment to obtain a shipment from the plant (i.e., the production
 * run), and the shipment is copied into the cargo array passed by the truck.
 *
 * getShipment throws the exception Shutdown if the bottling plant is closing down and cargo is not changed.
 * A flag variable is necessary to know when to raise Shutdown on the correct task stack.
 *
 * The bottling plant does not start another production run until the truck has picked up the current run.
 */
BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
             unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
             unsigned int timeBetweenShipments ) :
        printer(prt),
        nameServer(nameServer),
        numVendingMachines(numVendingMachines),
        maxShippedPerFlavour(maxShippedPerFlavour),
        maxStockPerFlavour(maxStockPerFlavour),
        timeBetweenShipments(timeBetweenShipments),
        inventory(VendingMachine::Flavours::FLAVOUR_COUNT, 0) {
    printer.print(Printer::Kind::BottlingPlant, 'S');
}

void BottlingPlant::main() {
    {
        Truck truck(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour); // make the truck
        for (;;) {
            productionRun();                                                             // accept a pickup
            _Accept(~BottlingPlant) {                                                    // be ready to die
                break;
            } or _Accept(getShipment);
        }
        printer.print(Printer::Kind::BottlingPlant, 'F');
        _Accept(getShipment) {}                                                          // one last shipment to die gracefully
    }
}

void BottlingPlant::productionRun() {                                                    // produce bottles for truck
    yield(timeBetweenShipments);
    int b = 0;
    for (int i = 0; i < VendingMachine::Flavours::FLAVOUR_COUNT; i += 1) {
        b += (inventory[i] = mprng(0, maxStockPerFlavour));
    }
    printer.print(Printer::Kind::BottlingPlant, 'G', b);
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {                                // overwrite truck cargo
    printer.print(Printer::Kind::BottlingPlant, 'P');
    memcpy(cargo, inventory.data(), inventory.size()*sizeof(int));
}
