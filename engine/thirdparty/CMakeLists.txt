cmake_minimum_required(VERSION 3.2)

find_package(OpenGL REQUIRED)

######### GLAD #########

add_library(
    glad STATIC

    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/KHR/khrplatform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
)

target_include_directories(
    glad 

    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

######### GLFW #########

add_subdirectory(glfw)

######### IMGUI ######### 

add_library(
    imgui STATIC 

    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h

    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
)

target_include_directories(
    imgui

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/glfw/include
)

target_link_libraries(
    imgui

    PUBLIC glfw
)