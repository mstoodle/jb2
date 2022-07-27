OMRDIR=../omr
JITB=jitbuilder
LIBJITB=lib$(JITB).a
OMRBUILDDIR=$(OMRDIR)/build
LIBJITBDIR=$(OMRBUILDDIR)/jitbuilder

GTESTDIR=$(OMRDIR)/third_party/gtest-1.8.0

JITB2=jitbuilder2
LIBJITB2=lib$(JITB2).a

all: testsemver \
	testbase \
	testcompiler

#LINK_OPTIONS=-L. -l$(JITB2) -L$(LIBJITBDIR) -l$(JITB) -lomrGtest
LINK_OPTIONS=-L.. -ljbcore -L../Base -lbase -lpthread -ldl

testsemver: shared_libs main.o SemVerTest.o ../SemanticVersion.o gtest-all.o
	g++ -g -o $@ main.o SemVerTest.o ../SemanticVersion.o gtest-all.o $(LINK_OPTIONS)

testcompiler: shared_libs simple.o testCompiler.o gtest-all.o
	g++ -g -o $@ simple.o testCompiler.o gtest-all.o $(LINK_OPTIONS)

testbase: shared_libs testBase.o gtest-all.o
	g++ -O0 -g -o $@ testBase.o gtest-all.o  $(LINK_OPTIONS)

gtest-all.o: $(GTESTDIR)/src/gtest-all.cc
	g++ -O0 -g -c -o gtest-all.o -I$(GTESTDIR)/include -I$(GTESTDIR) $(GTESTDIR)/src/gtest-all.cc

shared_libs: libjbcore.so libbase.so

libjbcore.so: ../libjbcore.so
	ln -sf ../libjbcore.so

libbase.so: ../Base/libbase.so
	ln -sf ../Base/libbase.so

#CXXFLAGS=-O3 -g -std=c++0x -fno-rtti -fPIC -Wwritable-strings
#CXXFLAGS=-I$(OMRDIR)/third_party/gtest-1.8.0/include
CXXFLAGS=-I$(GTESTDIR)/include \
	 -I./ \
	 -I../ \
	 -O0 -g \
	 -std=c++0x \
	 -fno-rtti \
	 -fPIC \
	 -D_XOPEN_SOURCE=0 \
	 -pthread
	 #-O0 -g \
	 #-Wno-writable-strings \

.cc.o:
	g++ $(CXXFLAGS) -c $<

.cpp.o:
	g++ $(CXXFLAGS) -c $<

clean:
	rm -f libjbcore.so libbase.so testbase testcompiler testsemver *.o
