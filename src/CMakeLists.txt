
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET( lib_srcs states.cpp statemachine.cpp events.cpp exceptions.cpp )

ADD_LIBRARY( JPStateMachineStatic STATIC ${lib_srcs})
ADD_LIBRARY( JPStateMachine SHARED ${lib_srcs})

