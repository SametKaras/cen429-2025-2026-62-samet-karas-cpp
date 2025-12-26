/**
 * @file EventDetails.h
 * @brief Header file for event management functionality.
 *
 * This header declares functions for creating, managing, and
 * listing events, as well as database operations.
 */

#ifndef EVENT_DETAILS_H
#define EVENT_DETAILS_H

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
/// Include SQLite3 header
#include "sqlite3.h"

/// C linkage for C++ compatibility
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Main event details function.
 *
 * Displays event details menu and handles user input.
 */
LOCAL_EVENT_PLANNER_API void eventDetails();

/**
 * @brief Creates a new event.
 *
 * Prompts user for event information and stores it in the database.
 */
LOCAL_EVENT_PLANNER_API void createEvent();

/**
 * @brief Manages existing events.
 *
 * Allows editing and updating of event details.
 */
LOCAL_EVENT_PLANNER_API void manageEvents();

/**
 * @brief Lists all events.
 *
 * Retrieves and displays all events from the database.
 */
LOCAL_EVENT_PLANNER_API void listEvents();

/**
 * @brief Opens the event database.
 *
 * Opens or creates the SQLite database for event storage.
 *
 * @return sqlite3* Pointer to database connection, or nullptr on failure.
 */
LOCAL_EVENT_PLANNER_API sqlite3 *openEventDatabase();

/// End C linkage
#ifdef __cplusplus
}

#endif

#endif
