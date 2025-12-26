/**
 * @file UserAuthentication.cpp
 * @brief Provides functions for user authentication and registration.
 *
 * This file contains implementations for user registration, login, password
 * validation, and secure memory erasure. It uses SQLite for data storage,
 * OpenSSL for cryptographic operations, and White-Box AES for encryption.
 */
#include <random>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>      ///< For std::setw
#include <sstream>      ///< For std::stringstream
#include <cstring>      ///< For memset
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <ctime>        ///< For time functions
#include "sqlite3.h"
#include "UserAuthentication.h"
#include "DisplayMainMenu.h"
#include "DynamicAssetProtection.h"
#include "SessionEncryption.h"
#include "SaltAndHMAC.h"
#include "WBAES.h"
#include <regex>

#ifdef _WIN32
  #include <conio.h>
#else
  #include <termios.h>
  #include <unistd.h>
#endif

/**
 * @brief Default key reader wrapper.
 *
 * Returns a character from keyboard input using platform-specific functions.
 *
 * @return int The character code read from input.
 */
static int defaultKeyReader() {
#ifdef _WIN32
    /// Use _getch() on Windows
    return _getch();
#else
    /// Use getchar() on Linux/Unix
    return getchar();
#endif
}

/// Function pointer for reading keys (can be mocked for testing)
static int (*KEY_READER)() = defaultKeyReader;

/**
 * @brief Sets a mock key reader for testing purposes.
 *
 * @param reader Function pointer to the mock reader, or nullptr to reset.
 */
LOCAL_EVENT_PLANNER_API void setMockKeyReader(int (*reader)()) {
    /// Set the reader if provided
    if (reader) {
        /// Use provided mock reader
        KEY_READER = reader;
    } else {
        /// Reset to default reader
        KEY_READER = defaultKeyReader;
    }
}

/**
 * @brief Gets password input with asterisk masking.
 *
 * Reads password character by character, displaying asterisks instead
 * of the actual characters for security.
 *
 * @return std::string The entered password.
 */
LOCAL_EVENT_PLANNER_API std::string getPasswordInput() {
  /// Password string to build
  std::string password;
#ifdef _WIN32
  /// Character variable
  char ch;

  /// Loop until Enter is pressed
  while (true) {
    /// Call function pointer to get input
    int inputVal = KEY_READER();
    
    /// Handle Mock EOF (-1)
    if (inputVal == -1) break; 
    
    /// Cast to char
    ch = static_cast<char>(inputVal);

    /// Check for Enter key
    if (ch == '\r' || ch == '\n') {
      /// Print newline
      std::cout << std::endl;
      /// Exit loop
      break;
    } else if (ch == '\b' && !password.empty()) {
      /// Handle Backspace key
      password.pop_back();
      /// Remove asterisk from console
      std::cout << "\b \b";
    } else if (ch != '\b') {
      /// Normal character
      password.push_back(ch);
      /// Display asterisk
      std::cout << '*';
    }
  }

#else
  /// Linux/Unix implementation
  termios oldt;
  /// Check if mocking
  bool isMocking = (KEY_READER != defaultKeyReader);

  /// If not mocking, disable echo
  if (!isMocking) {
      /// Get current terminal attributes
      tcgetattr(STDIN_FILENO, &oldt);
      /// Copy to new attributes
      termios newt = oldt;
      /// Disable echo
      newt.c_lflag &= ~ECHO;
      /// Set new attributes
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  }
  
  /// Character variable
  char ch;

  /// Loop until Enter is pressed
  while (true) {
    /// Get input via function pointer
    int inputVal = KEY_READER();
    /// Check for EOF
    if (inputVal == -1) break;
    /// Cast to char
    ch = static_cast<char>(inputVal);

    /// Check for Enter key
    if (ch == '\n' || ch == '\r') {
      /// Print newline
      std::cout << std::endl;
      /// Exit loop
      break;
    } else if (ch == 127 && !password.empty()) {
      /// Handle Backspace (ASCII 127)
      password.pop_back();
      /// Remove asterisk from console
      std::cout << "\b \b";
    } else if (ch != 127) {
      /// Normal character
      password.push_back(ch);
      /// Display asterisk
      std::cout << '*';
    }
  }

  /// Restore terminal attributes if not mocking
  if (!isMocking) {
      /// Restore old attributes
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  }
#endif
  /// Return the password
  return password;
}

