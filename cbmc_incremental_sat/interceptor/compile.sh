rm *.o *.gch
cp ~/workspace/riss/release/lib/libriss-coprocessor.a .
objcopy --redefine-syms=rename.txt libriss-coprocessor.a
g++ -I. -c main.cpp -L. -lriss-coprocessor -lpthread  
ar q libriss-coprocessor.a main.o