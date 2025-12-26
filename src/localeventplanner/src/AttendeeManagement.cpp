/**
 * @file AttendeeManagement.cpp
 * @brief Provides functions for managing attendees.
 *
 * This file contains implementations for attendee registration, tracking,
 * and database interactions, including necessary security measures like
 * White-Box AES encryption and secure memory erasure.
 */
#include <iostream>
#include <string>
#include <vector>
#include "AttendeeManagement.h"
#include "sqlite3.h"
#include "UserAuthentication.h"
#include "DisplayMainMenu.h"
#include "WBAES.h"
#include "SessionEncryption.h"

/**
 * @brief Checks if a number is prime.
 *
 * This function determines whether the given integer is a prime number.
 * It iterates from 2 up to the square root of the number.
 *
 * @param value The number to check for primality.
 * @return true If the number is prime.
 * @return false If the number is not prime (less than 2 or divisible by another number).
 */
bool sxxxlfdw(int value) {
  /// Check if the value is less than 2, which is not prime
  if (value < 2) return false;

  /// Iterate from 2 to the square root of the value
  for (int i = 2; i <= std::sqrt(value); ++i) {
    /// If value is divisible by i, it is not prime
    if (value % i == 0) return false;
  }

  /// If no divisors are found, the number is prime
  return true;
}

/**
 * @brief Performs unnecessary complex calculations.
 *
 * This function executes a series of mathematical operations and loops
 * using hardcoded data. It appears to be for obfuscation or generating
 * CPU load/entropy, as the results are mostly local or unused aside from
 * a 'step_counter' (if it were global, but here it uses local variables).
 * Note: 'step_counter' is not used here, but logic suggests obfuscation.
 *
 * @return void
 */
