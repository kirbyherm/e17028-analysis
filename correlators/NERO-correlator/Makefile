.EXPORT_ALL_VARIABLES:

.PHONY: clean all

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTINC      := -I$(shell root-config --incdir)

CPP             = g++
CFLAGS		= -pedantic -Wall -Wno-long-long -g $(ROOTCFLAGS) -fPIC #-O3
DFLAGS		= -Wall -Wno-long-long -g $(ROOTCFLAGS) -fPIC #-O3

BASELIBS = -lm $(ROOTLIBS) $(ROOTGLIBS) -L$(LIB_DIR)
ALLIBS  =  $(BASELIBS) -lCommandLineInterface 
LIBS 		= $(ALLIBS)

LFLAGS		= -g -fPIC
#LFLAGS		= -g -fPIC -m32


#O_FILES = UnpackedEvent.o \
#	Calibration.o \
#	Settings.o \
#	Gretina.o \
#	RunInfo.o \
#	RawHistograms.o \
#	CalHistograms.o
#
#RO_FILES = S800.o \
#	S800Calc.o \
#	Gretina.o \
#	GretinaCalc.o \
#	RunInfo.o \
#	Trace.o \
#	Mode3Calc.o \
#	Scaler.o

all: libs  Misc
	echo Done

libs: libCommandLineInterface.so 
	echo Done

Misc: e14041t
	echo Done

e14041t: e14041t.cc
	$(CPP) $(CFLAGS) $(INCLUDES) e14041t.cc $(LIBS) $(O_FILES) -o $@

docs: *.cc *.hh *.h
	doxygen doxy-config

%.o: %.cc %.hh
	@echo Default .o rule
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

lib%.so: %.o
	$(CPP) $(LFLAGS) -shared -Wl,-soname,$@ -o $@ $^ $(BASELIBS) -lc
	cp $@ $(LIB_DIR)/$@

CommandLineInterface.o: CommandLineInterface.cc CommandLineInterface.hh libFit.so
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm  -f $(BIN_DIR)/* $(LIB_DIR)/* *.o *Dictionary.cc *Dictionary.h
	cd hfc; make clean

tar:
	@echo "creating zipped tar-ball ... "
	tar -chvzf GUnpack.tar.gz Makefile *LinkDef.h \
	*.hh *.cc *defs.h crmat.LINUX

