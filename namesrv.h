#ifndef NAMESRV_H
#define NAMESRV_H

#include "printer.h"

_Task VendingMachine;

#include <vector>

_Task NameServer {
    void main();
    Printer &printer;
    unsigned int numVendingMachines;
    unsigned int numStudents;

    std::vector<VendingMachine *> machineList;              // list of machines
    std::vector<int> studentStatus;                         // tracker for 
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