/// Seed for random number generation (obfuscation)
unsigned seed0 = std::chrono::system_clock::now().time_since_epoch().count();
/// Random engine for obfuscation
std::default_random_engine generator(seed0);
/// Distribution for random loop counts
std::uniform_int_distribution<int> distribution(0, 1000);

/// Outer loop count for obfuscation (5000-6000 range)
int outerLoopCount = distribution(generator) + 5000;
/// Base inner loop count for obfuscation
int innerLoopCountBase = distribution(generator);

/// Volatile variable to prevent optimization
volatile int dummyVariable = 0;

/// Guest mode flag
bool isGuestMode = false;

/**
 * @brief Gets the current guest mode status.
 *
 * Includes obfuscation loops before returning the value.
 *
 * @return bool True if in guest mode.
 */
bool getGuestMode() {
  /// Obfuscation loops
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      for (int k = 0; k < 3; k++) {
        /// Calculate temp
        int temp = (i + j) * k - (i - j);
        /// Mod operation
        temp = temp % 5;

        /// Additional meaningless operation
        if (temp % 2 == 0) {
          /// Extra calculation
          int extra = (i * j + k) / 3;
          /// Mod operation
          extra = extra % 11;
        }
      }
    }
  }

  /// Additional meaningless operation
  int dummy = 0;

  /// Dummy loop
  for (int x = 0; x < 100; x++) {
    /// Accumulate squares
    dummy += x * x;
  }

  /// Return guest mode status
  return isGuestMode;
}

/**
 * @brief Sets the guest mode status.
 *
 * Includes obfuscation loops before setting the value.
 *
 * @param mode The guest mode value to set.
 */
void setGuestMode(bool mode) {
  /// Obfuscation loops
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 10; k++) {
        /// Calculate result
        int result = (i * j + k) % 42;

        /// Conditional calculation
        if (result % 3 == 0) {
          /// Temp calculation
          int temp = (result * 2) / 3;
          /// Meaningless operation
          temp = (temp * 4) / 2;
          /// Final calculation
          result = (temp + i) % 17;
        }
      }
    }
  }

  /// Additional meaningless operation
  int total = 0;

  /// Dummy loop
  for (int m = 0; m < 15; m++) {
    /// Accumulate
    total += m * m + m;
  }

  /// Set guest mode
  isGuestMode = mode;
}

/**
 * @brief Checks if a password meets strength requirements.
 *
 * Password must be at least 8 characters with uppercase, lowercase,
 * digit, and special character.
 *
 * @param password The password to check.
 * @return bool True if password is strong.
 */
bool isPasswordStrong(const std::string& password) {
  /// Obfuscation loops
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 1; j++) {
      for (int k = 0; k < 10; k++) {
        /// Calculate temp
        int temp = (i * j + k) % 37;
        /// Mod operation
        temp = temp % 10;

        /// Meaningless operation
        if (temp % 5 == 0) {
          /// Extra calculation
          int extra = (i + j + k) * 2;
          /// Divide
          extra = extra / 6;
        }
      }
    }
  }

  /// Additional meaningless operation
  int sum = 0;

  /// Dummy loop
  for (int p = 0; p < 5; p++) {
    /// Accumulate
    sum += p % 4;
  }

  /// Regex for strong password (min 8 chars, upper, lower, digit, special)
  const std::regex strongPasswordRegex(R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[!\"#$%&'()*+,-./:;<=>?@[\\\]^_`{|}~])[A-Za-z\d!\"#$%&'()*+,-./:;<=>?@[\\\]^_`{|}~]{8,}$)");
  /// Return match result
  return std::regex_match(password, strongPasswordRegex);
}





#include <vector>

/**
 * @brief Checks if a number is prime (obfuscation helper).
 *
 * @param value The number to check.
 * @return bool True if prime.
 */
