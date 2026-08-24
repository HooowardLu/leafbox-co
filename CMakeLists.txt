cmake_minimum_required(VERSION 3.20)
project(coroutine_test C CXX ASM)

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
    set(ASM_SOURCES arch/arm64/co.S)

elseif(ARCH STREQUAL "x64")
    # 原 Makefile 用 g++ 编译所有文件，这里分离 C/C++ 编译器更规范
    set(CMAKE_C_COMPILER   gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_CXX_STANDARD 17)
    set(ASM_SOURCES arch/x64/co.S)

else()
    message(FATAL_ERROR "Unknown ARCH '${ARCH}'. Expected: x64 or arm64")
endif()

set(COMMON_SOURCES 
    coro/co.cc
    sched/sched.cc
)

# ========== 头文件搜索路径 (-I.) ==========
include_directories(${CMAKE_SOURCE_DIR})

# ========== 生成可执行文件 ==========
add_executable(test1 ${ASM_SOURCES} ${COMMON_SOURCES} test/test1.cc)

# ========== 保持与原 Makefile 严格一致 ==========
# 原 Makefile 中 x64 的 CC=g++，意味着 test1.c 实际按 C++ 编译
# 如果你需要完全还原这个行为，取消下面这行的注释：
# set_source_files_properties(test1.c test2.c PROPERTIES LANGUAGE CXX)

# 显式标记 .S 为汇编（CMake 3.20+ 通常自动识别，显式设置更保险）
set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE ASM)