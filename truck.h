#ifndef TRUCK_H
#define TRUCK_H

#include "printer.h"
#include "namesrv.h"
#include "bottle.h"

_Task Truck {
    void main();

    Printer &printer;
    NameServer &nameServer;
    BottlingPlant &plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;

    unsigned int cargoSum(unsigned int * cargo); 			// helper method for quantity of cargo
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif//TRUCK_H
