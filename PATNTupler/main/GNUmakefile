ARCH         := $(shell $(ROOTSYS)/bin/root-config --arch)

ROOTCFLAGS    = $(shell ${ROOTSYS}/bin/root-config --cflags)
ROOTLIBS      = $(shell ${ROOTSYS}/bin/root-config --libs)
ROOTGLIBS     = $(shell ${ROOTSYS}/bin/root-config --glibs)
CERNLIB       = /cern/pro/lib

CXX = g++ -std=c++11
LD  = g++ -std=c++11

CXXFLAGS      = -g -O2 -Wall -fPIC -pg
#CXXFLAGS      = -O2 -Wall -fPIC
LDFLAGS       = -g 
SOFLAGS       = -shared

CXX           += -I${PWD}/../interface 
CXXFLAGS      += $(ROOTCFLAGS)

LIBS           = $(ROOTLIBS)

NGLIBS         = $(ROOTGLIBS)
NGLIBS        += -lMinuit -lHtml -lTreePlayer -lEG



GLIBS          = $(filter-out -lNew, $(NGLIBS))
CLIBS       = -L$(CERNLIB) -lpacklib -lmathlib -lkernlib -lpawlib -lg2c -lnsl

OBJDIR		= ${PWD}/../obj
SRCDIR		= ${PWD}/../src
INCDIR		= ${PWD}/../interface
TMPDIR		= ${PWD}/../tmp

#vpath %.cc ../tmp
#vpath %.hh ../include

.SUFFIXES: .cc,.C


# ================================================================================

nTupAnaNMSSM:     mainNMSSM.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
nTupAna:	main.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)

%.o: 	%.cc %.hh
	$(info generic.o target's commands)
	$(CXX) $(CXXFLAGS) -c $< -o $@
${OBJDIR}/libRalParticle.so:	${OBJDIR}/JetStruct.o  ${OBJDIR}/FatJetStruct.o ${OBJDIR}/ElectronStruct.o ${OBJDIR}/MuonStruct.o ${OBJDIR}/PhotonStruct.o ${OBJDIR}/TrackStruct.o ${OBJDIR}/MetStruct.o ${OBJDIR}/ParticleDict.o 
	$(LD) $(SOFLAGS) -o $@ $^ $(GLIBS)	
${OBJDIR}/ParticleDict.o: ${TMPDIR}/ParticleDict.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@
${OBJDIR}/%.o: ${SRCDIR}/%.cc ${INCDIR}/%.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@
#${OBJDIR}/%Struct.o: ${SRCDIR}/%Struct.cc ${INCDIR}/%Struct.hh
#	$(CXX) $(CXXFLAGS) -c $< -o $@
${TMPDIR}/ParticleDict.cc:	${INCDIR}/Particles.hh ${INCDIR}/ntpLinkDef.h
	rm -rf $@  ${TMPDIR}/ParticleDict.h 
	rootcint -f $@ -c $< ${INCDIR}/ntpLinkDef.h

clean:
	# rm -rf nTupAnaNMSSM mainNMSSM.o ${OBJDIR}/* ${TMPDIR}/*
	# rm -rf nTupAna main.o ${OBJDIR}/* ${TMPDIR}/*

	rm -rf nTupAnaNMSSM mainNMSSM.o ${OBJDIR}/*
	rm -rf nTupAna main.o ${OBJDIR}/*