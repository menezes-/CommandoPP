cmake_minimum_required(VERSION 3.6)
project(Commando)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

set(SOURCE_FILES main.cpp src/Entity.cpp include/Entity.hpp src/Joe.cpp include/Joe.hpp include/Observer.hpp src/EventDispatcher.cpp include/EventDispatcher.hpp src/PlayState.cpp include/PlayState.hpp include/utils.hpp)
add_executable(Commando ${SOURCE_FILES})

# LTO
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-flto GXX_HAS_LTO_FLAG)
message(STATUS "Suporta LTO = ${GXX_HAS_LTO_FLAG}")

target_compile_options(Commando PRIVATE
                       -std=c++11 -Wall -Wextra -Wno-unused-function
                       $<$<CONFIG:Debug>:-g -O0>
                       $<$<CONFIG:Release>:-g0 -O2 -s $<$<BOOL:${GXX_HAS_LTO_FLAG}>:-flto>>)

add_custom_command(TARGET Commando POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:Commando>/resources)

add_subdirectory(lib/CGF)

target_link_libraries(Commando CGF)

