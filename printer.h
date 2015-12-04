#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <vector>

class LetterData {                                                              // base info class
public:
    char letter;                                                                // contains a character

    virtual ~LetterData() {}
    LetterData(char l) : letter(l) { }
    virtual void print() {                                                      // print method outputs state
    	std::cout << (char)letter; 
    }
};

class LetterNumberData : public LetterData {                                            // inheriting class - adds a number
public:
    unsigned int number;
    LetterNumberData(char l, unsigned int n) : LetterData(l), number(n) {}
    void print() {                                                                      //overloaded print method
        std::cout << letter << number;
    }
};

class LetterNumberNumberData : public LetterNumberData {                                                  //inheriting class - adds two numbers
public:
    unsigned int number2;
    LetterNumberNumberData(char l, unsigned int n, unsigned int n2) : LetterNumberData(l, n), number2(n2) {}
    void print() {                                                                                  //overloaded print method
        std::cout << letter << number << ',' << number2;
    }
};

_Monitor Printer{
    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;
    unsigned int total;
    std::vector<LetterData*> data;         //stores member info
    void flush(std::string bonus);         //send info to output

public:
    ~Printer();
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
};

#endif
