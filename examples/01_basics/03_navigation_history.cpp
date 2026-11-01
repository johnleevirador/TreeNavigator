/**
 * ============================================================
 * TreeNavigator - Navigation History
 * ============================================================
 *
 * @file navigation_history.cpp
 * @brief Demonstrates the difference between hierarchy-based
 *        navigation (back()) and history-based navigation
 *        (undo()).
 *
 * @version 1.0.0
 * @author John Virador
 *
 * This example demonstrates:
 * - How enter() saves navigation positions in the history stack.
 * - Restoring previously saved positions using undo().
 * - Navigating to parent containers using back().
 * - Why undo() and back() produce different navigation results.
 *
 * KEY DIFFERENCE:
 *
 * undo() restores a previously saved navigation state from the
 * history stack. It does not necessarily return to the parent
 * container of the currently selected element.
 *
 * back() follows the actual tree hierarchy, returning to the
 * parent container of the currently selected element.
 *
 * PERFORMANCE NOTE:
 *
 * undo() restores a previously saved navigation state directly
 * from the history stack, providing O(1) restoration.
 *
 * back() follows the tree hierarchy and performs a linear search
 * to locate the previously selected element in the parent
 * container, resulting in O(n) lookup.
 *
 * For applications that primarily use step-based navigation
 * (next(), prev(), and enter()) without go(), undo() is generally
 * the recommended way to restore previous navigation positions.
 *
 * ============================================================
 */

#include <iostream>
#include <vector>

#include "TreeNavigator.h"

/**
 * @brief Creates the tree configuration.
 *
 * Tree structure:
 *
 * root
 * ├── settings
 * │   ├── display
 * │   └── sound
 * ├── files
 * │   ├── documents
 * │   └── images
 * └── about
 */
std::vector<Metadata> createConfiguration() {
    return {
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
}

/**
 * @brief Prints the current navigation position.
 *
 * @param menu Reference to the TreeNavigator instance.
 */
void printNavigationState(const TreeNavigator& menu) {
    Metadata parent = menu.getParentMetadata();
    Metadata selected = menu.current();

    std::cout << "Container: "
              << parent.elementId << '\n';

    std::cout << "Selected: "
              << selected.elementId << "\n\n";
}

/**
 * @brief Demonstrates history-based navigation restoration.
 *
 * undo() restores a previously saved navigation position
 * instead of following the tree's parent-child relationships.
 *
 * This approach is recommended when the application primarily
 * relies on step-based navigation.
 */
void demonstrateUndo() {
    std::vector<Metadata> config = createConfiguration();
    TreeNavigator menu(config);

    std::cout << "---------- Using undo() ----------\n";

    // Initially, settings is selected inside root.

    // Select files instead of settings.
    menu.next();

    std::cout << "After next():\n";
    printNavigationState(menu);

    // Save the current position and enter files.
    // The previous position (root -> files) is saved in history.
    menu.enter();

    std::cout << "After entering files:\n";
    printNavigationState(menu);

    // Select images inside files.
    menu.next();

    std::cout << "After next():\n";
    printNavigationState(menu);

    std::cout << "Before undo():\n";
    printNavigationState(menu);

    // Restore the previously saved navigation position.
    // undo() returns to root -> files, NOT the parent of images.
    if (menu.undo()) {
        std::cout << "After undo():\n";
        printNavigationState(menu);
    }

    // Attempt another undo().
    // No previous navigation state remains in the history stack.
    if (menu.undo() == false) {
        std::cout << "No previous navigation state available.\n";
        printNavigationState(menu);
    }

    std::cout << "\033[F\r" << "----------------------------------\n\n";
}

/**
 * @brief Demonstrates hierarchy-based parent navigation.
 *
 * back() follows the actual parent-child relationship rather
 * than simply restoring the previous history entry.
 *
 * Unlike undo(), back() navigates upward through the tree
 * hierarchy, regardless of the previous navigation position.
 */
void demonstrateBack() {
    std::vector<Metadata> config = createConfiguration();
    TreeNavigator menu(config);

    std::cout << "---------- Using back() ----------\n";

    // Initially, settings is selected inside root.

    // Select files.
    menu.next();

    std::cout << "After next():\n";
    printNavigationState(menu);

    // Enter files.
    menu.enter();

    std::cout << "After entering files:\n";
    printNavigationState(menu);


    // Select images inside files.
    menu.next();

    std::cout << "After next():\n";
    printNavigationState(menu);

    // Enter images.
    menu.enter();

    std::cout << "After entering images:\n";
    printNavigationState(menu);

    std::cout << "Before back():\n";
    printNavigationState(menu);

    // Return to the structural parent: files.
    // back() does not restore the previous history entry.
    // Instead, it follows the tree hierarchy.
    if (menu.back()) {
        std::cout << "After back():\n";
        printNavigationState(menu);
    }

    std::cout << "\033[F\r" << "----------------------------------\n";
}

int main() {
    demonstrateUndo();
    demonstrateBack();

    return 0;
}

/**
 * ============================================================
 * back() vs undo()
 * ============================================================
 *
 * back():
 * - Follows the tree's parent-child relationships.
 * - Returns to the parent container of the current element.
 * - Does not simply restore the previous history entry.
 *
 * undo():
 * - Follows the navigation history stack.
 * - Restores the previously saved navigation position.
 * - Does not necessarily return to the structural parent.
 *
 * Use back() when you want to navigate upward through the tree.
 *
 * Use undo() when you want to restore a previously saved
 * navigation position.
 *
 * ============================================================
 */