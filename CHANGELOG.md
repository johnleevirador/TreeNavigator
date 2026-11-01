# [0.2.0] - 2026-10-24

This version represents the second development milestone of TreeNavigator, originally developed locally as version `1.1.0`.

### Added
- Introduced navigation history through the new `NavigationState` structure.
- Added `undo()` to restore previously saved navigation states.
- Added internal `saveHistory()` helper to record navigation states.
- Added duplicate-history protection to prevent consecutive identical entries.

### Changed
- Updated `current()` to use the cursor index directly instead of special-case leaf handling.
- Updated `enter()` to initialize the cursor at index `0` for leaf nodes and index `1` for containers with children.
- Updated `enter()` to prevent redundant navigation into the currently active leaf.
- Updated `enter()` and `go()` to save navigation history before changing containers.
- Improved `back()` to restore the cursor to the container previously selected instead of resetting to the first child.
- Changed `back()` return type from `void` to `bool`, allowing callers to determine whether navigation succeeded.
- Updated `go()` to prevent unnecessary state changes when navigating to the currently active container.
- Updated `go()` to correctly initialize the cursor for leaf nodes.

### Improved
- Improved cursor state consistency, particularly when navigating leaf nodes.
- Improved navigation restoration and handling of unsuccessful navigation operations.

---

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