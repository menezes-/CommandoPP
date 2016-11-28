cmake_minimum_required(VERSION 3.6)
project(Commando)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

set(SOURCE_FILES main.cpp src/Entity.cpp include/Entity.hpp src/Joe.cpp include/Joe.hpp include/events/Observer.hpp src/events/EventDispatcher.cpp include/events/EventDispatcher.hpp src/PlayState.cpp include/PlayState.hpp src/Weapon.cpp include/Weapon.hpp src/Bullet.cpp include/Bullet.hpp src/events/GameEvent.cpp include/events/GameEvent.hpp src/events/FireEvent.cpp include/events/FireEvent.hpp include/GameMath.hpp include/events/Event.hpp src/events/CollisionEvent.cpp include/events/CollisionEvent.hpp src/systems/CollisionSystem.cpp include/systems/CollisionSystem.hpp src/EntityManager.cpp include/EntityManager.hpp src/Dude.cpp include/Dude.hpp src/HUD.cpp include/HUD.hpp src/events/MObjectInView.cpp include/events/MObjectInView.hpp src/systems/RespawnSystem.cpp include/systems/RespawnSystem.hpp src/events/PlaySoundEvent.cpp include/events/PlaySoundEvent.hpp src/systems/SoundSystem.cpp include/systems/SoundSystem.hpp include/events/AddEntity.hpp)
add_executable(Commando ${SOURCE_FILES})

set_target_properties(Commando PROPERTIES
                      CXX_STANDARD 11
                      CXX_STANDARD_REQUIRED ON
                      CXX_EXTENSIONS OFF
                      )

target_include_directories(Commando PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# LTO
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-flto GXX_HAS_LTO_FLAG)
message(STATUS "Suporta LTO = ${GXX_HAS_LTO_FLAG}")

target_compile_options(Commando PUBLIC
                       -Wall -Wextra -Wno-unused-function -Wodr -m64
                       $<$<CONFIG:Debug>:-g -O0 >
                       $<$<CONFIG:Release>:-O2 -s -msse3 $<$<BOOL:${GXX_HAS_LTO_FLAG}>:-flto>>
                       $<$<CONFIG:RelWithDebInfo>:-g -O2 -msse3 $<$<BOOL:${GXX_HAS_LTO_FLAG}>:-flto>>
                       )

add_custom_command(TARGET Commando POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:Commando>/resources)


if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Commando PRIVATE -DNDEBUG -DDISABLE_LOGGING)
    set(CGF_DISABLE_LOGGING TRUE CACHE BOOL "Mensagens de debug desabilitadas em modo release" FORCE)
endif ()

add_subdirectory(lib/CGF)

target_link_libraries(Commando CGF)

