cmake_minimum_required(VERSION 3.20)
project(uco C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ========== 架构选择 ==========
# 用法: cmake -B build -DARCH=x64
#       cmake -B build -DARCH=arm64
# 不指定时自动检测当前平台
if(NOT DEFINED ARCH)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
        set(ARCH "arm64")
    else()
        set(ARCH "x64")
    endif()
endif()
message(STATUS "Target ARCH: ${ARCH}")

# ========== 编译器与标志 ==========
if(ARCH STREQUAL "arm64")
    set(CMAKE_C_COMPILER   aarch64-none-linux-gnu-gcc)
    set(CMAKE_CXX_COMPILER aarch64-none-linux-gnu-g++)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -static")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    set(ASM_SOURCES src/arch/arm64/co.S)
elseif(ARCH STREQUAL "x64")
    set(CMAKE_C_COMPILER   gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(ASM_SOURCES src/arch/x64/co.S)
else()
    message(FATAL_ERROR "Unknown ARCH '${ARCH}'. Expected: x64 or arm64")
endif()

set(COMMON_SOURCES
    src/coro/co.cc
    src/coro/idle.cc
    src/sched/sched.cc
)

# ========== 头文件搜索路径 ==========
include_directories(${CMAKE_SOURCE_DIR}/include)

# ========== 生成静态库 ==========
add_library(uco STATIC ${ASM_SOURCES} ${COMMON_SOURCES})

target_include_directories(uco
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# ========== 生成测试可执行文件 ==========
add_executable(test1 test/test1.cc)
target_link_libraries(test1 PRIVATE uco)

target_include_directories(test1
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# ========== 其它设置 ==========
# 显式标记 .S 为汇编（CMake 3.20+ 通常自动识别，显式设置更保险）
set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE ASM)