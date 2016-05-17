#include <iostream>
#include "MemMan.h"
#include "mynew.h"
#include "MemCheck.h"


using namespace std;


Process::Process()
{
  table = new QuadraticHashTable<int>(101);

  proc = new int;
}

Process::~Process()
{
  delete table;
  delete proc;
}


MemMan::MemMan(int ram, int proc, int op, MemCheck &memCheck)
{
  processes = new Process[100]();
  prevAdr = new int;
  *prevAdr = 0;
}// MemMan()


MemMan::~MemMan()
{
  delete [] processes;
} // ~MemMan()



bool MemMan::access(int proc, int address, int opNum, MemCheck &memCheck, 
  char print)
{
  //if(print != '0')
    cout << "Opnum: " << opNum << " access: proc: " << proc << " address: " << address << endl;
  
  // memCheck.printCurrentAllocations(proc);
 // memCheck.printOwner(address, address);
  if (this->processes[proc].table->find(address)) // if the address is in the address table stored under the process. Maybe?
  {
    return true;
  }
  else
    this->endProc(proc, opNum, memCheck, print);
  return false;
  // If seg fault, then free all memory assigned to the process, and return false.
  // If legitimate access, then return true;
}  // access()

int MemMan::alloc(int proc, int opNum, int size, MemCheck &memCheck, char print)
{
  int address = 0;  // to avoid warnings 
  //if(print != '0')
    cout << "Opnum: " << opNum << " alloc: proc: " << proc << " address: " 
      << *this->prevAdr << " size: " << size << endl;
  
   //memCheck.printOwner(address, endAddress);
  memCheck.printCurrentAllocations(proc);
  // allocates a block of the specified size, and returns its address.

  for (int i = 0; i < size; i++)
  {
    this->processes[proc].table->insert(*this->prevAdr + i);
  }

  *prevAdr += size; // just to do the niave approach, maybe, I think
  return *this->prevAdr;
} // alloc()


void MemMan::deAlloc(int proc, int opNum, int startAddress, MemCheck &memCheck,
  char print)
{
  //if (print != '0')
    cout << "Opnum: " << opNum << " daAlloc: proc: " << proc << " startAddress: " << startAddress << endl;
  //  memCheck.printCurrentAllocations(proc);
  // memCheck.printOwner(startAddress, endAddress);
  this->processes[proc].table->remove(startAddress);


  //for
 
} // deAlloc()


void MemMan::endProc(int proc, int opNum, MemCheck &memCheck, char print)
{
  //if(print != '0')
    cout << "Opnum: " << opNum << " endProc: proc: " << proc << endl;
  
   // memCheck.printCurrentAllocations(proc);
  // free all memory assigned to proc.
  delete this->processes[proc].table;
  this->processes[proc].table = new QuadraticHashTable<int>(100);
} // endProc()


