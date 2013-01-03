CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#####################################
## Option part
#####################################
option(statemachine_build_samples "Build state machine sample programs." OFF)
option(compile_with_debug "Build library with debug." OFF)

#####################################
## Definition of environment
#####################################
IF( DEFINED ENV{INSTALL_LIB_DIR} )
   SET(INSTALL_LIBS $ENV{INSTALL_LIB_DIR}/ )
ELSE()
   SET(INSTALL_LIBS ${CMAKE_HOME_DIRECTORY}/lib )
ENDIF()
IF( DEFINED ENV{INSTALL_INC_DIR} )
   SET(INSTALL_INCS $ENV{INSTALL_INC_DIR}/ )
ELSE()
   SET(INSTALL_INCS ${CMAKE_HOME_DIRECTORY}/include )
ENDIF()


SET(LIBRARY_OUTPUT_PATH .)
SET(EXECUTABLE_OUTPUT_PATH .)


#####################################
## Project name
#####################################
PROJECT(statemachine)

#####################################
## Include directories 
#####################################
INCLUDE_DIRECTORIES( include lib  ${INSTALL_LIBS} ${INSTALL_INCS})


#####################################
## Folders to be build
#####################################
ADD_SUBDIRECTORY( src lib )
#####################################
## Samples to be build
#####################################
if( logger_build_samples)
	ADD_SUBDIRECTORY( exampleProgram bin )
endif()

#####################################
# Search for needed lib
#####################################
FIND_LIBRARY(JPLoggerStatic REQUIRED)
FIND_LIBRARY(JPSemaphoresStatic REQUIRED)
FIND_LIBRARY( LOGGER_LIBRARY NAMES JPLoggerStatic
                PATHS ${LOCAL_LIBS} ${INSTALL_LIBS} )
message("install_libs:")
message(${INSTALL_LIBS})
SET( LIBS ${LIBS} ${LOGGER_LIBRARY})
FIND_LIBRARY( JPSEMAPHORES_LIBRARY NAMES JPSemaphoresStatic
                PATHS ${LOCAL_LIBS} ${INSTALL_LIBS} )
SET( LIBS ${LIBS} ${JPSEMAPHORES_LIBRARY})
message("Libs:")
message(${LIBS})
message("install_libs:")
message(${INSTALL_LIBS})

#####################################
# Instalation section
#####################################
INSTALL( FILES include/libJPStateMachine.hpp
         DESTINATION ${INSTALL_INCS} )

INSTALL( FILES lib/libJPStateMachineStatic.a
         DESTINATION ${INSTALL_LIBS} )

INSTALL( FILES lib/libJPStateMachine.so
         DESTINATION ${INSTALL_LIBS} )

#####################################
# Others
#####################################
IF( UNIX )
   ADD_CUSTOM_TARGET( distclean @@echo cleaning for source distribution)
  SET(DISTCLEANED
   cmake.depends
   cmake.check_depends
   CMakeCache.txt
   cmake.check_cache
   *.cmake
   Makefile
   core core.*
   gmon.out
   *~
   lib
   CMakeFiles
   bin
   install_*
   docs/docs/
   docs/logger.dox
  )

  ADD_CUSTOM_COMMAND(
    DEPENDS clean
    COMMENT "distribution clean"
    COMMAND rm
    ARGS    -Rf CMakeTmp ${DISTCLEANED}
    TARGET  distclean
  )
ENDIF( UNIX )
if( compile_with_debug )
	set(CMAKE_CXX_FLAGS "-DDEBUG=1")
	set(CMAKE_BUILD_TYPE Debug)
endif()

#####################################
# add a target to generate API documentation with Doxygen
#####################################
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/statemachine.dox.in ${CMAKE_CURRENT_BINARY_DIR}/docs/statemachine.dox @ONLY)
	add_custom_target(docs
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/docs/statemachine.dox
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating State Machine documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)
