#include <iostream>
#include <vector>

#include "MPRNG.h"
#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watoff.h"
#include "groupoff.h"
#include "namesrv.h"
#include "vending.h"
#include "bottle.h"
#include "student.h"

MPRNG mprng;                                                                        // globally visible random number generator
void cleanCouriers();

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

        case 1:
            break;

        default:
            std::cerr << "needs to be in format: soda [ config-gile [ seed ] ]" << std::endl;
            exit(-1);

    } // end of input switch

    mprng.seed(seed);                                                                           // random number generator

    ConfigParms config;                                                                         // configs
    processConfigFile(configFile, config);

    Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);         // printer
    
    Bank bank(config.numStudents);                                                              // bank
    
    Parent parent(printer, bank, config.numStudents, config.parentalDelay);                     // parent

    Groupoff groupoff(printer, config.numStudents, config.sodaCost, config.groupoffDelay);      // groupoff

    NameServer nameServer(printer, config.numVendingMachines, config.numStudents);              // name server

    {

        WATCardOffice watcardoffice(printer, bank, config.numCouriers);                         // WATCard office (with couriers)

        std::vector<VendingMachine *> machines;
        for (unsigned int i = 0; i < config.numVendingMachines; i += 1) {                       // vending machines
            machines.push_back(new VendingMachine(printer, nameServer, i, config.sodaCost, config.maxStockPerFlavour));
        }

        {
            BottlingPlant plant(printer, nameServer, config.numVendingMachines, config.maxShippedPerFlavour,   // bottling plant (with truck)
                config.maxStockPerFlavour, config.timeBetweenShipments);

            std::vector<Student *> students;
            for (unsigned int i = 0; i < config.numStudents; i += 1) {                          // students
                students.push_back(new Student(printer, nameServer, watcardoffice, groupoff, i, config.maxPurchases));
            }

            // now we delete everyone

            for (unsigned int i = 0; i < config.numStudents; i += 1) {
                delete students[i];
            }
        } //bottling plant and truck die here

        for (unsigned int i = 0; i < config.numVendingMachines; i += 1) {
            delete machines[i];
        }
    } // watcard office dies here
    cleanCouriers();    // couriers die here
} //everyone else dies here
