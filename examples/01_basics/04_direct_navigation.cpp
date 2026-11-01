/**
 * ============================================================
 * TreeNavigator - Direct Navigation
 * ============================================================
 *
 * @file direct_navigation.cpp
 * @brief Demonstrates navigating directly to containers using
 *        their element IDs.
 *
 * @version 1.0.0
 * @author John Virador
 *
 * This example demonstrates:
 * - Navigating directly using go().
 * - Navigating to deeply nested containers.
 * - Navigating directly to leaf elements.
 * - Handling nonexistent element IDs.
 * - Retrieving the current navigation state after navigation.
 *
 * Unlike next(), prev(), and enter(), go() does not require
 * manually traversing intermediate containers.
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
 * │   │   └── reports
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
        Metadata("files", Payload{"images", true}),

        // Documents children
        Metadata("documents", Payload{"reports", true})
    };
}

/**
 * @brief Prints the current navigation state.
 *
 * @param menu Reference to the TreeNavigator instance.
 */
void printNavigationState(const TreeNavigator& menu) {
    Metadata parent = menu.getParentMetadata();
    Metadata selected = menu.current();

    std::cout << "Current container: "
              << parent.elementId << '\n';

    std::cout << "Selected element: "
              << selected.elementId << "\n\n";
}

int main() {
    std::vector<Metadata> config = createConfiguration();

    TreeNavigator menu(config);

    std::cout << "--------------------- Direct Navigation ---------------------\n";

    // 1. Navigate directly to documents.
    // No need to enter files first.
    if (menu.go("documents")) {
        std::cout << "Successfully navigated to documents.\n";
        printNavigationState(menu);
    }

    // 2. Navigate directly to another container.
    if (menu.go("settings")) {
        std::cout << "Successfully navigated to settings.\n";
        printNavigationState(menu);
    }

    // 3. Navigate directly to a leaf element.
    // sound has no children, so the cursor remains on its self-entry.
    if (menu.go("about")) {
        std::cout << "Successfully navigated to about.\n";
        printNavigationState(menu);
    }

    // 4. Attempt to navigate to an element that does not exist.
    if (!menu.go("nonexistent")) {
        std::cout << "Navigation failed: element ID does not exist.\n";
    }

    std::cout << "-------------------------------------------------------------\n";

    return 0;
}