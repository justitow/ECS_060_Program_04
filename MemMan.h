#ifndef MemManH
#define MemManH
#include "mynew.h"
#include "MemCheck.h"

class MemMan
{


  public:
  MemMan(int ram, int proc, int op, MemCheck &memCheck);
  ~MemMan();
  bool access(int proc, int address, int opNum, MemCheck &memCheck, char print);
  void endProc(int proc, int opNum, MemCheck &memCheck, char print);
  int alloc(int proc, int opNum, int size, MemCheck &memCheck, char print);
  void deAlloc(int proc, int opNum, int startAddress, MemCheck &memCheck, char print );
}; // class MemMan




class Process
{


};

#endif
