# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(GETDecoder)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(
${ROOT_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/source/FRIBDAQFormat
${CMAKE_SOURCE_DIR}/source/GETCore
${CMAKE_SOURCE_DIR}/source/GETFormat
${CMAKE_SOURCE_DIR}/source/GETUtil
)
add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__FRIBDAQFormat
${CMAKE_SOURCE_DIR}/source/FRIBDAQFormat/RingItemHeader.hh
${CMAKE_SOURCE_DIR}/source/FRIBDAQFormat/RingItemBodyHeader.hh
${CMAKE_SOURCE_DIR}/source/FRIBDAQFormat/RingStateChangeItem.hh
${CMAKE_SOURCE_DIR}/source/FRIBDAQFormat/RingPhysicsEventItem.hh
LINKDEF ${CMAKE_SOURCE_DIR}/source/FRIBDAQFormat/libFRIBDAQFormatLinkDef.h)

ROOT_GENERATE_DICTIONARY(G__GETFormat
${CMAKE_SOURCE_DIR}/source/GETFormat/GETBasicFrame.hh
${CMAKE_SOURCE_DIR}/source/GETFormat/GETBasicFrameHeader.hh
${CMAKE_SOURCE_DIR}/source/GETFormat/GETCoboFrame.hh
${CMAKE_SOURCE_DIR}/source/GETFormat/GETHeaderBase.hh
${CMAKE_SOURCE_DIR}/source/GETFormat/GETLayerHeader.hh
${CMAKE_SOURCE_DIR}/source/GETFormat/GETLayeredFrame.hh
${CMAKE_SOURCE_DIR}/source/GETFormat/GETTopologyFrame.hh
${CMAKE_SOURCE_DIR}/source/GETFormat/GETMutantFrame.hh
LINKDEF ${CMAKE_SOURCE_DIR}/source/GETFormat/libGETFormatLinkDef.h)

ROOT_GENERATE_DICTIONARY(G__GETUtil
${CMAKE_SOURCE_DIR}/source/GETUtil/GETFileChecker.hh
${CMAKE_SOURCE_DIR}/source/GETUtil/GETMath.hh
LINKDEF ${CMAKE_SOURCE_DIR}/source/GETUtil/libGETUtilLinkDef.h)

ROOT_GENERATE_DICTIONARY(G__GETCore
${CMAKE_SOURCE_DIR}/source/GETCore/GETDecoder.hh
${CMAKE_SOURCE_DIR}/source/GETCore/GETFrameInfo.hh
LINKDEF ${CMAKE_SOURCE_DIR}/source/GETCore/libGETCoreLinkDef.h)

#---Create a shared library with geneated dictionary
add_library(FRIBDAQFormat SHARED
${CMAKE_SOURCE_DIR}/source/FRIBDAQFormat/RingItemHeader.cc
${CMAKE_SOURCE_DIR}/source/FRIBDAQFormat/RingItemBodyHeader.cc
${CMAKE_SOURCE_DIR}/source/FRIBDAQFormat/RingStateChangeItem.cc
${CMAKE_SOURCE_DIR}/source/FRIBDAQFormat/RingPhysicsEventItem.cc
G__FRIBDAQFormat.cxx
)
target_link_libraries(FRIBDAQFormat ${ROOT_LIBRARIES})

add_library(GETFormat SHARED
${CMAKE_SOURCE_DIR}/source/GETFormat/GETBasicFrame.cc
${CMAKE_SOURCE_DIR}/source/GETFormat/GETBasicFrameHeader.cc
${CMAKE_SOURCE_DIR}/source/GETFormat/GETCoboFrame.cc
${CMAKE_SOURCE_DIR}/source/GETFormat/GETHeaderBase.cc
${CMAKE_SOURCE_DIR}/source/GETFormat/GETLayerHeader.cc
${CMAKE_SOURCE_DIR}/source/GETFormat/GETLayeredFrame.cc
${CMAKE_SOURCE_DIR}/source/GETFormat/GETTopologyFrame.cc
${CMAKE_SOURCE_DIR}/source/GETFormat/GETMutantFrame.cc
G__GETFormat.cxx
)
target_link_libraries(GETFormat ${ROOT_LIBRARIES})

add_library(GETUtil SHARED
${CMAKE_SOURCE_DIR}/source/GETUtil/GETFileChecker.cc
${CMAKE_SOURCE_DIR}/source/GETUtil/GETMath.cc
G__GETUtil.cxx
)
target_link_libraries(GETUtil ${ROOT_LIBRARIES})

add_library(GETCore SHARED
${CMAKE_SOURCE_DIR}/source/GETCore/GETDecoder.cc
${CMAKE_SOURCE_DIR}/source/GETCore/GETFrameInfo.cc
G__GETCore.cxx
)
target_link_libraries(GETCore ${ROOT_LIBRARIES})
