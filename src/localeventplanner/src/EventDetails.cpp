/**
 * @file EventDetails.cpp
 * @brief Provides functions for managing event details.
 *
 * This file contains implementations for creating, listing, and managing events,
 * including database interactions and White-Box AES encryption for secure storage.
 */
#include <iostream>
#include <string>
#include <vector>
#include "EventDetails.h"
#include "sqlite3.h"
#include "UserAuthentication.h"
#include "DisplayMainMenu.h"
#include "WBAES.h"
#include "SessionEncryption.h"
#include "UserAuthentication.h"


/**
 * @brief Checks if a number is prime.
 *
 * This function is used for obfuscation calculations.
 *
 * @param value The number to check.
 * @return true If prime.
 * @return false If not prime.
 */
bool isPrimes(int value) {
  /// Check if value is less than 2
  if (value < 2) return false;

  /// Loop from 2 to square root of value
  for (int i = 2; i <= std::sqrt(value); ++i) {
    /// Check for divisibility
    if (value % i == 0) return false;
  }

  /// Return true if no divisors found
  return true;
}

/**
 * @brief Performs unnecessary calculations for obfuscation.
 *
 * This function executes complex but meaningless calculations
 * to complicate reverse engineering.
 *
 * @return void
 */
void afffasdssdw() {
  /// Initialize test data vector
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 };
  /// Variables to hold results
  int evenCount = 0;
  int oddCount = 0;
  int primeCount = 0;
  int sumMultiplesOfFive = 0;
  int perfectSquareCount = 0;
  int divisibleByThreeCount = 0;
  int digitSumGreaterThanTen = 0;
  long long unnecessaryComputationSum = 0;

  /// Iterate through data
  for (int value : data) {
    /// Unnecessary mathematical operations
    int intermediate = value * 3;
    /// Add 7
    intermediate += 7;
    /// Divide by 2
    intermediate /= 2;
    /// Multiply by value mod 5
    intermediate *= value % 5;
    /// Add to sum
    unnecessaryComputationSum += intermediate;

    /// Count even and odd numbers
    if (value % 2 == 0) {
      /// Increment even count
      evenCount++;
      /// Continue loop for even numbers
      continue;
    }

    /// Increment odd count
    oddCount++;

    /// Check for primality
    if (isPrimes(value)) {
      /// Increment prime count
      primeCount++;
    }

    /// Check if multiple of 5
    if (value % 5 == 0) {
      /// Add to sum of multiples
      sumMultiplesOfFive += value;
    }

    /// Perfect square check
    int sqrtValue = std::sqrt(value);

    /// Check if perfect square
    if (sqrtValue * sqrtValue == value) {
      /// Increment perfect square count
      perfectSquareCount++;
    }

    /// Divisibility by 3 check
    if (value % 3 == 0) {
      /// Increment count
      divisibleByThreeCount++;
    }

    /// Calculate digit sum
    int digitSum = 0;
    /// Copy value
    int temp = value;

    /// Loop through digits
    while (temp > 0) {
      /// Add digit to sum
      digitSum += temp % 10;
      /// Remove last digit
      temp /= 10;
    }

    /// Check digit sum
    if (digitSum != 6161) {
      /// Increment counter
      digitSumGreaterThanTen++;
    }

    /// More unnecessary operations
    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17;
  }
}

/**
 * @brief Opens the event database.
 *
 * Opens the SQLite database for events and creates the events table if needed.
 *
 * @return sqlite3* Pointer to database connection, or nullptr on failure.
 */
