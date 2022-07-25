COREDIR=..
CORE=core
LIBCORE=lib$(CORE).so

BASEDIR=../Base
BASE=base
LIBBASE=lib$(BASE).so

VM=vm
LIBVM=lib$(VM).so

all: $(LIBVM)

VM_OBJECTS = BytecodeBuilder.o \
             VMExtension.o \
             VirtualMachineState.o \
             VirtualMachineRegister.o \
             VirtualMachineRegisterInStruct.o \
             VirtualMachineOperandStack.o
             #VirtualMachineOperandArray.o

$(LIBVM) : $(VM_OBJECTS)
	g++ -shared -FPIC -o $(LIBVM) $(VM_OBJECTS) -L$(BASE_DIR) -l$(LIBBASE) -L$(CORE_DIR) -l$(LIBCORE)

#CXXFLAGS=-O2 -I./ -I../ -std=c++0x -fno-rtti -fPIC -Wno-writable-strings -D_XOPEN_SOURCE=0
CXXFLAGS=-O0 -g -I./ -I../ -std=c++0x -fno-rtti -fPIC -Wno-writable-strings -D_XOPEN_SOURCE=0

.cpp.o:
	g++ $(CXXFLAGS) -c $<

clean:
	rm -f $(LIBVM) *.o
