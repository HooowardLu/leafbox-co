cmake_minimum_required(VERSION 3.20)
project(uco C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(COMMON_SOURCES
    src/coro/co.cc
    src/coro/idle.cc
    src/sched/sched.cc
)

# 架构选择
if(NOT DEFINED ARCH)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
        set(ARCH "arm64")
    else()
        set(ARCH "x64")
    endif()
endif()
message(STATUS "Target ARCH: ${ARCH}")

# 只根据 ARCH 选汇编源文件，不动编译器
if(ARCH STREQUAL "arm64")
    set(ASM_SOURCES src/arch/arm64/co.S)
elseif(ARCH STREQUAL "x64")
    set(ASM_SOURCES src/arch/x64/co.S)
else()
    message(FATAL_ERROR "Unknown ARCH '${ARCH}'. Expected: x64 or arm64")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(third_party/spdlog)

add_library(uco STATIC ${ASM_SOURCES} ${COMMON_SOURCES})
target_include_directories(uco PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(uco PUBLIC spdlog::spdlog)

add_executable(test1 test/test1.cc)
target_link_libraries(test1 PRIVATE uco)