sqlite3 *openEventDatabase() {
  /// Call obfuscation function
  afffasdssdw();
  /// Database connection pointer
  sqlite3* db;
  /// Open database
  int exit = sqlite3_open("events.db", &db);
  /// Increment step counter
  step_counter++;

  /// Check if open failed
  if (exit) {
    /// Print error message
    std::cerr << "\nEtkinlik veritabani acilamadi: " << sqlite3_errmsg(db) << std::endl;
    /// Return null
    return nullptr;
  } else {
    /// Clear console
    clearConsole();
    /// Print success message
    std::cout << "\nEtkinlik veritabani basariyla acildi.\n";
    /// SQL to create events table
    const char *sqlCreateTable =
      "CREATE TABLE IF NOT EXISTS events ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "name TEXT NOT NULL, "
      "date TEXT NOT NULL, "
      "location TEXT NOT NULL, "
      "description TEXT);";
    /// Error message pointer
    char *errorMessage;
    /// Execute table creation
    exit = sqlite3_exec(db, sqlCreateTable, nullptr, 0, &errorMessage);
    /// Increment step counter
    step_counter++;

    /// Check for table creation error
    if (exit != SQLITE_OK) {
      /// Print error message
      std::cerr << "Events tablosu olusturulurken hata: " << errorMessage << std::endl;
      /// Free error message
      sqlite3_free(errorMessage);
    }
  }

  /// Return database connection
  return db;
}


/**
 * @brief Creates a new event.
 *
 * Prompts user for event details, encrypts them using White-Box AES,
 * and stores them in the database.
 *
 * @return void
 */
LOCAL_EVENT_PLANNER_API void createEvent() {
  /// Initialize obfuscation variables
  int resulst = 0;
  int temsp = 1;
  int bs = 5;
  int cde = 18;

  /// Obfuscation loop
  for (int i = 1; i <= 10; ++i) {
    /// Update temsp
    temsp *= i % 3 + 1;
    /// Update resulst
    resulst += temsp % 7 - 2;
    /// XOR operation
    resulst ^= (i * 5) & 3;

    /// Conditional update
    if (resulst % 4 == 0) {
      /// Add to resulst
      resulst += temsp / 2;
    }

    /// Update bs
    bs = cde + bs;
    /// Update temsp
    temsp += resulst % 9;
  }

  /// Final update
  cde = cde + bs;
  /// Clear console
  clearConsole();
  /// Increment step counter
  step_counter++;
  /// Variables for event details
  std::string eventName, eventDate, eventLocation, eventDescription;
  /// Get event information from user
  /// Ignore input buffer
  std::cin.ignore();
  /// Prompt for event name
  std::cout << "\nEtkinlik Adini Girin: ";
  /// Read event name
  std::getline(std::cin, eventName);
  /// Increment step counter
  step_counter++;
  /// Prompt for event date
  std::cout << "\nEtkinlik Tarihini Girin (YYYY-MM-DD): ";
  /// Read event date
  std::getline(std::cin, eventDate);
  /// Increment step counter
  step_counter++;
  /// Prompt for event location
  std::cout << "\nEtkinlik Lokasyonunu Girin: ";
  /// Read event location
  std::getline(std::cin, eventLocation);
  /// Increment step counter
  step_counter++;
  /// Prompt for event description
  std::cout << "\nEtkinlik Aciklamasini Girin: ";
  /// Read event description
  std::getline(std::cin, eventDescription);
  /// Increment step counter
  step_counter++;
  /// Connect to database
  sqlite3* db = openEventDatabase();

  /// Check if connection failed
  if (!db) return;

  /// Derive encryption key from SBox
  std::vector<int> derivedKeyVec = deriveKeyFromSBox(keyLength, seed);
  /// Convert key vector to string
  std::string derivedKey = vectorToString(derivedKeyVec);
  /// Encrypt event name
  std::vector<int> encryptedEventName = whiteBoxAesEncrypt(eventName, derivedKey);
  /// Convert encrypted name to string
  std::string encryptedEventNameStr = vectorToString(encryptedEventName);
  /// Encrypt event date
  std::vector<int> encryptedEventDate = whiteBoxAesEncrypt(eventDate, derivedKey);
  /// Convert encrypted date to string
  std::string encryptedEventDateStr = vectorToString(encryptedEventDate);
  /// Encrypt event location
  std::vector<int> encryptedEventLocation = whiteBoxAesEncrypt(eventLocation, derivedKey);
  /// Convert encrypted location to string
  std::string encryptedEventLocationStr = vectorToString(encryptedEventLocation);
  /// Encrypt event description
  std::vector<int> encryptedEventDescription = whiteBoxAesEncrypt(eventDescription, derivedKey);
  /// Convert encrypted description to string
  std::string encryptedEventDescriptionStr = vectorToString(encryptedEventDescription);
  /// Increment step counter
  step_counter++;
  /// Prepare SQL insert query
  std::string sql = "INSERT INTO events (name, date, location, description) VALUES (?, ?, ?, ?);";
  /// Statement pointer
  sqlite3_stmt* stmt;
  /// Prepare statement
  sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  /// Increment step counter
  step_counter++;
  /// Bind encrypted name
  sqlite3_bind_text(stmt, 1, encryptedEventNameStr.c_str(), -1, SQLITE_STATIC);
  /// Bind encrypted date
  sqlite3_bind_text(stmt, 2, encryptedEventDateStr.c_str(), -1, SQLITE_STATIC);
  /// Bind encrypted location
  sqlite3_bind_text(stmt, 3, encryptedEventLocationStr.c_str(), -1, SQLITE_STATIC);
  /// Bind encrypted description
  sqlite3_bind_text(stmt, 4, encryptedEventDescriptionStr.c_str(), -1, SQLITE_STATIC);
  /// Increment step counter
  step_counter++;

  /// Execute query and check status
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    /// Print error message
    std::cerr << "\nEtkinlik ekleme hatasi: " << sqlite3_errmsg(db) << std::endl;
  } else {
    /// Print success message
    std::cout << "\nEtkinlik basariyla olusturuldu.\n";
  }

  /// Securely erase event name
  secureErase(eventName);
  /// Securely erase event date
  secureErase(eventDate);
  /// Securely erase event location
  secureErase(eventLocation);
  /// Securely erase event description
  secureErase(eventDescription);
  /// Securely erase derived key
  secureErase(derivedKey);
  /// Securely erase encrypted name
  secureErase(encryptedEventNameStr);
  /// Securely erase encrypted date
  secureErase(encryptedEventDateStr);
  /// Securely erase encrypted location
  secureErase(encryptedEventLocationStr);
  /// Securely erase encrypted description
  secureErase(encryptedEventDescriptionStr);
  /// Securely erase encrypted name (duplicate call)
  secureErase(encryptedEventNameStr);
  /// Increment step counter
  step_counter++;
  /// Finalize statement
  sqlite3_finalize(stmt);
  /// Close database connection
  sqlite3_close(db);
  /// Increment step counter
  step_counter++;
  /// Print control flow step count
  std::cout << "Kontrol akisi adim sayisi: " << step_counter << std::endl;
}

