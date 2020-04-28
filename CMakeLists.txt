cmake_minimum_required(VERSION 3.8)
project(MyRootData)

# Need the Core, I/O and Tree/Player libs
find_package(ROOT REQUIRED RIO Tree TreePlayer)

# For "dictionary" generation, ROOT supplies a CMake extension
include(${ROOT_USE_FILE})
include(${ROOT_DIR}/modules/ROOTNewMacros.cmake)

#include_directories(${PROJECT_SOURCE_DIR})
root_generate_dictionary(
  MyData_dict # This will be the name of the generated C++ file
  MyData.h    # The interfaces for classes to have dictionaries generated
  # Module is the name of the library the dictionary is compiled into
  MODULE MyData
  # Linkdef file is where we list specs for classes/dicts
  LINKDEF "${PROJECT_SOURCE_DIR}/MyData_linkdef.h"
  )

# Make the library
add_library(MyData SHARED MyData.h MyData_dict.cxx)
target_include_directories(MyData PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(MyData PRIVATE ROOT::Core)

# Just a program for now to demo (production cases is to have
# data model classes in dedicated library+dictionary that can be
# shared across the project
# Note we add the generated dictionary source. ROOT/CMake ensure
# this is built for us.
add_executable(MyDataRW main.cxx)
target_link_libraries(MyDataRW PRIVATE MyData ROOT::RIO ROOT::Tree ROOT::TreePlayer)


