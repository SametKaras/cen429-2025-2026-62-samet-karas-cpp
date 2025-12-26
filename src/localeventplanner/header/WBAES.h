/**
 * @file WBAES.h
 * @brief Public interface for White-Box AES–related utilities and helper functions.
 *
 * This header defines the APIs and shared data used for White-Box AES–style
 * encryption/decryption, key derivation, and vector/string conversion helpers.
 */

#ifndef WBAES_H                                   ///< Header include guard (start).
#define WBAES_H                                   ///< Header include guard definition.

#include <vector>                                 ///< std::vector container.
#include <string>                                 ///< std::string type.

#ifdef LOCAL_EVENT_PLANNER_LIB_EXPORTS             ///< When building the DLL.
  #define LOCAL_EVENT_PLANNER_API __declspec(dllexport)  /**< Export symbols from the DLL. */
#else                                              ///< When consuming the DLL.
  #define LOCAL_EVENT_PLANNER_API __declspec(dllimport)  /**< Import symbols from the DLL. */
#endif

/**
 * @brief Creates and returns a White-Box AES lookup table.
 *
 * The generated table is typically used internally for obfuscated AES operations.
 *
 * @return A vector containing the generated White-Box lookup table.
 */
LOCAL_EVENT_PLANNER_API std::vector<int> createWhiteBoxTable();

/**
 * @brief Encrypts plaintext using a White-Box AES approach.
 *
 * @param plaintext Plain-text input to be encrypted.
 * @param key Encryption key used for the White-Box AES operation.
 * @return Ciphertext represented as a vector of integers.
 */
LOCAL_EVENT_PLANNER_API std::vector<int> whiteBoxAesEncrypt(const std::string& plaintext,
                                                            const std::string& key);

/**
 * @brief Decrypts White-Box AES ciphertext back to plaintext.
 *
 * @param ciphertext Ciphertext represented as a vector of integers.
 * @param key Decryption key used for the White-Box AES operation.
 * @return Decrypted plaintext string.
 */
LOCAL_EVENT_PLANNER_API std::string whiteBoxAesDecrypt(const std::vector<int>& ciphertext,
                                                       const std::string& key);

/**
 * @brief Converts a vector of integers into a string representation.
 *
 * @param vec Vector to be converted.
 * @return String representation of the vector.
 */
LOCAL_EVENT_PLANNER_API std::string vectorToString(const std::vector<int>& vec);

/**
 * @brief Converts a string representation back into a vector of integers.
 *
 * @param str String to be converted.
 * @return Vector of integers parsed from the string.
 */
LOCAL_EVENT_PLANNER_API std::vector<int> stringToVector(const std::string& str);

/**
 * @brief Derives a cryptographic key from an S-Box and a seed value.
 *
 * @param keyLength Desired length of the derived key.
 * @param seed Seed value used during key derivation.
 * @return Derived key as a vector of integers.
 */
LOCAL_EVENT_PLANNER_API std::vector<int> deriveKeyFromSBox(size_t keyLength,
                                                           unsigned char seed);

/**
 * @brief Global seed value used for White-Box AES key derivation.
 *
 * @note This variable is defined elsewhere and imported/exported via the DLL.
 */
LOCAL_EVENT_PLANNER_API extern unsigned char seed;

/**
 * @brief Global key length used for White-Box AES operations.
 *
 * @note This variable is defined elsewhere and imported/exported via the DLL.
 */
LOCAL_EVENT_PLANNER_API extern size_t keyLength;

/**
 * @brief Global AES S-Box table used in White-Box transformations.
 *
 * @note This table is constant and shared across White-Box AES operations.
 */
LOCAL_EVENT_PLANNER_API extern const std::vector<int> S_BOX;

#endif // WBAES_H                                 ///< Header include guard (end).
