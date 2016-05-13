//
// Created by Justin Weich on 5/13/16.
//

#include "MemCheck.h"

int MemCheck::getBadAddress(int proc)
{

}

int MemCheck::getGoodAddress(int proc, int blockNum)
{

}

int MemCheck::getGoodBlockAddress(int proc, int blockNum)
{

}

bool MemCheck::endProc(int proc, int opNum)
{

}

bool MemCheck::checkAlloc(multiset<BlockInfo7> &blockSet, Operation2 &operation)
{

}

bool MemCheck::checkDeAlloc(multiset<BlockInfo7> &blockSet,
                            Operation2 &operation)
{

}

void MemCheck::alloc(int proc, int address, int size, int opNum, int blockNum)
{

}

MemCheck::MemCheck(int Ram, int proc, int op)
{

}

int MemCheck::checkOperations(Operation *operations)
{

}

bool MemCheck::deAlloc(int proc, int address, int opNum)
{

}

void MemCheck::printCurrentAllocations(int proc)
{

}

void MemCheck::printOwner(int address, int endAddress)
{

}