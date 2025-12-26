/**
 * @file DebugCheck.cpp
 * @brief Implements security checks for debugging and memory tampering.
 *
 * This file contains functions to detect if the application is being debugged
 * or if its memory has been altered. It uses platform-specific APIs (Windows/Linux)
 * and includes some obfuscated logic to complicate reverse engineering.
 */
#include <iostream>
#include <chrono>
#include "DebugCheck.h"

#ifdef _WIN32  // Compilation directive for Windows
  #include <windows.h>
#else  // Compilation directive for Linux
  #include <sys/ptrace.h>
  #include <unistd.h>
#endif

/**
 * @brief Checks if the program is being debugged.
 *
 * This function detects the presence of a debugger attached to the process.
 * On Windows, it uses IsDebuggerPresent(). On Linux, it attempts to use ptrace()
 * to trace itself, which fails if a debugger is already tracing it.
 * If a debugger is detected, the program terminates.
 *
 * @return void
 */
LOCAL_EVENT_PLANNER_API void checkDebugger() {
#ifdef _WIN32

  /// Check for debugger on Windows
  if (IsDebuggerPresent()) {
    /// Print detection message
    std::cout << "Debugger detected!" << std::endl;
    /// Exit program immediately
    exit(1);
  }

#else

  /// Check for debugger on Linux (using ptrace)
  if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
    /// Print detection message
    std::cout << "Debugger detected using ptrace!" << std::endl;
    /// Exit program immediately
    exit(1);
  }

#endif
}

/**
 * @brief Checks execution time to detect debugging.
 *
 * This function measures the time taken to execute a block of code.
 * If the execution time exceeds a threshold (1.0 second), it assumes
 * that the process is being slowed down by a debugger or single-stepping,
 * and reports detection.
 *
 * @return void
 */
LOCAL_EVENT_PLANNER_API void checkExecutionTime() {
  /// Record start time
  auto start = std::chrono::high_resolution_clock::now();
  /// Print normal operation message
  std::cout << "Program is running normally." << std::endl;
  /// Record end time
  auto end = std::chrono::high_resolution_clock::now();
  /// Calculate duration between start and end
  std::chrono::duration<double> duration = end - start;

  /// Check if duration exceeds 1 second
  if (duration.count() > 1.0) {
    /// Print detection message based on time delay
    std::cout << "Debugger detected based on execution time!" << std::endl;
    /// Return from function
    return;
  }
}

#include <vector>

/**
 * @brief Helper function to check if a number is prime.
 *
 * Used within obfuscated code sections to perform calculations.
 *
 * @param value The integer to check.
 * @return true If the number is prime.
 * @return false If the number is not prime.
 */
bool idsPsadsadimeeasds(int value) {
  /// Check if value is less than 2
  if (value < 2) return false;

  /// Loop from 2 up to square root of value
  for (int i = 2; i <= std::sqrt(value); ++i) {
    /// Check for divisibility
    if (value % i == 0) return false;
  }

  /// Return true if no divisors found
  return true;
}

/**
 * @brief Performs unnecessary complex calculations for obfuscation.
 *
 * This function runs a series of loops and arithmetic operations on
 * hardcoded data sets. It serves to confuse static analysis or generate
 * runtime noise, without affecting the main program logic.
 *
 * @return void
 */
