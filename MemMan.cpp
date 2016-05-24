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
  last = head;
}

MemSpace::~MemSpace()
{

}

int MemSpace::find_block(int adr)
{
  curr = head;
  while (curr->address != adr)
  {
    curr = curr->next;
  }

  return curr->block_size;
}

void MemSpace::insert(int adr, int size)
{

  MemBlock* myblock = new MemBlock(adr, size);
  if (this->head == NULL)
  {
    head = myblock;
    last = head;
  }
  else // for niave approach, this will always be where the thing is getting inserted, I think
  {
    last->next = myblock;
    myblock->prev = last;
    last = myblock;
  }


}

void MemSpace::remove(int adr)
{
  curr = head;
  while(adr != curr->address)
  {
    curr = curr->next;
  }

  if (curr == head) // head delete
  {
    if (curr->next != NULL)
    {
      head->next->prev = NULL;
      head = head->next;
    }
    else
      head = NULL;
    delete curr;
  }

  else if (curr->next == NULL) // tail delete
  {
    curr->prev->next = NULL;
    delete curr;
  }

  else // body delete
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
    cout << curr->address << ":" << curr->block_size <<  endl;
    curr = curr->next;
  }
  cout << curr->address << ":" << curr->block_size <<  endl;

}

bool MemSpace::check_for_adr(int adr)
{
  if (head != NULL)
  {
    for (curr = head; curr->next != NULL && curr->address < adr; curr = curr->next);

    if (curr != head)
    {
      if (curr != last || (curr == last && adr < curr->address))
      {
        curr = curr->prev;
      }
    }

    if ((adr <= curr->address + curr->block_size - 1) && (adr >= curr->address))
    {
      return true;
    }
  }

  return false;
}

void MemSpace::make_empty()
{

}


MemMan::MemMan(int ram, int proc, int op, MemCheck &memCheck)
{
  memSpaces = new MemSpace[100];
  prevAdr = new int;
  *prevAdr = 0;
}// MemMan()


MemMan::~MemMan()
{
  delete [] memSpaces;

} // ~MemMan()



bool MemMan::access(int proc, int address, int opNum, MemCheck &memCheck, 
  char print)
{
  if(print != '0')
    cout << "Opnum: " << opNum << " access: proc: " << proc << " address: " << address << endl;
  
  memCheck.printCurrentAllocations(proc);

  //memCheck.printOwner(address, address);

  if (this->memSpaces[proc].check_for_adr(address)) // if the address is in the address table stored under the process. Maybe?
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
  if(print != '0')
    cout << "Opnum: " << opNum << " alloc: proc: " << proc << " address: " 
      << *this->prevAdr << " size: " << size << endl;
  
   //memCheck.printOwner(address, endAddress);
  // allocates a block of the specified size, and returns its address.

  this->memSpaces[proc].insert(*this->prevAdr, size);

  *prevAdr += size; // just to do the niave approach, maybe, I think

  //memCheck.printCurrentAllocations(proc);
  //this->processes[proc].space->print();
  //cout << "Allocated: " << *this->prevAdr - size << endl;
  return *this->prevAdr - size;
} // alloc()


void MemMan::deAlloc(int proc, int opNum, int startAddress, MemCheck &memCheck,
  char print)
{
  if (print != '0')
    cout << "Opnum: " << opNum << " daAlloc: proc: " << proc << " startAddress: " << startAddress << endl;
    //memCheck.printCurrentAllocations(proc);
    //memCheck.printOwner(startAddress, startAddress + this->processes[proc].space->find_block(startAddress) - 1);


  this->memSpaces[proc].remove(startAddress);

} // deAlloc()


void MemMan::endProc(int proc, int opNum, MemCheck &memCheck, char print)
{
  if(print != '0')
    cout << "Opnum: " << opNum << " endProc: proc: " << proc << endl;
  
  //memCheck.printCurrentAllocations(proc);
  // free all memory assigned to proc.
  this->memSpaces[proc].print();


  while (this->memSpaces[proc].head != NULL)
  {
    memCheck.deAlloc(proc, this->memSpaces[proc].head->address, opNum);
    this->memSpaces[proc].remove(this->memSpaces[proc].head->address);
  }


  //this->processes[proc].space->head = NULL;

  //this->processes[proc].table->makeEmpty();


  //memCheck.printCurrentAllocations(proc);
} // endProc()