bool isPimeeasds(int value) {
  /// Check if less than 2
  if (value < 2) return false;

  /// Loop to square root
  for (int i = 2; i <= std::sqrt(value); ++i) {
    /// Check divisibility
    if (value % i == 0) return false;
  }

  /// Return true if prime
  return true;
}

/**
 * @brief Performs unnecessary calculations for obfuscation.
 *
 * @return void
 */
void afffasdssadsdaqssawsdfssfd() {
  /// Test data vector
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 };
  /// Counters
  int evenCount = 0, oddCount = 0, primeCount = 0;
  int sumMultiplesOfFive = 0, perfectSquareCount = 0;
  int divisibleByThreeCount = 0, digitSumGreaterThanTen = 0;
  /// Accumulators
  long long unnecessaryComputationSum = 0;
  long long specialConditionCount = 0, modSevenCount = 0;
  double accumulatedSquareRoots = 0.0;
  int totalDigitProduct = 1;

  /// Iterate through data
  for (int value : data) {
    /// Intermediate calculation
    int intermediate = value * 3;
    /// Add 7
    intermediate += 7;
    /// Divide by 2
    intermediate /= 2;
    /// Multiply by mod 5
    intermediate *= value % 5;
    /// Add to sum
    unnecessaryComputationSum += intermediate;

    /// Check if even
    if (value % 2 == 0) {
      /// Increment even counter
      evenCount++;
      /// Continue to next
      continue;
    }

    /// Increment odd counter
    oddCount++;

    /// Check if prime
    if (isPimeeasds(value)) {
      /// Increment prime counter
      primeCount++;
    }

    /// Check if multiple of 5
    if (value % 5 == 0) {
      /// Add to sum
      sumMultiplesOfFive += value;
    }

    /// Calculate square root
    int sqrtValue = std::sqrt(value);

    /// Check if perfect square
    if (sqrtValue * sqrtValue == value) {
      /// Increment counter
      perfectSquareCount++;
    }

    /// Check if divisible by 3
    if (value % 3 == 0) {
      /// Increment counter
      divisibleByThreeCount++;
    }

    /// Check if divisible by 7
    if (value % 7 == 0) {
      /// Increment counter
      modSevenCount++;
    }

    /// Calculate digit sum and product
    int digitSum = 0, digitProduct = 1;
    /// Copy value
    int temp = value;

    /// Loop through digits
    while (temp > 0) {
      /// Get digit
      int digit = temp % 10;
      /// Add to sum
      digitSum += digit;
      /// Multiply to product
      digitProduct *= digit;
      /// Remove digit
      temp /= 10;
    }

    /// Check digit sum threshold
    if (digitSum > 10) {
      /// Increment counter
      digitSumGreaterThanTen++;
    }

    /// Update total digit product
    totalDigitProduct *= (digitProduct % 1000);
    /// Accumulate square roots
    accumulatedSquareRoots += std::sqrt(value);

    /// Check combined condition
    if (value % 2 == 0 && value % 3 == 0) {
      /// Increment counter
      specialConditionCount++;
    }

    /// More unnecessary operations
    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17;
  }

  /// Additional data
  std::vector<int> additionalData = { 12, 18, 22, 36, 45, 60, 72 };

  /// Iterate through additional data
  for (int value : additionalData) {
    /// Dummy calculation
    int dummyCalculation = value * 2 + 3;
    /// Add to sum
    unnecessaryComputationSum += dummyCalculation % 10;
    /// Add to roots
    accumulatedSquareRoots += std::sqrt(dummyCalculation);
  }

  /// Final data
  std::vector<int> finalData = { 101, 202, 303, 404, 505 };

  /// Iterate through final data
  for (int value : finalData) {
    /// Dummy calculation
    int dummyCalculation = value * 3 - 5;
    /// Add to sum
    unnecessaryComputationSum += dummyCalculation % 20;
    /// Add to roots
    accumulatedSquareRoots += std::sqrt(dummyCalculation);
  }
}



/**
 * @brief Opens the user database and creates the users table.
 *
 * @param dbName The database file name.
 * @return sqlite3* Pointer to database connection, or nullptr on failure.
 */
