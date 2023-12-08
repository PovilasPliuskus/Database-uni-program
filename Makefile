program: main.o
	g++ main.o -o program.exe

main.o: main.cpp
	g++ -c main.cpp

clean:
	del *.o program.exe