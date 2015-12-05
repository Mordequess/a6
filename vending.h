#ifndef VENDING_H
#define VENDING_H

#include "namesrv.h"
#include "watcard.h"
#include <vector>

_Task VendingMachine {
    void main();

    Printer &printer;
    NameServer &nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int maxStockPerFlavour;
    std::vector<unsigned int> _inventory;

  public:
    enum Flavours { 
        PETER_BURHS_BLUES_BLACK_CHERRY,
        PETER_BURHS_CLASSICAL_CREAM_SODA,
        PETER_BURHS_ROCK_ROOT_BEER,
        PETER_BURHS_JAZZ_LIME,

        FLAVOUR_COUNT
    };
    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif//VENDING_H
