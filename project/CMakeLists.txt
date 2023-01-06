cmake_minimum_required(VERSION 3.2)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# set(COMPILE_IN_RELEASE_MODE CACHE INTERNAL "")
# add_definitions(-DNDEBUG -DCMAKE_BUILT_TYPE=Release)

set(APPLICATION_NAME EngineEditor)

file(GLOB_RECURSE APPLICATION_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "code/src/*.cpp")

add_executable(
    ${APPLICATION_NAME}

    ${CMAKE_SOURCE_DIR}/engine/main.cpp
    ${APPLICATION_SOURCES}
)

target_include_directories(
    ${APPLICATION_NAME}

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code/include
    PUBLIC ${CMAKE_SOURCE_DIR}/engine/include
    PUBLIC ${CMAKE_SOURCE_DIR}/engine/thirdparty
    PUBLIC ${CMAKE_SOURCE_DIR}/engine/thirdparty/glad/include
    PUBLIC ${CMAKE_SOURCE_DIR}/engine/thirdparty/glfw/include
    PUBLIC ${CMAKE_SOURCE_DIR}/engine/thirdparty/glm/
)

target_link_libraries(
    ${APPLICATION_NAME}

    PUBLIC ${GAME_ENGINE_NAME}
)