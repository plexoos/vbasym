#
# Usage:
#
# mkdir build
# cd build
# cmake ..
# make
#


CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(VBASYM)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

OPTION(FORCE_32BIT "compile 32-bit application" ON)
OPTION(Boost_NO_BOOST_CMAKE "Due to bug in boost cmake files in Scientific Linux" ON)

IF(FORCE_32BIT)
   SET_PROPERTY(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS OFF)
ENDIF()

#INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/Site-Config.cmake)

# Find boost headers and library locations
set(Boost_USE_MULTITHREADED OFF)
find_package( Boost 1.41.0 COMPONENTS program_options )
if(NOT Boost_FOUND)
   message(FATAL_ERROR "Fatal error: Boost (version >= 1.41.0) not found in standard location")
endif()


find_package(MySQL)
find_package(ROOT)

EXECUTE_PROCESS( COMMAND git submodule init WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} )
EXECUTE_PROCESS( COMMAND git submodule update WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} )

ADD_SUBDIRECTORY(contrib/root-helper)

include_directories(
   ${Boost_INCLUDE_DIRS}
   ${ROOT_INCLUDE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/src
   ${CMAKE_CURRENT_SOURCE_DIR}
   $ENV{VBASYM_DIR}/StRoot
   /afs/rhic.bnl.gov/star/packages/$ENV{STAR_VERSION}/.$ENV{STAR_HOST_SYS}/include
   /afs/rhic.bnl.gov/star/packages/$ENV{STAR_VERSION}/StRoot
   /afs/rhic.bnl.gov/star/packages/$ENV{STAR_VERSION}/StRoot/StJetMaker/tracks
   ${ROOT_HELPER_DIR}
)

link_directories(
   ${Boost_LIBRARY_DIRS}
   ${ROOT_LIBRARY_DIR}
   $ENV{VBASYM_DIR}/.$ENV{STAR_HOST_SYS}/lib
   $ENV{STAR_LIB}
   /afs/rhic.bnl.gov/star/packages/$ENV{STAR_VERSION}/.$ENV{STAR_HOST_SYS}/LIB
   $ENV{OPTSTAR}/lib
)

set( STAR_LIBRARIES
fastjet
siscone
siscone_spherical
fastjetplugins
St_base
StChain
StarRoot
St_Tables
StUtilities
StTreeMaker
StIOMaker
StarClassLibrary
StTriggerDataMaker
StBichsel
StEvent
StEventUtilities
StDbLib
StEmcUtil
StTofUtil
StPmdUtil
StPreEclMaker
StStrangeMuDstMaker
StMuDSTMaker
StDaqLib
StFtpcSlowSimMaker
StFtpcTrackMaker
StFtpcClusterMaker
StFtpcTrackMaker
StDAQMaker
StDetectorDbMaker
StTpcDb
StDbUtilities
StDbBroker
St_db_Maker
StEEmcUtil
StEEmcDbMaker
StTriggerFilterMaker
StTriggerUtilities
StSpinDbMaker
StEmcRawMaker
StEmcADCtoEMaker
StJetSkimEvent
StJets
StSpinDbMaker
StEmcTriggerMaker
StTriggerUtilities
StMCAsymMaker
StRandomSelector
StJetEvent
StJetFinder
StJetMaker
StMcEvent
StMcEventMaker
StEmcSimulatorMaker
StEEmcSimulatorMaker
StEpcMaker
StSvtClassLibrary
StVecBosAna
)


add_executable( stana src/stana.cc )
target_link_libraries( stana ${Boost_LIBRARIES} ${STAR_LIBRARIES} ${MYSQL_LIBRARY} root-helper-utils-shared ${ROOT_LIBRARIES} Table Geom pthread)

add_executable( vbana src/vbana.cc )
target_link_libraries( vbana ${Boost_LIBRARIES} ${STAR_LIBRARIES} ${MYSQL_LIBRARY} root-helper-utils-shared ${ROOT_LIBRARIES} Table Geom pthread)

add_executable( lumi_calc src/lumi_calc.cc )
target_link_libraries( lumi_calc root-helper-utils-shared ${ROOT_LIBRARIES} pthread)

add_executable( events_calc src/events_calc.cc )

IF(FORCE_32BIT)
   SET_TARGET_PROPERTIES(stana PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32 -Wl,--warn-unresolved-symbols")
   SET_TARGET_PROPERTIES(vbana PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32 -Wl,--warn-unresolved-symbols")
   SET_TARGET_PROPERTIES(lumi_calc PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
   SET_TARGET_PROPERTIES(events_calc PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
ENDIF()
