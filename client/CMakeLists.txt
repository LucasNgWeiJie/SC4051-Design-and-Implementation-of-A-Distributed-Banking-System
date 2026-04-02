cmake_minimum_required(VERSION 3.10)
project(DistributedBankingClient CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(client_app ${SRC_FILES})

# Compiler-specific warning flags
if(MSVC)
    # MSVC: /W4 is good default
    target_compile_options(client_app PRIVATE /W4)
else()
    # GCC/Clang
    target_compile_options(client_app PRIVATE -Wall -Wextra)
endif()

# Windows Winsock library
if(WIN32)
    target_link_libraries(client_app PRIVATE ws2_32)
endif()