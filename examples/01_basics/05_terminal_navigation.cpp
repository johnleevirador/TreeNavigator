/**
 * ============================================================
 * TreeNavigator - Terminal Navigation
 * ============================================================
 *
 * @file terminal_navigation.cpp
 * @brief Demonstrates an interactive terminal-based navigation
 *        menu using TreeNavigator's core navigation APIs.
 *
 * @version 1.0.0
 * @author John Virador
 *
 * This example demonstrates:
 * - Initializing TreeNavigator with a predefined tree structure.
 * - Navigating between siblings using next() and prev().
 * - Entering containers using enter().
 * - Returning to parent containers using back().
 * - Restoring previous navigation states using undo().
 * - Navigating directly to elements using go().
 * - Retrieving sibling and child counts.
 * - Displaying the tree structure and current selection.
 * - Validating user input and handling invalid entries.
 * - Handling unsuccessful navigation operations.
 *
 * The example combines the concepts introduced in the basic
 * navigation examples into an interactive terminal application.
 * Users can select operations from a numbered menu and observe
 * how TreeNavigator responds to each operation.
 *
 * TreeNavigator handles the navigation logic, while the
 * application manages user input, terminal rendering, and
 * interaction.
 *
 * Note:
 * This example currently targets Windows because it uses
 * Windows-specific console functionality, including Windows.h,
 * Sleep(), and system("cls").
 *
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <Windows.h>
#include "TreeNavigator.h"


/**
 * ============================================================
 * Menu Options
 * ============================================================
 *
 * @enum Menu
 * @brief Defines the available operations in the terminal menu.
 *
 * Each enumerator represents an operation that the user can
 * select through the terminal interface.
 *
 * The assigned values correspond to the numbered options
 * displayed by printAPI().
 *
 * ============================================================
 */
enum Menu : int {
    Next = 1,           // Move to the next sibling.
    Prev,               // Move to the previous sibling.
    Enter,              // Enter the currently selected element.
    Go,                 // Navigate directly to an element by ID.
    Back,               // Return to the structural parent container.
    Undo,               // Restore the previous saved navigation state.
    GetSiblingCount,    // Retrieve the number of siblings.
    GetChildrenCount,   // Retrieve the number of children.
    Exit                // Terminate the application.
};


/**
 * ============================================================
 * Configuration
 * ============================================================
 */

/**
 * @brief Creates the initial tree configuration.
 *
 * Defines the parent-child relationships used to initialize
 * TreeNavigator.
 *
 * The first entry represents the root. Every subsequent entry
 * defines an element and its parent container.
 *
 * Elements must be declared after their respective parents.
 *
 * @return A vector containing the metadata for the entire tree.
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
 * ============================================================
 * Terminal Display
 * ============================================================
 */

/**
 * @brief Displays the predefined tree structure.
 *
 * Prints the complete directory hierarchy in the terminal.
 * The currently selected element is marked with a left-pointing
 * indicator.
 *
 * This function only handles visual representation. It does
 * not retrieve or modify the navigation state maintained by
 * TreeNavigator.
 *
 * @param selectedId The element ID of the currently selected
 *                   element.
 */
void printDirectory(const std::string& selectedId) {
    std::cout << "root " << (selectedId == "root" ? "◄" : "") << "\n";
    std::cout << "├── settings " << (selectedId == "settings" ? "◄" : "") << "\n";
    std::cout << "│   ├── display " << (selectedId == "display" ? "◄" : "") << "\n";
    std::cout << "│   └── sound " << (selectedId == "sound" ? "◄" : "") << "\n";
    std::cout << "├── files " << (selectedId == "files" ? "◄" : "") << "\n";
    std::cout << "│   ├── documents " << (selectedId == "documents" ? "◄" : "") << "\n";
    std::cout << "│   │   └── reports " << (selectedId == "reports" ? "◄" : "") << "\n";
    std::cout << "│   └── images " << (selectedId == "images" ? "◄" : "") << "\n";
    std::cout << "└── about " << (selectedId == "about" ? "◄" : "") << "\n";
}


/**
 * @brief Displays the available TreeNavigator operations.
 *
 * Prints the numbered menu options that users can select.
 *
 * The displayed numbers correspond to the values defined
 * in the Menu enum.
 */
void printAPI() {
    std::cout <<
    "====== TreeNav API ======\n"
    "[1] Next\n"
    "[2] Prev\n"
    "[3] Enter\n"
    "[4] Go\n"
    "[5] Back\n"
    "[6] Undo\n"
    "[7] GetSiblingCount\n"
    "[8] GetChildrenCount\n"
    "[9] Exit\n"
    "=========================\n";
}


/**
 * @brief Clears the terminal screen.
 *
 * Uses the Windows command interpreter to clear the console.
 *
 * Note:
 * This implementation is Windows-specific because it uses
 * the "cls" command.
 */
