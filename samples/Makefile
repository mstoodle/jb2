OMRDIR=../omr
JITB=jitbuilder
LIBJITB=lib$(JITB).a
OMRBUILDDIR=$(OMRDIR)/build
LIBJITBDIR=$(OMRBUILDDIR)/jitbuilder

JITB2=jitbuilder2
LIBJITB2=lib$(JITB2).a

all: matmult

#LINK_OPTIONS=-L. -l$(JITB2) -L$(LIBJITBDIR) -l$(JITB) -lomrGtest
LINK_OPTIONS=-L../ -ljbcore -L../Base/ -lbase -lpthread -ldl

#CXXFLAGS=-O3 -g -std=c++0x -fno-rtti -fPIC -Wwritable-strings
#CXXFLAGS=-I$(OMRDIR)/third_party/gtest-1.8.0/include
CXXFLAGS=-I./ \
		-I../ \
		-O0 -g \
		-std=c++0x \
		-fno-rtti \
		-fPIC \
		-D_XOPEN_SOURCE=0 \
		-pthread

	 #-Wno-writable-strings \

matmult: MatMult.cpp MatMult.hpp
	g++ -O0 -o $@ $+ $(CXXFLAGS) $(LINK_OPTIONS)
	ln -sf ../libjbcore.so
	ln -sf ../Base/libbase.so

.cpp.o:
	g++ $(CXXFLAGS) -c $<

clean:
	rm -f matmult MatMult.o
