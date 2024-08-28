cmake_minimum_required(VERSION 3.26.3)

project(CmakeTest2)

#All source Files

set(SOURCE_FILES main.cpp filename.cpp glad.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


add_subdirectory(external/glfw)
target_include_directories(${PROJECT_NAME} PUBLIC external/glfw/include)

target_link_directories(${PROJECT_NAME} PRIVATE external/glfw/src)

target_link_libraries(${PROJECT_NAME} glfw)