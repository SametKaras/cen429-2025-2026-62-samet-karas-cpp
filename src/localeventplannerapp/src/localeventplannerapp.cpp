/**
 * @file LocalEventPlannerapp.cpp
 * @brief Main file for the Local Event Planner application.
 *
 * This file contains the main function, menu functions, and application
 * initialization logic for the Local Event Planner application.
 */

/// Include iostream for console I/O
#include <iostream>
/// Include user authentication header
#include "UserAuthentication.h"
/// Include session encryption header
#include "SessionEncryption.h"
/// Include dynamic asset protection header
#include "DynamicAssetProtection.h"
/// Include version and device binding header
#include "VersionAndDeviceBinding.h"
/// Include RASP (Runtime App Self-Protection) header
#include "Rasp.h"
/// Include debug check header
#include "DebugCheck.h"
/// Include signature verification header
#include "SignatureVerification.h"
/// Include string header
#include <string>
/// Include vector header
#include <vector>
/// Include attendee management header
#include "AttendeeManagement.h"
/// Include SQLite3 header
#include "sqlite3.h"
/// Include display main menu header
#include "DisplayMainMenu.h"
/// Include White-Box AES header
#include "WBAES.h"
/// Include event details header
#include "EventDetails.h"
/// Include random number generation
#include <random>
/// Include chrono for time functions
#include <chrono>
/// Include iostream (duplicate)
#include <iostream>
/// Include string (duplicate)
#include <string>
/// Include vector (duplicate)
#include <vector>
/// Include iomanip for formatting
#include <iomanip>
/// Include sstream for string streams
#include <sstream>
/// Include cstring for memset
#include <cstring>
/// Include OpenSSL EVP header
#include <openssl/evp.h>
/// Include OpenSSL SHA header
#include <openssl/sha.h>
/// Include OpenSSL HMAC header
#include <openssl/hmac.h>
/// Include OpenSSL random header
#include <openssl/rand.h>
/// Include ctime for time functions
#include <ctime>
/// Include SQLite3 (duplicate)
#include "sqlite3.h"
/// Include user authentication (duplicate)
#include "UserAuthentication.h"
/// Include display main menu (duplicate)
#include "DisplayMainMenu.h"
/// Include dynamic asset protection (duplicate)
#include "DynamicAssetProtection.h"
/// Include session encryption (duplicate)
#include "SessionEncryption.h"
/// Include salt and HMAC header
#include "SaltAndHMAC.h"
/// Include White-Box AES (duplicate)
#include "WBAES.h"
/// Include regex header
#include <regex>
/// Include iostream (duplicate)
#include <iostream>
/// Include string (duplicate)
#include <string>
/// Include vector (duplicate)
#include <vector>
/// Include event details (duplicate)
#include "EventDetails.h"
/// Include SQLite3 (duplicate)
#include "sqlite3.h"
/// Include user authentication (duplicate)
#include "UserAuthentication.h"
/// Include display main menu (duplicate)
#include "DisplayMainMenu.h"
/// Include White-Box AES (duplicate)
#include "WBAES.h"
/// Include session encryption (duplicate)
#include "SessionEncryption.h"
/// Include user authentication (duplicate)
#include "UserAuthentication.h"
/// Include iostream (duplicate)
#include <iostream>
/// Include string (duplicate)
#include <string>
/// Include vector (duplicate)
#include <vector>
/// Include SQLite3 (duplicate)
#include "sqlite3.h"
/// Include user authentication (duplicate)
#include "UserAuthentication.h"
/// Include display main menu (duplicate)
#include "DisplayMainMenu.h"
/// Include White-Box AES (duplicate)
#include "WBAES.h"
/// Include session encryption (duplicate)
#include "SessionEncryption.h"


/**
 * @brief Manages attendees for events.
 *
 * Displays a menu for attendee registration and listing.
 * Handles guest mode restrictions.
 */
LOCAL_EVENT_PLANNER_API void attendeeManagement() {
  /// Variable to store user choice
  int choice;

  /// Menu loop
  do {
    /// Display attendee management menu header
    std::cout << "===============================\n";
    /// Display menu title
    std::cout << "Katilimci Yonetimi:\n\n";
    /// Option 1: Register attendee
    std::cout << "1. Katilimci Kayit\n";
    /// Option 2: List attendees
    std::cout << "2. Katilimcilari Listele\n";
    /// Option 0: Return to main menu
    std::cout << "0. Ana Menuye Don\n";
    /// Display menu footer
    std::cout << "===============================\n\n";
    /// Prompt for selection
    std::cout << "Bir secenek secin: ";
    /// Read user input
    std::cin >> choice;

    /// Handle user choice
    switch (choice) {
      case 1:

        /// Check if not in guest mode
        if (!getGuestMode()) {
          /// Register attendees
          registerAttendees();
          /// Increment step counter
          step_counter++;
        } else {
          /// Guest mode error message
          std::cout << "\nBu islem icin yetkiniz yok.\n";
        }

        break;

      case 2:
        /// List attendees
        trackAttendees();
        /// Increment step counter
        step_counter++;
        break;

      case 0:
        /// Clear console
        clearConsole();
        /// Increment step counter
        step_counter++;
        /// Return to main menu message
        std::cout << "\nAna Menuye Donuluyor...\n";
        break;

      default:
        /// Invalid selection message
        std::cout << "\nGecersiz secim. Lutfen tekrar deneyin.\n";
    }
  } while (choice != 0); /// Continue until user selects 0
}

