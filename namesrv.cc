#include "namesrv.h"
#include "vending.h"

/**
 * The NameServer is an administrator task used to manage the vending-machine names. The name server is
 * passed the number of vending machines, NumVendingMachines, and the number of students, NumStudents.
 * 
 * It begins by logically distributing the students evenly across the vending machines in a round-robin
 * fashion. That is, student id 0 is assigned to the first registered vending-machine, student id 1 is
 * assigned to the second registered vending-machine, etc., until there are no more registered
 * vending-machines, and then start again with the first registered vending-machine.
 * 
 * Vending machines call VMregister to register themselves so students can subsequently locate them.
 * 
 * A student calls getMachine to find a vending machine, and the name server must cycle through the vending
 * machines separately for each student starting from the initial position via modulo incrementing to ensure
 * a student has a chance to visit every machine.
 * 
 * The truck calls getMachineList to obtain an array of pointers to vending machines so it can visit each
 * machine to deliver new soda.
*/

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) :
        printer(prt),
        numVendingMachines(numVendingMachines),
        numStudents(numStudents),
        machineList(numVendingMachines, NULL),
        studentStatus(numStudents, 0)
    {
    printer.print(Printer::Kind::NameServer, 'S');
}

void NameServer::main() {
    for (;;) {
        _Accept(~NameServer) {
            break;
        } or _Accept(VMregister) {}
        or _Accept(getMachine) {}
        or _Accept(getMachineList) {}
    }
    printer.print(Printer::Kind::NameServer, 'F');
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    unsigned int id = vendingmachine->getId();
    printer.print(Printer::Kind::NameServer, 'R', id);       // register a vending machine
    machineList[id] = vendingmachine;
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
    int assignedId = (id + studentStatus[id]) % numVendingMachines;                 // each student is tracked, given the next machine in the circle
    VendingMachine *assigned = machineList[assignedId];
    studentStatus[id] = (studentStatus[id] + 1) % numVendingMachines;
    printer.print(Printer::Kind::NameServer, 'N', id, assignedId);
    return assigned;
}

VendingMachine **NameServer::getMachineList() {
    return machineList.data();
}