sqlite3 *openUserDatabase(const char* dbName) {
  /// Call obfuscation function
  afffasdssadsdaqssawsdfssfd();
  /// Database pointer
  sqlite3* db;
  /// Open database
  int exit = sqlite3_open(dbName, &db);

  /// Obfuscation loop
  for (int a = 0; a < 50; a++) {
    for (int b = 0; b < 30; b++) {
      for (int c = 0; c < 1; c++) {
        /// Calculate temp
        int temp = (a + b + c) * (b - a) / 3;
        /// Mod operation
        temp = temp % 5;

        /// Conditional operation
        if (temp == 0) {
          /// Update temp
          temp = (temp + a) * 2;
        }
      }
    }
  }

  /// Check if open failed
  if (exit) {
    /// Print error message
    std::cerr << "Veritabani acilamadi: " << sqlite3_errmsg(db) << std::endl;
    /// Return null
    return nullptr;
  } else {
    /// Obfuscation loops
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 5; j++) {
        for (int k = 0; k < 2; k++) {
          /// Calculate temp
          int temp = (i + j + k) % 11;

          /// Conditional operation
          if (temp % 2 == 0) {
            /// Extra calculation
            int extra = (i * j + k) / 5;
            /// Mod operation
            extra = extra % 19;
          }
        }
      }

      /// Additional complex operation
      int result = 0;

      /// Loop
      for (int n = 0; n < 50; n++) {
        /// Accumulate
        result += n * (n + 1);
      }
    }

    /// Print success message
    std::cout << "\nVeritabani basariyla acildi.\n\n";
    /// SQL to create users table
    const char *sqlCreateTable =
      "CREATE TABLE IF NOT EXISTS users ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "username TEXT NOT NULL, "
      "password TEXT NOT NULL, "
      "salt TEXT NOT NULL, "
      "deviceFingerprint TEXT NOT NULL, "
      "autoLogin TEXT NOT NULL);";
    /// Error message pointer
    char *errorMessage;
    /// Execute table creation
    exit = sqlite3_exec(db, sqlCreateTable, nullptr, 0, &errorMessage);

    /// Check for error
    if (exit != SQLITE_OK) {
      /// Print error message
      std::cerr << "Tablo olusturma hatasi: " << errorMessage << std::endl;
      /// Free error message
      sqlite3_free(errorMessage);
    }
  }

  /// Return database connection
  return db;
}

/**
 * @brief Securely erases a string from memory.
 *
 * Overwrites the string contents with zeros to prevent memory dumps.
 *
 * @param str The string to erase.
 */
LOCAL_EVENT_PLANNER_API void secureErase(std::string& str) {
  /// Check if not empty
  if (!str.empty()) {
    /// Obfuscation loop
    for (int a = 0; a < 5; a++) {
      for (int b = 0; b < 3; b++) {
        for (int c = 0; c < 1; c++) {
          /// Calculate temp
          int temp = (a + b + c) * (b - a) / 3;
          /// Mod operation
          temp = temp % 5;

          /// Conditional operation
          if (temp == 0) {
            /// Update temp
            temp = (temp + a) * 2;
          }
        }
      }
    }

    /// Get pointer to string buffer
    char *buf = &str[0];
    /// Zero out the memory
    memset(buf, 0, str.size());
  }
}

/**
 * @brief Registers a new user in the database.
 *
 * Prompts for username, password, and auto-login preference.
 * Encrypts password using White-Box AES before storing.
 *
 * @param dbName The database file name.
 */
