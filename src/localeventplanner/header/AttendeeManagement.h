/**
 * @file AttendeeManagement.h
 * @brief Header file for attendee management functionality.
 *
 * This header declares functions for managing event attendees,
 * including registration, tracking, and database operations.
 */

#ifndef ATTENDEE_MANAGEMENT_H
#define ATTENDEE_MANAGEMENT_H

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
 * @brief Main attendee management function.
 *
 * Displays the attendee management menu and handles user input.
 */
LOCAL_EVENT_PLANNER_API void attendeeManagement();

/**
 * @brief Registers attendees for an event.
 *
 * Prompts for attendee information and stores it in the database.
 */
LOCAL_EVENT_PLANNER_API void registerAttendees();

/**
 * @brief Tracks and displays registered attendees.
 *
 * Lists all attendees for a specific event from the database.
 */
LOCAL_EVENT_PLANNER_API void trackAttendees();

/**
 * @brief Opens the attendee database.
 *
 * Opens or creates the SQLite database for attendee storage.
 *
 * @return sqlite3* Pointer to database connection, or nullptr on failure.
 */
LOCAL_EVENT_PLANNER_API sqlite3 *openAttendeeDatabase();

/// End C linkage
#ifdef __cplusplus
}

#endif

#endif
