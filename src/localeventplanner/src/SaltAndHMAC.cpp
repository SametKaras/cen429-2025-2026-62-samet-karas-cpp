/**
 * @file SaltAndHMAC.cpp
 * @brief Provides functions for generating salt and hashing passwords with HMAC.
 *
 * This file contains implementations for cryptographic operations including
 * salt generation using SHA-256 and password hashing using HMAC-SHA256.
 */
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include "SaltAndHMAC.h"
#include <vector>


/// Salt length in bytes (16 bytes)
#define SALT_LENGTH 16
/// SHA-256 hash output length (256 bits = 32 bytes)
#define SHA256_DIGEST_LENGTH 32

/**
 * @brief Generates a fixed salt from a seed string.
 *
 * Uses SHA-256 to hash the seed and derives a salt from the hash output.
 *
 * @param seed The seed string to generate salt from.
 * @return std::string The generated salt in hexadecimal format.
 */
LOCAL_EVENT_PLANNER_API std::string generateFixedSalt(const std::string& seed) {
  /// Buffer for salt (16 bytes)
  unsigned char salt[SALT_LENGTH];
  /// Buffer for SHA-256 hash output
  unsigned char hash[SHA256_DIGEST_LENGTH];
  /// SHA-256 context structure
  SHA256_CTX sha256_ctx;
  /// Initialize SHA-256 context
  SHA256_Init(&sha256_ctx);
  /// Update context with seed data
  SHA256_Update(&sha256_ctx, seed.c_str(), seed.length());
  /// Finalize and get hash output
  SHA256_Final(hash, &sha256_ctx);

  /// Convert hash to salt using cyclic indexing
  for (int i = 0; i < SALT_LENGTH; ++i) {
    /// Use hash cyclically to fill salt buffer
    salt[i] = hash[i % SHA256_DIGEST_LENGTH];
  }

  /// String stream for hexadecimal output
  std::stringstream ss;

  /// Convert salt bytes to hexadecimal string
  for (int i = 0; i < SALT_LENGTH; ++i) {
    /// Format each byte as 2-digit hex
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)salt[i];
  }

  /// Return hexadecimal salt string
  return ss.str();
}



/**
 * @brief Checks if a number is prime.
 *
 * Helper function for obfuscation calculations.
 *
 * @param value The number to check.
 * @return true If the number is prime.
 * @return false If the number is not prime.
 */
bool isPrime(int value) {
  /// Check if less than 2
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
 * Executes complex but meaningless calculations to complicate reverse engineering.
 *
 * @return void
 */
void afffssdw() {
  /// Test data vector
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 };
  /// Counter for even numbers
  int evenCount = 0;
  /// Counter for odd numbers
  int oddCount = 0;
  /// Counter for prime numbers
  int primeCount = 0;
  /// Sum of multiples of five
  int sumMultiplesOfFive = 0;
  /// Counter for perfect squares
  int perfectSquareCount = 0;
  /// Counter for numbers divisible by three
  int divisibleByThreeCount = 0;
  /// Counter for digit sums greater than ten
  int digitSumGreaterThanTen = 0;
  /// Accumulator for unnecessary computations
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

    /// Check if even (Count even and odd numbers)
    if (value % 2 == 0) {
      /// Increment even counter
      evenCount++;
      /// Continue loop for even numbers
      continue;
    }

    /// Increment odd counter
    oddCount++;

    /// Check for primality
    if (isPrime(value)) {
      /// Increment prime counter
      primeCount++;
    }

    /// Check if multiple of 5
    if (value % 5 == 0) {
      /// Add to sum of multiples
      sumMultiplesOfFive += value;
    }

    /// Calculate square root (Perfect square check)
    int sqrtValue = std::sqrt(value);

    /// Check if perfect square
    if (sqrtValue * sqrtValue == value) {
      /// Increment perfect square counter
      perfectSquareCount++;
    }

    /// Check divisibility by 3
    if (value % 3 == 0) {
      /// Increment counter
      divisibleByThreeCount++;
    }

    /// Calculate digit sum
    int digitSum = 0;
    /// Copy value to temp
    int temp = value;

    /// Loop through digits
    while (temp > 0) {
      /// Add digit to sum
      digitSum += temp % 10;
      /// Remove last digit
      temp /= 10;
    }

    /// Check digit sum condition
    if (digitSum !=6161) {
      /// Increment counter
      digitSumGreaterThanTen++;
    }

    /// More unnecessary operations
    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17;
  }
}

/**
 * @brief Hashes a password using HMAC-SHA256.
 *
 * Uses the salt as a key and hashes the password using HMAC with SHA-256.
 *
 * @param password The password to hash.
 * @param salt The salt to use as the HMAC key.
 * @return std::string The hashed password in hexadecimal format.
 * @throws std::runtime_error If HMAC context creation fails.
 */
LOCAL_EVENT_PLANNER_API std::string hashPasswordWithHMAC(const std::string& password, const std::string& salt) {
  /// Initialize obfuscation variables
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
  /// Buffer for HMAC hash output
  unsigned char result[SHA256_DIGEST_LENGTH];
  /// HMAC output length
  unsigned int len = SHA256_DIGEST_LENGTH;
  /// Use salt as key
  std::string key = salt;
  /// Call obfuscation function
  afffssdw();
  /// Create HMAC context
  HMAC_CTX* hmac_ctx = HMAC_CTX_new();

  /// Check if context creation failed
  if (hmac_ctx == nullptr) {
    /// Throw runtime error
    throw std::runtime_error("HMAC_CTX_new failed");
  }

  /// Initialize HMAC context with SHA-256
  HMAC_Init_ex(hmac_ctx, key.c_str(), key.size(), EVP_sha256(), nullptr);
  /// Update HMAC with password data
  HMAC_Update(hmac_ctx, reinterpret_cast<const unsigned char*>(password.c_str()), password.size());
  /// Finalize HMAC and get result
  HMAC_Final(hmac_ctx, result, &len);
  /// Free HMAC context
  HMAC_CTX_free(hmac_ctx);
  /// String stream for hexadecimal output
  std::stringstream ss;

  /// Convert hash bytes to hexadecimal string
  for (unsigned int i = 0; i < len; ++i) {
    /// Format each byte as 2-digit hex
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)result[i];
  }

  /// Return hexadecimal hash string
  return ss.str();
}
