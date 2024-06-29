CC = g++
STD = c++17
prog = main  
prog2 = ycsb
objects = main.o radix_tree.o compressed_radix_tree.o
objects2 = ycsb.o radix_tree.o compressed_radix_tree.o

all: $(prog)

$(prog): $(objects)
	$(CC) -std=$(STD) -g -o $(prog) $(objects) -O3

$(prog2): $(objects2)
	$(CC) -std=$(STD) -g -o $(prog2) $(objects2) -O3

main.o: main.cpp radix_tree.hpp compressed_radix_tree.hpp
	$(CC) -std=$(STD) -g -c main.cpp -O3

radix_tree.o: radix_tree.cpp radix_tree.hpp
	$(CC) -std=$(STD) -g -c radix_tree.cpp -O3

compressed_radix_tree.o: compressed_radix_tree.cpp compressed_radix_tree.hpp
	$(CC) -std=$(STD) -g -c compressed_radix_tree.cpp -O3

ycsb.o : ycsb.cpp radix_tree.hpp compressed_radix_tree.hpp
	$(CC) -std=$(STD) -g -c ycsb.cpp 
	
grade: $(prog)
ifeq ($(OS), Windows_NT)
	@powershell.exe -ExecutionPolicy Bypass -File .\grade.ps1
else
	@./grade.sh
endif

clean:
ifeq ($(OS), Windows_NT)
	@del /q .\*.o .\output\* 2>nul
	@del /q .\$(prog) 2>nul  
else
	@rm -rf ./*.o ./output/* $(prog)
endif