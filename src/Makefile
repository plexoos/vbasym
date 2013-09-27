WANA_DIR      = .
LIB_DIR       = $(WANA_DIR)
SRC_DIR       = $(WANA_DIR)
INC_DIR       = $(WANA_DIR)
BIN_DIR       = $(WANA_DIR)

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

STAR_INC_DIRS = -I../ -I../StRoot -I../.sl53_gcc432/include -I/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/StRoot/ \
                -I/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/.sl53_gcc432/include/ \
                -I/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/StRoot/StJetMaker/tracks/ \
                -I../.sl53_gcc432/obj/StRoot

STAR_LIB_DIRS = -L../.sl53_gcc432/lib -L/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/.sl53_gcc432/lib \
                -L/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/.sl53_gcc432/LIB

MYSQL_LIB_DIR = -L/usr/lib/mysql
MYSQL_LIB     = -lmysqlclient
UTILS_LIB_DIR = -L/star/u/smirnovd/root-helper/build/utils
UTILS_INC_DIR = -I/star/u/smirnovd/root-helper/
UTILS_LIB     = -lroot-helper-utils

# if you plan to do a whole lot of debugging, remove '-O' switch(es)
#CXXFLAGS      = -O2
CXXFLAGS      = -ggdb -g -fno-inline -Wall -fPIC $(ROOTCFLAGS)
CXXFLAGS     += -I. -I$(INC_DIR) $(UTILS_INC_DIR) $(STAR_INC_DIRS) -D__ROOT__ -DNEW_DAQ_READER

CFLAGS        = -g -O -I. -I$(INC_DIR)
LD            = g++
#LDFLAGS       = -g
LDFLAGS       =
SOFLAGS       = -shared $(ROOTCFLAGS)

LIBS          = $(ROOTLIBS) -lTable -lMinuit -lTreePlayer -lGeom -lrt

LDFLAGS      += $(shell root-config --ldflags)
LDFLAGS      += -L$(OPTSTAR)/lib
#LDFLAGS      += -Wl,-rpath,.sl53_gcc432/lib:/afs/rhic.bnl.gov/star/packages/$(STAR_VERSION)/.sl53_gcc432/lib:/afs/rhic.bnl.gov/star/ROOT/5.22.00/.sl53_gcc432/rootdeb/lib,--warn-unresolved-symbols
LDFLAGS      += -Wl,-Bdynamic -Wl,--warn-unresolved-symbols -Wl,-rpath,/star/u/smirnovd/root-helper/utils

#------------------------------------------------------------------------------

WANA_LIB      = $(LIB_DIR)/libwana.so

STAR_LIBS     = -lfastjet -lCDFConesPlugin -lSt_base \
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
                -lStVecBosAna


WANA_DICT_SRC = $(SRC_DIR)/wana_dict.cxx
WANA_DICT_HDR = $(WANA_DICT_SRC:.cxx=.h)
WANA_DICT_OBJ = $(WANA_DICT_SRC:.cxx=.o)
WANA_DICT     = $(WANA_DICT_SRC) $(WANA_DICT_HDR) $(WANA_DICT_OBJ)

BIN     = stana vbana lumi_calc events_calc
BIN_SRC = $(BIN:=.cc)
BIN_OBJ = $(BIN:=.o)

HDRS       = $(filter-out $(INC_DIR)/MyLinkDef%, $(wildcard $(INC_DIR)/*.h) )
SRCS_CXX   = $(filter-out $(WANA_DICT_SRC), $(wildcard $(SRC_DIR)/*.cxx))
OBJS       = $(SRCS_CXX:.cxx=.o)

#------------------------------------------------------------------------------

all: lib bin

lib: $(WANA_LIB)

bin: $(BIN)

wana: $(WANA_LIB) wana.o
	$(LD) $(LDFLAGS) $(WANA_LIB) wana.o -o $@
	@echo "$@ done"

stana: stana.o
	$(LD) $(LDFLAGS) $(STAR_LIB_DIRS) $(STAR_LIBS) $(MYSQL_LIB_DIR) $(MYSQL_LIB) $(UTILS_LIB_DIR) $(UTILS_LIB) $(LIBS) stana.o -o $@
	@echo "$@ done"

vbana: vbana.o
	$(LD) $(LDFLAGS) $(STAR_LIB_DIRS) $(STAR_LIBS) $(UTILS_LIB_DIR) $(UTILS_LIB) $(LIBS) vbana.o -o $@
	@echo "$@ done"

lumi_calc: lumi_calc.o
	$(LD) $(LDFLAGS) $(STAR_LIB_DIRS) $(STAR_LIBS) $(UTILS_LIB_DIR) $(UTILS_LIB) $(LIBS) lumi_calc.o -o $@
	@echo "$@ done"

events_calc: events_calc.o
	$(LD) $(LDFLAGS) $(STAR_LIB_DIRS) $(STAR_LIBS) $(UTILSLIBDIR) $(UTILSLIB) $(LIBS) events_calc.o -o $@
	@echo "$@ done"

$(WANA_LIB): $(OBJS) $(WANA_DICT_OBJ)
	$(LD) $(SOFLAGS) $(OBJS) $(WANA_DICT_OBJ) $(LIBS) -o $(WANA_LIB)
	@echo "$@ done"

$(WANA_DICT_SRC): $(HDRS) $(INC_DIR)/MyLinkDef.h
	@echo "Generating dictionary $(WANA_DICT_SRC)..."
	@echo rootcint -f $(WANA_DICT_SRC) -c -I$(INC_DIR) $^
	@rootcint -f $(WANA_DICT_SRC) -c -I$(INC_DIR) $^

$(WANA_DICT_OBJ) : $(WANA_DICT_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/%.o : $(SRC_DIR)/%.cxx $(INC_DIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/%.o : $(SRC_DIR)/%.cc $(INC_DIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "removing BIN and supplementary files..."
	rm -f $(BIN) $(BIN_OBJ)
	rm -f $(OBJS)
	rm -f $(WANA_LIB)
	rm -f $(WANA_DICT)

cleanall: clean
	#@echo "removing executables in bin/..."
	rm -f $(SRC_DIR)/*~
	rm -f $(INC_DIR)/*~

echoenv:
	@echo ""

echo-%:
	@echo "$* = $($*)"
