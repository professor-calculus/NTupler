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
SRCMACDIR	= ${PWD}/../src_macros
INCDIR		= ${PWD}/../interface
TMPDIR		= ${PWD}/../tmp

#vpath %.cc ../tmp
#vpath %.hh ../include

.SUFFIXES: .cc,.C


# ================================================================================

# plotting_exampleOfHowToUsePlottingClasses: plotting_exampleOfHowToUsePlottingClasses.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/Plotter.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
# 	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
# plotting_exampleOfHowToUse2DPlottingClasses: plotting_exampleOfHowToUse2DPlottingClasses.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/Plotter.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
# 	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
plotting_checkTtbarHtStitching: plotting_checkTtbarHtStitching.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/Plotter.o ${SRCMACDIR}/MassRegionCuts.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
plotting_dbtRatio: plotting_dbtRatio.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/Plotter.o ${SRCMACDIR}/MassRegionCuts.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
plotting_compareDifferentSamples: plotting_compareDifferentSamples.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/Plotter.o ${SRCMACDIR}/MassRegionCuts.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
plotting_compareDifferentCuts: plotting_compareDifferentCuts.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/Plotter.o ${SRCMACDIR}/MassRegionCuts.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
plotting_2dMassDistributionsAndCount: plotting_2dMassDistributionsAndCount.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/Plotter.o ${SRCMACDIR}/MassRegionCuts.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
plotting_compare2D: plotting_compare2D.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/Plotter.o ${SRCMACDIR}/MassRegionCuts.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
plotting_investigateEventSelectionCuts: plotting_investigateEventSelectionCuts.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/Plotter.o ${SRCMACDIR}/MassRegionCuts.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
table_investigateEventSelectionCuts: table_investigateEventSelectionCuts.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/MassRegionCuts.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
plotting_shapeStudies2D: plotting_shapeStudies2D.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/Plotter.o ${SRCMACDIR}/MassRegionCuts.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)
table_dbtRegionSignalContamination: table_dbtRegionSignalContamination.o ${SRCMACDIR}/PlotEntry.o ${SRCMACDIR}/PlotEntry2D.o ${SRCMACDIR}/Plotter.o ${SRCMACDIR}/MassRegionCuts.o ${OBJDIR}/NtpReader.o ${OBJDIR}/EventInfo.o ${OBJDIR}/GoodLumiChecker.o ${OBJDIR}/libRalParticle.so
	$(LD) $(LDFLAGS) -o $@ $^ $(GLIBS)

%.o: 	%.cc %.hh
	$(info generic.o target's commands)
	$(CXX) $(CXXFLAGS) -c $< -o $@
${OBJDIR}/libRalParticle.so:	${OBJDIR}/JetStruct.o  ${OBJDIR}/FatJetStruct.o ${OBJDIR}/ElectronStruct.o ${OBJDIR}/MuonStruct.o ${OBJDIR}/MetStruct.o ${OBJDIR}/ParticleDict.o 
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
	rm -rf plotting_checkTtbarHtStitching plotting_checkTtbarHtStitching.o ${OBJDIR}/* ${TMPDIR}/*
	rm -rf plotting_dbtRatio plotting_dbtRatio.o ${OBJDIR}/* ${TMPDIR}/*
	rm -rf plotting_compareDifferentSamples plotting_compareDifferentSamples.o ${OBJDIR}/* ${TMPDIR}/*
	rm -rf plotting_compareDifferentCuts plotting_compareDifferentCuts.o ${OBJDIR}/* ${TMPDIR}/*
	rm -rf plotting_2dMassDistributionsAndCount plotting_2dMassDistributionsAndCount.o ${OBJDIR}/* ${TMPDIR}/*
	rm -rf plotting_compare2D plotting_compare2D.o ${OBJDIR}/* ${TMPDIR}/*
	rm -rf plotting_investigateEventSelectionCuts plotting_investigateEventSelectionCuts.o ${OBJDIR}/* ${TMPDIR}/*
	rm -rf table_investigateEventSelectionCuts table_investigateEventSelectionCuts.o ${OBJDIR}/* ${TMPDIR}/*
	rm -rf plotting_shapeStudies2D plotting_shapeStudies2D.o ${OBJDIR}/* ${TMPDIR}/*
	rm -rf table_dbtRegionSignalContamination table_dbtRegionSignalContamination.o ${OBJDIR}/* ${TMPDIR}/*