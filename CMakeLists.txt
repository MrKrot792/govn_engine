cmake_minimum_required(VERSION 3.16)
project(Govn_Engine)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/*.c)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS include/*.hpp include/*.h)

add_executable(govnengine ${SRC_FILES})

target_include_directories(govnengine PRIVATE include)

find_package(Vulkan REQUIRED)
target_link_libraries(govnengine Vulkan::Vulkan)

find_package(glfw3 REQUIRED)
target_link_libraries(govnengine glfw)

find_package(glm REQUIRED)
target_include_directories(govnengine PRIVATE ${GLM_INCLUDE_DIRS})