//////////////////////////////displaymainmenu///////////////////

/// Global step counter for control flow tracking
int step_counter = 0;

/**
 * @brief Displays the main menu and handles user input.
 *
 * Shows options for event details, attendee management,
 * calendar, feedback, and exit.
 *
 * @return bool True if program should exit, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool displayMainMenu() {
  /// Variable to store user choice
  int choice;

  /// Menu loop
  do {
    /// Display main menu header
    std::cout << "===============================\n";
    /// Display menu title
    std::cout << "Ana Menu:\n\n";
    /// Option 1: Event details
    std::cout << "1. Etkinlik Detaylari\n";
    /// Option 2: Attendee management
    std::cout << "2. Katilimci Yonetimi\n";
    /// Option 3: Calendar editor
    std::cout << "3. Takvim Duzenleyici\n";
    /// Option 4: Collect feedback
    std::cout << "4. Geri Bildirim Toplama\n";
    /// Option 5: End session
    std::cout << "5. Oturumu Sonlandir\n";
    /// Option 0: Exit program
    std::cout << "0. Cikis\n";
    /// Display menu footer
    std::cout << "===============================\n\n";
    /// Prompt for selection
    std::cout << "Bir secenek secin: ";
    /// Read user input
    std::cin >> choice;
    /// Clear input buffer
    std::cin.ignore();

    /// Handle user choice
    switch (choice) {
      case 1:
        /// Clear console
        clearConsole();
        /// Increment step counter
        step_counter++;
        /// Show event details
        eventDetails();
        /// Increment step counter
        step_counter++;
        break;

      case 2:
        /// Clear console
        clearConsole();
        /// Increment step counter
        step_counter++;
        /// Open attendee management
        attendeeManagement();
        /// Increment step counter
        step_counter++;
        break;

      case 3:
        /// Clear console
        clearConsole();
        /// Increment step counter
        step_counter++;
        /// End session message
        std::cout << "\nOturum sonlandiriliyor...\n";
        /// Return to authentication
        return false;
        break;

      case 0:
        /// Exit program message
        std::cout << "\nProgramdan cikiliyor...\n";
        /// Exit program
        return true;
        break;

      default:
        /// Invalid selection message
        std::cout << "\nGecersiz secim. Lutfen tekrar deneyin.\n";
    }
  } while (true); /// Continue until exit

  /// Default return
  return false;
}

//////////////userauthentication/////////////////////

/**
 * @brief Handles user authentication menu.
 *
 * Displays login, register, and guest mode options.
 *
 * @return bool True if program should exit, false otherwise.
 */
LOCAL_EVENT_PLANNER_API bool userAuthentication() {
  /// Variable to store user choice
  int choice;

  /// Authentication menu loop
  do {
    /// Display authentication menu header
    std::cout << "===============================\n";
    /// Display menu title
    std::cout << "Kullanici Kimlik Dogrulama:\n\n";
    /// Option 1: Login
    std::cout << "1. Giris Yap\n";
    /// Option 2: Register
    std::cout << "2. Kayit Ol\n";
    /// Option 3: Guest mode
    std::cout << "3. Misafir Modu\n";
    /// Option 0: Exit
    std::cout << "0. Cikis Yap\n";
    /// Display menu footer
    std::cout << "===============================\n\n";
    /// Prompt for selection
    std::cout << "Bir secenek secin: ";
    /// Read user input
    std::cin >> choice;
    /// Clear input buffer
    std::cin.ignore();

    /// Handle user choice
    switch (choice) {
      case 1:
        /// Attempt login
        if (loginUser()) {
          /// Show main menu on success
          displayMainMenu();
          /// Return to authentication
          return false;
        }

        break;

      case 2:
        /// Clear console
        clearConsole();
        /// Register new user
        registerUser();
        break;

      case 3:
        /// Enable guest mode
        setGuestMode(true);
        /// Guest mode message
        std::cout << "\nMisafir Modu aktif. Sinirli erisim saglandi.\n";

        /// Show main menu
        if (displayMainMenu()) {
          /// Exit if requested
          return true;
        }

        break;

      case 0:
        /// Exit message
        std::cout << "\nCikis Yapiliyor...\n";
        /// Exit program
        return true;
        break;

      default:
        /// Invalid selection message
        std::cout << "\nGecersiz secim. Lutfen tekrar deneyin.\n";
    }
  } while (choice != 0); /// Continue until exit

  /// Default return
  return false;
}