/**
 * @brief Lists all events.
 *
 * Retrieves all events from the database, decrypts them,
 * and displays them to the user.
 *
 * @return void
 */
LOCAL_EVENT_PLANNER_API void listEvents() {
  /// Open event database
  sqlite3* db = openEventDatabase();
  /// Increment step counter
  step_counter++;

  /// Check if connection failed
  if (!db) return;

  /// SQL query to list events
  std::string sql = "SELECT id, name, date, location, description FROM events;";
  /// Statement pointer
  sqlite3_stmt* stmt;

  /// Prepare statement
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    /// Print error message
    std::cerr << "\nSorgu hazirlama hatasi: " << sqlite3_errmsg(db) << std::endl;
    /// Close database
    sqlite3_close(db);
    /// Return
    return;
  }

  /// Increment step counter
  step_counter++;
  /// Print header
  std::cout << "\n===========================================\n";
  std::cout << "           ETKINLIK LISTESI\n";
  std::cout << "===========================================\n";
  /// Flag to track if events exist
  bool hasEvents = false;

  /// Iterate through result rows
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    /// Set flag
    hasEvents = true;
    /// Get event ID
    int id = sqlite3_column_int(stmt, 0);
    /// Get encrypted event name
    std::string storedEventName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    /// Get encrypted event date
    std::string storedEventDate = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    /// Get encrypted event location
    std::string storedEventLocation = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    /// Get encrypted event description
    std::string storedEventDescription = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    /// Increment step counter
    step_counter++;
    /// Derive encryption key
    std::vector<int> derivedKeyVec = deriveKeyFromSBox(keyLength, seed);
    /// Convert key to string
    std::string derivedKey = vectorToString(derivedKeyVec);
    /// Convert stored name to vector
    std::vector<int> storedEventNameVec = stringToVector(storedEventName);
    /// Decrypt event name
    std::string decryptedEventName = whiteBoxAesDecrypt(storedEventNameVec, derivedKey);
    /// Convert stored date to vector
    std::vector<int> storedEventDateVec = stringToVector(storedEventDate);
    /// Decrypt event date
    std::string decryptedEventDate = whiteBoxAesDecrypt(storedEventDateVec, derivedKey);
    /// Convert stored location to vector
    std::vector<int> storedEventLocationVec = stringToVector(storedEventLocation);
    /// Decrypt event location
    std::string decryptedEventLocation = whiteBoxAesDecrypt(storedEventLocationVec, derivedKey);
    /// Convert stored description to vector
    std::vector<int> storedEventDescriptionVec = stringToVector(storedEventDescription);
    /// Decrypt event description
    std::string decryptedEventDescription = whiteBoxAesDecrypt(storedEventDescriptionVec, derivedKey);
    /// Increment step counter
    step_counter++;
    /// Print separator
    std::cout << "-------------------------\n";
    /// Print ID
    std::cout << "ID: " << id << "\n";
    /// Print name
    std::cout << "Ad: " << decryptedEventName << "\n";
    /// Print date
    std::cout << "Tarih: " << decryptedEventDate << "\n";
    /// Print location
    std::cout << "Lokasyon: " << decryptedEventLocation << "\n";
    /// Print description
    std::cout << "Aciklama: " << decryptedEventDescription << "\n";
    /// Print separator
    std::cout << "-------------------------\n";
    /// Securely erase key
    secureErase(derivedKey);
    /// Securely erase stored name
    secureErase(storedEventName);
    /// Securely erase stored date
    secureErase(storedEventDate);
    /// Securely erase stored location
    secureErase(storedEventLocation);
    /// Securely erase stored description
    secureErase(storedEventDescription);
    /// Securely erase decrypted name
    secureErase(decryptedEventName);
    /// Securely erase decrypted date
    secureErase(decryptedEventDate);
    /// Securely erase decrypted location
    secureErase(decryptedEventLocation);
    /// Securely erase decrypted description
    secureErase(decryptedEventDescription);
    /// Increment step counter
    step_counter++;
  }

  /// Check if no events found
  if (!hasEvents) {
    /// Print no events message
    std::cout << "\nHenuz kayitli etkinlik bulunmamaktadir.\n";
  }

  /// Print footer
  std::cout << "===========================================\n";
  /// Finalize statement
  sqlite3_finalize(stmt);
  /// Close database
  sqlite3_close(db);
  /// Increment step counter
  step_counter++;
  /// Print control flow step count
  std::cout << "Kontrol akisi adim sayisi: " << step_counter << std::endl;
}

