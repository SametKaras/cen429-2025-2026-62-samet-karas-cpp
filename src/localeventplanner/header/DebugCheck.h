/**
 * @file DebugCheck.h
 * @brief Header file for debugger detection and security checks.
 *
 * This header declares functions for detecting debuggers,
 * analyzing execution time, and checking memory integrity.
 */

#ifndef DEBUG_CHECK_H
#define DEBUG_CHECK_H

/// DLL export/import macro definition
#ifdef LOCAL_EVENT_PLANNER_LIB_EXPORTS
  /// Export symbol when building the DLL
  #define LOCAL_EVENT_PLANNER_API __declspec(dllexport)
#else
  /// Import symbol when using the DLL
  #define LOCAL_EVENT_PLANNER_API __declspec(dllimport)
#endif

/// C linkage for C++ compatibility
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Checks if a debugger is attached to the process.
 *
 * Uses platform-specific methods to detect debugger presence.
 * Terminates the program if a debugger is detected.
 */
LOCAL_EVENT_PLANNER_API void checkDebugger();

/**
 * @brief Checks execution time for debugging detection.
 *
 * Analyzes code execution time to detect breakpoints or
 * single-stepping by a debugger.
 */
LOCAL_EVENT_PLANNER_API void checkExecutionTime();

/**
 * @brief Checks memory integrity.
 *
 * Verifies that critical memory regions have not been
 * tampered with or modified by external tools.
 */
LOCAL_EVENT_PLANNER_API void checkMemory();

/// End C linkage
#ifdef __cplusplus
}

#endif

#endif