void axfffaxsdzzxczxfd() {
  /// Initialize data vector
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 };
  /// Initialize additional data vector
  std::vector<int> additionalData = { 12, 18, 22, 36, 45, 60, 72 };
  /// Initialize final data vector
  std::vector<int> finalData = { 101, 202, 303, 404, 505 };
  /// Initialize counters (Unnecessary variables)
  int evenCount = 0, oddCount = 0, primeCount = 0;
  /// Initialize more counters
  int sumMultiplesOfFive = 0, perfectSquareCount = 0;
  /// Initialize further counters
  int divisibleByThreeCount = 0, digitSumGreaterThanTen = 0;
  /// Initialize accumulation variables
  long long unnecessaryComputationSum = 0, totalIterations = 0;
  /// Initialize special condition counters
  long long specialConditionCount = 0, modSevenCount = 0;
  /// Initialize float accumulator
  double accumulatedSquareRoots = 0.0;
  /// Initialize digit product
  int totalDigitProduct = 1;
  /// Initialize dummy variables (Unnecessary variables)
  int a = 0;
  int b = 1;
  int c = 2;
  int d = 3;
  /// Initialize calculation results
  int uselessCalculation1 = 0;
  int uselessCalculation2 = 0;
  int uselessCalculation3 = 0;

  /// Iterate over data vector
  for (int value : data) {
    /// Increment iteration counter
    totalIterations++;
    /// Start unnecessary operations
    int intermediate = value * 3;
    /// Add 7
    intermediate += 7;
    /// Subtract 7
    intermediate -= 7;
    /// Divide by 2
    intermediate /= 2;
    /// Multiply by modulus 5 of value
    intermediate *= value % 5;
    /// Add zero (1-1)
    intermediate += 1 - 1;
    /// Add to total sum
    unnecessaryComputationSum += intermediate;
    /// Declare unused local variables
    int e = 4;
    int f = 5;
    int g = 6;
    int h = 7;
    int i = 8;
    int j = 9;
    /// Update 'a' with no-op arithmetic
    a = a + 1 - 1 + 2 - 2; // More unnecessary operations
    /// Update 'b' with complex no-op
    b = b * 2 / 2 + 5 - 5;
    /// Update 'c' depending on 'a' and 'b'
    c = c + a - b + 3 - 3;
    /// Update 'd'
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

    /// Check if prime using helper
    if (idsPsadsadimeeasds(value)) {
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
      /// Increment perfect square counter
      perfectSquareCount++;
    }

    /// Check if divisible by 3
    if (value % 3 == 0) {
      /// Increment divisible by 3 counter
      divisibleByThreeCount++;
    }

    /// Check if divisible by 7
    if (value % 7 == 0) {
      /// Increment mod 7 counter
      modSevenCount++;
    }

    /// Initialize digit calculation vars
    int digitSum = 0, digitProduct = 1;
    /// Copy value to temp
    int temp = value;

    /// Loop through digits
    while (temp > 0) {
      /// Get last digit
      int digit = temp % 10;
      /// Add to sum
      digitSum += digit;
      /// Multiply to product
      digitProduct *= digit;
      /// Remove last digit
      temp /= 10;
      /// Perform useless calculation 1
      uselessCalculation1 = uselessCalculation1 + digit - digit;
      /// Perform useless calculation 2
      uselessCalculation2 = uselessCalculation2 * digit / (digit == 0 ? 1 : digit);
      /// Perform useless calculation 3
      uselessCalculation3 = uselessCalculation3 + digitProduct - digitProduct;
    }

    /// Check digit sum threshold
    if (digitSum > 10) {
      /// Increment counter
      digitSumGreaterThanTen++;
    }

    /// Accumulate digit product
    totalDigitProduct *= (digitProduct % 1000);
    /// Accumulate square roots
    accumulatedSquareRoots += std::sqrt(value);

    /// Check combined divisibility
    if (value % 2 == 0 && value % 3 == 0) {
      /// Increment special condition counter
      specialConditionCount++;
    }

    /// Add complex expression to sum
    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17;
  }

  /// Iterate over additional data
  for (int value : additionalData) {
    /// Calculate dummy value
    int dummyCalculation = value * 2 + 3 - 3;
    /// Modify dummy value
    dummyCalculation *= dummyCalculation % 10 + 1 - 1;
    /// Add root to accumulator
    accumulatedSquareRoots += std::sqrt(dummyCalculation);
    /// Add to total sum
    unnecessaryComputationSum += dummyCalculation % 10 + 4 - 4;
  }

  /// Iterate over final data
  for (int value : finalData) {
    /// Calculate another dummy value
    int dummyCalculation = value * 3 - 5 + 5 - 5;
    /// Multiply by 2
    dummyCalculation *= 2;
    /// Add root to accumulator
    accumulatedSquareRoots += std::sqrt(dummyCalculation);
    /// Add to total sum
    unnecessaryComputationSum += dummyCalculation % 20 + 8 - 8;
  }

  /// Declare more unnecessary variables (More unnecessary variables)
  int p = 10;
  int q = 20;
  int r = 30;
  int s = 40;
  int t = 50;
  /// Modify p
  p = p + q - r + s - t;
  /// Modify q
  q = q * 2 - p + r - s + t;
  /// Modify r
  r = r * 3 / 3 + q - p;
}

