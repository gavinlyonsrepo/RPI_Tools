# Gavin Lyons 12-2023
# Makefile to build example program for blinkWithoutDelay
# URL: https://github.com/gavinlyonsrepo/RPI_tools

# ********************
# Options
# 1. make  
# 2. make clean
# 3. make run (note :: wraps sudo)
# ********************

SRC=src
CC=g++
LDFLAGS= -lbcm2835 -lrt
CFLAGS = -std=c++2a -Iinclude/ -c -Wall 
MD=mkdir
OBJ=obj
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp,  $(OBJ)/%.o, $(SRCS))
BIN=./bin
TARGET=$(BIN)/blinkWithoutDelay.out

all: clean pre-build $(TARGET) 

run:
	sudo $(TARGET) 
	
pre-build:
	@echo '*******'
	@echo '[START!]'
	$(MD) -vp $(BIN)
	$(MD) -vp $(OBJ)


$(TARGET): $(OBJS) $(OBJ)
	@echo '[MAKE EXE FILE!]'
	$(CC)  $(OBJS) -o $@ $(LDFLAGS)
	@echo '[DONE!]'

$(OBJ)/%.o :  $(SRC)/%.cpp   $(OBJ) 
	@echo '[MAKE OBJECT FILE!]'
	$(CC) $(CFLAGS) $< -o  $@ 

clean:
	@echo '*******'
	@echo '[CLEANUP!]'
	rm -vf $(OBJS) $(TARGET)
	@echo '[DONE!]'
