/**
 * ============================================================
 * TreeNavigator - Basic Navigation
 * ============================================================
 *
 * @file basic_navigation.cpp
 * @brief Demonstrates the fundamental navigation operations
 *        provided by TreeNavigator.
 *
 * @version 1.0.0
 * @author John Virador
 *
 * This example demonstrates:
 * - Creating a tree configuration.
 * - Initializing a TreeNavigator instance.
 * - Moving between siblings using next() and prev().
 * - Retrieving the currently selected element.
 * - Retrieving the number of siblings.
 *
 * Navigation starts at the root container, with the cursor
 * positioned at its first child.
 *
 * ============================================================
 */

#include <iostream>
#include <vector>

#include "TreeNavigator.h"

int main() {
    /**
     * @brief Creates the initial tree configuration.
     *
     * TreeNavigator requires the root to be the first entry.
     * Parents must also be defined before their children.
     *
     * Tree structure:
     *
     * root
     * ├── settings
     * ├── files
     * └── about
     */
    std::vector<Metadata> config = {
        // Root
        Metadata("root", Payload{"root", true}),
        // Root children
        Metadata("root", Payload{"settings", true}),
        Metadata("root", Payload{"files", true}),
        Metadata("root", Payload{"about", true})
    };

    // 2. Initialize the navigator.
    TreeNavigator menu(config);

    std::cout << "---------- Basic Navigation ----------\n";

    // The cursor initially points to the first child of root.
    Metadata selected = menu.current();

    std::cout << "Selected element: "
              << selected.elementId << '\n';

    // 3. Move to the next sibling.
    menu.next();

    std::cout << "\nAfter next():\n";

    selected = menu.current();
    std::cout << "Selected element: "
              << selected.elementId << '\n';

    // 4. Move to the next sibling again.
    menu.next();

    std::cout << "\nAfter another next():\n";

    selected = menu.current();
    std::cout << "Selected element: "
              << selected.elementId << '\n';

    // 5. Move back to the previous sibling.
    menu.prev();

    std::cout << "\nAfter prev():\n";
    
    selected = menu.current();
    std::cout << "Selected element: "
              << selected.elementId << '\n';

    // 6. Retrieve the number of children in the current container.
    std::cout << "\nNumber of siblings: "
              << menu.getSiblingCount() << '\n';

    std::cout << "--------------------------------------\n";

    return 0;
}
