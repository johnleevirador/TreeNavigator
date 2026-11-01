# [0.1.0] - 2026-10-18

This version represents the initial development milestone of TreeNavigator, originally developed locally as version `1.0.0`.

### Added
- Introduced the initial version of TreeNavigator, a lightweight, tree-based navigation library for C++ applications.
- Added `Metadata` structure for defining tree elements, including required identifiers and optional user-defined metadata.
- Implemented tree configuration through a `std::vector<Metadata>`.
- Added automatic directory construction using `std::unordered_map` for efficient element lookup.
- Added configuration validation to enforce:
  - A valid root declaration.
  - Non-empty parent and element IDs.
  - Unique element IDs.
  - Previously defined parent elements.
- Implemented hierarchical navigation through:
  - `next()` and `prev()` for sibling navigation.
  - `enter()` for entering the selected container.
  - `back()` for returning to the parent container.
  - `go()` for directly navigating to a container using its ID.
- Added configurable wrapping behavior for sibling navigation.
- Added metadata retrieval methods:
  - `current()`
  - `getParentMetadata()`
  - `getSiblings()`
  - `getSiblingsRef()`
  - `getChildren()`
  - `getChildrenRef()`
- Added navigation and tree inspection methods:
  - `getSiblingCount()`
  - `getChildrenCount()`
  - `isRoot()`
  - `getCursorState()`
- Introduced `CursorState` with equality and inequality operators for comparing navigation positions.
- Added support for custom root identifiers and optional positional and dimensional metadata.

### Notes
- This milestone established the foundational tree structure and navigation functionality of TreeNavigator.
- Rendering and user-interface handling were left to the application using the library.