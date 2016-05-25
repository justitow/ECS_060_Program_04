#include <iostream>
#include "MemMan.h"
#include "mynew.h"
#include "MemCheck.h"


using namespace std;

MemBlock::MemBlock(int adr, int blk, int mx) : address(adr), block_size(blk), max_size(mx)
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



int MemSpace::insert(int adr, int size, MemSpace* blankspace) // returns true if new memory is created
{
  int address;
  //if there is already a memory block
  MemBlock* myblock = blankspace->findBlock(size);
  if (myblock != NULL)
  {
    myblock->block_size = size;
    address =  myblock->address;
    myblock->next = NULL;
    myblock->prev = NULL;
  }
  //else
  else
  {
    myblock = new MemBlock(adr, size, size);
    address = adr;
  }

  this->insert_alloc(myblock);

  return address;
}

void MemSpace::print()
{
  if (head == NULL)
    return;
  for (curr = head; curr != NULL; curr = curr->next)
    cout << curr->address << " : " << curr->max_size << endl;

}

void MemSpace::insert_alloc(MemBlock *block) // sorted by address
{
  block->next = NULL;
  block->prev = NULL;

  if (head == NULL)
  {
    head = block;
  }
  else
  {
    for (curr = head; curr->next != NULL && curr->address < block->address; curr = curr->next);

    if (curr == head && curr->address > block->address) // head insert
    {
      curr->prev = block;
      block->next = curr;
      head = block;
    }
    else if (curr->next == NULL && curr->address < block->address) // tail insert
    {
        curr->next = block;
        block->prev = curr;
    }
    else // body insert
    {
      if (curr->address > block->address)
      {
        if (curr->prev)
          curr->prev->next = block;

        block->prev = curr->prev;
        block->next = curr;
        curr->prev = block;
      }
      else
      {
        if (curr->next)
          curr->next->prev = block;

        block->next = curr->next;
        block->prev = curr;
        curr->next = block;
      }
    }
  }
}

void MemSpace::insert_dealloc(MemBlock* block) // sorted by max_size
{
  this->print();

  block->next = NULL;
  block->prev = NULL;

  if (head == NULL)
  {
    head = block;
  }
  else
  {
    for (curr = head; curr->next != NULL && curr->max_size < block->max_size; curr = curr->next);

    if (curr == head && curr->max_size > block->max_size) // head insert
    {
      curr->prev = block;
      block->next = curr;
      head = block;
    }
    else if (curr->next == NULL && curr->max_size < block->max_size) // tail insert
    {
      curr->next = block;
      block->prev = curr;
    }
    else // body insert
    {
      if (curr->max_size > block->max_size)
      {
        if (curr->prev)
          curr->prev->next = block;

        block->prev = curr->prev;
        block->next = curr;
        curr->prev = block;
      }
      else
      {
        if (curr->next)
          curr->next->prev = block;

        block->next = curr->next;
        block->prev = curr;
        curr->next = block;
      }
    }
  }
}

void MemSpace::remove(int adr, MemSpace* blankspace)
{
  curr = head;
  while(adr != curr->address)
  {
    curr = curr->next;
    cout << curr->address << endl;
  }

  if (curr == head) // head delete
  {
    if (curr->next != NULL)
    {
      head->next->prev = NULL;
      head = head->next;
    }
    else
    {
      head = NULL;
    }
  }

  else if (curr->next == NULL) // tail delete
  {
    curr->prev->next = NULL;
  }

  else // body delete
  {
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
  }

  cout << "Starting blankspace insesrt" << endl;
  blankspace->insert_dealloc(curr);
}



bool MemSpace::check_for_adr(int adr)
{
  if (head != NULL)
  {
    for (curr = head; curr->next != NULL && curr->address < adr; curr = curr->next);


    if (curr != head && adr < curr->address)
      curr = curr->prev;

    if ((adr <= curr->address + curr->block_size - 1) && (adr >= curr->address))
      return true;

  }

  return false;
}



MemBlock* MemSpace::findBlock(int size) // only for blankspace, returns a block from the blankspace structure
{
  if (head == NULL)
    return NULL;

  else
  {
    for (curr = head; curr->next != NULL && curr->max_size < size; curr = curr->next);

    if (curr->next == NULL && curr->max_size < size)
      return NULL;

    else
    {
      if (curr->prev != NULL)
        curr->prev->next = curr->next;

      if (curr->next != NULL)
        curr->next->prev = curr->prev;

      return curr;
    }
  }
}

MemMan::MemMan(int ram, int proc, int op, MemCheck &memCheck)
{
  memSpaces = new MemSpace[100];
  blankSpace = new MemSpace();
  address_mark = new int;
  *address_mark = 0;
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
  
  //memCheck.printCurrentAllocations(proc);

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
      << *address_mark << " size: " << size << endl;
  
   //memCheck.printOwner(address, endAddress);
  // allocates a block of the specified size, and returns its address.
  int new_adr = this->memSpaces[proc].insert(*address_mark, size, blankSpace);
  if (new_adr == *address_mark)
  {
    *address_mark += size;
  }

  return new_adr;
  //memCheck.printCurrentAllocations(proc);
  //this->processes[proc].space->print();
  //cout << "Allocated: " << *this->prevAdr - size << endl;
} // alloc()


void MemMan::deAlloc(int proc, int opNum, int startAddress, MemCheck &memCheck,
  char print)
{
  if (print != '0')
    cout << "Opnum: " << opNum << " daAlloc: proc: " << proc << " startAddress: " << startAddress << endl;
    memCheck.printCurrentAllocations(proc);
    //memCheck.printOwner(startAddress, startAddress + this->processes[proc].space->find_block(startAddress) - 1);


  this->memSpaces[proc].remove(startAddress, blankSpace);

} // deAlloc()


void MemMan::endProc(int proc, int opNum, MemCheck &memCheck, char print)
{
  if(print != '0')
    cout << "Opnum: " << opNum << " endProc: proc: " << proc << endl;
  
  //memCheck.printCurrentAllocations(proc);
  // free all memory assigned to proc.
  //this->memSpaces[proc].print();


  while (this->memSpaces[proc].head != NULL)
  {
    memCheck.deAlloc(proc, this->memSpaces[proc].head->address, opNum);
    this->memSpaces[proc].remove(this->memSpaces[proc].head->address, blankSpace);
  }


  //this->processes[proc].space->head = NULL;

  //this->processes[proc].table->makeEmpty();


  //memCheck.printCurrentAllocations(proc);
} // endProc()


