#include <iostream>
#include <vector>

#include "MPRNG.h"
#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watoff.h"
#include "groupoff.h"

MPRNG mprng;                                                                        // globally visible random number generator

void uMain::main(){
    unsigned int seed = getpid();
    char fileName[] = "soda.config";
    char* configFile = (char *) &fileName;

    switch (argc) {                                                                    // parse input
        case 3:
            seed = std::atoi(argv[2]);
            if (seed <= 0) {
                  std::cerr << "Error: seed must be greater than 0." << std::endl;
                exit(-1);
              }

        case 2:
            configFile = argv[1];
            //if (configFile.size() <= 0) {
            //    std::cerr << "Error: must name a file." << std::endl;
            //    exit(-1);
            //}

        case 1:
            break;

        default:
            std::cerr << "needs to be in format: soda [ config-gile [ seed ] ]" << std::endl;
            exit(-1);

    } // end of input switch

    mprng.seed(seed);                                                 // random number generator

    ConfigParms config;
    processConfigFile(configFile, config);

    Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);        // printer
    
    Bank bank(config.numStudents);        // bank
    
    Parent parent(printer, bank, config.numStudents, config.parentalDelay);        // parent

    WATCardOffice watcardoffice(printer, bank, config.numCouriers);     // WATCard office(couriers)

    Groupoff groupoff(printer, config.numStudents, config.sodaCost, config.groupoffDelay);
    // groupoff

    // name server

    for (unsigned int i = 0; i < config.numVendingMachines; i += 1) {     //vending machines
    }

    // bottling plant(truck)

    for (unsigned int i = 0; i < config.numStudents; i += 1) {     // students
    }

    // run ?
    while (true) {}

    // free all memory
    // NOTE: delete the bottling plant before deleting the vending machines to allow the truck to complete its final 
    //          deliveries to the vending machines; otherwise, a deadlock can occur
    for (unsigned int i = 0; i < config.numVendingMachines; i += 1) {
    }

    for (unsigned int i = 0; i < config.numStudents; i += 1) {
    }

    //parent.~Parent();
    //printer.~Printer();
}
