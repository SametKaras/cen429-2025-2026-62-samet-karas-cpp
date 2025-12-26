/**
 * @file SignatureVerification.h
 * @brief Public API for executable signature verification and caller process inspection.
 *
 * This header defines the interfaces used to retrieve the caller process path,
 * read files from disk, and verify digital signatures for integrity and authenticity checks.
 */

#ifndef SIGNATURE_VERIFICATION_H                     ///< Header include guard (start).
#define SIGNATURE_VERIFICATION_H                     ///< Header include guard definition.

#ifdef LOCAL_EVENT_PLANNER_LIB_EXPORTS               ///< When building the DLL.
  #define LOCAL_EVENT_PLANNER_API __declspec(dllexport)  /**< Export symbols from the DLL. */
#else                                                ///< When consuming the DLL.
  #define LOCAL_EVENT_PLANNER_API __declspec(dllimport)  /**< Import symbols from the DLL. */
#endif

#include <string>                                   ///< std::string type.

#ifdef __cplusplus                                  ///< If compiled with a C++ compiler.
extern "C" {                                       ///< Disable C++ name mangling for exported symbols.
#endif

/**
 * @brief Retrieves the full file system path of the calling process.
 *
 * This function determines the executable path of the process that invoked the DLL.
 *
 * @return Absolute path of the caller process executable.
 */
LOCAL_EVENT_PLANNER_API std::string getCallerProcessPath();

/**
 * @brief Reads the contents of a file into a string.
 *
 * The file is read in binary mode and returned as a single string.
 *
 * @param filePath Absolute or relative path to the file.
 * @return File contents as a string.
 */
LOCAL_EVENT_PLANNER_API std::string readFile(const std::string& filePath);

/**
 * @brief Verifies the digital signature of the calling executable.
 *
 * Typically used to ensure that the executable has not been tampered with
 * and was signed using a trusted private key.
 *
 * @return True if the signature verification succeeds, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool verifySignature();

/**
 * @brief Starts the signature verification process and reports the result.
 *
 * This function acts as a high-level entry point that triggers signature
 * verification and handles the success/failure flow.
 *
 * @return True if verification succeeds, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool startVerification();

#ifdef __cplusplus                                  ///< End of C++ compatibility block.
}                                                   ///< extern "C" block end.
#endif

#endif // SIGNATURE_VERIFICATION_H                  ///< Header include guard (end).
