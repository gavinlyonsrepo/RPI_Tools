#==============================
# Gavin Lyons 12-2023
# Make file to build i2cDetectGL Program
# URL:: https://github.com/gavinlyonsrepo/RPI_Tools/blob/main/src/i2cdetect/i2cdetect.md
# Run make help for options
#==============================

SRC=src/
CC=g++
LDFLAGS= -lbcm2835
CFLAGS = -std=c++2a -Iinclude/ -c -Wall  -Wextra
MD=mkdir
BUILD=Build
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp,  $(BUILD)/%.o, $(SRCS))
BIN=./Bin
EXENAME=i2cDetectGL
TARGET=$(BIN)/$(EXENAME)
PREFIX=/usr/local/bin

# The --no-print-directory option of make tells make not to print the message about entering 
# and leaving the working directory. 
MAKEFLAGS += --no-print-directory

# Main task,  makes build directory, updates your objects, builds your executable
.PHONY: all
all: clean build


# Task producing target from built files
$(TARGET): $(OBJS) $(BUILD)
	@echo 'MAKE EXE FILE'
	$(CXX)  $(OBJS) -o $@ $(LDFLAGS)
	@echo 'DONE!'

# Compile all cpp files
$(BUILD)/%.o :  $(SRC)/%.cpp   $(BUILD) 
	@echo 'MAKE OBJECT FILE'
	$(CXX) $(CFLAGS) $< -o  $@ 


# Build task
.PHONY:	build
build:
	@echo 'BUILDING: ' $(SRC)
	$(MD) -vp $(BIN)
	$(MD) -vp $(BUILD)
	$(MAKE) $(TARGET)
	@echo '***************'

# Run task
.PHONY: run
run:
	sudo $(TARGET)

# Clean task :: removes obj and executbale
.PHONY: clean
clean:
	@echo '*******'
	@echo '[CLEANUP!]'
	rm -rvf $(BUILD) $(BIN)
	@echo '[DONE!]'


# Help task :: explains the options
.PHONY: help
help:
	@echo 'HELP!'
	@echo "make           - Cleans + builds project"
	@echo "make clean     - Cleans : Removes object file folder and executable"
	@echo "make build     - Complies and Builds project"
	@echo "make run       - Runs exe(wraps sudo, bcm2835 lib needs sudo)"
	@echo "make install   - Installs exe, may need sudo"
	@echo "make uninstall - Uninstalls exe, may need sudo"
	@echo "make help      - Prints help message"
	@echo '***************'

#install exe
.PHONY: install
install:
	@echo "[INSTALL EXE]"
	cp -v $(TARGET) $(PREFIX)/$(EXENAME)
	@echo "*****************"

# Uninstall the exe
.PHONY: uninstall
uninstall:
	@echo "[UNINSTALL EXE]"
	@rm -vf $(PREFIX)/$(EXENAME)
	@echo "******************"
