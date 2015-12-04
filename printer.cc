#include "printer.h"

void Printer::flush(std::string bonus = "") {            //send info to output
    for (unsigned int i = 0; i < total; i += 1) { 
        if (data[i] != NULL) {
            data[i]->print();                             //if there is info, print it
            delete data[i];
            data[i] = NULL;
        } 
        else if (bonus.length() > 0) {                     //if we are flushing due to a finish, print '...'
            std::cout << bonus;
        }
        std::cout << '\t';
    }
    std::cout << std::endl;
}

Printer::~Printer() {                                         //print completion message
    std::cout << "***********************" << std::endl;
    for (unsigned int i = 0; i < total; i += 1) {
        delete data[i];                                     //free memory
    }
}

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers  ) : 
    numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {         //printer header for each member

    total = 6 + numStudents + numVendingMachines + numCouriers;

    std::cout << "Parent\t" << "Gropoff\t" << "WATOff\t" << "Names\t" << "Truck\t" << "Plant\t";

    for (unsigned int i = 0; i < 6; i += 1) {
        data.push_back(NULL);
    }

    for (unsigned int i = 0; i < numStudents; i += 1) {
        data.push_back(NULL);
        std::cout << "Stud" << i << "\t";
    }

    for (unsigned int i = 0; i < numVendingMachines; i+= 1) {
        data.push_back(NULL);
        std::cout << "Mach" << i << "\t";
    }

    for (unsigned int i = 0; i < numCouriers; i+= 1) {
        data.push_back(NULL);
        std::cout << "Cour" << i << "\t";
    }

    std::cout << std::endl;

    for (unsigned int i = 0; i < total; i += 1) {
        std::cout << "*******\t";
    }
    std::cout << std::endl;
}

void Printer::print( Kind kind, char state ) {
    int id = kind;
    if (data[id] != NULL) {
        flush();
    }
    data[id] = new LetterData(state);
    if (state == 'F') flush("...");
}

void Printer::print( Kind kind, char state, int value1 ) {
    int id = kind;
    if (data[id] != NULL) {
        flush();
    }
    data[id] = new LetterNumberData(state, value1);
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
    int id = kind;
    if (data[id] != NULL) {
        flush();
    }
    data[id] = new LetterNumberNumberData(state, value1, value2);
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
    int id = kind + lid;
    if (kind == Kind::Vending) id += numStudents;
    if (kind == Kind::Courier) id += numStudents + numVendingMachines;
    if (data[id] != NULL) {
        flush();
    }
    data[id] = new LetterData(state);
    if (state == 'F') flush("...");
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    int id = kind + lid;
    if (kind == Kind::Vending) id += numStudents;
    if (kind == Kind::Courier) id += numStudents + numVendingMachines;
    if (data[id] != NULL) {
        flush();
    }
    data[id] = new LetterNumberData(state, value1);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    int id = kind + lid;
    if (kind == Kind::Vending) id += numStudents;
    if (kind == Kind::Courier) id += numStudents + numVendingMachines;
    if (data[id] != NULL) {
        flush();
    }
    data[id] = new LetterNumberNumberData(state, value1, value2);
}
