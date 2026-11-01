# TreeNavigator

**A lightweight, tree-based navigation library for C++ applications.**

TreeNavigator provides a simple way to navigate and manage hierarchical structures through parent-child relationships. Originally designed for UI menu navigation, it allows developers to build navigation systems without being tied to a particular display, input method, or rendering framework.

Whether you're building a button-controlled menu, a file explorer, or another hierarchical interface, TreeNavigator handles the navigation logic so you can focus on your application.

## ✨ Features

- **Tree-based navigation** – Organize elements using parent-child relationships.
- **Independent rendering** – No built-in display or rendering dependencies.
- **Flexible input handling** – Integrate with physical buttons, keyboards, touchscreens, or other input devices.
- **Sibling navigation** – Move between elements with configurable wrapping behavior.
- **Navigation history** – Return to previous navigation positions or undo navigation changes.
- **Direct navigation** – Navigate directly to a container using its element ID.
- **Dynamic tree management** – Append siblings, append children, delete nodes, and update element payloads at runtime.
- **Subtree deletion** – Delete an element together with all its descendants.
- **Customizable payloads** – Extend element metadata with application-specific fields.
- **Copy and reference access** – Choose between independent copies and read-only references when retrieving elements.

## 📖 How It Works

TreeNavigator organizes elements into a hierarchical tree.

Each element has:
- An `elementId` that uniquely identifies it.
- A `parentId` that identifies its parent.
- A `wrap` setting that determines whether sibling navigation wraps around.
- Optional custom payload fields defined by the developer.

For example, consider this tree:

```text
root
├── settings
│   ├── display
│   └── sound
├── files
│   ├── documents
│   └── images
└── about
```

Each element can contain its own children, allowing the navigator to move deeper into the hierarchy and return to its parent.

### Navigation concept

TreeNavigator maintains a cursor representing the currently selected element inside a container.

- `next()` moves to the next sibling.
- `prev()` moves to the previous sibling.
- `enter()` enters the selected element.
- `back()` returns to the parent container.
- `go()` navigates directly to a container by ID.
- `undo()` restores a previously saved navigation state.

The library manages the navigation state internally. Your application decides how to interpret and display the selected element.

## 📦 Installation

TreeNavigator is a source-based C++ library that supports both manual compilation and CMake integration.

To get started:

1. Download or clone this repository.
2. Choose your preferred integration method:
   - [Manual Setup](docs/MANUAL_SETUP.md)
   - [CMake Setup](docs/CMAKE_SETUP.md)

Once integrated, include the public header wherever you need to use TreeNavigator:

```cpp
#include <TreeNavigator.h>
```

## Integration Guides

- [Manual Setup](docs/MANUAL_SETUP.md) – Integrate TreeNavigator using your C++ compiler.
- [CMake Setup](docs/CMAKE_SETUP.md) – Integrate TreeNavigator into a CMake project.

## 🚀 Getting Started

Let's create a simple navigation tree.

### 1. Define your configuration

TreeNavigator uses a `std::vector<Metadata>` to define the initial tree structure.

```cpp
#include <TreeNavigator.h>

std::vector<Metadata> config = {
    // Root
    Metadata("root", Payload{"root", true}),

    // Root children
    Metadata("root", Payload{"settings", true}),
    Metadata("root", Payload{"files", true}),
    Metadata("root", Payload{"about", true}),

    // Settings children
    Metadata("settings", Payload{"display", true}),
    Metadata("settings", Payload{"sound", true}),

    // Files children
    Metadata("files", Payload{"documents", true}),
    Metadata("files", Payload{"images", true})
};
```

**Important configuration rules:**

- The first entry must define the root.
- The root's `parentId` and `elementId` must match and must not be empty.
- At least one child must be defined.
- Parent elements must be defined before their children.
- Element IDs must be unique.

### 2. Create the navigator

```cpp
TreeNavigator menu(config);
```

The navigator starts at the root container, with its cursor positioned at the first child.

### 3. Navigate through the tree

```cpp
menu.next();  // Move to the next sibling
menu.prev();  // Move to the previous sibling

menu.enter(); // Enter the selected element
menu.back();  // Return to the parent container
```

### 4. Retrieve the selected element

```cpp
Metadata selected = menu.current();

std::cout << "Selected: " << selected.elementId << '\n';
```

You can also retrieve the metadata of the current container:

```cpp
Metadata parent = menu.getParentMetadata();

std::cout << "Current container: "
          << parent.elementId << '\n';
```

## 🛠️ Customizing Payloads

TreeNavigator allows you to add your own data fields to every element.

This is useful when your application needs to associate additional information with individual elements.

Open `Config.h` and locate the customization area:

