WANA_DIR      = .
SRC_DIR       = $(WANA_DIR)
INC_DIR       = $(WANA_DIR)

ARCH         := $(shell root-config --arch)

ObjSuf        = o
SrcSuf        = cxx
ExeSuf        =
DllSuf        = so

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --glibs)
ROOTGLIBS    := $(shell root-config --glibs)

# Linux with egcs, gcc 2.9x, gcc 3.x (>= RedHat 5.2)
CXX           = g++

STAR_INC_DIRS = -I../ -I../StRoot -I$(VBASYM_DIR)/.$(STAR_HOST_SYS)/include \
                -I/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/.$(STAR_HOST_SYS)/include/ \
                -I/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/StRoot/ \
                -I/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/StRoot/StJetMaker/tracks/

STAR_LIB_DIRS = -L$(VBASYM_DIR)/.$(STAR_HOST_SYS)/lib \
                -L$(STAR_LIB) \
                -L/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/.$(STAR_HOST_SYS)/LIB \
                -L$(OPTSTAR)/lib

MYSQL_LIB_DIR = -L/usr/lib/mysql
MYSQL_LIB     = -lmysqlclient
UTILS_LIB_DIR = /star/u/smirnovd/root-helper/build/utils
UTILS_INC_DIR = -I/star/u/smirnovd/root-helper/
UTILS_LIB     = -lroot-helper-utils-shared
BOOST_INC_DIR = -I/star/u/smirnovd/usr/local/include/
BOOST_LIB_DIR = /star/u/smirnovd/usr/local/lib/
BOOST_LIBS    = -lboost_program_options

# if you plan to do a whole lot of debugging, remove '-O' switch(es)
#CXXFLAGS      = -O2
CXXFLAGS      = -ggdb -Wall -fPIC $(ROOTCFLAGS)
CXXFLAGS     += -I. -I$(INC_DIR) $(UTILS_INC_DIR) $(STAR_INC_DIRS) -D__ROOT__ -DNEW_DAQ_READER $(BOOST_INC_DIR)

LD            = g++
SOFLAGS       = -shared $(ROOTCFLAGS)

LIBS          = $(STAR_LIBS) -lStVecBosAna $(UTILS_LIB) $(ROOTLIBS) $(BOOST_LIBS) -lTable -lMinuit -lTreePlayer -lGeom -lrt
LIB_DIRS      = -L$(BOOST_LIB_DIR) $(STAR_LIB_DIRS) -L$(UTILS_LIB_DIR)

LDFLAGS       = $(shell root-config --ldflags)
#LDFLAGS      += -Wl,-rpath,.sl53_gcc432/lib:/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/.sl53_gcc432/lib:/afs/rhic.bnl.gov/star/ROOT/5.22.00/.sl53_gcc432/rootdeb/lib,--warn-unresolved-symbols
LDFLAGS      += -Wl,-Bdynamic -Wl,--warn-unresolved-symbols -Wl,-rpath,$(UTILS_LIB_DIR) \
                -Wl,-rpath,$(BOOST_LIB_DIR)

#------------------------------------------------------------------------------

STAR_LIBS     = -lfastjet \
                -lsiscone \
                -lsiscone_spherical \
                -lfastjetplugins \
                -lSt_base \
                -lStChain \
                -lStarRoot \
                -lSt_Tables \
                -lStUtilities \
                -lStTreeMaker \
                -lStIOMaker \
                -lStarClassLibrary \
                -lStTriggerDataMaker \
                -lStBichsel \
                -lStEvent \
                -lStEventUtilities \
                -lStDbLib \
                -lStEmcUtil \
                -lStTofUtil \
                -lStPmdUtil \
                -lStPreEclMaker \
                -lStStrangeMuDstMaker \
                -lStMuDSTMaker \
                -lStDaqLib \
                -lStFtpcSlowSimMaker \
                -lStFtpcTrackMaker \
                -lStFtpcClusterMaker \
                -lStFtpcTrackMaker \
                -lStDAQMaker \
                -lStDetectorDbMaker \
                -lStTpcDb \
                -lStDbUtilities \
                -lStDbBroker \
                -lSt_db_Maker \
                -lStEEmcUtil \
                -lStEEmcDbMaker \
                -lStTriggerFilterMaker \
                -lStTriggerUtilities \
                -lStSpinDbMaker \
                -lStEmcRawMaker \
                -lStEmcADCtoEMaker \
                -lStJetSkimEvent \
                -lStJets \
                -lStSpinDbMaker \
                -lStEmcTriggerMaker \
                -lStTriggerUtilities \
                -lStMCAsymMaker \
                -lStRandomSelector \
                -lStJetEvent \
                -lStJetFinder \
                -lStJetMaker \
                -lStMcEvent \
                -lStMcEventMaker \
                -lStEmcSimulatorMaker \
                -lStEEmcSimulatorMaker \
                -lStEpcMaker \
                -lStSvtClassLibrary

BIN     = stana vbana lumi_calc events_calc
BIN_SRC = $(BIN:=.cc)
BIN_OBJ = $(BIN:=.o)

#------------------------------------------------------------------------------

all: bin

bin: $(BIN)

stana: stana.o
	$(LD) -o $@ stana.o $(LDFLAGS) $(LIB_DIRS) $(MYSQL_LIB_DIR) $(MYSQL_LIB) $(LIBS)
	@echo "$@ done"

vbana: vbana.o
	$(LD) -o $@ vbana.o $(LDFLAGS) $(LIB_DIRS) $(MYSQL_LIB_DIR) $(MYSQL_LIB) $(LIBS)
	@echo "$@ done"

wana: wana.o
	$(LD) -o $@ wana.o $(LDFLAGS) $(LIB_DIRS) $(LIBS)
	@echo "$@ done"

lumi_calc: lumi_calc.o
	$(LD) -o $@ lumi_calc.o $(LDFLAGS) $(LIB_DIRS) $(LIBS)
	@echo "$@ done"

events_calc: events_calc.o
	$(LD) -o $@ events_calc.o $(LDFLAGS)
	@echo "$@ done"

$(SRC_DIR)/%.o : $(SRC_DIR)/%.cxx $(INC_DIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/%.o : $(SRC_DIR)/%.cc $(INC_DIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Removing binaries and supplementary files..."
	rm -f $(BIN) $(BIN_OBJ)

cleanall: clean
	rm -f $(SRC_DIR)/*~
	rm -f $(INC_DIR)/*~

echo-%:
	@echo "$* = $($*)"
