cmake_minimum_required (VERSION 3.10)

project (image_editor)

project (gui_test)

project (gui_test_2)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
# -Wall -Werror")
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_library(glad "external/include/glad/glad.h" "external/src/glad.c")
target_include_directories(glad PUBLIC external/include)

# set(glm_DIR "external/include/glm/cmake/glm/")
# find_package(glm REQUIRED)
# message(STATUS "glm included at ${glm_INCLUDE_DIR}")

include_directories("external/include/glm/")

set(SOURCE_FILES src/app.cpp src/drawing_space.cpp src/image_handler.cpp src/gui_elements.cpp)

set(GUI_SOURCE_FILES src/test_gui.cpp src/image_handler.cpp src/gui_elements.cpp)

set(GUI_2_SOURCE_FILES src/test_gui_2.cpp src/image_handler.cpp src/new_gui_elements.cpp)

add_executable (image_editor ${SOURCE_FILES})

add_executable (gui_test ${GUI_SOURCE_FILES})

add_executable (gui_test_2 ${GUI_2_SOURCE_FILES})

target_include_directories (image_editor PUBLIC include)

target_include_directories (gui_test PUBLIC include)

target_include_directories (gui_test_2 PUBLIC include)

find_package(glfw3 REQUIRED)

target_link_libraries(image_editor glfw)
target_link_libraries(image_editor glad)

target_link_libraries(gui_test glfw)
target_link_libraries(gui_test glad)

target_link_libraries(gui_test_2 glfw)
target_link_libraries(gui_test_2 glad)