```cpp
#define TREENAV_CUSTOM_PAYLOAD \
    int xPos = 0;              \
    int yPos = 0;              \
    int width = 0;             \
    int height = 0;
```

You can modify these declarations to suit your application.

For example, you might add a color, a visibility flag, or an application-specific identifier.

```cpp
#define TREENAV_CUSTOM_PAYLOAD \
    int xPos = 0;              \
    int yPos = 0;              \
    int width = 0;             \
    int height = 0;            \
    bool visible = true;
```

Your custom fields become direct members of both `Payload` and `Metadata`.

You can access them directly:

```cpp
Metadata selected = menu.current();

std::cout << selected.width << '\n';
std::cout << selected.visible << '\n';
```

**Customization guidelines:**

- Only modify the declarations inside `TREENAV_CUSTOM_PAYLOAD`.
- Every member declaration must end with a semicolon.
- Do not modify `DEFINE_CUSTOM_PAYLOAD`.
- Do not remove its invocation inside `Payload`.
- Do not modify the required members of `Payload` or `Metadata`.

## 🔧 Dynamic Tree Management

TreeNavigator also supports modifying the tree after construction.

### Append a sibling

Adds a new element to the current container.

```cpp
ErrorCode result = menu.appendSibling(
    Payload{"newItem", true}
);
```

### Append a child

Adds a new child to the currently selected element.

```cpp
ErrorCode result = menu.appendChild(
    Payload{"newChild", true}
);
```

### Delete a node

Deletes the currently selected element and all its descendants.

```cpp
ErrorCode result = menu.deleteNode();
```

The root cannot be deleted.

### Update an element's payload

You can update an element's properties without changing its ID.

```cpp
Metadata selected = menu.current();

ErrorCode result = menu.updatePayload(
    Payload{selected.elementId, true, 0, 2, 4, 5}
);
```

The `elementId` must match the currently selected element. Other payload fields, including custom fields, can be updated.

## 📚 API Reference

### Navigation

| Method | Description |
|---|---|
| `next()` | Moves to the next sibling. |
| `prev()` | Moves to the previous sibling. |
| `enter()` | Enters the selected element. |
| `back()` | Returns to the parent container. |
| `undo()` | Restores the previous saved navigation state. |
| `go(id)` | Navigates directly to a container by ID. |
| `isRoot()` | Checks whether the current container is the root. |
| `isWrap()` | Checks whether the current container enabled wrap. |

### Element Retrieval

| Method | Description |
|---|---|
| `current()` | Returns the currently selected element's metadata. |
| `getParentMetadata()` | Returns the current container's metadata. |
| `getSiblings()` | Returns a copy of the current container's children. |
| `getChildren()` | Returns a copy of the selected element's children. |
| `getSiblingsRef()` | Returns a read-only reference to the current container's internal vector. |
| `getChildrenRef()` | Returns a read-only reference to the selected element's internal vector. |
| `getSiblingCount()` | Returns the number of children in the current container. |
| `getChildrenCount()` | Returns the number of children belonging to the selected element. |
| `getCursorState()` | Returns the current container ID and cursor index. |

### Tree Modification

| Method | Description |
|---|---|
| `appendSibling(payload)` | Adds a sibling to the current container. |
| `appendChild(payload)` | Adds a child to the selected element. |
| `deleteNode()` | Deletes the selected element and its descendants. |
| `updatePayload(payload)` | Updates the selected element's payload without changing its ID. |

### Important Notes

**Copy vs. reference retrieval**

`getSiblings()` and `getChildren()` return independent copies. Changes to those returned vectors do not modify the navigator's internal storage.

`getSiblingsRef()` and `getChildrenRef()` return read-only references to internal vectors. They avoid vector copying, but references and iterators may become invalid after certain mutations, especially when the associated directory entry is erased.

Use reference-returning methods carefully.

**Cursor indexing**

The internal vector for each container has a self-entry at index `0`. Its children begin at index `1`.

This is an implementation detail that is useful when working with `getCursorState()` and the reference-returning methods.

## ⚠️ Current Limitations

- TreeNavigator handles navigation and tree management, not rendering.
- Input handling must be implemented by the application.
- Custom payload declarations are configured at compile time.
- Element IDs cannot be changed through `updatePayload()`.
- The root cannot be deleted.
- The current implementation targets C++ applications.

## 🗺️ Roadmap

TreeNavigator is an evolving project. Some potential future improvements include:

- Additional language implementations, including JavaScript and Python.
- More comprehensive tests and validation.
- Further improvements to dynamic tree management.
- Additional examples and integrations.

## ❓ Frequently Asked Questions

### 1. What is the difference between `back()` and `undo()`?

Both methods restore a previous navigation position, but they work differently.

