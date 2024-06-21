cmake_minimum_required(VERSION 3.24)

cmake_policy(SET CMP0091 NEW)

project(StateMachine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (VCPKG_TARGET_TRIPLET MATCHES "-static$")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

add_library(StateMachine INTERFACE)
target_include_directories(StateMachine INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_executable(Example1 examples/Example1.cpp)
target_link_libraries(Example1 PRIVATE StateMachine)

add_executable(Example2 examples/Example2.cpp)
target_link_libraries(Example2 PRIVATE StateMachine)

add_executable(Example3 examples/Example3.cpp)
target_link_libraries(Example3 PRIVATE StateMachine)

add_executable(Example4 examples/Example4.cpp)
target_link_libraries(Example4 PRIVATE StateMachine)
