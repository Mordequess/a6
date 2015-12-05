#ifndef BOTTLE_H
#define BOTTLE_H

#include "printer.h"
#include "namesrv.h"

#include <vector>

_Task BottlingPlant {
    void main();
    Printer &printer;
    NameServer &nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;
    std::vector<int> inventory;

    void productionRun();

  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};

#endif//BOTTLE_H
