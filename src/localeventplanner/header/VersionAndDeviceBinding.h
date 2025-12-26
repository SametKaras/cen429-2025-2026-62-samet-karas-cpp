/**
 * @file VersionAndDeviceBinding.h
 * @brief Public API for application version and device compatibility checks.
 *
 * This header defines interfaces for validating application version compatibility,
 * runtime environment suitability, and device brand–based binding checks.
 */

#ifndef VERSION_AND_DEVICE_BINDING_H               ///< Header include guard (start).
#define VERSION_AND_DEVICE_BINDING_H               ///< Header include guard definition.

#ifdef LOCAL_EVENT_PLANNER_LIB_EXPORTS             ///< When building the DLL.
  #define LOCAL_EVENT_PLANNER_API __declspec(dllexport)  /**< Export symbols from the DLL. */
#else                                              ///< When consuming the DLL.
  #define LOCAL_EVENT_PLANNER_API __declspec(dllimport)  /**< Import symbols from the DLL. */
#endif

#include <string>                                  ///< std::string type.

#ifdef __cplusplus                                 ///< If compiled with a C++ compiler.
extern "C" {                                      ///< Disable C++ name mangling for exported symbols.
#endif

/**
 * @brief Checks whether the current application version is compatible.
 *
 * @param currentVersion Current application version string.
 * @param requiredVersion Required (minimum or exact) version string.
 * @return True if the version is compatible, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool isVersionCompatible(const std::string& currentVersion,
                                                 const std::string& requiredVersion);

/**
 * @brief Checks whether the runtime environment is compatible with the application.
 *
 * This function typically aggregates version checks and other environment validations.
 *
 * @return True if the environment is compatible, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool isEnvironmentCompatible();

/**
 * @brief Retrieves the device brand or manufacturer.
 *
 * Platform-specific mechanisms are used to obtain this information.
 *
 * @return Device brand string, or "Unknown" if it cannot be determined.
 */
LOCAL_EVENT_PLANNER_API std::string getDeviceBrand();

/**
 * @brief Checks whether the current device is compatible with the application.
 *
 * Device compatibility is typically determined based on the device brand
 * and additional internal validation logic.
 *
 * @return True if the device is compatible, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool isDeviceCompatible();

#ifdef __cplusplus                                 ///< End of C++ compatibility block.
}                                                  ///< extern "C" block end.
#endif

#endif // VERSION_AND_DEVICE_BINDING_H              ///< Header include guard (end).
