CXX = g++
CXXFLAGS = -O2 -Wall -pipe -std=c++17
LDFLAGS = -lfmt
INCLUDE_DIRS = -I/usr/include
LIB_DIRS = -L/usr/lib

SRC = cppfetch.cpp
OBJ = $(SRC:.cpp=.o)
EXE = cppfetch

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $(EXE) $(LIB_DIRS) $(LDFLAGS)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(SRC)

install: $(EXE)
	install -m 755 $(EXE) /usr/local/bin/

clean:
	rm -f $(OBJ) $(EXE)

