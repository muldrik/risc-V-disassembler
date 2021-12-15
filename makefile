.PHONY: all clean

CXX=g++
CXXFLAGS=-std=c++17 -Wall -pedantic

all: lab_03

obj:
	mkdir -p obj

lab_03: src/main.cpp obj/elf_parsing.o obj/instructions.o obj/printing.o include/*.h obj
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude $< obj/*


obj/%.o: src/%.cpp include/*.h obj
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<


clean:
	rm -rf obj lab_03