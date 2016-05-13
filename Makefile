MemRunner.out : MemCheck.o MemMan.o MemRunner.o mynew.o  
	g++ -ansi -Wall -g -o MemRunner.out MemCheck.o MemMan.o MemRunner.o mynew.o  

# MemCheck.o : MemCheck.cpp MemCheck.h 
#	g++ -ansi -Wall -c MemCheck.cpp

MemMan.o : MemMan.cpp MemMan.h mynew.h MemCheck.h 
	g++ -ansi -Wall -g -c MemMan.cpp

MemRunner.o : MemRunner.cpp CPUTimer.h MemMan.h mynew.h 
	g++ -ansi -Wall -g -c MemRunner.cpp

mynew.o : mynew.cpp mynew.h 
	g++ -ansi -Wall -g -c mynew.cpp

clean : 
	rm -f MemRunner.out  MemMan.o  MemRunner.o  mynew.o   