void ttassxcfd() {
  /// Initialize a vector of integers with specific data points
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 };
  /// Initialize a vector with additional integer data points
  std::vector<int> additionalData = { 12, 18, 22, 36, 45, 60, 72 };
  /// Initialize a vector with final integer data points
  std::vector<int> finalData = { 101, 202, 303, 404, 505 };

  /// Initialize counters for various logical conditions (Unnecessary variables)
  int evenCount = 0, oddCount = 0, primeCount = 0;
  /// Initialize sums and counts for specific modulo checks
  int sumMultiplesOfFive = 0, perfectSquareCount = 0;
  /// Initialize counts for divisibility by three and digit sum checks
  int divisibleByThreeCount = 0, digitSumGreaterThanTen = 0;
  /// Initialize long long variables for accumulation to avoid overflow
  long long unnecessaryComputationSum = 0, totalIterations = 0;
  /// Initialize counters for special combined conditions
  long long specialConditionCount = 0, modSevenCount = 0;
  /// Initialize double for accumulating square roots
  double accumulatedSquareRoots = 0.0;
  /// Initialize variable for total product of digits
  int totalDigitProduct = 1;

  /// Initialize variables for arithmetic operations (Unnecessary variables)
  int a = 0;
  int b = 1;
  int c = 2;
  int d = 3;
  /// Initialize variables to store results of useless calculations
  int uselessCalculation1 = 0;
  int uselessCalculation2 = 0;
  int uselessCalculation3 = 0;

  /// Loop through each value in the main data vector
  for (int value : data) {
    /// Increment the total iteration counter
    totalIterations++;
    /// Perform intermediate calculation: multiply value by 3 (Unnecessary operations start)
    int intermediate = value * 3;
    /// Add 7 to the intermediate value
    intermediate += 7;
    /// Subtract 7 from the intermediate value (redundant)
    intermediate -= 7;
    /// Divide the intermediate value by 2
    intermediate /= 2;
    /// Multiply by the result of value modulo 5
    intermediate *= value % 5;
    /// Add (1 - 1) which is 0
    intermediate += 1 - 1;
    /// Add the result to the global computation sum
    unnecessaryComputationSum += intermediate;

    /// Initialize temporary loop variables
    int e = 4;
    int f = 5;
    int g = 6;
    int h = 7;
    int i = 8;
    int j = 9;

    /// Perform redundant arithmetic instruction on 'a'
    a = a + 1 - 1 + 2 - 2; // More unnecessary operations
    /// Perform complex arithmetic on 'b'
    b = b * 2 / 2 + 5 - 5;
    /// Calculate 'c' using 'a' and 'b' and redundant constants
    c = c + a - b + 3 - 3;
    /// Calculate 'd' using 'c' and redundant constants
    d = d * 4 / 4 - c + 6 - 6;

    /// Check if the value is even
    if (value % 2 == 0) {
      /// Increment even count
      evenCount++;
      /// Skip to the next iteration
      continue;
    }

    /// Increment odd count if not even
    oddCount++;

    /// Check if the value is prime using helper function
    if (sxxxlfdw(value)) {
      /// Increment prime number count
      primeCount++;
    }

    /// Check if the value is divisible by 5
    if (value % 5 == 0) {
      /// Add value to sum of multiples of five
      sumMultiplesOfFive += value;
    }

    /// Calculate integer square root of the value
    int sqrtValue = std::sqrt(value);

    /// Check if the value is a perfect square
    if (sqrtValue * sqrtValue == value) {
      /// Increment perfect square count
      perfectSquareCount++;
    }

    /// Check if the value is divisible by 3
    if (value % 3 == 0) {
      /// Increment count for numbers divisible by 3
      divisibleByThreeCount++;
    }

    /// Check if the value is divisible by 7
    if (value % 7 == 0) {
      /// Increment count for numbers divisible by 7
      modSevenCount++;
    }

    /// Initialize digit sum to 0 and digit product to 1
    int digitSum = 0, digitProduct = 1;
    /// Create a temporary copy of the value for digit processing
    int temp = value;

    /// Loop while there are digits left in temp
    while (temp > 0) {
      /// Extract the last digit
      int digit = temp % 10;
      /// Add digit to digit sum
      digitSum += digit;
      /// Multiply digit into digit product
      digitProduct *= digit;
      /// Remove the last digit from temp
      temp /= 10;

      /// Perform useless calculation 1 with the digit
      uselessCalculation1 = uselessCalculation1 + digit - digit;
      /// Perform useless calculation 2 with check for zero division
      uselessCalculation2 = uselessCalculation2 * digit / (digit == 0 ? 1 : digit);
      /// Perform useless calculation 3
      uselessCalculation3 = uselessCalculation3 + digitProduct - digitProduct;
    }

    /// Check if the sum of digits is greater than 10
    if (digitSum > 10) {
      /// Increment the counter
      digitSumGreaterThanTen++;
    }

    /// Update total digit product with current product mod 1000
    totalDigitProduct *= (digitProduct % 1000);
    /// Accumulate the square root of the value
    accumulatedSquareRoots += std::sqrt(value);

    /// Check if value is divisible by both 2 and 3
    if (value % 2 == 0 && value % 3 == 0) {
      /// Increment special condition counter
      specialConditionCount++;
    }

    /// Add a complex calculation result to unnecessary computation sum
    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17;
  }

  /// Loop through additional data vector
  for (int value : additionalData) {
    /// Perform dummy calculation
    int dummyCalculation = value * 2 + 3 - 3;
    /// Apply further dummy modifications
    dummyCalculation *= dummyCalculation % 10 + 1 - 1;
    /// Add square root of dummy calc to accumulator
    accumulatedSquareRoots += std::sqrt(dummyCalculation);
    /// Update computation sum with dummy calc result
    unnecessaryComputationSum += dummyCalculation % 10 + 4 - 4;
  }

  /// Loop through final data vector
  for (int value : finalData) {
    /// Perform another dummy calculation pattern
    int dummyCalculation = value * 3 - 5 + 5 - 5;
    /// Double the dummy result
    dummyCalculation *= 2;
    /// Add square root to accumulator
    accumulatedSquareRoots += std::sqrt(dummyCalculation);
    /// Update computation sum
    unnecessaryComputationSum += dummyCalculation % 20 + 8 - 8;
  }

  /// Initialize more unnecessary variables (More unnecessarily variables)
  int p = 10;
  int q = 20;
  int r = 30;
  int s = 40;
  int t = 50;
  /// Update p with arithmetic combination
  p = p + q - r + s - t;
  /// Update q with arithmetic combination
  q = q * 2 - p + r - s + t;
  /// Update r with arithmetic combination
  r = r * 3 / 3 + q - p;
}

/**
 * @brief Opens the attendee database connection.
 *
 * This function handles the connection to the SQLite database 'attendees.db'.
 * It includes obfuscation loops before opening the database. It also ensures the
 * 'attendees' table exists.
 *
 * @return sqlite3* Pointer to the SQLite database connection, or nullptr on failure.
 */
