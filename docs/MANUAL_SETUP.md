# Manual Setup Guide

This guide explains how to integrate TreeNavigator into your C++ project without using CMake.

With this approach, you compile TreeNavigator's implementation files directly alongside your application using your C++ compiler.

## Requirements

Before getting started, make sure you have:

- A C++ compiler, such as GCC/G++.
- TreeNavigator downloaded or cloned into your project.
- A terminal to execute compiler commands.

## 1. Project Structure

First, place the TreeNavigator directory inside your application project.

Your directory should look something like this:

```text
MyApplication/
│
├── TreeNavigator/
│   ├── include/
│   │   └── TreeNavigator.h
│   │
│   └── src/
│       └── TreeNavigator.cpp
│
├── main.cpp
│
└── ...
```

You can place TreeNavigator in another location if you prefer. Just make sure to adjust the compiler paths accordingly.

## 2. Include the Library

Inside your `main.cpp`, include the public TreeNavigator header:

```cpp
#include <TreeNavigator.h>
```

You don't need to manually specify the relative path to the header inside your source code.

Instead, you'll tell the compiler where to find it when compiling.

## 3. Compile Your Application

Open a terminal in your `MyApplication` directory and execute:

```bash
g++ -ITreeNavigator/include main.cpp TreeNavigator/src/TreeNavigator.cpp -o main
```

Let's break down the command:

| Argument | Description |
|---|---|
| `g++` | Invokes the C++ compiler. |
| `-I` | Specifies an additional header search directory. |
| `TreeNavigator/include` | Location of TreeNavigator's public headers. |
| `main.cpp` | Your application's source file. |
| `TreeNavigator/src/TreeNavigator.cpp` | TreeNavigator's implementation file. |
| `-o main` | Names the generated executable `main`. |

**Note:** If TreeNavigator adds more implementation files in the future, include those `.cpp` files in your compilation command as well.

## 4. Run Your Application

After compilation succeeds, execute your generated program.

On Windows:

```powershell
.\main.exe
```

On Linux or macOS:

```bash
./main
```

That's it! You've successfully integrated TreeNavigator without using a build system.

## 5. Working With Multiple Source Files

Your application can have multiple `.cpp` files.

For example:

```text
MyApplication/
│
├── TreeNavigator/
│
├── src/
│   ├── main.cpp
│   └── application.cpp
│
└── ...
```

You can compile them together:

```bash
g++ -ITreeNavigator/include \
    src/main.cpp \
    src/application.cpp \
    TreeNavigator/src/TreeNavigator.cpp \
    -o main
```

All source files that provide required implementations must be compiled and linked into your application.

## Troubleshooting

### Header file not found

If you encounter:

```text
fatal error: TreeNavigator.h: No such file or directory
```

Make sure your compiler command includes the correct header search path:

```bash
-ITreeNavigator/include
```

Also verify that `TreeNavigator.h` exists inside the specified directory.

### Undefined reference / Undefined reference to TreeNavigator

This usually means the compiler found the header, but the implementation was not linked into your application.

Make sure you included:

```bash
TreeNavigator/src/TreeNavigator.cpp
```

in your compilation command.

---

**You're ready to use TreeNavigator!**

For a more automated build process, see the [CMake Setup Guide](CMAKE_SETUP.md).