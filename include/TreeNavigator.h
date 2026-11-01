// ================================== //
// --   CREATED BY: John Virador   -- //
// --  MIT License Software @2026  -- //
// ================================== //
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstddef>

#include "ErrorCode.h"
#include "Config.h"

class TreeNavigator {
    private:
        std::string root;

        std::string parentId;
        bool wrap;
        std::size_t currentIndex = 1;

        std::unordered_map<
            std::string,
            std::vector<Metadata>
        > directory;

        struct NavigationState {
            std::string parentId;
            std::size_t index;
        };
        std::vector<NavigationState> history;

        void validateElement(const Metadata& element) const;
        void registerElement(const Metadata& element);
        void saveHistory();

        struct TraversalFrame {
            std::string elementId;
            std::size_t nextChildIndex;
        };
        std::vector<std::string> collectPostOrder(
            const std::string& startId
        ) const;

    public:

        /**
         * @brief Constructs a TreeNavigator using the provided configuration.
         *
         * @param config Tree configuration containing the root and its descendants.
         *
         * @throws std::runtime_error If the configuration is invalid, the root
         * is missing, or the configuration contains invalid elements.
         *
         * @note The first configuration entry must define the root, with
         * matching, non-empty parentId and elementId.
         * At least one child must also be defined.
         * Parents must be defined before their children.
         */
        TreeNavigator(const std::vector<Metadata>& config);

        /**
         * @brief Represents the current cursor position.
         *
         * containerId identifies the container currently being navigated.
         * index identifies the cursor's position inside that container.
         *
         * @note Index 0 represents the container's self-entry.
         * Child elements begin at index 1.
         */
        struct CursorState {
            std::string containerId;
            std::size_t index;

            bool operator==(const CursorState& other) const {
                return containerId == other.containerId && index == other.index;
            }

            bool operator!=(const CursorState& other) const {
                return !(*this == other);
            }
        };

        /**
         * @brief Returns the metadata of the element currently under the cursor.
         *
         * @return A copy of the selected element's metadata.
         *
         * @note When the current container has no children, its self-entry
         * is considered the current element.
         */
        Metadata current() const;

        /**
         * @brief Returns the metadata of the container currently being navigated.
         *
         * @return A copy of the current container's metadata.
         *
         * @note This is the self-entry stored at index 0 of the container's
         * internal vector.
         */
        Metadata getParentMetadata() const;

        /**
         * @brief Moves the cursor to the next sibling.
         *
         * @note If wrapping is enabled, reaching the last sibling moves
         * the cursor back to the first sibling.
         * Does nothing if the current container has no children.
         */
        void next();

        /**
         * @brief Moves the cursor to the previous sibling.
         *
         * @note If wrapping is enabled, reaching the first sibling moves
         * the cursor to the last sibling.
         * Does nothing if the current container has no children.
         */
        void prev();

        /**
         * @brief Enters the element currently selected by the cursor.
         *
         * @note Saves the current navigation position before entering.
         * If the selected element is a leaf, the cursor remains on that
         * element's self-entry.
         * Does nothing if the selected element is already the current container.
         */
        void enter();

        /**
         * @brief Returns to the parent container.
         *
         * @return true if navigation moved to the parent; false if already
         * at the root.
         *
         * @note The cursor is restored to the element that was previously
         * entered inside the parent container.
         */
        bool back();

        /**
         * @brief Restores the previous saved navigation position.
         *
         * @return true if a previous navigation state was restored;
         * false if the history is empty.
         *
         * @note Unlike back(), undo() uses the navigation history rather
         * than following the tree's parent relationship.
         */
        bool undo();

        /**
         * @brief Navigates directly into a container using its element ID.
         *
         * @param id The ID of the container to navigate into.
         *
         * @return true if the specified ID exists and navigation succeeds;
         * false if the ID does not exist.
         *
         * @note Saves the current navigation position before navigating.
         * If the target is already the current container, no navigation
         * occurs and the method returns true.
         * The cursor starts at the first child, or at the self-entry
         * if the target has no children.
         */
        bool go(const std::string& id);

        /**
         * @brief Returns the number of siblings in the current container.
         *
         * @return The number of child elements, excluding the self-entry.
         */
        std::size_t getSiblingCount() const;

        /**
         * @brief Returns the number of children belonging to the selected element.
         *
         * @return The number of child elements, excluding the self-entry.
         *
         * @note Returns 0 when the selected element is a leaf.
         */
        std::size_t getChildrenCount() const;

        /**
         * @brief Returns a copy of the current container's children.
         *
         * @return An independent vector containing the current container's
         * child metadata.
         *
         * @note The self-entry at index 0 is excluded.
         * The returned vector is independent of TreeNavigator's internal
         * vector storage and will not be affected by subsequent mutations.
         */
        std::vector<Metadata> getSiblings() const;

