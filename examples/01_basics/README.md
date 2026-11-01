# Basic Examples

This directory contains introductory examples demonstrating the fundamental features of TreeNavigator.

These examples are intended to help developers understand the library's navigation model before exploring its more advanced capabilities.

## Examples

| File | Description |
|---|---|
| `basic_navigation.cpp` | Introduces TreeNavigator initialization and basic sibling navigation. |
| `nested_navigation.cpp` | Demonstrates navigating through multiple levels of a hierarchical tree. |
| `navigation_history.cpp` | Explains navigation history and the differences between `back()` and `undo()`. |
| `direct_navigation.cpp` | Demonstrates navigating directly to elements using their IDs. |
| `terminal_navigation.cpp` | Combines the concepts from the previous four examples into an interactive terminal-based menu.|

## Recommended Learning Order

1. Start with `basic_navigation.cpp` to understand the cursor and sibling navigation.
2. Continue with `nested_navigation.cpp` to understand parent-child relationships.
3. Then `navigation_history.cpp` to understand navigation history and restoration.
4. And `direct_navigation.cpp` to learn how to navigate directly using element IDs.
5. Finish with `terminal_navigation.cpp` to explore how these features work together in an interactive application.

Each example is self-contained and can be explored independently.

## Requirements

### Compilers (tested or known to work)

| Compiler | Minimum version | Notes |
|---|---|---|
| **GCC** | 7.0 | `-std=c++17` (or `-std=gnu++17`). GCC 7 was the first to support `extract`. |
| **Clang** | 5.0 | `-std=c++17` (or `-std=c++1z` for older). |
| **MSVC** | Visual Studio 2017 (15.7) | `/std:c++17`. VS 2019 / 2022 also work. |
| **Arduino / ESP-IDF (xtensa GCC)** | ESP-IDF 4.x with GCC 8+ | Works on ESP32 (standard library is present). |

**Recommended minimum:** GCC 8, Clang 7, MSVC 2019. Earlier versions may work but are not tested

### Standard library

- A conforming C++17 standard library (libstdc++, libc++, or MSVC STL).
- No third-party libraries are required.
- No platform-specific APIs are used.