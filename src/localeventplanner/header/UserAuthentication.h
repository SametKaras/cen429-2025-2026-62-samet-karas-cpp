/**
 * @file UserAuthentication.h
 * @brief Header file for user authentication and registration functionality.
 *
 * This header declares functions for user login, registration,
 * password validation, and secure memory operations.
 */

#ifndef USER_AUTHENTICATION_H
#define USER_AUTHENTICATION_H

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
 * @brief Main user authentication function.
 *
 * Displays authentication menu and handles user selection.
 *
 * @return bool True if authentication successful.
 */
LOCAL_EVENT_PLANNER_API bool userAuthentication();

/**
 * @brief Logs in an existing user.
 *
 * Validates credentials against the database.
 *
 * @param dbName Database file name (default: "users.db").
 * @return bool True if login successful.
 */
LOCAL_EVENT_PLANNER_API bool loginUser(const char* dbName = "users.db");

/**
 * @brief Registers a new user.
 *
 * Prompts for user information and stores it securely.
 *
 * @param dbName Database file name (default: "users.db").
 */
LOCAL_EVENT_PLANNER_API void registerUser(const char* dbName = "users.db");

/**
 * @brief Global guest mode flag.
 *
 * Indicates whether the application is in guest mode.
 */
LOCAL_EVENT_PLANNER_API extern bool isGuestMode;

/**
 * @brief Gets the current guest mode status.
 *
 * @return bool True if in guest mode.
 */
LOCAL_EVENT_PLANNER_API bool getGuestMode();

/**
 * @brief Sets the guest mode status.
 *
 * @param mode Guest mode value to set.
 */
LOCAL_EVENT_PLANNER_API void setGuestMode(bool mode);

/**
 * @brief Securely erases a string from memory.
 *
 * Overwrites the string contents with zeros to prevent memory dumps.
 *
 * @param str Reference to the string to erase.
 */
LOCAL_EVENT_PLANNER_API void secureErase(std::string& str);

/**
 * @brief Opens the user database.
 *
 * Opens or creates the SQLite database for user storage.
 *
 * @param dbName Database file name (default: "users.db").
 * @return sqlite3* Pointer to database connection, or nullptr on failure.
 */
LOCAL_EVENT_PLANNER_API sqlite3 *openUserDatabase(const char* dbName = "users.db");

/**
 * @brief Checks if a password meets strength requirements.
 *
 * Password must be at least 8 characters with uppercase, lowercase,
 * digit, and special character.
 *
 * @param password The password to check.
 * @return bool True if password is strong.
 */
LOCAL_EVENT_PLANNER_API bool isPasswordStrong(const std::string& password);

/**
 * @brief Gets password input with asterisk masking.
 *
 * Reads password character by character, displaying asterisks.
 *
 * @return std::string The entered password.
 */
LOCAL_EVENT_PLANNER_API std::string getPasswordInput();

/**
 * @brief Sets a mock key reader for testing purposes.
 *
 * @param reader Function pointer to the mock reader, or nullptr to reset.
 */
LOCAL_EVENT_PLANNER_API void setMockKeyReader(int (*reader)());

/// End C linkage
#ifdef __cplusplus
}

#endif

#endif // USER_AUTHENTICATION_H