/**
 * @brief Starts the application with security checks.
 *
 * Performs execution time check, memory check, signature verification,
 * SSL initialization, emulator detection, hook detection, version
 * compatibility, and device compatibility checks before starting.
 *
 * @return int 0 on success, 1 on security failure.
 */
int startApp() {
  /// Check execution timing
  checkExecutionTime();
  /// Check memory integrity
  checkMemory();
  /// Verify application signature
  startVerification();
  /// Initialize SSL
  StartSSL();

  /// Check for emulator
  if (isEmulator()) {
    /// Emulator detected error
    std::cerr << "Emulator tespit edildi. Uygulama sonlandiriliyor.\n";
    /// Return error
    return 1;
  } else {
    /// No unsafe device message
    std::cout << "Guvensiz cihaz tespit edilmedi.\n";
  }

  /// Check for API hooks
  if (checkHooks()) {
    /// Hook detected error
    std::cerr << "Hook saldirisi tespit edildi, uygulama sonlandiriliyor.\n";
    /// Return error
    return 1;
  } else {
    /// No hook detected message
    std::cout << "Herhangi bir HOOK saldirisi tespit edilmedi.\n";
  }

  /// Check environment compatibility
  if (!isEnvironmentCompatible()) {
    /// Version not supported error
    std::cerr << "Bu uygulama surumunuz desteklenmiyor. Lutfen en son surume guncelleyin.\n";
    /// Return error
    return 1;
  }

  /// Check device compatibility
  if (!isDeviceCompatible()) {
    /// Device not supported error
    std::cerr << "Bu uygulama yalnizca HP ve ASUS cihazlarinda calistirabilir.\n";
    /// Return error
    return 1;
  }

  /// Compatibility success message
  std::cout << "Mevcut cihaz ve surum desteklenmektedir.\n";

  /// Main authentication loop
  while (true) {
    /// Run authentication
    if (userAuthentication()) {
      /// Break if exit requested
      break;
    }
  }

  /// Program end message
  std::cout << "Program sonlandirildi.\n";
  /// Return success
  return 0;
}

///////////////////////////////eventdetails/////////////////////

/**
 * @brief Handles event details menu.
 *
 * Displays options for creating, managing, and listing events.
 * Handles guest mode restrictions.
 */
LOCAL_EVENT_PLANNER_API void eventDetails() {
  /// Variable to store user choice
  int choice;

  /// Event details menu loop
  do {
    /// Display event details menu header
    std::cout << "===============================\n";
    /// Display menu title
    std::cout << "Etkinlik Detaylari:\n\n";
    /// Option 1: Create event
    std::cout << "1. Etkinlik Olustur\n";
    /// Option 2: Manage events
    std::cout << "2. Etkinlikleri Yonet\n";
    /// Option 3: List events
    std::cout << "3. Etkinlikleri Listele\n";
    /// Option 0: Return to main menu
    std::cout << "0. Ana Menuye Don\n";
    /// Display menu footer
    std::cout << "===============================\n";
    /// Prompt for selection
    std::cout << "Bir secenek secin: ";
    /// Read user input
    std::cin >> choice;

    /// Handle user choice
    switch (choice) {
      case 1:

        /// Check if not in guest mode
        if (!getGuestMode()) {
          /// Create new event
          createEvent();
          /// Increment step counter
          step_counter++;
        } else {
          /// Guest mode error message
          std::cout << "\nBu islem icin yetkiniz yok.\n";
        }

        break;

      case 2:
        /// Check if not in guest mode
        if (!getGuestMode()) {
          /// Manage events
          manageEvents();
          /// Increment step counter
          step_counter++;
        } else {
          /// Guest mode error message
          std::cout << "\nBu islem icin yetkiniz yok.\n";
        }

        break;

      case 3:
        /// List all events
        listEvents();
        /// Increment step counter
        step_counter++;
        break;

      case 0:
        /// Clear console
        clearConsole();
        /// Increment step counter
        step_counter++;
        /// Return to main menu message
        std::cout << "\nAna Menuye Donuluyor...\n";
        break;

      default:
        /// Invalid selection message
        std::cout << "\nGecersiz secim. Lutfen tekrar deneyin.\n";
    }
  } while (choice != 0); /// Continue until user selects 0
}

/**
 * @brief Main function for the Local Event Planner application.
 *
 * Entry point of the application. Performs debugger check
 * and starts the application.
 *
 * @return int Exit code (0 for success).
 */
int main() {
  /// Check for attached debugger
  checkDebugger();
  /// Start the application
  startApp();
  /// Return success
  return 0;
}
