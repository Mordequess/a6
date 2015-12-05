#ifndef STUDENT_H
#define STUDENT_H

#include "namesrv.h"
#include "printer.h"
#include "watoff.h"
#include "groupoff.h"

_Task Student {
    Printer &printer;
    NameServer &nameServer;
    WATCardOffice &cardOffice;
    Groupoff &groupoff;

    unsigned int id;
    unsigned int maxPurchases;
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases );
};

#endif//STUDENT_H
