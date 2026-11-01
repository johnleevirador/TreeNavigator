# Changelog

All notable changes to TreeNavigator are documented in this file.

Versions `0.1.0` and `0.2.0` represent development milestones created before the first official public release. Version `1.0.0` marks the first official public release of TreeNavigator.

---

# TreeNavigator v1.0.0 — Initial Public Release

The first official public release of TreeNavigator!

TreeNavigator provides a lightweight, tree-based navigation and management system for C++ applications. It allows developers to define hierarchical elements and navigate through or modify them using a simple API.

This release introduces the redesigned payload system, dynamic tree management, configuration validation, sibling navigation, parent-child traversal, direct navigation, and metadata retrieval.

Rendering is intentionally left to the application, allowing TreeNavigator to work independently of any particular display or user-interface library.

## [1.0.0] - 2026-10-30

### Added
- Introduced the `Payload` structure to separate element-specific data from navigation metadata.
- Updated `Metadata` to inherit from `Payload`, providing direct access to payload members while retaining the required `parentId`.
- Added `TREENAV_CUSTOM_PAYLOAD` macro for defining application-specific payload members.
- Introduced `DEFINE_CUSTOM_PAYLOAD` macro to inject custom members into `Payload`.
- Added compile-time validation through `static_assert` to ensure the custom payload injection mechanism remains defined.
- Added dynamic tree management capabilities:
  - `appendSibling()` to append a new sibling to the current container.
  - `appendChild()` to append a new child to the currently selected element.
  - `deleteNode()` to delete the selected element and its entire subtree.
  - `updatePayload()` to update the selected element's payload without changing its ID.
- Added internal `validateElement()` helper for validating elements before registration.
- Added internal `registerElement()` helper for registering new elements into the directory.
- Added internal `collectPostOrder()` helper for collecting elements in post-order traversal, supporting subtree deletion.
- Introduced `ErrorCode` enum for standardized error reporting across tree management operations.
- Added error codes for:
  - `Success`
  - `ParentIdEmpty`
  - `ElementIdEmpty`
  - `DuplicateElement`
  - `ParentNotDefined`
  - `CannotDeleteRoot`
  - `CannotChangeElementId`

### Changed
- Redesigned element configuration to use `Payload` as the source of element-specific data.
- Updated the `Metadata` constructor to accept a parent ID and a `Payload` object.
- Changed payload customization from individually defined metadata fields to a centralized customization area in `Config.h`.
- Updated the configuration API to support the new `Payload` and `Metadata` relationship.
- Extended navigation history handling to account for dynamically modified tree structures.
- Updated `directory` management to support runtime registration and removal of elements.

### Improved
- Improved extensibility by allowing developers to define custom payload members without modifying the required members of `Payload` or `Metadata`.
- Improved tree management flexibility by allowing elements to be created, updated, and deleted after initialization.
- Improved subtree deletion by supporting the removal of an element and all its descendants.
- Improved API flexibility by allowing developers to choose between independent copies and read-only references when retrieving sibling and child collections.

### Notes
- This release introduces changes to the configuration and payload API. Existing configurations from previous versions may require modifications.
- Custom payload members must be declared in the designated customization area inside `Config.h`.
- Element IDs remain immutable through `updatePayload()`.
- The root element cannot be deleted.
- Rendering and input handling remain the responsibility of the application using TreeNavigator.

---

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