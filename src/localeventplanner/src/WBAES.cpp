/**
 * @file WBAES.cpp
 * @brief Provides functions for White-Box AES encryption.
 *
 * This file contains functions for encrypting and decrypting data using
 * White-Box AES encryption. It includes the S-Box table, encryption/decryption
 * algorithms, and helper functions for data conversion.
 */
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include "WBAES.h"

/// AES S-Box substitution table (256 entries)
const std::vector<int> S_BOX = {
  0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
  0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
  0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
  0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
  0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
  0x53, 0xD1, 0x5F, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
  0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
  0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
  0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
  0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
  0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
  0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
  0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
  0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
  0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
  0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

/// Seed for S-Box key derivation
unsigned char seed = 0x42;
/// Key length for encryption
size_t keyLength = 4;

/**
 * @brief Creates a White-Box encryption table.
 *
 * Generates a 256-element lookup table based on the S-Box values.
 *
 * @return std::vector<int> The generated White-Box table.
 */
std::vector<int> createWhiteBoxTable() {
  /// Create a table with 256 elements
  std::vector<int> table(256);

  /// Fill the table using S-Box values
  for (int i = 0; i < 256; ++i) {
    /// Use cyclic S-Box indexing
    table[i] = S_BOX[i % S_BOX.size()];
  }

  /// Return the generated table
  return table;
}

/**
 * @brief Encrypts data using White-Box AES.
 *
 * Encrypts the plaintext using a key-dependent lookup in the White-Box table.
 *
 * @param plaintext The data to encrypt.
 * @param key The encryption key.
 * @return std::vector<int> The encrypted data as integer values.
 * @throws std::invalid_argument If key is empty.
 */
std::vector<int> whiteBoxAesEncrypt(const std::string& plaintext, const std::string& key) {
  /// Create the White-Box encryption table
  const auto whiteBoxTable = createWhiteBoxTable();

  /// Validate key is not empty
  if (key.empty()) {
    /// Throw exception for empty key
    throw std::invalid_argument("Anahtar bos olamaz!");
  }

  /// Convert key to vector of integers
  std::vector<int> derivedKey(key.begin(), key.end());
  /// Vector to store encrypted values
  std::vector<int> encrypted;

  /// Encrypt each character
  for (size_t i = 0; i < plaintext.size(); ++i) {
    /// Get plaintext character value
    int charValue = static_cast<unsigned char>(plaintext[i]);
    /// Get cyclic key part mod 256
    int keyPart = derivedKey[i % derivedKey.size()] % 256;
    /// Look up encrypted value in table
    encrypted.push_back(whiteBoxTable[(charValue + keyPart) % 256]);
  }

  /// Return encrypted vector
  return encrypted;
}



/**
 * @brief Derives a key from the S-Box table.
 *
 * Generates a key of specified length using S-Box values starting from seed.
 *
 * @param keyLength The desired key length.
 * @param seed The seed value to start from.
 * @return std::vector<int> The derived key.
 * @throws std::invalid_argument If keyLength is 0.
 */
std::vector<int> deriveKeyFromSBox(size_t keyLength, unsigned char seed) {
  /// Validate key length is not zero
  if (keyLength == 0) {
    /// Throw exception for zero key length
    throw std::invalid_argument("keyLength 0 olamaz!");
  }

  /// Create key vector
  std::vector<int> key(keyLength);

  /// Fill key from S-Box
  for (size_t i = 0; i < keyLength; ++i) {
    /// Get S-Box value at (seed + i) mod 256
    key[i] = static_cast<int>(S_BOX[(seed + i) % 256]);
  }

  /// Return derived key
  return key;
}


/**
 * @brief Decrypts data using White-Box AES.
 *
 * Decrypts the ciphertext by reversing the White-Box table lookup.
 *
 * @param ciphertext The encrypted data as integer values.
 * @param key The decryption key.
 * @return std::string The decrypted plaintext.
 * @throws std::runtime_error If decryption fails.
 */
std::string whiteBoxAesDecrypt(const std::vector<int> &ciphertext, const std::string& key) {
  /// Create the White-Box encryption table
  const auto whiteBoxTable = createWhiteBoxTable();
  /// Convert key to vector of integers
  std::vector<int> derivedKey(key.begin(), key.end());
  /// String to store decrypted text
  std::string decrypted;

  /// Decrypt each value
  for (size_t i = 0; i < ciphertext.size(); ++i) {
    /// Get encrypted value
    int charValue = ciphertext[i];
    /// Get cyclic key part
    int keyPart = derivedKey[i % derivedKey.size()];
    /// Find index in White-Box table
    auto it = std::find(whiteBoxTable.begin(), whiteBoxTable.end(), charValue);

    /// Check if value was found
    if (it == whiteBoxTable.end()) {
      /// Throw error if not found
      throw std::runtime_error("Ciphertext çözülemiyor!");
    }

    /// Calculate original character value
    int originalChar = (std::distance(whiteBoxTable.begin(), it) - keyPart + 256) % 256;
    /// Append to decrypted string
    decrypted += static_cast<char>(originalChar);
  }

  /// Return decrypted text
  return decrypted;
}

/**
 * @brief Converts a vector of integers to a string.
 *
 * Each integer is cast to a character and appended to the result.
 *
 * @param vec The vector to convert.
 * @return std::string The resulting string.
 */
std::string vectorToString(const std::vector<int> &vec) {
  /// Result string
  std::string result;

  /// Convert each integer to character
  for (int value : vec) {
    /// Cast to char and append
    result += static_cast<char>(value);
  }

  /// Return the string
  return result;
}

#include <vector>

/**
 * @brief Checks if a number is prime (obfuscation helper).
 *
 * @param value The number to check.
 * @return bool True if prime.
 */
bool idddvgggs(int value) {
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
void ttasxcfd() {
  /// Test data vectors
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 };
  std::vector<int> additionalData = { 12, 18, 22, 36, 45, 60, 72 };
  std::vector<int> finalData = { 101, 202, 303, 404, 505 };
  /// Unnecessary variables
  int evenCount = 0, oddCount = 0, primeCount = 0;
  int sumMultiplesOfFive = 0, perfectSquareCount = 0;
  int divisibleByThreeCount = 0, digitSumGreaterThanTen = 0;
  long long unnecessaryComputationSum = 0, totalIterations = 0;
  long long specialConditionCount = 0, modSevenCount = 0;
  double accumulatedSquareRoots = 0.0;
  int totalDigitProduct = 1;
  /// More unnecessary variables
  int a = 0;
  int b = 1;
  int c = 2;
  int d = 3;
  int uselessCalculation1 = 0;
  int uselessCalculation2 = 0;
  int uselessCalculation3 = 0;

  /// Iterate through data
  for (int value : data) {
    /// Increment iteration counter
    totalIterations++;
    /// Unnecessary operations
    int intermediate = value * 3;
    /// Add 7
    intermediate += 7;
    /// Subtract 7
    intermediate -= 7;
    /// Divide by 2
    intermediate /= 2;
    /// Multiply by mod 5
    intermediate *= value % 5;
    /// Add 1-1 (zero)
    intermediate += 1 - 1;
    /// Add to sum
    unnecessaryComputationSum += intermediate;
    /// Declare unused local variables
    int e = 4;
    int f = 5;
    int g = 6;
    int h = 7;
    int i = 8;
    int j = 9;
    /// More unnecessary operations on 'a'
    a = a + 1 - 1 + 2 - 2;
    /// Operations on 'b'
    b = b * 2 / 2 + 5 - 5;
    /// Operations on 'c'
    c = c + a - b + 3 - 3;
    /// Operations on 'd'
    d = d * 4 / 4 - c + 6 - 6;

    /// Check if even
    if (value % 2 == 0) {
      /// Increment even counter
      evenCount++;
      /// Continue to next iteration
      continue;
    }

    /// Increment odd counter
    oddCount++;

    /// Check if prime
    if (idddvgggs(value)) {
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
    /// Copy value to temp
    int temp = value;

    /// Loop through digits
    while (temp > 0) {
      /// Get digit
      int digit = temp % 10;
      /// Add to sum
      digitSum += digit;
      /// Multiply to product
      digitProduct *= digit;
      /// Remove last digit
      temp /= 10;
      /// Useless calculation 1
      uselessCalculation1 = uselessCalculation1 + digit - digit;
      /// Useless calculation 2
      uselessCalculation2 = uselessCalculation2 * digit / (digit == 0 ? 1 : digit);
      /// Useless calculation 3
      uselessCalculation3 = uselessCalculation3 + digitProduct - digitProduct;
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

    /// Check combined condition (6's multiple)
    if (value % 2 == 0 && value % 3 == 0) {
      /// Increment counter
      specialConditionCount++;
    }

    /// More unnecessary operations
    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17;
  }

  /// Iterate through additional data
  for (int value : additionalData) {
    /// Dummy calculation
    int dummyCalculation = value * 2 + 3 - 3;
    /// More operations
    dummyCalculation *= dummyCalculation % 10 + 1 - 1;
    /// Add to roots
    accumulatedSquareRoots += std::sqrt(dummyCalculation);
    /// Add to sum
    unnecessaryComputationSum += dummyCalculation % 10 + 4 - 4;
  }

  /// Iterate through final data
  for (int value : finalData) {
    /// Dummy calculation
    int dummyCalculation = value * 3 - 5 + 5 - 5;
    /// Multiply by 2
    dummyCalculation *= 2;
    /// Add to roots
    accumulatedSquareRoots += std::sqrt(dummyCalculation);
    /// Add to sum
    unnecessaryComputationSum += dummyCalculation % 20 + 8 - 8;
  }

  /// More unnecessary variables
  int p = 10;
  int q = 20;
  int r = 30;
  int s = 40;
  int t = 50;
  /// Update p
  p = p + q - r + s - t;
  /// Update q
  q = q * 2 - p + r - s + t;
  /// Update r
  r = r * 3 / 3 + q - p;
}

/**
 * @brief Converts a string to a vector of integers.
 *
 * Each character is cast to an integer and added to the vector.
 * Includes obfuscation calculations.
 *
 * @param str The string to convert.
 * @return std::vector<int> The resulting vector.
 */
std::vector<int> stringToVector(const std::string& str) {
  /// Call obfuscation function
  ttasxcfd();
  /// Obfuscation variables
  int resulst = 0;
  int temsp = 1;
  int bs = 5;
  int cde = 18;

  /// Obfuscation loop
  for (int i = 1; i <= 10; ++i) {
    /// Update temsp (Mod and multiplication)
    temsp *= i % 3 + 1;
    /// Update resulst (Mod, addition and subtraction)
    resulst += temsp % 7 - 2;
    /// XOR operation (XOR and AND)
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
  /// Result vector
  std::vector<int> result;

  /// Convert each character to integer
  for (unsigned char c : str) {
    /// Cast to int and add to vector
    result.push_back(static_cast<int>(c));
  }

  /// Return the vector
  return result;
}
