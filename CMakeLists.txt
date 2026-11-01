cmake_minimum_required(VERSION 3.16)

project(TreeNavigator
    VERSION 1.0.0
    DESCRIPTION "A lightweight, tree-based navigation library for C++ applications"
    LANGUAGES CXX
)

# ==========================================
# Library
# ==========================================

add_library(TreeNavigator STATIC
    src/TreeNavigator.cpp
)

# Public header directory
target_include_directories(TreeNavigator PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Convenient namespaced target for consumers
add_library(TreeNavigator::TreeNavigator ALIAS TreeNavigator)