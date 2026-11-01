/**
 * ============================================================
 * TreeNavigator - Nested Navigation
 * ============================================================
 *
 * @file nested_navigation.cpp
 * @brief Demonstrates navigating through multiple levels
 *        of a hierarchical tree.
 *
 * @version 1.0.0
 * @author John Virador
 *
 * This example demonstrates:
 * - Entering child containers using enter().
 * - Retrieving the current container using getParentMetadata().
 * - Navigating between children at different hierarchy levels.
 * - Returning to parent containers using back().
 * - Detecting leaf elements using getChildrenCount().
 *
 * ============================================================
 */

#include <iostream>
#include <vector>

#include "TreeNavigator.h"

/**
 * @brief Creates a multi-level tree configuration.
 *
 * Parents are always declared before their children.
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

        // Display children
        Metadata("display", Payload{"brightness", true}),
        Metadata("display", Payload{"contrast", true}),

        // Sound children
        Metadata("sound", Payload{"volume", true}),
        Metadata("sound", Payload{"mute", true}),

        // Files children
        Metadata("files", Payload{"documents", true}),
        Metadata("files", Payload{"images", true}),

        // Documents children
        Metadata("documents", Payload{"reports", true}),

        // Images children
        Metadata("images", Payload{"photos", true})
    };
}

/**
 * @brief Prints the current container and selected element.
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

    // Initialize the navigator.
    TreeNavigator menu(config);

    std::cout << "---------- Nested Navigation ----------\n";

    // Initially, the current container is root.
    printNavigationState(menu);

    // 1. Enter the selected element: settings.
    menu.enter();

    std::cout << "After entering settings:\n";
    printNavigationState(menu);

    // 2. Enter the selected element: display.
    menu.enter();

    std::cout << "After entering display:\n";
    printNavigationState(menu);

    // 3. Select the next child: contrast.
    menu.next();

    std::cout << "After next():\n";
    printNavigationState(menu);

    // 4. Check whether the selected element is a leaf.
    if (menu.getChildrenCount() == 0) {
        std::cout << "- The selected element is a leaf.\n\n";
    }

    // 5. Return to the parent container: settings.
    if (menu.back()) {
        std::cout << "After back():\n";
        printNavigationState(menu);
    }

    // 6. Return to the root container.
    if (menu.back()) {
        std::cout << "After another back():\n";
        printNavigationState(menu);
    }

    // 7. Feel free to experiment with the remaining elements in the
    //    configuration (e.g., files, documents, images, and their children).
    //
    //    You can reuse the navigation functions demonstrated above:
    //    - next() and prev() to move between siblings.
    //    - enter() to navigate into a child container.
    //    - back() to return to the previous container.
    //    - getParentMetadata() and current() to inspect the
    //      current navigation state.
    //    - getChildrenCount() to determine whether an element
    //      has children.
    //
    //    You can also reuse printNavigationState() to display the
    //    current container and selected element after each operation.
    
    std::cout << "\033[F\r" << "---------------------------------------\n";

    return 0;
}