#ifndef MemManH
#define MemManH
#include "mynew.h"
#include "MemCheck.h"

class MemBlock
{
public:
  MemBlock(int address, int block_size);
  ~MemBlock();
  int address;
  int block_size;
  MemBlock* next;
  MemBlock* prev;
};

class MemSpace
{
public:
  MemSpace();
  ~MemSpace();
  MemBlock* head;
  MemBlock* curr;
  MemBlock* last;
  int find_block(int adr);
  bool check_for_adr(int adr);
  void insert(int adr, int size);
  void remove(int adr);
  void make_empty();
  void print();
};


class MemMan
{
  MemSpace* memSpaces;
  public:
  MemMan(int ram, int proc, int op, MemCheck &memCheck);
  ~MemMan();
  bool access(int proc, int address, int opNum, MemCheck &memCheck, char print);
  void endProc(int proc, int opNum, MemCheck &memCheck, char print);
  int alloc(int proc, int opNum, int size, MemCheck &memCheck, char print);
  void deAlloc(int proc, int opNum, int startAddress, MemCheck &memCheck, char print );

  int* prevAdr;

}; // class MemMan






#endif