        /**
         * @brief Returns a copy of the selected element's children.
         *
         * @return An independent vector containing the selected element's
         * child metadata.
         *
         * @note The self-entry at index 0 is excluded.
         * The returned vector is independent of TreeNavigator's internal
         * vector storage and will not be affected by subsequent mutations.
         */
        std::vector<Metadata> getChildren() const;

        /**
         * @brief Returns a read-only reference to the current container's
         * internal vector.
         *
         * @return A reference to the internal vector, including its
         * self-entry at index 0.
         *
         * @note Avoids the overhead of copying the vector.
         * References and iterators to individual elements may be invalidated
         * by vector mutations.
         * The returned vector reference becomes dangling if its corresponding
         * directory entry is erased, such as when its container is deleted.
         *
         * @warning Handle this reference carefully. Do not retain or use it
         * after an operation that may invalidate the referenced storage.
         * Use getSiblings() if an independent copy is preferred.
         */
        const std::vector<Metadata>& getSiblingsRef() const;

        /**
         * @brief Returns a read-only reference to the selected element's
         * internal vector.
         *
         * @return A reference to the internal vector, including its
         * self-entry at index 0.
         *
         * @note Avoids the overhead of copying the vector.
         * References and iterators to individual elements may be invalidated
         * by vector mutations.
         * The returned vector reference becomes dangling if its corresponding
         * directory entry is erased, such as when the selected element
         * or its subtree is deleted.
         *
         * @warning Handle this reference carefully. Do not retain or use it
         * after an operation that may invalidate the referenced storage.
         * Use getChildren() if an independent copy is preferred.
         */
        const std::vector<Metadata>& getChildrenRef() const;

        /**
         * @brief Appends a new sibling to the current container.
         *
         * @param sibling Payload describing the new sibling.
         *
         * @return 
         * ErrorCode::Success if successful.
         * 
         * Returns ErrorCode::ElementIdEmpty if the element ID is empty.
         * 
         * Returns ErrorCode::DuplicateElement if the ID already exists.
         *
         * @note The new element is appended to the end of the current
         * container's child list.
         */
        ErrorCode appendSibling(const Payload& sibling);

        /**
         * @brief Appends a new child to the currently selected element.
         *
         * @param child Payload describing the new child.
         *
         * @return 
         * ErrorCode::Success if successful.
         * 
         * Returns ErrorCode::ElementIdEmpty if the element ID is empty.
         * 
         * Returns ErrorCode::DuplicateElement if the ID already exists.
         *
         * @note The new element is appended to the end of the selected
         * element's child list.
         */
        ErrorCode appendChild(const Payload& child);

        /**
         * @brief Deletes the element currently selected by the cursor,
         * including all of its descendants.
         *
         * @return 
         * ErrorCode::Success if deletion succeeds.
         * 
         * ErrorCode::CannotDeleteRoot if the selected element
         * is the root.
         *
         * @note The cursor moves to a surviving sibling after deletion.
         * If no siblings remain, it moves to the parent's self-entry.
         * Navigation history entries referring to deleted containers
         * are also removed.
         *
         * @warning References and iterators associated with deleted
         * directory entries become invalid. References and iterators
         * to affected vector elements may also be invalidated.
         */
        ErrorCode deleteNode();


        /**
         * @brief Updates the payload of the element currently selected by the cursor.
         *
         * @param newNode The updated payload. Its elementId must match the currently
         * selected element's ID.
         *
         * @return 
         * ErrorCode::Success if the update succeeds.
         * 
         * ErrorCode::ElementIdEmpty if the provided elementId is empty.
         * 
         * ErrorCode::CannotChangeElementId if the provided elementId
         * differs from the selected element's ID.
         *
         * @note All other fields can be updated, including wrap and user-defined
         * custom payload members.
         * The elementId cannot be changed.
         * 
         * @example
         * const Metadata current = menu.current();
         * menu.updatePayload({current.elementId, true, 0, 2, 4, 5});
         */
        ErrorCode updatePayload(const Payload& newNode);

        /**
         * @brief Checks whether the current container is the root.
         *
         * @return true if currently navigating the root container;
         * false otherwise.
         */
        bool isRoot() const;

        /**
         * @brief Checks whether wrapping is enabled for the current container.
         *
         * @return true if wrapping is enabled; false otherwise.
         */
        bool isWrap() const;

        /**
         * @brief Returns the current cursor state.
         *
         * @return A CursorState containing the current container ID
         * and cursor index.
         *
         * @note The returned state is a copy and does not modify
         * the navigator's internal navigation state.
         */
        CursorState getCursorState() const;
};