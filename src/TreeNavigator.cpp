// ================================== //
// --   CREATED BY: John Virador   -- //
// --  MIT License Software @2026  -- //
// ================================== //
#include <TreeNavigator.h>

#include <stdexcept>
#include <utility>
#include <unordered_set>
#include <algorithm>

void TreeNavigator::validateElement(const Metadata& element) const {
    const std::string& parId = element.parentId;
    const std::string& elId = element.elementId;

    if (parId.empty()) {
        throw std::runtime_error(
            "TreeNavigator: Parent ID cannot be empty."
        );
    }

    if (elId.empty()) {
        throw std::runtime_error(
            "TreeNavigator: Element ID cannot be empty."
        );
    }

    // The element must not already exist.
    if (directory.count(elId) > 0) {
        throw std::runtime_error(
            "TreeNavigator: Element ID must be unique."
        );
    }

    // The parent must already exist.
    if (directory.count(parId) == 0) {
        throw std::runtime_error(
            "TreeNavigator: Parent ID has not been defined."
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
    // Defensive: prevent push of duplicate to the current top.
    // The no-op guards in enter()/go() prevent this in normal navigation,
    // but this check ensures the invariant even if a future change
    // bypasses those guards.
    if (!history.empty()) {
        const NavigationState& top = history.back();
        if (top.parentId == parentId && top.index == currentIndex) return;
    }
    history.push_back({
        parentId,
        currentIndex,
    });
}

std::vector<std::string> TreeNavigator::collectPostOrder(
    const std::string& startId
) const {
    std::vector<std::string> result;
    std::vector<TraversalFrame> stack;

    // Start at the requested node.
    // Index 0 is metadata, so the first child is at index 1.
    stack.push_back({startId, 1});

    while (!stack.empty()) {
        // Get the node we're currently processing.
        TraversalFrame& frame = stack.back();

        const std::vector<Metadata>& entries =
            directory.at(frame.elementId);

        // If this node has another child to visit...
        if (frame.nextChildIndex < entries.size()) {
            // Get the next child's ID.
            std::string childId =
                entries.at(frame.nextChildIndex).elementId;

            // Advance before descending into the child.
            frame.nextChildIndex++;

            // Begin processing the child.
            stack.push_back({childId, 1});
        }
        else {
            // All children have been processed.
            // We can now visit this node itself.
            result.push_back(frame.elementId);

            // Return to the previous node.
            stack.pop_back();
        }
    }

    return result;
}

TreeNavigator::TreeNavigator(const std::vector<Metadata>& config) {    
    // 1. A root declaration is required.
    if (config.size() < 2) {
        throw std::runtime_error(
            "TreeNavigator: Configuration must define a root and at least one child."
        );
    }

    // 2. The first entry must describe the root.
    const Metadata& rootMetadata = config[0];

    if (rootMetadata.parentId.empty() ||
        rootMetadata.parentId != rootMetadata.elementId) {
        throw std::runtime_error(
            "TreeNavigator: The first entry must define the root with matching, non-empty IDs."
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

// Returns the metadata of the element under the cursor.
Metadata TreeNavigator::current() const {
    return directory.at(parentId).at(currentIndex);
}

// Returns the metadata of the container the cursor is currently inside.
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

bool TreeNavigator::go(const std::string& id) {
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

std::size_t TreeNavigator::getSiblingCount() const {
    return directory.at(parentId).size() - 1;
}

std::vector<Metadata> TreeNavigator::getSiblings() const {
    const std::vector<Metadata>& source = directory.at(parentId);

    // Guard against empty or single-element collections to prevent out-of-bounds crashes
    if (source.size() <= 1) {
        return {}; 
    }

    // Copy elements from index 1 up to the end (Index 0 is not included - Parent Metadata)
    std::vector<Metadata> siblings(source.begin() + 1, source.end());

    return siblings;
}

const std::vector<Metadata>& TreeNavigator::getSiblingsRef() const {
    return directory.at(parentId);
}

std::size_t TreeNavigator::getChildrenCount() const {
    const Metadata selected = current();
    return directory.at(selected.elementId).size() - 1;
}

std::vector<Metadata> TreeNavigator::getChildren() const {
    const Metadata selected = current();
    const std::vector<Metadata>& source = directory.at(selected.elementId);

    if (source.size() <= 1) {
        return {}; 
    }

    // Copy elements from index 1 up to the end (Index 0 is not included - Self Entry)
    std::vector<Metadata> children(source.begin() + 1, source.end());

    return children;
}

const std::vector<Metadata>& TreeNavigator::getChildrenRef() const {
    return directory.at(current().elementId);
}

ErrorCode TreeNavigator::appendSibling(const Payload& sibling) {
    const std::string& elId = sibling.elementId;

    if (elId.empty()) {
        return ErrorCode::ElementIdEmpty;
    }
    // The element must not already exist.
    if (directory.count(elId) > 0) {
        return ErrorCode::DuplicateElement;
    }

    const Metadata siblingForm{parentId, sibling};

    directory.at(parentId).push_back(siblingForm);

    directory.emplace(
        elId,
        std::vector<Metadata>{siblingForm}
    );

    return ErrorCode::Success;
}

ErrorCode TreeNavigator::appendChild(const Payload& child) {
    const std::string& elId = child.elementId;
    const Metadata childForm{parentId, child};

    if (elId.empty()) {
        return ErrorCode::ElementIdEmpty;
    }
    // The element must not already exist.
    if (directory.count(elId) > 0) {
        return ErrorCode::DuplicateElement;
    }

    const Metadata selected = current();

    directory.at(selected.elementId).push_back(childForm);

    directory.emplace(
        elId,
        std::vector<Metadata>{childForm}
    );

    return ErrorCode::Success;
}

ErrorCode TreeNavigator::deleteNode() {
    const Metadata selected = current();
    const std::string& targetId = selected.elementId;

    // 1. Reject deleting the root.
    if (targetId == root) {
        return ErrorCode::CannotDeleteRoot;
    }

    // 2. Collect the subtree to delete (IDs in post-order).
    const std::vector<std::string> toDelete = collectPostOrder(targetId);
    const std::unordered_set<std::string> deletedSet(toDelete.begin(), toDelete.end());

    // 3. Capture the deleted node's real parent and its index BEFORE erasing.
    const std::string targetParentId = directory.at(targetId).at(0).parentId;
    std::size_t targetOldIndex = SIZE_MAX;
    // This search should always succeed in a consistent tree. It would only fail
    // if the target's Metadata.parentId were wrong (which indicates a bug elsewhere).
    {
        const std::vector<Metadata>& sib = directory.at(targetParentId);
        for (std::size_t k = 1; k < sib.size(); ++k) {   // skip self-entry at [0]
            if (sib[k].elementId == targetId) {
                targetOldIndex = k;
                break;
            }
        }
    }

    // 4. If at self-entry case, move cursor up to the parent.
    const bool cursorWasOnTarget = (targetId == parentId);
    if (cursorWasOnTarget) {
        back();
    }

    // 5. Remove each deleted element from history.
    history.erase(
        std::remove_if(history.begin(), history.end(),
            [&](const NavigationState& s) {
                return deletedSet.count(s.parentId) > 0;
            }),
        history.end()
    );

    // 6. Erase each deleted element's directory entry.
    for (const std::string& id : toDelete) {
        directory.erase(id);
    }

    // 7. Remove the deleted subtree from its parent's child list.
    std::vector<Metadata>& siblingsRef = directory.at(targetParentId);
    siblingsRef.erase(
        std::remove_if(siblingsRef.begin() + 1, siblingsRef.end(),
            [&](const Metadata& m) {
                return deletedSet.count(m.elementId) > 0;
            }),
        siblingsRef.end()
    );

    // 8. Update the cursor to its surviving neighbor.
    if (targetOldIndex != SIZE_MAX) {
        const std::vector<Metadata>& sib = directory.at(targetParentId);
        std::size_t nextCursorIndex;
        if (targetOldIndex < sib.size()) {
            nextCursorIndex = targetOldIndex; // next sibling slid down
        } else if (targetOldIndex > 1) {
            nextCursorIndex = targetOldIndex - 1; // was last -> previous sibling
        } else {
            nextCursorIndex = 0; // only self-entry remains
        }
        currentIndex = nextCursorIndex;
    }

    return ErrorCode::Success;
}

ErrorCode TreeNavigator::updatePayload(const Payload& newNode) {
    const Metadata selected = current();

    // 1. Reject empty ID (The caller should pass the current elementId)
    if (newNode.elementId.empty()) {
        return ErrorCode::ElementIdEmpty;
    }

    // 2. Reject any attempt to change the element's ID.
    if (newNode.elementId != selected.elementId) {
        return ErrorCode::CannotChangeElementId;
    }

    // 3. Instantiate the new metadata with the SAME parentId and elementId.
    const Metadata newMetadata{parentId, newNode};

    // Update the parent's child list entry (the element's position in
    // its parent's children).
    directory.at(parentId).at(currentIndex) = newMetadata;

    // Update the element's own container's self-entry (slot 0).
    directory.at(selected.elementId).at(0) = newMetadata;

    return ErrorCode::Success;
}

bool TreeNavigator::isRoot() const {
    return parentId == root;
}

bool TreeNavigator::isWrap() const {
    return wrap;
}

TreeNavigator::CursorState TreeNavigator::getCursorState() const {
    return CursorState{ parentId, currentIndex };
}