/// Function for database connection
sqlite3 *openAttendeeDatabase() {
  /// Declare database pointer
  sqlite3* db;
  /// Initialize result variable for obfuscation
  int resulst = 0;
  /// Initialize temp variable for obfuscation
  int temsp = 1;
  /// Initialize bs variable for obfuscation
  int bs = 5;
  /// Initialize cde variable for obfuscation
  int cde = 18;

  /// Execute an obfuscation loop 10 times
  for (int i = 1; i <= 10; ++i) {
    /// Multiply temsp by (i mod 3 + 1) (Mod and multiplication)
    temsp *= i % 3 + 1;
    /// Add (temsp mod 7 - 2) to resulst (Mod, addition and subtraction)
    resulst += temsp % 7 - 2;
    /// XOR resulst with ((i * 5) AND 3) (XOR and AND operation)
    resulst ^= (i * 5) & 3;

    /// Check if resulst is divisible by 4 (Conditional transformation)
    if (resulst % 4 == 0) {
      /// Add half of temsp to resulst
      resulst += temsp / 2;
    }

    /// Update bs by adding cde
    bs = cde + bs;
    /// Add (resulst mod 9) to temsp (Additional operation on loop variable)
    temsp += resulst % 9;
  }

  /// Update cde by adding bs
  cde = cde + bs;
  /// Open database connection to "attendees.db"
  int exit = sqlite3_open("attendees.db", &db);
  /// Increment control flow step counter (Database open operation)
  step_counter++;

  /// Execute nested loops for delay/obfuscation (50x10x20 iterations)
  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 10; j++) {
      for (int k = 0; k < 20; k++) {
        /// Check specific modulo conditions
        if (i % 2 == 0 && j % 3 == 0 && k % 5 == 0) {
           /// Calculate temp value
          int temp = (i * j * k) / 42;
          /// Reset temp with modulo
          temp = temp % 67;
        }
      }
    }
  }

  /// Check if database open failed
  if (exit) {
    /// Print error message to stderr if connection failed
    std::cerr << "Katilimci veritabani acilamadi: " << sqlite3_errmsg(db) << std::endl;
    /// Return nullptr indicating failure
    return nullptr;
  } else {
    /// Define SQL query to create 'attendees' table if it doesn't exist
    const char *sqlCreateTable =
      "CREATE TABLE IF NOT EXISTS attendees ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "name TEXT NOT NULL, "
      "email TEXT NOT NULL, "
      "phone TEXT);";
    /// Declare error message pointer
    char *errorMessage;
    /// Execute the SQL statement
    exit = sqlite3_exec(db, sqlCreateTable, nullptr, 0, &errorMessage);
    /// Increment control flow step counter (Table creation operation)
    step_counter++;

    /// Check if table creation was not successful
    if (exit != SQLITE_OK) {
      /// Print error message if table creation failed
      std::cerr << "Attendees tablosu olusturulurken hata: " << errorMessage << std::endl;
      /// Free the error message memory
      sqlite3_free(errorMessage);
    }

    /// Execute complex nested loops for obfuscation
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 50; j++) {
        for (int k = 0; k < 30; k++) {
          /// Calculate result based on indices
          int result = (i * j * k) + (i + j + k) * 3;

          /// Check if result is divisible by 9
          if (result % 9 == 0) {
            /// Initialize temp as double result
            int temp = result * 2;
            /// Divide temp by 7
            temp = temp / 7;
          }
        }
      }
    }
  }

  /// Execute more complex loops (Other complex loops)
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      for (int k = 0; k < 50; k++) {
        /// Calculate temp value
        int temp = (i + j) * (k - 3);

        /// Check if temp is divisible by 4
        if (temp % 4 == 0) {
          /// Halve the temp value
          temp = temp / 2;
        }
      }
    }
  }

  /// Call obfusc function
  ttassxcfd();
  /// Return the database connection
  return db;
}


/**
 * @brief Registers a new attendee.
 *
 * This function prompts the user for attendee details (Name, Email, Phone),
 * encrypts this data using White-Box AES, and stores it in the database.
 * It also securely erases sensitive data from memory.
 *
 * @return void
 */