- **`back()`** follows the tree hierarchy. It returns to the parent container and searches for the previously selected element within that parent's children to restore the cursor position.
- **`undo()`** uses the navigation history stack to restore the previously saved navigation state directly.

In terms of performance:

| Method | Time Complexity | Behavior |
|---|---|---|
| `back()` | O(n) | Searches the parent's children to locate the previous cursor position. |
| `undo()` | O(1) | Retrieves the previous navigation state from the history stack. |

**Which one should I use?**

If your application primarily uses step-based navigation through `next()`, `prev()`, and `enter()`, consider using `undo()` when navigating back up the hierarchy.

Since `enter()` saves the previous navigation position, `undo()` can restore that position directly without searching through the parent's children.

However, if you want to return to the structural parent of the current container regardless of the navigation history, use `back()`.

**Important:** `undo()` restores the previous saved navigation state, not necessarily the parent container. These methods are not interchangeable in every situation.

### 2. What is the difference between `getChildren()` and `getChildrenRef()`?

Both methods retrieve the children of the currently selected element, but they differ in how the data is returned.

- **`getChildren()`** returns an independent copy of the selected element's children.
- **`getChildrenRef()`** returns a read-only reference to the internal vector containing the selected element's children.

Example:

```cpp
// Independent copy
std::vector<Metadata> children = menu.getChildren();

// Read-only reference
const std::vector<Metadata>& childrenRef =
    menu.getChildrenRef();
```

**When should I use each one?**

Use `getChildren()` when you need an independent collection that you can modify without affecting TreeNavigator's internal storage.

Use `getChildrenRef()` when you only need to inspect or iterate through the internal collection and want to avoid the overhead of copying the vector.

**Important:** `getChildren()` excludes the self-entry. Its returned vector
contains only the element's children. `getChildrenRef()` however, includes the
self-entry at index `0`; start iterating at index `1` if you only want
children.

**Important warnings about references:**

A reference returned by `getChildrenRef()` is tied to TreeNavigator's internal storage.

- You cannot modify the vector through the returned const reference.
- References and iterators to individual elements may become invalidated by vector mutations.
- The returned vector reference becomes dangling if its corresponding directory entry is erased, such as when the selected element or its subtree is deleted.
- Do not retain the reference across operations that may invalidate its underlying storage.

When in doubt, use `getChildren()`.

### 3. What is the difference between `getSiblings()` and `getSiblingsRef()`?

These methods follow the same copy-versus-reference behavior as their children counterparts.

- **`getSiblings()`** returns an independent copy of the current container's children.
- **`getSiblingsRef()`** returns a read-only reference to the current container's internal vector.

Example:

```cpp
// Independent copy
std::vector<Metadata> siblings = menu.getSiblings();

// Read-only reference
const std::vector<Metadata>& siblingsRef =
    menu.getSiblingsRef();
```

**Important:** The two methods differ in whether they include the self-entry.

- `getSiblings()` excludes the self-entry: the returned vector contains only the
  container's children.
- `getSiblingsRef()` includes the self-entry: the returned vector has the
  container's own metadata at index `0`, and its children at indices `1..N`.

If you iterate `getSiblingsRef()` directly, start at index `1` to skip the
self-entry.

The same reference invalidation warnings and rules that apply to `getChildrenRef()` also apply here.

### 4. Why does TreeNavigator return copies instead of references by default?

Returning a copy provides an independent collection.

This means developers can manipulate their retrieved data without accidentally modifying TreeNavigator's internal structure.

For example:

```cpp
std::vector<Metadata> children = menu.getChildren();

children.clear();
```

Clearing this vector does not remove any elements from TreeNavigator.

The reference-returning methods are provided as an alternative when avoiding vector-copying overhead is more important.

### 5. Why does the cursor start at index 1?

TreeNavigator internally stores each container's own metadata as the first element of its corresponding vector.

The internal structure follows this convention:

| Index | Purpose |
|---|---|
| `0` | Container's own metadata (self-entry). |
| `1` | First child. |
| `2` | Second child. |
| `3` | Third child. |

This allows TreeNavigator to retrieve the container's own metadata directly without performing another lookup.

The self-entry is exposed by the reference-returning methods (`getChildrenRef()`,
`getSiblingsRef()`) at index 0. The copy-returning methods (`getChildren()`,
`getSiblings()`) exclude it for convenience, so their returned vectors contain
only children.

If you use the reference methods directly, remember to start iterating at
index 1 to skip the self-entry.

### 6. Does TreeNavigator handle rendering or input events?

No.

TreeNavigator focuses exclusively on navigation and hierarchical tree management.

It does not impose any particular rendering framework or input-handling system.

You can integrate it with:

