// ================================== //
// --   CREATED BY: John Virador   -- //
// --  MIT License Software @2026  -- //
// ================================== //

#include "TreeNavigator.h"

#include <stdexcept>

// ============================================================ //
// -- PRIVATE METHODS                                        -- //
// ============================================================ //
void TreeNavigator::validateElement(const Metadata& element) const {
    const std::string& parId = element.parentId;
    const std::string& elId = element.elementId;

    if (parId.empty()) {
        throw std::runtime_error(
            "Parent ID cannot be empty."
        );
    }

    if (elId.empty()) {
        throw std::runtime_error(
            "Element ID cannot be empty."
        );
    }

    // The element must not already exist.
    if (directory.count(elId) > 0) {
        throw std::runtime_error(
            "Element ID must be unique."
        );
    }

    // The parent must already exist.
    if (directory.count(parId) == 0) {
        throw std::runtime_error(
            "Parent ID has not been defined."
        );
    }
}

void TreeNavigator::registerElement(const Metadata& element) {
    validateElement(element);

    directory[element.parentId].push_back(element);

    directory.emplace(
        element.elementId,
        std::vector<Metadata>{element}
    );
}

void TreeNavigator::saveHistory() {
    // Defensive: don't push a duplicate of the current top.
    // The no-op guards in enter()/go() prevent this in normal navigation,
    // but this check ensures the invariant even if a future change
    // bypasses those guards.
    if (!history.empty()) {
        const NavigationState& top = history.back();

        if (top.parentId == parentId && top.index == currentIndex) {
            return;
        }
    }

    history.push_back({
        parentId,
        currentIndex,
    });
}

// ============================================================ //
// -- CONSTRUCTOR                                            -- //
// ============================================================ //
TreeNavigator::TreeNavigator(const std::vector<Metadata>& config) {
    // 1. A root declaration is required.
    if (config.size() < 2) {
        throw std::runtime_error(
            "Configuration must define a root and at least one child."
        );
    }

    // 2. The first entry must describe the root.
    const Metadata& rootMetadata = config[0];

    if (rootMetadata.parentId.empty() ||
        rootMetadata.parentId != rootMetadata.elementId) {
        throw std::runtime_error(
            "The first entry must define the root with matching, non-empty IDs."
        );
    }

    root = rootMetadata.parentId;

    // 3. Initialize the root's directory.
    directory.emplace(
        root,
        std::vector<Metadata>{rootMetadata}
    );

    // 4. Process the remaining entries.
    for (std::size_t i = 1; i < config.size(); i++) {
        registerElement(config[i]);
    }

    // 5. Initialize the cursor using the custom root ID.
    parentId = root;
    wrap = directory[root][0].wrap;
}

// ============================================================ //
// -- CURSOR STATE                                           -- //
// ============================================================ //
bool TreeNavigator::CursorState::operator==(const CursorState& other) const {
    return containerId == other.containerId && index == other.index;
}

bool TreeNavigator::CursorState::operator!=(const CursorState& other) const {
    return !(*this == other);
}

// ============================================================ //
// -- NAVIGATION METHODS                                     -- //
// ============================================================ //
Metadata TreeNavigator::current() const {
    return directory.at(parentId).at(currentIndex);
}

// Get the metadata of the currently selected container or its parent
Metadata TreeNavigator::getParentMetadata() const {
    return directory.at(parentId).at(0);
}

void TreeNavigator::next() {
    const std::size_t siblingCount = directory.at(parentId).size();

    // No children to navigate.
    if (siblingCount <= 1) {
        return;
    }

    if (currentIndex >= siblingCount - 1) {
        if (wrap) {
            currentIndex = 1;
        }
        return;
    }

    currentIndex++;
}

void TreeNavigator::prev() {
    const std::size_t siblingCount = directory.at(parentId).size();

    // No children to navigate.
    if (siblingCount <= 1) {
        return;
    }

    if (currentIndex <= 1) {
        if (wrap) {
            currentIndex = siblingCount - 1;
        }
        return;
    }

    currentIndex--;
}

