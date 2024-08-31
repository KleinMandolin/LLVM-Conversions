CXX = wllvm++

CXX_FLAGS = -std=c++14
CXX_FLAGS += -Wall
CXX_FLAGS += -Wextra
CXX_FLAGS += -MMD
CXX_FLAGS += -MP
CXX_FLAGS += -Ofast
CXX_FLAGS += -march=native
CXX_FLAGS += -Wno-unknown-warning-option # ignore unknown warnings (as '-Wno-maybe-uninitialized' resulting from internal bugs)
CXX_FLAGS += -Qunused-arguments
CXX_FLAGS += -pipe
#CXX_FLAGS += -g

# Define useful make functions
recwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call recwildcard,$d/,$2))

# Name of the executable
EXE = main

# Basic project directories
BIN = bin/
OBJDIR = obj/
SRC = src/
ALL_SRC = $(sort $(dir $(call recwildcard,$(SRC)**/*/)))

# Set the virtual (search) path
VPATH = $(SRC):$(ALL_SRC)

# Determine all object files that we need in order to produce an executable
OBJ = $(addprefix $(OBJDIR),$(notdir $(patsubst %.cpp,%.o,$(call recwildcard,src/,*.cpp))))

# To determine source (.cpp) and header (.hh) dependencies
DEP = $(OBJ:.o=.d)

# Thread model to use
THREAD_MODEL := -pthread

# Libraries to link against
MATH_LIBS := -lm

.PHONY: clean

all: $(OBJDIR) $(BIN) $(BIN)$(EXE)

# To resolve header dependencies
-include $(DEP)

# Create object directory
$(OBJDIR):
	mkdir $@

# Create binary directory
$(BIN):
	mkdir $@

# Link all object code (against additional libraries) to produce the machine code
$(BIN)$(EXE): $(OBJ)
	$(CXX) $(CXX_FLAGS) $^ $(MATH_LIBS) -o $@ $(THREAD_MODEL)
	@echo "done ;-)"

# Compile source code to object code
$(OBJDIR)%.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(CXX_INCL) $(LLVM_FLAGS) -c $< -o $@

# Obivously a hell world program must be in here
hello_world:
	@echo "Hello, World!"

# Clean all auto-generated stuff
clean:
	rm -rf $(BIN)
	rm -rf $(OBJDIR)
