# https://stackoverflow.com/a/25966957

BIN := bin
SRC := raster
OBJ := objects

app     := $(BIN)/main
sources := $(wildcard $(SRC)/*.cpp)
objects := $(subst $(SRC),$(OBJ),$(sources:.cpp=.o))
deps    := $(objects:.o=.d)

-include $(deps)

CXX 	 := g++
CPPFLAGS := -I. -I./thirdparty/eigen/ -MMD -MP
CXXFLAGS := -std=c++20 -O3 -Wall -Wextra -pedantic-errors
LDFLAGS  :=
LDLIBS   := -lncurses

# Link apps
$(BIN)/main: $(objects)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compile objects
$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: all
all: $(app) $(objects)

.PHONY: clean
clean:
	$(RM) -r $(BIN)/* $(OBJ)/*
