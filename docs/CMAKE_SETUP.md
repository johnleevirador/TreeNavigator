# CMake Setup Guide

This guide explains how to integrate TreeNavigator into your C++ project using CMake.

Unlike manual compilation, CMake allows you to define your project's build configuration once and automatically handles compiling and linking the required source files.

This guide uses `add_subdirectory()` to integrate TreeNavigator into your application.

## Requirements

Before getting started, make sure you have:

- A C++ compiler, such as GCC/G++.
- [CMake](https://cmake.org/download/) installed.
- A build tool, such as [Ninja](https://ninja-build.org/) or Make.
- TreeNavigator downloaded or cloned into your project.

## 1. Project Structure

First, place the TreeNavigator directory inside your application project.

Your directory should look like this:

```text
MyApplication/
│
├── TreeNavigator/
│   ├── CMakeLists.txt
│   ├── include/
│   │   └── TreeNavigator.h
│   └── src/
│       └── TreeNavigator.cpp
│
├── src/
│   └── main.cpp
│
└── CMakeLists.txt
```

Your application and TreeNavigator each have their own `CMakeLists.txt`.

- TreeNavigator's `CMakeLists.txt` defines how the library is built.
- Your application's `CMakeLists.txt` defines how your application uses the library.

## 2. Include the Library

Inside your application's `main.cpp`, include the public header:

```cpp
#include <TreeNavigator.h>
```

You don't need to specify the library's relative directory in the include statement. CMake handles the include directory through the TreeNavigator target.

## 3. Configure Your Application's CMakeLists.txt

Inside the root directory of your application, create or edit `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)

project(MyApplication LANGUAGES CXX)

# Include TreeNavigator as a subdirectory
add_subdirectory(TreeNavigator)

# Create the application executable
add_executable(MyApplication
    src/main.cpp
)

# Link TreeNavigator to the application
target_link_libraries(MyApplication PRIVATE
    TreeNavigator::TreeNavigator
)
```

### Understanding the configuration

**`add_subdirectory(TreeNavigator)`**

Tells CMake to process TreeNavigator's own `CMakeLists.txt`.

This creates the TreeNavigator library target and makes it available to your application.

**`add_executable(MyApplication src/main.cpp)`**

Creates an executable target from your application's source files.

**`target_link_libraries(MyApplication PRIVATE TreeNavigator::TreeNavigator)`**

Connects TreeNavigator to your application.

Through this target, CMake automatically handles the library's implementation files and public header search directory.

You don't need to manually add `TreeNavigator.cpp` or specify its include directory.

## 4. Configure the Project

Open a terminal in your `MyApplication` directory.

Run:

```bash
cmake -S . -B build -G Ninja
```

This tells CMake to:

- Use the current directory as the source directory.
- Generate build files inside the `build/` directory.
- Use Ninja as the build system.

If you prefer another supported build system, you can remove `-G Ninja` and let CMake choose its default generator.

## 5. Build the Application

After configuration succeeds, execute:

```bash
cmake --build build
```

CMake will invoke your selected build tool and compile the necessary files, including TreeNavigator's implementation.

## 6. Run Your Application

After the build succeeds, your executable will be generated inside the build directory.

For a single-configuration Ninja build:

On Windows:

```powershell
.\build\MyApplication.exe
```

On Linux or macOS:

```bash
./build/MyApplication
```

That's it! Your application is now using TreeNavigator through CMake.

## 7. Working With Multiple Source Files

As your application grows, you can add more source files to your executable:

```cmake
add_executable(MyApplication
    src/main.cpp
    src/application.cpp
    src/user_interface.cpp
)
```

You don't need to change how TreeNavigator is linked.

As long as your application links against `TreeNavigator::TreeNavigator`, CMake handles the library dependency.

## Troubleshooting

### Could not find CMake

Make sure CMake is installed and available through your terminal's PATH.

Verify the installation:

```bash
cmake --version
```

### Ninja not found

If you're using Ninja, verify that it is installed and available through PATH:

```bash
ninja --version
```

Alternatively, configure your project without specifying `-G Ninja` to use another available generator.

### Header file not found

Make sure you have:

```cmake
add_subdirectory(TreeNavigator)
```

and:

```cmake
target_link_libraries(MyApplication PRIVATE
    TreeNavigator::TreeNavigator
)
```

The TreeNavigator target exposes its public include directory to linked targets.

### Cannot find TreeNavigator::TreeNavigator

Make sure the directory passed to `add_subdirectory()` points to the actual TreeNavigator repository and that its `CMakeLists.txt` defines the `TreeNavigator::TreeNavigator` alias.

---

**You're ready to use TreeNavigator with CMake!**

For traditional compiler-based integration, see the [Manual Setup Guide](MANUAL_SETUP.md).