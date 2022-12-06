cmake_minimum_required(VERSION 3.2)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(GAME_ENGINE_NAME Engine CACHE INTERNAL "")

add_subdirectory(thirdparty)

file(GLOB_RECURSE GAME_ENGINE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "include/onip/*.hpp")
file(GLOB_RECURSE GAME_ENGINE_SOURCES REALTIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_library(
    ${GAME_ENGINE_NAME} STATIC

    ${GAME_ENGINE_HEADERS}
    ${GAME_ENGINE_SOURCES}
)

target_include_directories(
    ${GAME_ENGINE_NAME}

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm/
)

target_link_libraries(
    ${GAME_ENGINE_NAME}

    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE glfw
    PRIVATE glad
)

if (MSVC)
    if (COMPILE_IN_RELEASE_MODE)
        set(FINAL_ASSETS_DIR ${CMAKE_BINARY_DIR}/project/Release/engine)
    else ()
        set(FINAL_ASSETS_DIR ${CMAKE_BINARY_DIR}/project/Debug/engine)
    endif ()
else ()
    set(FINAL_ASSETS_DIR ${CMAKE_BINARY_DIR}/onip/engine)
endif ()

add_custom_target(default_shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders
    ${FINAL_ASSETS_DIR}/shaders
)
add_custom_target(default_textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures
    ${FINAL_ASSETS_DIR}/texture
)

add_dependencies(
    ${GAME_ENGINE_NAME}

    default_shaders
    default_textures
)