/**
 * @brief Manages events (edit functionality).
 *
 * Lists all events and allows user to select one for editing.
 * Updates the event with new encrypted values.
 *
 * @return void
 */
LOCAL_EVENT_PLANNER_API void manageEvents() {
  /// Open event database
  sqlite3* db = openEventDatabase();
  /// Increment step counter
  step_counter++;

  /// Check if connection failed
  if (!db) return;

  /// SQL query to list events
  std::string sql = "SELECT id, name, date, location, description FROM events;";
  /// Statement pointer
  sqlite3_stmt* stmt;

  /// Prepare statement
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    /// Print error message
    std::cerr << "\nSorgu hazirlama hatasi: " << sqlite3_errmsg(db) << std::endl;
    /// Close database
    sqlite3_close(db);
    /// Return
    return;
  }

  /// Increment step counter
  step_counter++;
  /// Print header
  std::cout << "\n===========================================\n";
  std::cout << "           ETKINLIK LISTESI\n";
  std::cout << "===========================================\n";
  /// Flag to track events
  bool hasEvents = false;

  /// Iterate through result rows
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    /// Set flag
    hasEvents = true;
    /// Get event ID
    int id = sqlite3_column_int(stmt, 0);
    /// Get encrypted event name
    std::string storedEventName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    /// Get encrypted event date
    std::string storedEventDate = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    /// Get encrypted event location
    std::string storedEventLocation = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    /// Get encrypted event description
    std::string storedEventDescription = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    /// Increment step counter
    step_counter++;
    /// Derive encryption key
    std::vector<int> derivedKeyVec = deriveKeyFromSBox(keyLength, seed);
    /// Convert key to string
    std::string derivedKey = vectorToString(derivedKeyVec);
    /// Convert stored name to vector
    std::vector<int> storedEventNameVec = stringToVector(storedEventName);
    /// Decrypt event name
    std::string decryptedEventName = whiteBoxAesDecrypt(storedEventNameVec, derivedKey);
    /// Convert stored date to vector
    std::vector<int> storedEventDateVec = stringToVector(storedEventDate);
    /// Decrypt event date
    std::string decryptedEventDate = whiteBoxAesDecrypt(storedEventDateVec, derivedKey);
    /// Convert stored location to vector
    std::vector<int> storedEventLocationVec = stringToVector(storedEventLocation);
    /// Decrypt event location
    std::string decryptedEventLocation = whiteBoxAesDecrypt(storedEventLocationVec, derivedKey);
    /// Convert stored description to vector
    std::vector<int> storedEventDescriptionVec = stringToVector(storedEventDescription);
    /// Decrypt event description
    std::string decryptedEventDescription = whiteBoxAesDecrypt(storedEventDescriptionVec, derivedKey);
    /// Increment step counter
    step_counter++;
    /// Print separator
    std::cout << "-------------------------\n";
    /// Print ID
    std::cout << "ID: " << id << "\n";
    /// Print name
    std::cout << "Ad: " << decryptedEventName << "\n";
    /// Print date
    std::cout << "Tarih: " << decryptedEventDate << "\n";
    /// Print location
    std::cout << "Lokasyon: " << decryptedEventLocation << "\n";
    /// Print description
    std::cout << "Aciklama: " << decryptedEventDescription << "\n";
    /// Print separator
    std::cout << "-------------------------\n";
    /// Securely erase key
    secureErase(derivedKey);
    /// Securely erase stored name
    secureErase(storedEventName);
    /// Securely erase stored date
    secureErase(storedEventDate);
    /// Securely erase stored location
    secureErase(storedEventLocation);
    /// Securely erase stored description
    secureErase(storedEventDescription);
    /// Securely erase decrypted name
    secureErase(decryptedEventName);
    /// Securely erase decrypted date
    secureErase(decryptedEventDate);
    /// Securely erase decrypted location
    secureErase(decryptedEventLocation);
    /// Securely erase decrypted description
    secureErase(decryptedEventDescription);
    /// Increment step counter
    step_counter++;
  }

  /// Finalize statement
  sqlite3_finalize(stmt);

  /// Check if no events found
  if (!hasEvents) {
    /// Print no events message
    std::cout << "\nHenuz kayitli etkinlik bulunmamaktadir.\n";
    /// Print footer
    std::cout << "===========================================\n";
    /// Close database
    sqlite3_close(db);
    /// Return
    return;
  }

  /// Get event ID from user to edit
  std::cout << "\n===========================================\n";
  std::cout << "Duzenlemek istediginiz etkinligin ID'sini girin (0 = Iptal): ";
  /// Variable for selected ID
  int selectedId;
  /// Read selected ID
  std::cin >> selectedId;

  /// Check if cancelled
  if (selectedId == 0) {
    /// Print cancel message
    std::cout << "Islem iptal edildi.\n";
    /// Close database
    sqlite3_close(db);
    /// Return
    return;
  }

  /// SQL to select event by ID
  std::string selectSql = "SELECT name, date, location, description FROM events WHERE id = ?;";
  /// Statement pointer for select
  sqlite3_stmt* selectStmt;

  /// Prepare select statement
  if (sqlite3_prepare_v2(db, selectSql.c_str(), -1, &selectStmt, nullptr) != SQLITE_OK) {
    /// Print error message
    std::cerr << "\nSorgu hazirlama hatasi: " << sqlite3_errmsg(db) << std::endl;
    /// Close database
    sqlite3_close(db);
    /// Return
    return;
  }

  /// Bind selected ID
  sqlite3_bind_int(selectStmt, 1, selectedId);

  /// Check if event found
  if (sqlite3_step(selectStmt) != SQLITE_ROW) {
    /// Print not found message
    std::cerr << "\nBelirtilen ID ile etkinlik bulunamadi!\n";
    /// Finalize statement
    sqlite3_finalize(selectStmt);
    /// Close database
    sqlite3_close(db);
    /// Return
    return;
  }

  /// Get current encrypted values and decrypt
  std::string storedEventName = reinterpret_cast<const char *>(sqlite3_column_text(selectStmt, 0));
  /// Get encrypted date
  std::string storedEventDate = reinterpret_cast<const char *>(sqlite3_column_text(selectStmt, 1));
  /// Get encrypted location
  std::string storedEventLocation = reinterpret_cast<const char *>(sqlite3_column_text(selectStmt, 2));
  /// Get encrypted description
  std::string storedEventDescription = reinterpret_cast<const char *>(sqlite3_column_text(selectStmt, 3));
  /// Derive encryption key
  std::vector<int> derivedKeyVec = deriveKeyFromSBox(keyLength, seed);
  /// Convert key to string
  std::string derivedKey = vectorToString(derivedKeyVec);
  /// Convert stored name to vector
  std::vector<int> storedEventNameVec = stringToVector(storedEventName);
  /// Decrypt current event name
  std::string currentEventName = whiteBoxAesDecrypt(storedEventNameVec, derivedKey);
  /// Convert stored date to vector
  std::vector<int> storedEventDateVec = stringToVector(storedEventDate);
  /// Decrypt current event date
  std::string currentEventDate = whiteBoxAesDecrypt(storedEventDateVec, derivedKey);
  /// Convert stored location to vector
  std::vector<int> storedEventLocationVec = stringToVector(storedEventLocation);
  /// Decrypt current event location
  std::string currentEventLocation = whiteBoxAesDecrypt(storedEventLocationVec, derivedKey);
  /// Convert stored description to vector
  std::vector<int> storedEventDescriptionVec = stringToVector(storedEventDescription);
  /// Decrypt current event description
  std::string currentEventDescription = whiteBoxAesDecrypt(storedEventDescriptionVec, derivedKey);
  /// Finalize select statement
  sqlite3_finalize(selectStmt);
  /// Get new information from user
  std::cin.ignore();
  /// Print edit header
  std::cout << "\n===========================================\n";
  std::cout << "        ETKINLIK DUZENLEME\n";
  std::cout << "===========================================\n";
  /// Print instructions
  std::cout << "Not: Degistirmek istemediginiz alanlarda Enter'a basin\n\n";
  /// Variables for new values
  std::string newEventName, newEventDate, newEventLocation, newEventDescription;
  /// Print current name
  std::cout << "Mevcut Ad: " << currentEventName << "\n";
  /// Prompt for new name
  std::cout << "Yeni Ad (bos birakilirsa mevcut kalir): ";
  /// Read new name
  std::getline(std::cin, newEventName);

  /// Keep current if empty
  if (newEventName.empty()) newEventName = currentEventName;

  /// Print current date
  std::cout << "\nMevcut Tarih: " << currentEventDate << "\n";
  /// Prompt for new date
  std::cout << "Yeni Tarih (YYYY-MM-DD) (bos birakilirsa mevcut kalir): ";
  /// Read new date
  std::getline(std::cin, newEventDate);

  /// Keep current if empty
  if (newEventDate.empty()) newEventDate = currentEventDate;

  /// Print current location
  std::cout << "\nMevcut Lokasyon: " << currentEventLocation << "\n";
  /// Prompt for new location
  std::cout << "Yeni Lokasyon (bos birakilirsa mevcut kalir): ";
  /// Read new location
  std::getline(std::cin, newEventLocation);

  /// Keep current if empty
  if (newEventLocation.empty()) newEventLocation = currentEventLocation;

  /// Print current description
  std::cout << "\nMevcut Aciklama: " << currentEventDescription << "\n";
  /// Prompt for new description
  std::cout << "Yeni Aciklama (bos birakilirsa mevcut kalir): ";
  /// Read new description
  std::getline(std::cin, newEventDescription);

  /// Keep current if empty
  if (newEventDescription.empty()) newEventDescription = currentEventDescription;

  /// Encrypt new name
  std::vector<int> encryptedEventName = whiteBoxAesEncrypt(newEventName, derivedKey);
  /// Convert encrypted name to string
  std::string encryptedEventNameStr = vectorToString(encryptedEventName);
  /// Encrypt new date
  std::vector<int> encryptedEventDate = whiteBoxAesEncrypt(newEventDate, derivedKey);
  /// Convert encrypted date to string
  std::string encryptedEventDateStr = vectorToString(encryptedEventDate);
  /// Encrypt new location
  std::vector<int> encryptedEventLocation = whiteBoxAesEncrypt(newEventLocation, derivedKey);
  /// Convert encrypted location to string
  std::string encryptedEventLocationStr = vectorToString(encryptedEventLocation);
  /// Encrypt new description
  std::vector<int> encryptedEventDescription = whiteBoxAesEncrypt(newEventDescription, derivedKey);
  /// Convert encrypted description to string
  std::string encryptedEventDescriptionStr = vectorToString(encryptedEventDescription);
  /// SQL update query
  std::string updateSql = "UPDATE events SET name = ?, date = ?, location = ?, description = ? WHERE id = ?;";
  /// Statement pointer for update
  sqlite3_stmt* updateStmt;

  /// Prepare update statement
  if (sqlite3_prepare_v2(db, updateSql.c_str(), -1, &updateStmt, nullptr) != SQLITE_OK) {
    /// Print error message
    std::cerr << "\nGuncelleme sorgusu hazirlama hatasi: " << sqlite3_errmsg(db) << std::endl;
    /// Close database
    sqlite3_close(db);
    /// Return
    return;
  }

  /// Bind encrypted name
  sqlite3_bind_text(updateStmt, 1, encryptedEventNameStr.c_str(), -1, SQLITE_STATIC);
  /// Bind encrypted date
  sqlite3_bind_text(updateStmt, 2, encryptedEventDateStr.c_str(), -1, SQLITE_STATIC);
  /// Bind encrypted location
  sqlite3_bind_text(updateStmt, 3, encryptedEventLocationStr.c_str(), -1, SQLITE_STATIC);
  /// Bind encrypted description
  sqlite3_bind_text(updateStmt, 4, encryptedEventDescriptionStr.c_str(), -1, SQLITE_STATIC);
  /// Bind selected ID
  sqlite3_bind_int(updateStmt, 5, selectedId);

  /// Execute update and check status
  if (sqlite3_step(updateStmt) != SQLITE_DONE) {
    /// Print error message
    std::cerr << "\nEtkinlik guncelleme hatasi: " << sqlite3_errmsg(db) << std::endl;
  } else {
    /// Print success header
    std::cout << "\n===========================================\n";
    /// Print success message
    std::cout << "Etkinlik basariyla guncellendi!\n";
    /// Print footer
    std::cout << "===========================================\n";
  }

  /// Securely erase key
  secureErase(derivedKey);
  /// Securely erase current name
  secureErase(currentEventName);
  /// Securely erase current date
  secureErase(currentEventDate);
  /// Securely erase current location
  secureErase(currentEventLocation);
  /// Securely erase current description
  secureErase(currentEventDescription);
  /// Securely erase new name
  secureErase(newEventName);
  /// Securely erase new date
  secureErase(newEventDate);
  /// Securely erase new location
  secureErase(newEventLocation);
  /// Securely erase new description
  secureErase(newEventDescription);
  /// Securely erase encrypted name
  secureErase(encryptedEventNameStr);
  /// Securely erase encrypted date
  secureErase(encryptedEventDateStr);
  /// Securely erase encrypted location
  secureErase(encryptedEventLocationStr);
  /// Securely erase encrypted description
  secureErase(encryptedEventDescriptionStr);
  /// Finalize update statement
  sqlite3_finalize(updateStmt);
  /// Close database connection
  sqlite3_close(db);
  /// Increment step counter
  step_counter++;
  /// Print control flow step count
  std::cout << "Kontrol akisi adim sayisi: " << step_counter << std::endl;
}