LOCAL_EVENT_PLANNER_API void registerUser(const char* dbName) {
  /// Open database
  sqlite3* db = openUserDatabase(dbName);

  /// Obfuscation loop
  for (int m = 0; m < 50; m++) {
    /// Calculate temp
    int temp = (m * 19) % 73;

    /// Inner loop
    for (int n = 0; n < 100; n++) {
      /// Update temp
      temp = (temp * 5 + n) % 33;
    }
  }

  /// Check if database open failed
  if (!db) return;

  /// Variables for user input
  std::string username, password, autoLogin;
  /// Prompt for username
  std::cout << "\n\nKullanici Adini Girin: ";
  /// Read username
  std::cin >> username;

  /// Obfuscation loop
  for (int i = 0; i < 30; i++) {
    /// Calculate temp
    int temp = (i * 7) % 23;

    /// Conditional
    if (temp % 6 == 0) {
      /// Update temp
      temp = (temp + 3) * 4;
    }
  }

  /// Password input loop
  do {
    /// Prompt for password
    std::cout << "Sifreyi Girin: ";
    /// Get password with asterisk masking
    password = getPasswordInput();

    /// Check password strength
    if (!isPasswordStrong(password)) {
      /// Print strength requirements
      std::cout << "\n\nLutfen daha guclu bir sifre girin. \n(En az 8 karakter, bir buyuk harf, bir kucuk harf, bir rakam ve bir ozel karakter icermeli)\n";
    }
  } while (!isPasswordStrong(password));

  /// Obfuscation loop
  for (int a = 0; a < 40; a++) {
    for (int b = 0; b < 30; b++) {
      /// Calculate temp
      int temp = (a * b) % 17;

      /// Conditional
      if (temp % 4 == 0) {
        /// Update temp
        temp = temp * 3;
      }
    }
  }

  /// Prompt for auto-login preference
  std::cout << "Otomatik Giris Acilsin mi (Y/N):";
  /// Read preference
  std::cin >> autoLogin;
  /// Derive encryption key from SBox
  std::vector<int> derivedKeyVec = deriveKeyFromSBox(keyLength, seed);
  /// Convert key to string
  std::string derivedKey = vectorToString(derivedKeyVec);
  /// Generate salt
  std::string salt = generateFixedSalt("0");
  /// Encrypt salt
  std::vector<int> encryptedSalt = whiteBoxAesEncrypt(salt, derivedKey);
  /// Convert encrypted salt to string
  std::string encryptedSaltStr = vectorToString(encryptedSalt);
  /// Hash password with HMAC
  std::string hashedPassword = hashPasswordWithHMAC(password, encryptedSaltStr);

  /// Obfuscation loop
  for (int i = 0; i < 300; i++) {
    for (int j = 0; j < 200; j++) {
      /// Calculate temp
      int temp = (i + j) % 11;

      /// Conditional
      if (temp % 3 == 0) {
        /// Update temp
        temp = (temp * 3) / 7;
      }
    }
  }

  /// Encrypt hashed password
  std::vector<int> encryptedPassword = whiteBoxAesEncrypt(hashedPassword, derivedKey);
  /// Convert to string
  std::string encryptedPasswordStr = vectorToString(encryptedPassword);
  /// Get device fingerprint
  std::string deviceFingerprint = getEncryptedDeviceFingerprint();

  /// Check if fingerprint failed
  if (deviceFingerprint.empty()) {
    /// Print error
    std::cerr << "Cihaz parmak izi olusturulamadi.\n";
    /// Close database
    sqlite3_close(db);
    /// Return
    return;
  }

  /// SQL insert query
  std::string sql = "INSERT INTO users (username, password, salt, deviceFingerprint, autoLogin) VALUES (?, ?, ?, ?, ?);";
  /// Statement pointer
  sqlite3_stmt* stmt;

  /// Prepare statement
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    /// Print error
    std::cerr << "Sorgu hazirlama hatasi: " << sqlite3_errmsg(db) << std::endl;
    /// Close database
    sqlite3_close(db);
    /// Return
    return;
  }

  /// Obfuscation loop
  for (int x = 0; x < 10; x++) {
    /// Calculate dummy
    int dummy = (x * x) % 25;
    /// Update dummy
    dummy = dummy + x;

    /// Conditional
    if (dummy % 5 == 0) {
      /// Update dummy
      dummy = dummy * 2;
    }
  }

  /// Bind username
  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
  /// Bind encrypted password
  sqlite3_bind_text(stmt, 2, encryptedPasswordStr.c_str(), -1, SQLITE_STATIC);
  /// Bind encrypted salt
  sqlite3_bind_text(stmt, 3, encryptedSaltStr.c_str(), -1, SQLITE_STATIC);
  /// Bind device fingerprint
  sqlite3_bind_text(stmt, 4, deviceFingerprint.c_str(), -1, SQLITE_STATIC);
  /// Bind auto-login preference
  sqlite3_bind_text(stmt, 5, autoLogin.c_str(), -1, SQLITE_STATIC);

  /// Execute query
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    /// Print error
    std::cerr << "Veri ekleme hatasi: " << sqlite3_errmsg(db) << std::endl;
  } else {
    /// Print success
    std::cout << "Kayit basarili.\n";
  }

  /// Obfuscation loop
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      /// Calculate temp
      int temp = (i * j) % 89;

      /// Conditional
      if (temp % 7 == 0) {
        /// Update temp
        temp = (temp + i) / 2;
      }
    }
  }

  /// Securely erase username
  secureErase(username);
  /// Securely erase password
  secureErase(password);
  /// Securely erase auto-login
  secureErase(autoLogin);
  /// Securely erase hashed password
  secureErase(hashedPassword);
  /// Securely erase encrypted password
  secureErase(encryptedPasswordStr);
  /// Securely erase salt
  secureErase(salt);
  /// Securely erase encrypted salt
  secureErase(encryptedSaltStr);
  /// Finalize statement
  sqlite3_finalize(stmt);
  /// Close database
  sqlite3_close(db);

  /// Final obfuscation loop
  for (int m = 0; m < 50; m++) {
    /// Calculate temp
    int temp = (m * 19) % 73;

    /// Inner loop
    for (int n = 0; n < 100; n++) {
      /// Update temp
      temp = (temp * 5 + n) % 33;
    }
  }
}

