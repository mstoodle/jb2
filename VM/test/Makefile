COREDIR=../..
CORE=jbcore
LIBCORE=lib$(CORE).so

BASEDIR=../../Base
BASE=base
LIBBASE=lib$(BASE).so

VMDIR=../../VM
VM=vm
LIBVM=lib$(VM).so

all: vmregister operandstacktest

vmregister: VMRegister.o $(LIBCORE) $(LIBBASE) $(LIBVM)
	g++ -FPIC -o vmregister VMRegister.o -L$(VMDIR) -l$(VM) -L$(BASEDIR) -l$(BASE) -L$(COREDIR) -l$(CORE) -ldl

operandstacktest: OperandStackTests.o $(LIBCORE) $(LIBBASE) $(LIBVM)
	g++ -FPIC -o operandstacktest OperandStackTests.o -L$(VMDIR) -l$(VM) -L$(BASEDIR) -l$(BASE) -L$(COREDIR) -l$(CORE) -ldl

$(LIBVM): $(VMDIR)/$(LIBVM)
	cp $(VMDIR)/$(LIBVM) $(LIBVM)

$(LIBBASE): $(BASEDIR)/$(LIBBASE)
	cp $(BASEDIR)/$(LIBBASE) $(LIBBASE)

$(LIBCORE): $(COREDIR)/$(LIBCORE)
	cp $(COREDIR)/$(LIBCORE) $(LIBCORE)

#CXXFLAGS=-O2 -I./ -I../ -std=c++0x -fno-rtti -fPIC -Wno-writable-strings -D_XOPEN_SOURCE=0
CXXFLAGS=-O0 -g -I../../ -I./ -std=c++0x -fno-rtti -fPIC -Wno-writable-strings -D_XOPEN_SOURCE=0

.cpp.o:
	g++ $(CXXFLAGS) -c $<

clean:
	rm -f $(LIBVM) $(LIBBASE) $(LIBCORE) vmregister *.o
