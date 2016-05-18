#include <iostream>
#include "MemMan.h"
#include "mynew.h"
#include "MemCheck.h"


using namespace std;

MemBlock::MemBlock(int adr, int blk) : address(adr), block_size(blk)
{
  next = NULL;
  prev = NULL;
}


MemBlock::~MemBlock()
{

}

MemSpace::MemSpace()
{
  head = NULL;
  curr = head;
}

MemSpace::~MemSpace()
{

}

int MemSpace::find_block(int adr)
{
  for (curr = head; curr->next != NULL && adr < curr->address; curr = curr->next);

  return curr->block_size;
}

void MemSpace::insert(int adr, int size)
{

  MemBlock* myblock = new MemBlock(adr, size);
  if (this->head == NULL)
  {
    head = myblock;
  }
  else
  {
    for (curr = head; curr->next != NULL && adr > curr->address; curr = curr->next)
      prev = curr;

    if (curr == head)
    {
      if (adr < curr->address)
      {
        myblock->next = head;
        head->prev = myblock;
        head = myblock;
      }
      else
      {
        head->next = myblock;
        myblock->prev = head;
      }
    }

    else if (curr->next == NULL)
    {
      if (adr < curr->address)
      {
        curr->prev->next = myblock;
        myblock->prev = curr->prev;
        myblock->next = curr;
        curr->prev = myblock;
      }
      else
      {
        curr->next = myblock;
        myblock->prev = curr;
      }
    }

    else
    {
      curr->prev->next = myblock;
      myblock->prev = curr->prev;
      myblock->next = curr;
      curr->prev = myblock;
    }

  }


}

void MemSpace::remove(int adr)
{
  for (curr = head; curr->next != NULL && adr < curr->address; curr = curr->next);

  if (curr == head)
  {
    curr->next->prev = NULL;
    head = curr->next;
    delete curr;
  }

  else if (curr->next == NULL)
  {
    curr->prev->next = NULL;
    delete curr;
  }

  else
  {
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    delete curr;
  }

}

void MemSpace::print()
{
  curr = head;
  if (curr == NULL)
    return;

  while (curr->next != NULL)
  {
    cout << curr->address << endl;
    curr = curr->next;
  }
  cout << curr->address << endl;
}

Process::Process()
{
  table = new QuadraticHashTable<int>(-1);
  space = new MemSpace();
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
  
  memCheck.printCurrentAllocations(proc);
  memCheck.printOwner(address, address);

  if (this->processes[proc].table->find(address) != -1) // if the address is in the address table stored under the process. Maybe?
  {
    cout << "It is rightfully accessed!" << endl;
    return true;
  }
  else
    this->endProc(proc, opNum, memCheck, print);

  cout << "THIEF!" << endl;
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
  // allocates a block of the specified size, and returns its address.

  for (int i = 0; i < size; i++)
  {
    this->processes[proc].table->insert(*this->prevAdr + i);
  }

  this->processes[proc].space->insert(*this->prevAdr, size);
  *prevAdr += size; // just to do the niave approach, maybe, I think

  memCheck.printCurrentAllocations(proc);
  this->processes[proc].space->print();

  return *this->prevAdr - size;
} // alloc()


void MemMan::deAlloc(int proc, int opNum, int startAddress, MemCheck &memCheck,
  char print)
{
  //if (print != '0')
    cout << "Opnum: " << opNum << " daAlloc: proc: " << proc << " startAddress: " << startAddress << endl;
    memCheck.printCurrentAllocations(proc);
    memCheck.printOwner(startAddress, startAddress + this->processes[proc].space->find_block(startAddress) - 1);
  int ref = this->processes[proc].space->find_block(startAddress);

  for (int i = 0; i < ref; i++)
  {
    this->processes[proc].table->remove(startAddress + i);
  }



} // deAlloc()


void MemMan::endProc(int proc, int opNum, MemCheck &memCheck, char print)
{
  //if(print != '0')
    cout << "Opnum: " << opNum << " endProc: proc: " << proc << endl;
  
  memCheck.printCurrentAllocations(proc);
  // free all memory assigned to proc.
  this->processes[proc].space->print();

  while (this->processes[proc].space->head->next != NULL)
  {
    cout << "Ending: " << this->processes[proc].space->head->address << endl;
    memCheck.deAlloc(proc, this->processes[proc].space->head->address, opNum);
    this->processes[proc].space->head = this->processes[proc].space->head->next;
    if (this->processes[proc].space->head->prev != NULL)
      delete this->processes[proc].space->head->prev;
  }
  memCheck.deAlloc(proc, this->processes[proc].space->head->address, opNum);
  delete this->processes[proc].space->head;

  this->processes[proc].table->makeEmpty();

  memCheck.printCurrentAllocations(proc);
} // endProc()


