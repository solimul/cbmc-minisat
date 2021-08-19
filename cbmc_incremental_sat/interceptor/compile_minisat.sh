rm *.o *.gch
cp /workspace/ipasir/sat/minisat220/libipasirminisat220.a .
objcopy --redefine-syms=rename.txt libipasirminisat220.a
g++ -I. -c main.cpp -L. -lipasirminisat220 -lpthread  
ar q libipasirminisat220.a main.o