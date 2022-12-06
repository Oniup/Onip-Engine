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
# ...