- Physical buttons.
- Keyboards.
- Touchscreens.
- TFT displays.
- OLED displays.
- Serial monitors.
- Other custom interfaces.

Your application is responsible for handling input events and displaying the current navigation state.

### 7. Can I add or remove elements after creating a TreeNavigator instance?

Yes!

TreeNavigator supports runtime tree modification.

You can use:

- `appendSibling()` to add a sibling to the current container.
- `appendChild()` to add a child to the currently selected element.
- `deleteNode()` to remove the selected element and all its descendants.
- `updatePayload()` to update the selected element's payload.

You do not need to reconstruct the entire navigator whenever you want to modify the tree.

### 8. Can I change an element's ID using `updatePayload()`?

No.

An element's `elementId` is used to identify it throughout the tree structure.

Changing it would require updating its directory entry, parent-child relationships, and potentially navigation history.

For this reason, `updatePayload()` rejects attempts to change an existing element's ID.

It returns `ErrorCode::CannotChangeElementId` when the provided ID does not match the currently selected element.

All other payload fields, including custom fields, can be updated.

**Example:**

```cpp
// Get the current element's metadata.
const Metadata elem = menu.current();

// Update its fields. The elementId must match the current element's ID;
// otherwise, updatePayload returns ErrorCode::CannotChangeElementId.
ErrorCode result = menu.updatePayload({
    elem.elementId,   // same elementId (required)
    false,            // wrap
    0, 2,             // xPos, yPos
    1, 5              // width, height
});

if (result == ErrorCode::CannotChangeElementId) {
    // The elementId doesn't match — the payload was rejected.
}
```

### 9. Can I customize the metadata structure?

Yes!

TreeNavigator provides a dedicated customization area inside `Config.h`.

You can add your own members through `TREENAV_CUSTOM_PAYLOAD`.

For example:

```cpp
#define TREENAV_CUSTOM_PAYLOAD \
    int xPos = 0;              \
    int yPos = 0;              \
    int width = 0;             \
    int height = 0;            \
    bool visible = true;
```

These fields become directly accessible through both `Payload` and `Metadata`.

**Important:** Only modify the designated customization area. Do not modify the required members or the payload injection mechanism.

### 10. Can I delete the root element?

No.

The root is the starting point of the entire hierarchy. Deleting it would leave the navigator without a valid starting container.

Calling `deleteNode()` while the selected element is the root returns `ErrorCode::CannotDeleteRoot`.

### 11. What happens when I delete an element that has children?

TreeNavigator deletes the selected element together with its entire subtree.

For example:

```text
root
├── A
│   ├── B
│   │   └── C
│   └── D
└── E
```

If `A` is deleted, both `B`, `C`, and `D` are deleted as well.

The navigator also updates its navigation state and removes history entries referring to deleted containers.

### 12. What happens if an element has no children?

An element without children is considered a leaf node.

When entering a leaf, the cursor remains on that element's self-entry rather than moving into a nonexistent child.

You can check whether the currently selected element has children using:

```cpp
if (menu.getChildrenCount() == 0) {
    // The selected element is a leaf.
}
```

### 13. What does the `wrap` setting do?

The `wrap` setting controls whether sibling navigation loops around when reaching the beginning or end of a container.

When wrapping is enabled:

```text
A → B → C → A
```

When wrapping is disabled:

```text
A → B → C
```

Calling `next()` at `C` or `prev()` at `A` will not move the cursor beyond the available children.

The setting belongs to the current container's metadata.

### 14. What happens when `go()` navigates to a leaf element?

`go()` navigates directly into the specified container using its element ID.

If the target has children, the cursor starts at its first child.

If the target is a leaf, the cursor remains on its self-entry.

This allows direct navigation to both containers and leaf elements.

### 15. Can I use TreeNavigator without an embedded system?

Absolutely!

Although TreeNavigator was initially designed with embedded UI navigation in mind, it is not restricted to Arduino or microcontroller projects.

It can be used in standard C++ applications that need hierarchical navigation, such as:

- Console-based menu systems.
- File and directory explorers.
- Configuration managers.
- Hierarchical data browsers.
- Custom navigation interfaces.

As long as your project supports the required C++ standard and dependencies, you can integrate TreeNavigator into it.

## 🤝 Contributing

Contributions, suggestions, bug reports, and improvements are welcome!

If you encounter a problem or have an idea that could improve TreeNavigator, feel free to open an issue or submit a pull request.

Whether you're fixing a bug, improving documentation, or suggesting a new feature, every contribution is appreciated.

## 📄 License

TreeNavigator is released under the **MIT License**.

See [LICENSE](LICENSE) for more information.

---

**Created by John Virador**

*Built to make navigating hierarchical structures simpler.*