program: Code/main.o
	g++ Code/main.o -o program.exe

Code/main.o: Code/main.cpp
	g++ -c Code/main.cpp -o Code/main.o

clean:
	del Code\*.o program.exe