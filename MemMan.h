#ifndef MemManH
#define MemManH
#include "mynew.h"
#include "MemCheck.h"
#include "QuadraticProbing.h"

class MemBlock
{
public:
  MemBlock();
  ~MemBlock();
  int address;
  int block_size;
  int proc;
};

class Process
{
public:


  int* proc;
  QuadraticHashTable<int>* table;
  QuadraticHashTable<MemBlock>* sizeTable;
  Process();
  ~Process();
};

class MemMan
{
  Process* processes;
  int* prevAdr;

  public:
  MemMan(int ram, int proc, int op, MemCheck &memCheck);
  ~MemMan();
  bool access(int proc, int address, int opNum, MemCheck &memCheck, char print);
  void endProc(int proc, int opNum, MemCheck &memCheck, char print);
  int alloc(int proc, int opNum, int size, MemCheck &memCheck, char print);
  void deAlloc(int proc, int opNum, int startAddress, MemCheck &memCheck, char print );
}; // class MemMan






#endif
