ObjSuf        = o
SrcSuf        = cc
ExeSuf        = 
DllSuf        = so
OutPutOpt     = -o
HeadSuf       = h

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)

# Linux with egcs
DEFINES       = -DNO_ORCA_CLASSES
CXX           = g++
CXXFLAGS	= -O -Wall -fPIC $(DEFINES)


LD		= g++
LDFLAGS		= -g -O -Wall -fPIC
SOFLAGS		= -shared

CXXFLAGS	+= $(ROOTCFLAGS)
LIBS		= $(ROOTLIBS)   
GLIBS		= $(ROOTGLIBS)
#------------------------------------------------------------------------------
SOURCES		= $(wildcard src/*.cc)
HEADERS		= $(wildcard interface/*.h)
OBJECTS		= $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
DEPENDS		= $(SOURCES:.$(SrcSuf)=.d)
SOBJECTS	= $(SOURCES:.$(SrcSuf)=.$(DllSuf))


all:  libObjects.so ; mv  libObjects.so ../lib ; 

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(DEPENDS) *Dict.* core 

.SUFFIXES: .$(SrcSuf) .C .o .so


libObjects.so: $(OBJECTS)
	@echo "Building libObjects..."
	$(LD) $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