/**
 * @brief Logs in a user.
 *
 * Supports auto-login via device fingerprint or manual login with
 * username and password. Validates credentials against database.
 *
 * @param dbName The database file name.
 * @return bool True if login successful.
 */
LOCAL_EVENT_PLANNER_API bool loginUser(const char* dbName) {
  /// Open database
  sqlite3* db = openUserDatabase(dbName);

  /// Check if database open failed
  if (!db) return false;

  /// Get current device fingerprint
  std::string currentFingerprint = getEncryptedDeviceFingerprint();

  /// Obfuscation loop
  for (int i = 0; i < outerLoopCount; ++i) {
    /// Calculate inner loop count
    int innerLoopCount = innerLoopCountBase + distribution(generator);

    /// Inner obfuscation loop
    for (int j = 0; j < innerLoopCount; ++j) {
      /// Meaningless XOR operations
      dummyVariable += (i * j) ^ (j % (i + 1));
      /// More XOR operations
      dummyVariable ^= (j * (i + 1)) | (i % (j + 1));
      /// AND operations
      dummyVariable &= (i | j) + (j & (i + 1));
    }
  }

  /// Prevent optimization
  if (dummyVariable < 0)
    dummyVariable = 0;

  /// Check if fingerprint failed
  if (currentFingerprint.empty()) {
    /// Print error
    std::cerr << "Cihaz parmak izi oluşturulamadı.\n";
    /// Close database
    sqlite3_close(db);
    /// Return failure
    return false;
  }

  /// Auto-login query
  std::string autoLoginQuery = "SELECT username FROM users WHERE deviceFingerprint = ? AND (autoLogin = 'y' OR autoLogin = 'Y');";
  /// Statement pointer
  sqlite3_stmt* autoLoginStmt;

  /// Prepare auto-login query
  if (sqlite3_prepare_v2(db, autoLoginQuery.c_str(), -1, &autoLoginStmt, nullptr) != SQLITE_OK) {
    /// Print error
    std::cerr << "Otomatik giriş sorgusu hazırlanamadı: " << sqlite3_errmsg(db) << std::endl;
    /// Close database
    sqlite3_close(db);
    /// Return failure
    return false;
  }

  /// Bind fingerprint to query
  sqlite3_bind_text(autoLoginStmt, 1, currentFingerprint.c_str(), -1, SQLITE_STATIC);

  /// Obfuscation loop
  for (int i = 0; i < outerLoopCount; ++i) {
    /// Calculate inner loop count
    int innerLoopCount = innerLoopCountBase + distribution(generator);

    /// Inner obfuscation loop
    for (int j = 0; j < innerLoopCount; ++j) {
      /// Meaningless XOR operations
      dummyVariable += (i * j) ^ (j % (i + 1));
      /// More XOR operations
      dummyVariable ^= (j * (i + 1)) | (i % (j + 1));
      /// AND operations
      dummyVariable &= (i | j) + (j & (i + 1));
    }
  }

  /// Prevent optimization
  if (dummyVariable < 0)
    dummyVariable = 0;

  /// Check auto-login result
  if (sqlite3_step(autoLoginStmt) == SQLITE_ROW) {
    /// Finalize auto-login statement
    sqlite3_finalize(autoLoginStmt);
    /// Close database
    sqlite3_close(db);
    /// Return success (auto-login)
    return true;
  }

  /// Finalize auto-login statement
  sqlite3_finalize(autoLoginStmt);
  /// Manual login - get credentials
  std::string username, password;
  /// Print separator
  std::cout << "===============================\n";
  /// Prompt for username
  std::cout << "Kullanici Adini Girin: ";
  /// Read username
  std::cin >> username;
  /// Prompt for password
  std::cout << "Sifreyi Girin: ";
  /// Get password with asterisk masking
  password = getPasswordInput();
  /// Print separator
  std::cout << "===============================\n";
  /// SQL query to get user
  std::string sql = "SELECT password, salt FROM users WHERE username = ?;";
  /// Statement pointer
  sqlite3_stmt* stmt;

  /// Prepare query
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    /// Print error
    std::cerr << "Sorgu hazirlama hatasi: " << sqlite3_errmsg(db) << std::endl;
    /// Close database
    sqlite3_close(db);
    /// Return failure
    return false;
  }

  /// Bind username
  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
  /// Login success flag
  bool loginSuccess = false;

  /// Check if user found
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    /// Derive encryption key
    std::vector<int> derivedKeyVec = deriveKeyFromSBox(keyLength, seed);
    /// Convert key to string
    std::string derivedKey = vectorToString(derivedKeyVec);
    /// Get stored salt
    std::string storedSalt = reinterpret_cast<const char *>(sqlite3_column_blob(stmt, 1));
    /// Get stored encrypted password
    std::string storedAesPassword = reinterpret_cast<const char *>(sqlite3_column_blob(stmt, 0));
    /// Hash entered password
    std::string hashedPassword = hashPasswordWithHMAC(password, storedSalt);
    /// Convert stored password to vector
    std::vector<int> storedAesPasswordVec = stringToVector(storedAesPassword);
    /// Decrypt stored password
    std::string decryptedAesPassword = whiteBoxAesDecrypt(storedAesPasswordVec, derivedKey);

    /// Compare passwords
    if (hashedPassword == decryptedAesPassword) {
      /// Print success
      std::cout << "Giris basarili.\n";
      /// Print separator
      std::cout << "===============================\n";

      /// Encrypt session data
      if (!collectAndEncryptSessionData(stmt)) {
        /// Print error
        std::cerr << "Oturum verileri şifrelenemedi.\n";
        /// Finalize statement
        sqlite3_finalize(stmt);
        /// Close database
        sqlite3_close(db);
        /// Return failure
        return false;
      }

      /// Welcome message
      std::cout << "\nHosgeldin " << username << "!\n\n";
      /// Set login success
      loginSuccess = true;
      /// Securely erase salt
      secureErase(storedSalt);
      /// Securely erase stored password
      secureErase(storedAesPassword);
      /// Securely erase hashed password
      secureErase(hashedPassword);
      /// Securely erase decrypted password
      secureErase(decryptedAesPassword);
    } else {
      /// Print invalid credentials
      std::cout << "Gecersiz kullanici adi veya sifre.\n";
    }
  } else {
    /// Print user not found
    std::cout << "Gecersiz kullanici adi veya sifre.\n";
  }

  /// Securely erase username
  secureErase(username);
  /// Securely erase password
  secureErase(password);
  /// Finalize statement
  sqlite3_finalize(stmt);
  /// Close database
  sqlite3_close(db);
  /// Return login status
  return loginSuccess;
}