LOCAL_EVENT_PLANNER_API void registerAttendees() {
  /// Clear the console screen
  clearConsole();
  /// Increment control flow step counter (Console clear op)
  step_counter++;
  /// Declare strings for attendee details
  std::string attendeeName, attendeeEmail, attendeePhone;
  /// Get attendee information from user
  /// Ignore remaining newline in input buffer
  std::cin.ignore();
  /// Prompt for attendee name
  std::cout << "\nKatilimci Adini Girin: ";
  /// Read attendee name line
  std::getline(std::cin, attendeeName);
  /// Increment control flow step counter (Get name op)
  step_counter++;
  /// Prompt for attendee email
  std::cout << "\nKatilimci Email'ini Girin: ";
  /// Read attendee email line
  std::getline(std::cin, attendeeEmail);
  /// Increment control flow step counter (Get email op)
  step_counter++;
  /// Prompt for attendee phone
  std::cout << "\nKatilimci Telefonunu Girin: ";
  /// Read attendee phone line
  std::getline(std::cin, attendeePhone);
  /// Increment control flow step counter (Get phone op)
  step_counter++;
  /// Open database connection
  sqlite3* db = openAttendeeDatabase();

  /// Check if database connection is valid
  if (!db) return; // Exit if connection failed

  /// Derive encryption key from SBox using global seed and key length
  std::vector<int> derivedKeyVec = deriveKeyFromSBox(keyLength, seed);
  /// Convert derived key vector to string
  std::string derivedKey = vectorToString(derivedKeyVec);
  /// Encrypt attendee name
  std::vector<int> encryptedAttendeeName = whiteBoxAesEncrypt(attendeeName, derivedKey);
  /// Convert encrypted name vector to string
  std::string encryptedAttendeeNameStr = vectorToString(encryptedAttendeeName);
  /// Encrypt attendee email
  std::vector<int> encryptedAttendeeEmail = whiteBoxAesEncrypt(attendeeEmail, derivedKey);
  /// Convert encrypted email vector to string
  std::string encryptedAttendeeEmailStr = vectorToString(encryptedAttendeeEmail);
  /// Encrypt attendee phone
  std::vector<int> encryptedAttendeePhone = whiteBoxAesEncrypt(attendeePhone, derivedKey);
  /// Convert encrypted phone vector to string
  std::string encryptedAttendeePhoneStr = vectorToString(encryptedAttendeePhone);
  /// Increment control flow step counter (AES encryption op)
  step_counter++;
  /// Prepare SQL query string
  std::string sql = "INSERT INTO attendees (name, email, phone) VALUES (?, ?, ?);";
  /// Declare statement pointer
  sqlite3_stmt* stmt;

  /// Prepare the SQL statement
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    /// In case of error, print message
    std::cerr << "Sorgu hazirlama hatasi: " << sqlite3_errmsg(db) << std::endl;
    /// Close database connection
    sqlite3_close(db);
    /// Return from function
    return;
  }

  /// Increment control flow step counter (Prepare statement op)
  step_counter++;
  /// Bind encrypted name to statement
  sqlite3_bind_text(stmt, 1, encryptedAttendeeNameStr.c_str(), -1, SQLITE_STATIC);
  /// Bind encrypted email to statement
  sqlite3_bind_text(stmt, 2, encryptedAttendeeEmailStr.c_str(), -1, SQLITE_STATIC);
  /// Bind encrypted phone to statement
  sqlite3_bind_text(stmt, 3, encryptedAttendeePhoneStr.c_str(), -1, SQLITE_STATIC);
  /// Increment control flow step counter (Bind params op)
  step_counter++;

  /// Execute the query and check status
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    /// Clear console on failure
    clearConsole();
    /// Increment control flow step counter (Execute op)
    step_counter++;
    /// Print error message
    std::cerr << "Katilimci ekleme hatasi: " << sqlite3_errmsg(db) << std::endl;
  } else {
    /// Clear console on success
    clearConsole();
    /// Increment control flow step counter (Success op)
    step_counter++;
    /// Print success message
    std::cout << "\nKatilimci basariyla kaydedildi.\n";
  }

  /// Securely erase sensitive data (name)
  secureErase(attendeeName);
  /// Securely erase sensitive data (email)
  secureErase(attendeeEmail);
  /// Securely erase sensitive data (phone)
  secureErase(attendeePhone);
  /// Securely erase sensitive data (key)
  secureErase(derivedKey);
  /// Securely erase sensitive data (encrypted name)
  secureErase(encryptedAttendeeNameStr);
  /// Securely erase sensitive data (encrypted email)
  secureErase(encryptedAttendeeEmailStr);
  /// Securely erase sensitive data (encrypted phone)
  secureErase(encryptedAttendeePhoneStr);
  /// Increment control flow step counter (Memory cleanup op)
  step_counter++;
  /// Finalize the statement
  sqlite3_finalize(stmt);
  /// Close the database connection
  sqlite3_close(db);
  /// Increment control flow step counter (DB Close op)
  step_counter++;
  /// Print control flow step count
  std::cout << "Kontrol akisi adim sayisi: " << step_counter << std::endl;
}

