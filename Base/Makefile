JBCORE=jbcore
LIBJBCORE=lib$(JBCORE).a

BASE=base
LIBBASE=lib$(BASE).so

all: $(LIBBASE)

BASE_OBJECTS = BaseExtension.o \
	       BaseSymbols.o \
	       BaseTypes.o \
	       ConstOperations.o \
	       ControlOperations.o \
               Function.o \
               FunctionCompilation.o \
	       MemoryOperations.o \
	       NativeCallableContext.o

#	       BaseOperations.o \

$(LIBBASE) : $(BASE_OBJECTS)
	g++ -shared -FPIC -o $(LIBBASE) $(BASE_OBJECTS) -L../ -ljbcore

#CXXFLAGS=-O2 -I./ -I../ -std=c++0x -fno-rtti -fPIC -Wno-writable-strings -D_XOPEN_SOURCE=0
CXXFLAGS=-O0 -g -I./ -I../ -std=c++0x -fno-rtti -fPIC -Wno-writable-strings -D_XOPEN_SOURCE=0

.cpp.o:
	g++ $(CXXFLAGS) -c $<

clean:
	rm -f $(LIBBASE) *.o
