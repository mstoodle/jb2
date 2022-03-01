OMRDIR=../../omr
JITB=jitbuilder
LIBJITB=lib$(JITB).a
OMRBUILDDIR=$(OMRDIR)/build
LIBJITBDIR=$(OMRBUILDDIR)/jitbuilder

JITB2=jitbuilder2
LIBJITB2=lib$(JITB2).a

all: testsemver \
	testbase \
	testcompiler

	#testsimple \

#LINK_OPTIONS=-L. -l$(JITB2) -L$(LIBJITBDIR) -l$(JITB) -lomrGtest
LINK_OPTIONS=-L.. -ljbcore -L../Base -lbase -lpthread -ldl

testsemver: main.o SemVerTest.o ../SemanticVersion.o gtest/gtest-all.o
	g++ -g -o $@ main.o SemVerTest.o ../SemanticVersion.o gtest/gtest-all.o $(LINK_OPTIONS)

testcompiler: simple.o testCompiler.o gtest/gtest-all.o
	g++ -g -o $@ $+ $(LINK_OPTIONS)

testbase: testBase.o gtest/gtest-all.o
	g++ -O0 -g -o $@ $+ $(LINK_OPTIONS)

#CXXFLAGS=-O3 -g -std=c++0x -fno-rtti -fPIC -Wwritable-strings
#CXXFLAGS=-I$(OMRDIR)/third_party/gtest-1.8.0/include
CXXFLAGS=-Igtest/include \
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
	rm -f jb2test *.o