void TreeNavigator::enter() {
    const Metadata selected = current();

    // If we're already inside the selected container,
    if (selected.elementId == parentId) {
        return; // this only applies to leaf
    }

    saveHistory();

    parentId = selected.elementId;
    currentIndex = (directory.at(parentId).size() == 1) ? 0 : 1;   // 0 for leaves
    wrap = directory.at(parentId).at(0).wrap;
}

bool TreeNavigator::back() {
    // Already at the root; there is nowhere to go back to.
    if (isRoot()) {
        return false;
    }

    // The container we're leaving.
    const Metadata previous = getParentMetadata();
    const std::string& leavingId = previous.elementId;
    const std::string& targetParent = previous.parentId;

    // Move to the parent.
    parentId = targetParent;

    // Find the leaving container in the parent's children.
    const std::vector<Metadata>& siblings = directory.at(targetParent);

    for (std::size_t i = 1; i < siblings.size(); i++) { // i = 1: skip self-entry
        if (siblings[i].elementId == leavingId) {
            currentIndex = i;
            wrap = directory.at(parentId).at(0).wrap;
            return true;
        }
    }

    // Fallback: container not found in parent (shouldn't happen,
    // but defensive against dynamic mode). Land on the first child, or self
    // if the parent has no children.
    currentIndex = (siblings.size() == 1) ? 0 : 1;
    wrap = directory.at(parentId).at(0).wrap;

    return true;
}

bool TreeNavigator::undo() {
    if (history.empty()) {
        return false;
    }

    const NavigationState previous = history.back();
    history.pop_back();

    parentId = previous.parentId;
    currentIndex = previous.index;
    wrap = directory.at(parentId).at(0).wrap;

    return true;
}

bool TreeNavigator::go(std::string id) {
    if (directory.count(id) == 0) {
        return false;
    }

    // already there, the caller's request is satisfied
    if (id == parentId) {
        return true;
    }

    saveHistory();

    parentId = id;
    currentIndex = (directory.at(parentId).size() == 1) ? 0 : 1;
    wrap = directory.at(parentId).at(0).wrap;

    return true;
}

// ============================================================ //
// -- SIBLING METHODS                                        -- //
// ============================================================ //
std::size_t TreeNavigator::getSiblingCount() const {
    return directory.at(parentId).size() - 1;
}

std::vector<Metadata> TreeNavigator::getSiblings() const {
    std::vector<Metadata> siblings;

    std::size_t siblingCount = getSiblingCount();
    siblings.reserve(siblingCount);

    for (std::size_t i = 1; i < (siblingCount + 1); i++) {
        const Metadata child = directory.at(parentId).at(i);
        siblings.push_back(child);
    }

    return siblings;
}

const std::vector<Metadata>& TreeNavigator::getSiblingsRef() const {
    return directory.at(parentId);
}

// ============================================================ //
// -- CHILDREN METHODS                                       -- //
// ============================================================ //

std::size_t TreeNavigator::getChildrenCount() const {
    const Metadata selected = current();
    return directory.at(selected.elementId).size() - 1;
}

std::vector<Metadata> TreeNavigator::getChildren() const {
    std::vector<Metadata> children;
    const Metadata selected = current();

    std::size_t childrenCount = directory.at(selected.elementId).size() - 1;
    children.reserve(childrenCount);

    for (std::size_t i = 1; i < (childrenCount + 1); i++) {
        const Metadata child = directory.at(selected.elementId).at(i);
        children.push_back(child);
    }

    return children;
}

const std::vector<Metadata>& TreeNavigator::getChildrenRef() const {
    return directory.at(current().elementId);
}

// ============================================================ //
// -- STATE METHODS                                          -- //
// ============================================================ //
bool TreeNavigator::isRoot() const {
    return parentId == root;
}

TreeNavigator::CursorState TreeNavigator::getCursorState() const {
    return CursorState{ parentId, currentIndex };
}