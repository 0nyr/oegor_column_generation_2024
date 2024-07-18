# - Try to find CPLEX
#includes
find_path(CPLEX_INCLUDE_DIR
          NAMES ilcplex/cplex.h
          HINTS "${CPLEX_ROOT_DIR}/cplex/include"
          )

find_path(CPLEX_CONCERT_INCLUDE_DIR 
         ilconcert/ilomodel.h
          HINTS "${CPLEX_ROOT_DIR}/concert/include"
          )

 #libraries
if(WIN32) #windows
    message(STATUS "WINDOWS: WIN32...")
    #cplex.lib
    find_library( CPLEX_LIBRARY_RELEASE
                  cplex${CPLEX_VERSION}
                  HINTS "${CPLEX_ROOT_DIR}/cplex/lib/${CPLEX_COMPILER_FOLDER_NAME}/stat_mda"
                  )
    find_library( CPLEX_LIBRARY_DEBUG
                  cplex${CPLEX_VERSION}
                  HINTS "${CPLEX_ROOT_DIR}/cplex/lib/${CPLEX_COMPILER_FOLDER_NAME}/stat_mdd" 
                  )
    set(CPLEX_LIBRARY $<$<CONFIG:Debug>:${CPLEX_LIBRARY_DEBUG}> $<$<CONFIG:Release>:${CPLEX_LIBRARY_RELEASE}> $<$<CONFIG:RelWithDebInfo>:${CPLEX_LIBRARY_RELEASE}>)

    #ilocplex.lib
    find_library( CPLEX_ILO_LIBRARY_RELEASE
                  ilocplex
                  HINTS "${CPLEX_ROOT_DIR}/cplex/lib/${CPLEX_COMPILER_FOLDER_NAME}/stat_mda" 
                  )
    find_library( CPLEX_ILO_LIBRARY_DEBUG
                  ilocplex
                  HINTS "${CPLEX_ROOT_DIR}/cplex/lib/${CPLEX_COMPILER_FOLDER_NAME}/stat_mdd" 
                  )
    set(CPLEX_ILO_LIBRARY $<$<CONFIG:Debug>:${CPLEX_ILO_LIBRARY_DEBUG}> $<$<CONFIG:Release>:${CPLEX_ILO_LIBRARY_RELEASE}> $<$<CONFIG:RelWithDebInfo>:${CPLEX_ILO_LIBRARY_RELEASE}>)

    #concert.lib
    find_library( CPLEX_CONCERT_LIBRARY_RELEASE
                  concert
                  HINTS "${CPLEX_ROOT_DIR}/concert/lib/${CPLEX_COMPILER_FOLDER_NAME}/stat_mda"  
                  )  
    find_library( CPLEX_CONCERT_LIBRARY_DEBUG
                  concert
                  HINTS "${CPLEX_ROOT_DIR}/concert/lib/${CPLEX_COMPILER_FOLDER_NAME}/stat_mdd"  
                  )
    set(CPLEX_CONCERT_LIBRARY $<$<CONFIG:Debug>:${CPLEX_CONCERT_LIBRARY_DEBUG}> $<$<CONFIG:Release>:${CPLEX_CONCERT_LIBRARY_RELEASE}> $<$<CONFIG:RelWithDebInfo>:${CPLEX_CONCERT_LIBRARY_RELEASE}>)
else() #linux
    message(STATUS "LINUX...")
    #cplex.lib
    find_library( CPLEX_LIBRARY 
                  NAMES cplex${CPLEX_VERSION} cplex
                  HINTS "${CPLEX_ROOT_DIR}/cplex/lib/${CPLEX_COMPILER_FOLDER_NAME}/static_pic"
                  )

    #ilocplex.lib
    find_library( CPLEX_ILO_LIBRARY 
                  ilocplex
                  HINTS "${CPLEX_ROOT_DIR}/cplex/lib/${CPLEX_COMPILER_FOLDER_NAME}/static_pic" 
                  )

    #concert.lib
    find_library( CPLEX_CONCERT_LIBRARY 
                  concert
                  HINTS "${CPLEX_ROOT_DIR}/concert/lib/${CPLEX_COMPILER_FOLDER_NAME}/static_pic" 
                  )
endif()

# Apply the standard find package function.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CPLEX DEFAULT_MSG CPLEX_LIBRARY CPLEX_INCLUDE_DIR CPLEX_ILO_LIBRARY CPLEX_CONCERT_LIBRARY CPLEX_CONCERT_INCLUDE_DIR)

# Set the output variables for CPLEX
IF(CPLEX_FOUND)
    SET(CPLEX_INCLUDE_DIRS ${CPLEX_INCLUDE_DIR} ${CPLEX_CONCERT_INCLUDE_DIR})
    SET(CPLEX_LIBRARIES ${CPLEX_CONCERT_LIBRARY} ${CPLEX_ILO_LIBRARY} ${CPLEX_LIBRARY} )
    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        message("I am adding something to libraries")
        SET(CPLEX_LIBRARIES "${CPLEX_LIBRARIES};dl;pthread")
    ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
else()
    message(FATAL_ERROR "CPLEX not found. Please install it; if installed, try defining CPLEX_ROOT_DIR.")
ENDIF(CPLEX_FOUND)

# Mark variable advanced, but inspectable
MARK_AS_ADVANCED(CPLEX_LIBRARY CPLEX_INCLUDE_DIR CPLEX_ILO_LIBRARY CPLEX_CONCERT_INCLUDE_DIR CPLEX_CONCERT_LIBRARY)

# use c++ headers as default
set(CPLEX_COMPILER_FLAGS "-DIL_STD" CACHE STRING "Cplex Compiler Flags")

set(CPLEX_TESTSTR "${CPLEX_ROOT_DIR}/cplex/lib/${CPLEX_COMPILER_FOLDER_NAME}/stat_mda/cplex2210.lib")

find_library(CPLEX_TESTLIB cplex${CPLEX_VERSION} HINTS "${CPLEX_ROOT_DIR}/cplex/lib/${CPLEX_COMPILER_FOLDER_NAME}/stat_mda")

#message(STATUS "WIN32: ${WIN32}")
message(STATUS "CPLEX_INCLUDE_DIR: ${CPLEX_INCLUDE_DIR}")
#message(STATUS "CPLEX_TESTSTR: ${CPLEX_TESTSTR}")
#message(STATUS "CPLEX_TESTLIB: ${CPLEX_TESTLIB}")
message(STATUS "CPLEX_ROOT_DIR: ${CPLEX_ROOT_DIR}")
message(STATUS "CPLEX_VERSION: ${CPLEX_VERSION}")
message(STATUS "CPLEX_LIBRARY: ${CPLEX_LIBRARY}")
message(STATUS "CPLEX_COMPILER_FOLDER_NAME: ${CPLEX_COMPILER_FOLDER_NAME}") 
#message(STATUS "CPLEX_LIBRARY_DEBUG: ${CPLEX_LIBRARY_DEBUG}")
#message(STATUS "CPLEX_LIBRARY_RELEASE: ${CPLEX_LIBRARY_RELEASE}")

