/**
 * @file DisplayMainMenu.h
 * @brief Header file for main menu display and console utilities.
 *
 * This header declares functions for displaying the main menu,
 * clearing the console, and tracking control flow steps.
 */

#ifndef DISPLAY_MAIN_MENU_H
#define DISPLAY_MAIN_MENU_H

/// DLL export/import macro definition
#ifdef LOCAL_EVENT_PLANNER_LIB_EXPORTS
  /// Export symbol when building the DLL
  #define LOCAL_EVENT_PLANNER_API __declspec(dllexport)
#else
  /// Import symbol when using the DLL
  #define LOCAL_EVENT_PLANNER_API __declspec(dllimport)
#endif

/// Include string header
#include <string>

/// C linkage for C++ compatibility
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Displays the main menu to the user.
 *
 * Shows menu options and handles user selection.
 *
 * @return bool True if program should exit, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool displayMainMenu();

/**
 * @brief Clears the console screen.
 *
 * Uses platform-specific commands to clear the terminal.
 */
LOCAL_EVENT_PLANNER_API void clearConsole();

/**
 * @brief Global step counter for control flow tracking.
 *
 * Tracks the number of steps executed during program flow.
 */
LOCAL_EVENT_PLANNER_API extern int step_counter;

/// End C linkage
#ifdef __cplusplus
}

#endif

#endif
