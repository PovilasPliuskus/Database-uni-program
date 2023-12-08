CC = g++
CFLAGS = -std=c++11 -Wall
LDFLAGS = -I"C:\Program Files\PostgreSQL\16\include" -L"C:\Program Files\PostgreSQL\16\lib" -lpq
PROGRAM = program.exe

# DIRECTORIES
SRC_DIR = src
OBJ_DIR = obj
SHOPPING_SYSTEM_DIR = $(SRC_DIR)/ShoppingSystem
SHOPPING_SYSTEM_HEADER_DIR = $(SHOPPING_SYSTEM_DIR)/HeaderFiles

$(PROGRAM): $(OBJ_DIR)/main.o $(OBJ_DIR)/ShoppingSystem.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(SHOPPING_SYSTEM_HEADER_DIR)/ShoppingSystem.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/ShoppingSystem.o: $(SHOPPING_SYSTEM_DIR)/ShoppingSystem.cpp $(SHOPPING_SYSTEM_HEADER_DIR)/ShoppingSystem.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	del $(OBJ_DIR)\*.o $(PROGRAM)
