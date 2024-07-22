cmake_minimum_required (VERSION 3.8)

project ("CG_BinPacking")
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

set (IL_STD TRUE)

#####
# This has to be specified!
# Where to find CPLEX
#####
set(CPLEX_VERSION 2210)
set(CPLEX_ROOT_DIR "/home/onyr/cplex2210/CPLEX_Studio221") # default is generally "/opt/ibm/ILOG/CPLEX_Studio221"
# Win
# set(CPLEX_COMPILER_FOLDER_NAME x64_windows_msvc14)
# Linux
set(CPLEX_COMPILER_FOLDER_NAME x86-64_linux)
#####
# End: This has to be specified!
#####

# Add source to this project's executable.
add_executable (${CMAKE_PROJECT_NAME} 
"main.cpp" 
#
"bp_instance.cpp" 
"bp_rmp.cpp" 
"bp_pricing_mip.cpp" 
)

include(FindCPLEX.cmake)
find_package(CPLEX)

include_directories(${CPLEX_INCLUDE_DIRS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} ${CPLEX_LIBRARIES})

# TODO: Add tests and install targets if needed.