#ifndef MemCheckH
#define MemCheckH
#include <set>
using namespace std;

class BlockInfo7
{
public:
  int proc;
  int address;
  int size;
  BlockInfo7(int pr = -1, int addr = -1, int sz = -1) : proc(pr), address(addr),
    size(sz)  {}
  inline bool operator< (const BlockInfo7 &rhs)const
    {return address + size - 1 < rhs.address;}
};  // class BlockInfo7


class Operation2
{
public:
  char op;  //  'N' = alloc, 'D' = dealloc
  int address;
  int proc;
  int size;
  int opNum;
  Operation2(){}
  Operation2(char o, int addr, int pr, int sz, int oN) : op(o), address(addr),
    proc(pr),size(sz), opNum(oN) {}
};

class MemBlock2
{
public:
  MemBlock2(int addr = -2, int s = -2):
    address(addr), size(s) {}
  int address;
  int size;
  
}; // class MemBlock2

class MemMan;
class Operation;

class MemCheck {
  int RAM;
  int procs;
  int ops;
  int size;
  int **procBlocks;
  MemBlock2 memBlocks[300][500];
  int *counts;
  Operation2 *operations;
  int opCount;
  int numProcsAlive;
  int maxAddress;
  int getBadAddress(int proc);
  int getGoodAddress(int proc, int blockNum);
  int getGoodBlockAddress(int proc, int blockNum);
  bool endProc(int proc, int opNum);
  bool checkAlloc(multiset <BlockInfo7> &blockSet, Operation2 &operation);
  bool checkDeAlloc(multiset <BlockInfo7> &blockSet, Operation2 &operation);
  void alloc(int proc, int address, int size, int opNum, int blockNum);
  friend bool fault(Operation &operation, MemMan *memMan, int opNum, MemCheck &memCheck, char print);
  friend bool endProc(Operation &operation, MemMan *memMan, int opNum, MemCheck &memCheck, char print);
  friend bool access(Operation &operation, MemMan *memMan, int opNum, MemCheck &memCheck, char print);
  friend void deAlloc(Operation &operation, MemMan *memMan, int opNum, MemCheck &memCheck, char print);
  friend void alloc(Operation &operation, MemMan *memMan, int opNum, MemCheck &memCheck, char print);
  
public:
  MemCheck(int Ram, int proc, int op);
  int checkOperations(Operation *operations);
  bool deAlloc(int proc, int address, int opNum);
  void printCurrentAllocations(int proc);
  void printOwner(int address, int endAddress);
}; // class MemCheck
#endif
