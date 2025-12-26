/**
 * @file SessionEncryption.h
 * @brief Public API for session key management and session data encryption/decryption.
 *
 * This header defines the interfaces for encrypting/decrypting session data,
 * managing encrypted session keys and IVs, and Base64 helper utilities.
 */

#ifndef SESSION_ENCRYPTION_H                               ///< Header include guard (start).
#define SESSION_ENCRYPTION_H                               ///< Header include guard definition.

#ifdef LOCAL_EVENT_PLANNER_LIB_EXPORTS                     ///< When building the DLL.
  #define LOCAL_EVENT_PLANNER_API __declspec(dllexport)    /**< Export symbols from the DLL. */
#else                                                      ///< When consuming the DLL.
  #define LOCAL_EVENT_PLANNER_API __declspec(dllimport)    /**< Import symbols from the DLL. */
#endif

#include <string>                                         ///< std::string type.
#include "sqlite3.h"                                      ///< sqlite3_stmt type for database access.
#include <vector>                                         ///< std::vector container.

/* ====================== SESSION DATA ENCRYPTION ====================== */

/**
 * @brief Encrypts session-related data.
 *
 * @param data Plain-text session data to be encrypted.
 * @return Encrypted session data as a string.
 */
LOCAL_EVENT_PLANNER_API std::string encryptSessionData(const std::string& data);

/**
 * @brief Decrypts previously encrypted session data.
 *
 * @param encryptedData Encrypted session data.
 * @return Decrypted plain-text session data.
 */
LOCAL_EVENT_PLANNER_API std::string decryptSessionData(const std::string& encryptedData);

/* ====================== KEY ENCRYPTION / DECRYPTION ====================== */

/**
 * @brief Encrypts a raw binary key.
 *
 * @param key Pointer to the raw key buffer.
 * @param keyLen Length of the key in bytes.
 * @return Encrypted key as a string.
 */
LOCAL_EVENT_PLANNER_API std::string encryptKey(const unsigned char* key, size_t keyLen);

/**
 * @brief Decrypts an encrypted key.
 *
 * @param encryptedKey Encrypted key string.
 * @param key Output buffer where the decrypted key will be written.
 * @param keyLen Length of the output key buffer in bytes.
 * @return True if decryption succeeds, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool decryptKey(const std::string& encryptedKey,
                                       unsigned char* key,
                                       size_t keyLen);

/* ====================== SESSION KEY & IV MANAGEMENT ====================== */

/**
 * @brief Retrieves the encrypted session key.
 *
 * @return Encrypted session key.
 */
LOCAL_EVENT_PLANNER_API std::string getEncryptedSessionKey();

/**
 * @brief Retrieves the encrypted session initialization vector (IV).
 *
 * @return Encrypted session IV.
 */
LOCAL_EVENT_PLANNER_API std::string getEncryptedSessionIV();

/**
 * @brief Sets the encrypted session key.
 *
 * @param encryptedSessionKey Encrypted session key.
 * @return True if the key is set successfully, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool setSessionKey(const std::string& encryptedSessionKey);

/**
 * @brief Sets the encrypted session initialization vector (IV).
 *
 * @param encryptedSessionIV Encrypted session IV.
 * @return True if the IV is set successfully, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool setSessionIV(const std::string& encryptedSessionIV);

/* ====================== SESSION ENCRYPTION LIFECYCLE ====================== */

/**
 * @brief Initializes session encryption and manages key/IV generation.
 *
 * @return True if session encryption is successfully set up, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool setupSessionEncryption();

/**
 * @brief Collects user data and encrypts it as session data.
 *
 * Typically operates on user data retrieved via a SQLite prepared statement.
 *
 * @param stmt SQLite prepared statement containing user information.
 * @return True if the operation succeeds, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool collectAndEncryptSessionData(sqlite3_stmt* stmt);

/* ====================== BASE64 HELPER FUNCTIONS ====================== */

/**
 * @brief Encodes binary data into a Base64 string.
 *
 * @param buffer Pointer to the binary data buffer.
 * @param length Length of the buffer in bytes.
 * @return Base64-encoded string.
 */
LOCAL_EVENT_PLANNER_API extern std::string base64Encode(const unsigned char* buffer,
                                                        size_t length);

/**
 * @brief Decodes a Base64-encoded string into binary data.
 *
 * @param encodedData Base64-encoded input string.
 * @return Decoded binary data as a vector of unsigned char.
 */
LOCAL_EVENT_PLANNER_API std::vector<unsigned char> base64Decode(const std::string& encodedData);

#endif // SESSION_ENCRYPTION_H                              ///< Header include guard (end).