/**
 * @brief Tracks and lists attendees.
 *
 * This function retrieves all attendees from the database, decrypts their
 * information, and displays it to the console. It also handles secure
 * erasure of decrypted data from memory.
 *
 * @return void
 */
LOCAL_EVENT_PLANNER_API void trackAttendees() {
  /// Clear the console
  clearConsole();
  /// Increment control flow step counter
  step_counter++;
  /// Open database connection
  sqlite3* db = openAttendeeDatabase();
  /// Increment control flow step counter (DB Open op)
  step_counter++;

  /// Check if connection is valid
  if (!db) return; // Exit if failed

  /// Define SQL query to select all attendees
  std::string sql = "SELECT id, name, email, phone FROM attendees;";
  /// Declare statement pointer
  sqlite3_stmt* stmt;

  /// Prepare the SQL statement
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    /// Print error message on failure
    std::cerr << "Sorgu hazirlama hatasi: " << sqlite3_errmsg(db) << std::endl;
    /// Close database
    sqlite3_close(db);
    /// Return from function
    return;
  }

  /// Increment control flow step counter (Prepare op)
  step_counter++;
  /// Print header for attendee list
  std::cout << "\nKatilimci Listesi:\n";

  /// Iterate through all rows in resulting set
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    /// Get ID column
    int id = sqlite3_column_int(stmt, 0);
    /// Get encrypted name column
    std::string storedAttendeeName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    /// Get encrypted email column
    std::string storedAttendeeEmail = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    /// Get encrypted phone column
    std::string storedAttendeePhone = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    /// Increment control flow step counter (Fetch cols op)
    step_counter++;
    /// Derive encryption key
    std::vector<int> derivedKeyVec = deriveKeyFromSBox(keyLength, seed);
    /// Convert derived key to string
    std::string derivedKey = vectorToString(derivedKeyVec);
    /// Convert stored name to vector
    std::vector<int> storedAttendeeNameVec = stringToVector(storedAttendeeName);
    /// Decrypt name
    std::string decryptedAttendeeName = whiteBoxAesDecrypt(storedAttendeeNameVec, derivedKey);
    /// Convert stored email to vector
    std::vector<int> storedAttendeeEmailVec = stringToVector(storedAttendeeEmail);
    /// Decrypt email
    std::string decryptedAttendeeEmail = whiteBoxAesDecrypt(storedAttendeeEmailVec, derivedKey);
    /// Convert stored phone to vector
    std::vector<int> storedAttendeePhoneVec = stringToVector(storedAttendeePhone);
    /// Decrypt phone
    std::string decryptedAttendeePhone = whiteBoxAesDecrypt(storedAttendeePhoneVec, derivedKey);
    /// Increment control flow step counter (Decrypt op)
    step_counter++;
    /// Print separator
    std::cout << "-------------------------\n";
    /// Print ID
    std::cout << "ID: " << id << "\n";
    /// Print Name
    std::cout << "Ad: " << decryptedAttendeeName << "\n";
    /// Print Email
    std::cout << "Email: " << decryptedAttendeeEmail << "\n";
    /// Print Phone
    std::cout << "Telefon: " << decryptedAttendeePhone << "\n";
    /// Print separator
    std::cout << "-------------------------\n";
    /// Securely erase key
    secureErase(derivedKey);
    /// Securely erase stored name
    secureErase(storedAttendeeName);
    /// Securely erase stored email
    secureErase(storedAttendeeEmail);
    /// Securely erase stored phone
    secureErase(storedAttendeePhone);
    /// Securely erase decrypted name
    secureErase(decryptedAttendeeName);
    /// Securely erase decrypted email
    secureErase(decryptedAttendeeEmail);
    /// Securely erase decrypted phone
    secureErase(decryptedAttendeePhone);
    /// Increment control flow step counter (Secure erase op)
    step_counter++;
  }

  /// Finalize the statement
  sqlite3_finalize(stmt);
  /// Close the database
  sqlite3_close(db);
  /// Increment control flow step counter (Cleanup op)
  step_counter++;
  /// Print control flow step count
  std::cout << "Kontrol akisi adim sayisi: " << step_counter << std::endl;
}
