#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

class Mem
{
public:
  int size;
  int num;
  Mem(int s = 0, int n = 0):size(s), num(n){}
};  // class Mem


int fault(int proc, int *counts, vector<Mem> *procMems, ostringstream &outs,
  int &used)
{
  if(counts[proc] == 0)
    return 0;

  outs << "F " << proc << '\n';
  counts[proc] = 0;
  for(vector<Mem>::iterator itr = procMems[proc].begin();
    itr != procMems[proc].end(); itr++)
      used -= itr->size;
  procMems[proc].clear();
  return 1;
}

int endProc(int proc, int *counts, vector<Mem> *procMems, ostringstream &outs,
  int &used)
{
  if(counts[proc] == 0)
    return 0;
  outs << "E " << proc << '\n';
  counts[proc] = 0;
  for(vector<Mem>::iterator itr = procMems[proc].begin();
    itr != procMems[proc].end(); itr++)
      used -= itr->size;
  procMems[proc].clear();
  return 1;
}

void alloc(int proc, int *counts, vector<Mem> *procMems, ostringstream &outs,
  int &used, int &maxRAM)
{
  int size = 0x40000 / ((rand() & 0xFFFF) + 1);  // range 4 - 262,144 
  used += size;
  if(used > maxRAM)
    maxRAM = used;
  outs << "N " << proc << " " << counts[proc] << " " << size << '\n';
    procMems[proc].push_back(Mem(size, counts[proc]++));
}

int deAlloc(int proc, int *counts, vector<Mem> *procMems, ostringstream &outs,
  int &used)
{
  if(procMems[proc].size() > 0)
  {
    int pos = rand() % procMems[proc].size();
    outs << "D " <<  proc << " " << procMems[proc][pos].num << endl;
    used -= procMems[proc][pos].size;
    procMems[proc][pos] = procMems[proc].back();
    procMems[proc].pop_back();
    return 1;
  } // if proc has allocated some memory.
  return 0;
}

int access(int proc, int *counts, vector<Mem> *procMems, ostringstream &outs)
{
  if(procMems[proc].size() > 0)
  {
    int pos = rand() % procMems[proc].size();
    outs << "A " << proc << " " << procMems[proc][pos].num << endl;
    return 1;
  } // if proc has allocated some memory.
  
  return 0;
}

void createFile(char *filename, int procs, int ops, int *counts,
  vector<Mem> *procMems)
{
  ostringstream outs;
  int proc, temp, maxRAM = 0, used = 0;

  for(int i = 0; i < ops; )
  {
    proc = rand() % procs;
    temp = rand() % 100;

    if(temp < 1)
      i += fault(proc, counts, procMems, outs, used);
    else
      if(temp < 3)
        i += endProc(proc, counts, procMems, outs, used);
      else
        if(temp < 20)
        {
          alloc(proc, counts, procMems, outs, used, maxRAM);
          i++;
        }
        else
          if(temp < 30)
            i += deAlloc(proc, counts, procMems, outs, used);
          else
            i += access(proc, counts, procMems, outs);
  } // for i

  ofstream outf(filename);
  outf << maxRAM << ' ' << procs << ' ' << ops << endl;
  outf << outs.str();



} // createFile

int main(int argc, char* argv[])
{
  int procs, ops, seed, *counts;
  vector<Mem> *procMems;
  char filename[80];
  cout << "Processes: ";
  cin >> procs;
  cout << "Operations: ";
  cin >> ops;
  cout << "Seed: ";
  cin >> seed;
  srand(seed);
  sprintf(filename,"Mem-%d-%d-%d.txt",procs, ops, seed);
  procMems = new vector<Mem>[procs];
  counts = new int[procs];
  for(int i = 0; i < procs; i++)
    counts[i] = 0;
  createFile(filename, procs, ops, counts, procMems);

  return 0;
}
//---------------------------------------------------------------------------
