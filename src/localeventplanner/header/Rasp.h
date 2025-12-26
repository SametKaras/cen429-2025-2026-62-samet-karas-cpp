/**
 * @file Rasp.h
 * @brief Runtime Application Self-Protection (RASP) interface and helper macros.
 *
 * This header defines public APIs for emulator detection, hook detection,
 * CRC-based integrity verification, SSL/TLS initialization, and various
 * runtime security checks.
 */

#ifndef RASP_H                                  ///< Header include guard start.
#define RASP_H                                  ///< Header include guard definition.

#ifdef LOCAL_EVENT_PLANNER_LIB_EXPORTS           ///< When building the DLL (export mode).
  #define LOCAL_EVENT_PLANNER_API __declspec(dllexport)  /**< Exports symbols from the DLL. */
#else                                            ///< When consuming the DLL (import mode).
  #define LOCAL_EVENT_PLANNER_API __declspec(dllimport)  /**< Imports symbols from the DLL. */
#endif

#include <string>                               ///< std::string type.
#include <vector>                               ///< std::vector container.
#include <cstdint>                              ///< Fixed-width integer types.
#include <openssl/ssl.h>                        ///< OpenSSL SSL/TLS API (SSL_CTX, etc.).
#include <openssl/err.h>                        ///< OpenSSL error handling API.

#ifdef __cplusplus                              ///< If compiled with a C++ compiler.
extern "C" {                                   ///< Disable C++ name mangling.
#endif

/**
 * @brief List of known emulator manufacturers.
 *
 * The device manufacturer string is compared against this list
 * during emulator detection.
 */
LOCAL_EVENT_PLANNER_API extern const std::vector<std::string> EMULATOR_MANUFACTURERS;

/**
 * @brief List of known emulator models.
 *
 * The device model string is compared against this list
 * during emulator detection.
 */
LOCAL_EVENT_PLANNER_API extern const std::vector<std::string> EMULATOR_MODELS;

/**
 * @brief Retrieves the device manufacturer.
 *
 * Platform-specific mechanisms (e.g., system files or registry)
 * are used to obtain the manufacturer information.
 *
 * @return Manufacturer name, or "Unknown" if unavailable.
 */
LOCAL_EVENT_PLANNER_API std::string getDeviceManufacturer();

/**
 * @brief Retrieves the device model.
 *
 * @return Device model name, or "Unknown" if unavailable.
 */
LOCAL_EVENT_PLANNER_API std::string getDeviceModel();

/**
 * @brief Determines whether the application is running on an emulator.
 *
 * @return True if an emulator is detected, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool isEmulator();

/**
 * @brief Checks whether a file is writable.
 *
 * @param filePath Path to the file to be checked.
 * @return True if the file is writable, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool isFileWritable(const std::string& filePath);

/**
 * @brief Checks the integrity and accessibility of critical system files.
 *
 * @return True if all checks pass, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool checkCriticalSystemFiles();

/**
 * @brief Checks whether a function within a module has been hooked.
 *
 * @param moduleName Name of the module/DLL (e.g., "kernel32.dll").
 * @param functionName Name of the function to inspect.
 * @return True if a hook is suspected, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool isFunctionHooked(const char* moduleName, const char* functionName);

/**
 * @brief Performs hook detection on multiple critical API functions.
 *
 * @return True if any hook is detected, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool checkHooks();

/**
 * @brief Initializes the CRC32 lookup table.
 *
 * This function must be called before performing CRC32 calculations.
 */
LOCAL_EVENT_PLANNER_API void crc32_table_init();

/**
 * @brief Calculates the CRC32 checksum of a buffer.
 *
 * @param buf Pointer to the data buffer.
 * @param buf_len Length of the buffer in bytes.
 * @return Calculated CRC32 value.
 */
LOCAL_EVENT_PLANNER_API unsigned long crc32_calc(unsigned char* buf, int buf_len);

/**
 * @brief Verifies the integrity of a defined code block using CRC.
 *
 * @return True if integrity verification succeeds, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool verifyCodeBlock();

/**
 * @brief High-level helper function to start SSL/TLS operations.
 *
 * Typically initializes SSL libraries and performs a sample handshake.
 */
LOCAL_EVENT_PLANNER_API void StartSSL();

/**
 * @brief Creates and configures an OpenSSL client context.
 *
 * @return Pointer to an initialized SSL_CTX, or nullptr on failure.
 */
LOCAL_EVENT_PLANNER_API SSL_CTX* initializeSSLContext();

/**
 * @brief Performs TLS handshake and data exchange using an SSL context.
 *
 * @param ctx A previously initialized and configured SSL_CTX.
 */
LOCAL_EVENT_PLANNER_API void performSSLHandshakeAndDataExchange(SSL_CTX* ctx);

/**
 * @brief Application entry point after security checks.
 *
 * @return Application exit code.
 */
LOCAL_EVENT_PLANNER_API int startApp();

/* ============================ CRC MACROS ============================ */

/**
 * @brief Defines the start of a CRC-protected code block.
 *
 * @param label Identifier of the protected code block.
 *
 * @note This macro generates an empty function with the given label.
 */
#define CRC_START_BLOCK(label) void label(void) { }

/**
 * @brief Defines the end of a CRC-protected code block.
 *
 * @param label Identifier of the protected code block.
 *
 * @note This macro generates an empty function prefixed with an underscore.
 */
#define CRC_END_BLOCK(label) void _##label(void) { }

/**
 * @brief Calculates the byte length of a CRC-protected code block.
 *
 * @param label Identifier of the protected code block.
 * @return Length of the code block in bytes.
 */
#define CRC_BLOCK_LEN(label) (int)_##label - (int)label

/**
 * @brief Retrieves the start address of a CRC-protected code block.
 *
 * @param label Identifier of the protected code block.
 * @return Pointer to the start address of the code block.
 */
#define CRC_BLOCK_ADDR(label) (unsigned char*)label

#ifdef __cplusplus                              ///< End of C++ compatibility block.
}                                               ///< End of extern "C".
#endif

#endif // RASP_H                                 ///< Header include guard end.
