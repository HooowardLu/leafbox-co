# Uco

Uco is a lightweight coroutine/scheduler library written in C++ and assembly. It can be built as a static library and linked into your own programs.

## Features

- coroutine abstraction with `Co`
- scheduler-based execution via `Sched::Scheduler`
- static library output for reuse in other projects
- support for x64 and arm64 architecture selection

## Requirements

Before building this project, make sure the following tools are installed:

- CMake 3.20 or newer
- GCC / G++
- Make
- Linux environment

On Ubuntu/Debian you can install them with:

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake
```

## Build the project

Clone the repository:

```bash
git clone https://github.com/<your-user>/<your-repo>.git
cd <your-repo>
```

Configure for x64:

```bash
cmake -S . -B build -DARCH=x64
```

Or configure for arm64:

```bash
cmake -S . -B build -DARCH=arm64
```

Build the library and test program:

```bash
cmake --build build -j$(nproc)
```

After a successful build, you will get:

- static library: `build/libuco.a`
- test executable: `build/test1`

## Project layout

```text
.
├── include/
│   ├── coro/
│   │   ├── co.h
│   │   └── idle.h
│   └── sched/
│       └── sched.h
├── src/
│   ├── coro/
│   │   ├── co.cc
│   │   └── idle.cc
│   ├── sched/
│   │   └── sched.cc
│   └── arch/
│       ├── x64/
│       │   └── co.S
│       └── arm64/
│           └── co.S
├── test/
│   └── test1.cc
├── CMakeLists.txt
├── LICENSE
├── README.md
└── build/
```

## Use the library in your own project

### Method 1: Use the library from the build directory

If you already built the project locally, you can compile a program like this:

```bash
g++ main.cpp \
  -I/path/to/leafbox/include \
  -L/path/to/leafbox/build \
  -luco \
  -o my_app
```

Example:

```bash
g++ demo.cpp \
  -I/home/fish/workspace/leafbox-co/include \
  -L/home/fish/workspace/leafbox-co/build \
  -luco \
  -o demo
```

### Method 2: Use the released package

When you download the GitHub Release artifact, it usually contains:

```text
release/
├── include/
│   ├── coro/
│   │   ├── co.h
│   │   └── idle.h
│   └── sched/
│       └── sched.h
└── lib/
    └── libuco.a
```

Then compile with:

```bash
g++ main.cpp \
  -Irelease/include \
  -Lrelease/lib \
  -luco \
  -o my_app
```

## Example usage

Here is a minimal example using the coroutine and scheduler API:

```cpp
#include <iostream>
#include "coro/co.h"
#include "sched/sched.h"

using namespace Sched;

void hello() {
    std::cout << "hello from coroutine" << std::endl;
    scheduler.yieldCo();
    std::cout << "resumed" << std::endl;
    scheduler.returnCo();
}

int main() {
    Co *c = new Co("hello_co", hello);
    scheduler.addCo(c);
    scheduler.schedStart();
    return 0;
}
```

Compile it with:

```bash
g++ demo.cpp \
  -I/path/to/leafbox/include \
  -L/path/to/leafbox/build \
  -luco \
  -o demo
```

## API overview

### `Co`

`Co` is the coroutine object.

```cpp
Co(std::string name, CoroutineFunction co_fn);
```

The coroutine function type is:

```cpp
typedef void (*CoroutineFunction)();
```

### `Sched::Scheduler`

The scheduler exposes functions such as:

```cpp
scheduler.addCo(co);
scheduler.yieldCo();
scheduler.returnCo();
scheduler.wakeupCo("name");
scheduler.schedStart();
```

## Running the sample test

This repository includes a sample program under `test/test1.cc`.

To run it:

```bash
cmake -S . -B build -DARCH=x64
cmake --build build
./build/test1
```

## Notes

- The project builds a static library (`.a`) rather than a shared library (`.so`).
- You need to link the library and include the public headers in your own project.
- If you cross-compile for ARM64, set `-DARCH=arm64` when configuring CMake.

## License

This project is licensed under the MIT license. See the `LICENSE` file for details.
