
if(UNIX AND NOT APPLE)
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.20)
      cmake_policy(SET CMP0135 NEW)
    endif()
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    message(STATUS "ccache found: ${CCACHE_PROGRAM}")
else()
    message(STATUS "ccache not found")
endif()
find_program(LLD_PROGRAM lld)
if(LLD_PROGRAM)
    message(STATUS "lld found: ${LLD_PROGRAM}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")
else()
    message(STATUS "lld not found")
endif()