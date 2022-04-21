OMRDIR=omr/
JITB=jitbuilder
LIBJITB=lib$(JITB).a
OMRBUILDDIR=$(OMRDIR)/build
LIBJITBDIR=$(OMRBUILDDIR)/jitbuilder

JITB2=jitbuilder2
LIBJITB2=lib$(JITB2).a

LINK_OPTIONS=-L. -l$(JITB2) -L$(LIBJITBDIR) -l$(JITB)

CORE_OBJECTS = Builder.o \
	       Compilation.o \
	       Compiler.o \
	       Context.o \
	       Extension.o \
	       JB1.o \
	       JB1CodeGenerator.o \
	       JB1MethodBuilder.o \
	       Literal.o \
	       LiteralDictionary.o \
	       Location.o \
	       Loggable.o \
	       Operation.o \
	       Pass.o \
	       SemanticVersion.o \
	       Strategy.o \
	       Symbol.o \
	       SymbolDictionary.o \
	       TextWriter.o \
	       Transformer.o \
	       Type.o \
	       TypeDictionary.o \
	       Value.o \
	       Visitor.o

libjbcore.a : $(CORE_OBJECTS)
	ar -rc libjbcore.a $(CORE_OBJECTS)

libjbcore.so : $(CORE_OBJECTS)
	g++ -shared -fPIC -o libjbcore.so $(CORE_OBJECTS) -L$(LIBJITBDIR) -ljitbuilder

#CXXFLAGS=-O3 -g -std=c++0x -fno-rtti -fPIC -Wwritable-strings
CXXFLAGS=-O0 -g -std=c++0x -fno-rtti -fPIC -Wno-writable-strings -D_XOPEN_SOURCE=0
#CXXFLAGS=-O3 -std=c++0x -fno-rtti -fPIC -Wno-writable-strings -D_XOPEN_SOURCE=0

.cpp.o:
	g++ $(CXXFLAGS) -c $<

JB1CodeGenerator.o: JB1CodeGenerator.cpp JB1CodeGenerator.hpp
	g++ $(CXXFLAGS) -c -I.  -I${OMRDIR}/jitbuilder/x/amd64 -I${OMRDIR}/jitbuilder/x -I${OMRDIR}/jitbuilder -I${OMRDIR}/compiler/x/amd64 -I${OMRDIR}/compiler/x -I${OMRDIR}/compiler -I${OMRDIR}/ -I${OMRDIR}/include_core -I${OMRBUILDDIR} $<

JB1MethodBuilder.o: JB1MethodBuilder.cpp JB1MethodBuilder.hpp
	g++ $(CXXFLAGS) -c -I.  -I${OMRDIR}/jitbuilder/x/amd64 -I${OMRDIR}/jitbuilder/x -I${OMRDIR}/jitbuilder -I${OMRDIR}/compiler/x/amd64 -I${OMRDIR}/compiler/x -I${OMRDIR}/compiler -I${OMRDIR}/ -I${OMRDIR}/include_core -I${OMRBUILDDIR} $<

getomr:
	git clone git@github.com:eclipse/omr.git $(OMRDIR)

omrcfg:
	mkdir -p $(OMRBUILDDIR) && cd $(OMRBUILDDIR) && cmake .. -DOMR_COMPILER=1 -DOMR_JITBUILDER=1 -DCMAKE_BUILD_TYPE=Release

omrcfgdbg:
	mkdir -p $(OMRBUILDDIR) && cd $(OMRBUILDDIR) && cmake .. -DOMR_COMPILER=1 -DOMR_JITBUILDER=1 -DCMAKE_BUILD_TYPE=Debug

omrjb:
	cd $(OMRBUILDDIR) && make jitbuilder

cleanomrcfg:
	rm -rf $(OMRDIR)/build

cleanomr:
	rm -rf $(OMRDIR)

clean:
	rm -f *.o $(LIBJITB2)