void clearScreen() {
    system("cls");
}


/**
 * ============================================================
 * Input Validation
 * ============================================================
 *
 * @enum ErrorCodeInput
 * @brief Represents the possible results of integer input
 *        validation.
 *
 * These values allow getInputInt() to report why an input
 * could not be accepted.
 *
 * ============================================================
 */
enum class ErrorCodeInput : int {
    Success = 0,        // Input was successfully converted and validated.
    TrailingNonInt,     // Input contains characters after the integer.
    OutOfBounds,        // Converted integer is outside the allowed range.
    NonInteger,         // Input cannot be converted into an integer.
    IntegerOverflow,    // Input exceeds the range supported by int.
    InputClosed         // Input stream was closed or failed.
};


/**
 * @brief Reads and validates an integer from standard input.
 *
 * Reads a complete line from the terminal and attempts to
 * convert it into an integer using std::stoi().
 *
 * After conversion, the function checks:
 * 1. Whether unexpected characters remain after the integer.
 * 2. Whether the converted value is within the specified range.
 *
 * Leading whitespace is accepted by std::stoi(). Characters
 * remaining after the converted integer are rejected.
 *
 * @param number Reference to the variable where the converted
 *               integer will be stored.
 * @param min    Minimum accepted integer value.
 * @param max    Maximum accepted integer value.
 *
 * @return ErrorCodeInput indicating the result of validation:
 *         - Success: Input is valid.
 *         - TrailingNonInt: Unexpected trailing characters.
 *         - OutOfBounds: Value is outside the allowed range.
 *         - NonInteger: Input is not a valid integer.
 *         - IntegerOverflow: Value exceeds the int range.
 *         - InputClosed: Standard input could not be read.
 */
ErrorCodeInput getInputInt(int& number, const int min, const int max) {
    std::string input;

    // Read the complete input line.
    // Stop processing if the input stream is closed or fails.
    if (!std::getline(std::cin, input)) {
        return ErrorCodeInput::InputClosed;
    }

    try {
        size_t processedChars = 0;

        // Convert the input string into an integer.
        // processedChars records how many characters were consumed.
        number = std::stoi(input, &processedChars);

        // 1. Check for trailing non-numeric characters.
        // Example: "42abc" is rejected because not all characters
        // were consumed during conversion.
        if (processedChars != input.length()) {
            return ErrorCodeInput::TrailingNonInt;
        }

        // 2. Check whether the converted integer is within range.
        if (number < min || number > max) {
            return ErrorCodeInput::OutOfBounds;
        }

    } catch (const std::invalid_argument& e) {

        // std::stoi() throws invalid_argument when the input
        // does not contain a valid integer.
        return ErrorCodeInput::NonInteger;

    } catch (const std::out_of_range&) {

        // std::stoi() throws out_of_range when the converted
        // value cannot fit into an int.
        return ErrorCodeInput::IntegerOverflow;
    }

    return ErrorCodeInput::Success;
}


/**
 * ============================================================
 * Main Application
 * ============================================================
 */

/**
 * @brief Runs the interactive TreeNavigator demonstration.
 *
 * Initializes the tree configuration and creates a TreeNavigator
 * instance. The application then enters an interactive loop
 * where users can select navigation operations.
 *
 * Each iteration:
 * 1. Clears the terminal screen.
 * 2. Displays the tree and current selection.
 * 3. Displays the available API operations.
 * 4. Reads and validates the user's menu selection.
 * 5. Executes the selected TreeNavigator operation.
 * 6. Updates the displayed current metadata when necessary.
 *
 * The application terminates when the user selects Exit or
 * when standard input is closed.
 *
 * @return 0 when the application terminates normally.
 */
