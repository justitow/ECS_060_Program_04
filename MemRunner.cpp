#include <fstream>
#include <cstdlib>
#include <iostream>
#include "CPUTimer.h"
#include "MemMan.h"
#include "mynew.h"

using namespace std;

class Operation
{
public:
  char op;
  int proc;
  int blockNum;
  int size;
};  // class Operation

extern int maxRAM;


Operation* readFile(const char* filename, int *RAM, int *procs, int *ops)
{
  ifstream inf(filename);
  inf >> *RAM >> *procs >> *ops;
  inf.ignore(100, '\n');
  Operation *operations = new Operation[*ops];
  
  for(int i = 0; i < *ops; i++)
  {
    inf >> operations[i].op >> operations[i].proc;
    
    if(operations[i].op != 'E' && operations[i].op != 'F')
      inf >> operations[i].blockNum;
    
    if(operations[i].op == 'N')
      inf >> operations[i].size;
    
    inf.ignore(100, '\n');
  }  // for each op
  
  inf.close();
  return operations;
} // readFile()

bool fault(Operation &operation, MemMan *memMan, int opNum, MemCheck &memCheck,
  char print)
{
  int badAddress = memCheck.getBadAddress(operation.proc);
  
  if(memMan->access(operation.proc, badAddress, opNum, memCheck, print))
  {
    memCheck.printCurrentAllocations(26);
    cout  << "opNum: " << opNum << " Missed seg fault for proc: " 
      << operation.proc  << " address: " << badAddress << endl;
    memCheck.printOwner(badAddress, badAddress);
    return false;
  } // if bad access wasn't caught
  else
    return memCheck.endProc(operation.proc, opNum);
  
  return true;
} // fault

bool endProc(Operation &operation, MemMan *memMan, int opNum, 
  MemCheck &memCheck, char print)
{
  memMan->endProc(operation.proc, opNum, memCheck, print);
  return memCheck.endProc(operation.proc, opNum);
} // endProc()

void alloc(Operation &operation, MemMan *memMan, int opNum, MemCheck &memCheck,
  char print)
{
  int address = memMan->alloc(operation.proc, opNum, operation.size, 
    memCheck, print);
  memCheck.alloc(operation.proc, address, operation.size, opNum, 
    operation.blockNum);
} // alloc()

void deAlloc(Operation &operation, MemMan *memMan, int opNum, 
  MemCheck &memCheck, char print)
{
  int address = memCheck.getGoodBlockAddress(operation.proc, operation.blockNum);
  memMan->deAlloc(operation.proc, opNum, address, memCheck, print);
  memCheck.deAlloc(operation.proc, address, opNum);
} // deAlloc()

bool access(Operation &operation, MemMan *memMan, int opNum, 
  MemCheck &memCheck, char print)
{
  int address = memCheck.getGoodAddress(operation.proc, operation.blockNum);
  
  if(!memMan->access(operation.proc, address, opNum, memCheck, print))
  {
    cout  << "opNum: " << opNum << " Good access of proc: " << operation.proc 
      << " address: " << address << " caused seg fault.\n";
    return false;
  }
  
  return true;
} // access()

int main(int argc, char* argv[])
{
  int RAM, procs, ops;
  char print = '1'; // argv[2][0];
  Operation *operations = readFile(argv[1], &RAM, &procs, &ops);
  MemCheck memCheck(RAM, procs, ops);
  CPUTimer ct;
  initializeNew();
  ct.reset();
  MemMan *memMan = new MemMan(RAM, procs, ops, memCheck);
  
  for(int opNum = 0; opNum < ops; opNum++)
  {
   // if (opNum == 266086)
     // cout << "Help\n";
    
    switch(operations[opNum].op)
    {
      case 'F' : 
        if(! fault(operations[opNum], memMan, opNum, memCheck, print))
          opNum = ops;  // error so leave
        break;
      case 'E' : 
        if(! endProc(operations[opNum], memMan, opNum, memCheck, print))
          opNum = ops; // error so leave
        break;
      case 'N' : alloc(operations[opNum], memMan, opNum, memCheck, print); break;
      case 'D' : deAlloc(operations[opNum], memMan, opNum, memCheck, print); break;
      case 'A' : 
        if(! access(operations[opNum], memMan, opNum, memCheck, print))
          opNum = ops; // error so leave
        break;
    } // switch
  } // while more in file
 
  double CPUTime = ct.cur_CPUTime();
  int maxRAM2 = maxRAM;
  int yourRAM = memCheck.checkOperations(operations); 
  cout << "CPU Time: " << CPUTime << " Ideal RAM: " << RAM
    << " Your RAM: " << yourRAM << " Real RAM: " << maxRAM2 << endl;
  cout << memMan->prevAdr << endl;
  delete memMan;  // useful for determining final size of ADTs.
  return 0;
} // main()