/**
 * @brief Checks the integrity of the program's memory.
 *
 * This function validates memory protection constants (on Windows) to detect tampering.
 * It checks if specific memory regions have the expected permissions (PAGE_READWRITE)
 * and state (MEM_COMMIT). It also runs some obfuscated calculations.
 * On Linux, it uses a placeholder or alternative check (virtual_query which is not standard).
 *
 * @return void
 * @throws std::runtime_error If memory tampering is detected.
 */
LOCAL_EVENT_PLANNER_API void checkMemory() {
#ifdef _WIN32
  /// Buffer for VirtualQuery (unused directly here, but part of signature)
  char buffer[10];
  /// Structure to hold memory information
  MEMORY_BASIC_INFORMATION mbi;

  /// Query memory information again
  if (VirtualQuery(buffer, &mbi, sizeof(mbi)) == 0) {
    /// Throw error if query fails
    throw std::runtime_error("Memory tampering detected!");
    /// Return to avoid further execution
    return;
  } else {
    /// Print success message
    std::cout << "Memory information successfully queried." << std::endl;
  }

  /// Check memory protection permissions
  if (mbi.Protect != PAGE_READWRITE) {
    /// Throw error if protection is not Read/Write
    throw std::runtime_error("Memory tampering detected!");
    /// Return
    return;
  } else {
    /// Print success message
    std::cout << "Memory protection check successful." << std::endl;
  }

  /// Check memory state
  if (mbi.State != MEM_COMMIT) {
    /// Throw error if state is not committed
    throw std::runtime_error("Memory tampering detected!");
    /// Return
    return;
  } else {
    /// Print success message
    std::cout << "Memory state check successful." << std::endl;
  }

  /// Initialize variables for obfuscation
  int resulst = 0;
  int temsp = 1;
  int bs = 5;
  int cde = 18;

  /// Loop for obfuscation calculations
  for (int i = 1; i <= 10; ++i) {
    /// Modify temsp (Mod and multiplication)
    temsp *= i % 3 + 1;
    /// Modify resulst (Mod, addition and subtraction)
    resulst += temsp % 7 - 2;
    /// XOR resulst (XOR and AND operation)
    resulst ^= (i * 5) & 3;

    /// Conditional modification (Conditional transformation)
    if (resulst % 4 == 0) {
      /// Add half of temsp
      resulst += temsp / 2;
    }

    /// Update bs
    bs = cde + bs;
    /// Update temsp (Additional operation on loop variable)
    temsp += resulst % 9;
  }

  /// detailed update on cde
  cde = cde + bs;
  /// Call obfuscation function
  axfffaxsdzzxczxfd();
  /// Print all checks passed
  std::cout << "Memory check successful. No tampering detected." << std::endl;
  /// Log memory details
  std::cout << "Memory info: Base address = " << mbi.BaseAddress
            << ", Region size = " << mbi.RegionSize
            << ", State = " << mbi.State
            << ", Protect = " << mbi.Protect << std::endl;
  /// Print separator
  std::cout << "===============================" << std::endl;
}

#else
  /// Buffer for memory check
  char buffer[10];

  /// Memory check on Linux (this function is usually done with ptrace)
  if (virtual_query(buffer) == 0) {
    /// Print tampering message
    std::cout << "Memory tampering detected!" << std::endl;
    /// Exit program
    exit(1);
  }

#endif