int main() {

    // Configure the Windows console to display UTF-8 characters.
    // This allows the tree-drawing characters to render correctly
    // in terminals that support UTF-8 output.
    SetConsoleOutputCP(CP_UTF8);


    // --------------------------------------------------------
    // INITIALIZATION
    // --------------------------------------------------------

    // Create the predefined tree configuration.
    std::vector<Metadata> config = createConfiguration();

    // Initialize TreeNavigator using the configuration.
    TreeNavigator menu(config);

    // Retrieve the initial selection.
    // This metadata is used by printDirectory() to display
    // the currently selected element.
    Metadata current = menu.current();


    // --------------------------------------------------------
    // INTERACTIVE MENU LOOP
    // --------------------------------------------------------

    while (true) {

        // Clear the previous terminal output.
        clearScreen();

        // Display the tree and highlight the current selection.
        printDirectory(current.elementId);

        std::cout << "\n";

        // Display the available navigation operations.
        printAPI();


        // ----------------------------------------------------
        // READ USER INPUT
        // ----------------------------------------------------

        int userInput = 0;

        std::cout << "Enter: ";

        // Accept only menu selections between 1 and 9.
        ErrorCodeInput result = getInputInt(userInput, 1, 9);


        // ----------------------------------------------------
        // VALIDATE USER INPUT
        // ----------------------------------------------------

        bool isContinue = false;

        switch (result) {

            case ErrorCodeInput::OutOfBounds:
                std::cout << "Input must be between 1 and 9.";
                isContinue = true;
                break;

            case ErrorCodeInput::NonInteger:
                std::cout << "Please enter an integer.";
                isContinue = true;
                break;

            case ErrorCodeInput::TrailingNonInt:
                std::cout << "Unexpected characters detected.";
                isContinue = true;
                break;

            case ErrorCodeInput::IntegerOverflow:
                std::cout << "Integer is too large.";
                isContinue = true;
                break;

            case ErrorCodeInput::InputClosed:

                // Exit the application if standard input is closed.
                std::cout << "Input Closed: Exiting Tree Navigator....";
                return 0;

            case ErrorCodeInput::Success:

                // Continue to the menu operation switch below.
                break;
        }

        // If validation failed, display the error message
        // before returning to the beginning of the menu loop.
        if (isContinue) {
            Sleep(2000); // Wait 2 seconds for the user to read the error.
            continue;
        }


        // ----------------------------------------------------
        // EXECUTE SELECTED TREE NAVIGATION OPERATION
        // ----------------------------------------------------

        switch (userInput) {

            // ------------------------------------------------
            // NEXT
            // ------------------------------------------------
            case Menu::Next: {

                // Move the cursor to the next sibling.
                menu.next();

                // Refresh the currently selected metadata.
                current = menu.current();

                break;
            }


            // ------------------------------------------------
            // PREV
            // ------------------------------------------------
            case Menu::Prev: {

                // Move the cursor to the previous sibling.
                menu.prev();

                // Refresh the currently selected metadata.
                current = menu.current();

                break;
            }


            // ------------------------------------------------
            // ENTER
            // ------------------------------------------------
            case Menu::Enter: {

                // Enter the currently selected element.
                // If the selected element has children, navigation
                // moves into that container.
                menu.enter();

                // Refresh the currently selected metadata.
                current = menu.current();

                break;
            }


            // ------------------------------------------------
            // GO
            // ------------------------------------------------
            case Menu::Go: {

                // Move the terminal cursor to the previous line
                // and clear it before displaying the element ID prompt.
                std::cout << "\033[F\033[K" << "Enter Element Id: ";

                std::string input;

                // Read the element ID entered by the user.
                std::getline(std::cin, input);

                // Attempt to navigate directly to the specified ID.
                bool success = menu.go(input);

                if (!success) {

                    // Display an error if the requested element
                    // does not exist in the tree.
                    std::cout << "Element Id Doesnt Exist";

                    Sleep(3000);

                    break;
                }

                // Refresh the current metadata after successful navigation.
                current = menu.current();

                break;
            }


            // ------------------------------------------------
            // BACK
            // ------------------------------------------------
            case Menu::Back: {

                // Return to the structural parent container.
                // back() returns false if navigation cannot
                // move to a parent container.
                bool success = menu.back();

                if (!success) {
                    std::cout << "Cannot go back any further.";
                    Sleep(2000);
                }

                // Refresh the current metadata.
                current = menu.current();

                break;
            }


            // ------------------------------------------------
            // UNDO
            // ------------------------------------------------
            case Menu::Undo: {

                // Restore the previous saved navigation state.
                // undo() returns false if no history is available.
                bool success = menu.undo();

                if (!success) {
                    std::cout << "No navigation history available.";
                    Sleep(2000);
                }

                // Refresh the current metadata.
                current = menu.current();

                break;
            }


            // ------------------------------------------------
            // GET SIBLING COUNT
            // ------------------------------------------------
            case Menu::GetSiblingCount: {

                // Retrieve the number of children belonging to
                // the current navigation container.
                int siblingCount = menu.getSiblingCount();

                std::cout << "Sibling Count: " << siblingCount;

                // Keep the result visible before refreshing the menu.
                Sleep(2000);

                break;
            }


            // ------------------------------------------------
            // GET CHILDREN COUNT
            // ------------------------------------------------
            case Menu::GetChildrenCount: {

                // Retrieve the number of children belonging to
                // the currently selected element.
                int childrenCount = menu.getChildrenCount();

                std::cout << "Children Count: " << childrenCount;

                // Keep the result visible before refreshing the menu.
                Sleep(2000);

                break;
            }


            // ------------------------------------------------
            // EXIT
            // ------------------------------------------------
            case Menu::Exit: {

                // Terminate the application.
                std::cout << "Exiting TreeNavigator...";

                return 0;
            }
        }
    }

    return 0;
}