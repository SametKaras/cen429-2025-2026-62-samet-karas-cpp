// UserAuthentication.cpp
/*
* @file UserAuthentication.cpp
*
* @brief Provides functions for user authentication and registration
*/
#include <random>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>      // for std::setw
#include <sstream>      // for std::stringstream
#include <cstring>      // for memset
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <ctime>        // for time functions
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

/*
* @brief Gets password input with asterisk masking
*
* @return std::string
*/



// Default Key Reader Wrapper
static int defaultKeyReader() {
#ifdef _WIN32
    return _getch();
#else
    return getchar();
#endif
}

// Function pointer for Reading Keys
static int (*KEY_READER)() = defaultKeyReader;

LOCAL_EVENT_PLANNER_API void setMockKeyReader(int (*reader)()) {
    if (reader) {
        KEY_READER = reader;
    } else {
        KEY_READER = defaultKeyReader;
    }
}

LOCAL_EVENT_PLANNER_API std::string getPasswordInput() {
  std::string password;
#ifdef _WIN32
  char ch;

  while (true) {
    // Calling the function pointer - no if/else branch here
    int inputVal = KEY_READER();
    
    // Handle Mock EOF if implemented (-1) or just cast
    if (inputVal == -1) break; 
    
    ch = static_cast<char>(inputVal);

    if (ch == '\r' || ch == '\n') { // Enter tuşu
      std::cout << std::endl;
      break;
    } else if (ch == '\b' && !password.empty()) { // Backspace tuşu
      password.pop_back();
      std::cout << "\b \b"; // Konsoldaki yıldızı sil
    } else if (ch != '\b') { // Normal karakter
      password.push_back(ch);
      std::cout << '*'; // Yıldız göster
    }
  }

#else
  // Linux/Unix 
  termios oldt;
  bool isMocking = (KEY_READER != defaultKeyReader);

  if (!isMocking) {
      tcgetattr(STDIN_FILENO, &oldt);
      termios newt = oldt;
      newt.c_lflag &= ~ECHO;
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  }
  
  char ch;

  while (true) {
    int inputVal = KEY_READER();
    if (inputVal == -1) break;
    ch = static_cast<char>(inputVal);

    if (ch == '\n' || ch == '\r') {
      std::cout << std::endl;
      break;
    } else if (ch == 127 && !password.empty()) { // Backspace
      password.pop_back();
      std::cout << "\b \b";
    } else if (ch != 127) {
      password.push_back(ch);
      std::cout << '*';
    }
  }

  if (!isMocking) {
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  }
#endif
  return password;
}

// Opaklaştırma başlangıcı
unsigned seed0 = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed0);
std::uniform_int_distribution<int> distribution(0, 1000); // Rastgele döngü sayısı için dağılım

int outerLoopCount = distribution(generator) + 5000; // Dış döngü sayısı (5000-6000 arası)
int innerLoopCountBase = distribution(generator); // İç döngü için temel sayı

volatile int dummyVariable = 0; // Optimizasyonu engellemek için volatile değişken

bool isGuestMode = false; // Misafir modu kontrol değişkeni

bool getGuestMode() {
  // Karmaşık döngüler ekleyerek fonksiyonu daha uzun hale getirelim
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      for (int k = 0; k < 3; k++) {
        int temp = (i + j) * k - (i - j);
        temp = temp % 5;

        // Araya anlamsız işlem ekleyelim
        if (temp % 2 == 0) {
          int extra = (i * j + k) / 3;
          extra = extra % 11;
        }
      }
    }
  }

  // Araya başka bir anlamsız işlem ekleyelim
  int dummy = 0;

  for (int x = 0; x < 100; x++) {
    dummy += x * x;
  }

  return isGuestMode; // Misafir modunu döndür
}

void setGuestMode(bool mode) {
  // Karmaşık döngüler ekleyerek fonksiyonu daha uzun hale getirelim
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 10; k++) {
        int result = (i * j + k) % 42;

        if (result % 3 == 0) {
          int temp = (result * 2) / 3;
          // Araya anlamsız işlem ekleyelim
          temp = (temp * 4) / 2;
          result = (temp + i) % 17;
        }
      }
    }
  }

  // Başka bir anlamsız işlem
  int total = 0;

  for (int m = 0; m < 15; m++) {
    total += m * m + m;
  }

  isGuestMode = mode; // Misafir modunu ayarla
}

bool isPasswordStrong(const std::string& password) {
  // Karmaşık ve opak döngü ekleyelim
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 1; j++) {
      for (int k = 0; k < 10; k++) {
        int temp = (i * j + k) % 37;
        temp = temp % 10;

        // Araya anlamsız işlem ekleyelim
        if (temp % 5 == 0) {
          int extra = (i + j + k) * 2;
          extra = extra / 6;
        }
      }
    }
  }

  // Araya başka bir anlamsız işlem ekleyelim
  int sum = 0;

  for (int p = 0; p < 5; p++) {
    sum += p % 4;
  }

  // Minimum 8 karakter, en az bir büyük harf, bir küçük harf, bir rakam ve bir özel karakter içermeli
  const std::regex strongPasswordRegex(R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[!\"#$%&'()*+,-./:;<=>?@[\\\]^_`{|}~])[A-Za-z\d!\"#$%&'()*+,-./:;<=>?@[\\\]^_`{|}~]{8,}$)");
  return std::regex_match(password, strongPasswordRegex);
}






#include <vector>

bool isPimeeasds(int value) {
  if (value < 2) return false;

  for (int i = 2; i <= std::sqrt(value); ++i) {
    if (value % i == 0) return false;
  }

  return true;
}

void afffasdssadsdaqssawsdfssfd() {
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 };
  int evenCount = 0, oddCount = 0, primeCount = 0;
  int sumMultiplesOfFive = 0, perfectSquareCount = 0;
  int divisibleByThreeCount = 0, digitSumGreaterThanTen = 0;
  long long unnecessaryComputationSum = 0;
  long long specialConditionCount = 0, modSevenCount = 0;
  double accumulatedSquareRoots = 0.0;
  int totalDigitProduct = 1;

  for (int value : data) {
    int intermediate = value * 3;
    intermediate += 7;
    intermediate /= 2;
    intermediate *= value % 5;
    unnecessaryComputationSum += intermediate;

    if (value % 2 == 0) {
      evenCount++;
      continue;
    }

    oddCount++;

    if (isPimeeasds(value)) {
      primeCount++;
    }

    if (value % 5 == 0) {
      sumMultiplesOfFive += value;
    }

    int sqrtValue = std::sqrt(value);

    if (sqrtValue * sqrtValue == value) {
      perfectSquareCount++;
    }

    if (value % 3 == 0) {
      divisibleByThreeCount++;
    }

    if (value % 7 == 0) {
      modSevenCount++;
    }

    int digitSum = 0, digitProduct = 1;
    int temp = value;

    while (temp > 0) {
      int digit = temp % 10;
      digitSum += digit;
      digitProduct *= digit;
      temp /= 10;
    }

    if (digitSum > 10) {
      digitSumGreaterThanTen++;
    }

    totalDigitProduct *= (digitProduct % 1000);
    accumulatedSquareRoots += std::sqrt(value);

    if (value % 2 == 0 && value % 3 == 0) {
      specialConditionCount++;
    }

    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17;
  }

  std::vector<int> additionalData = { 12, 18, 22, 36, 45, 60, 72 };

  for (int value : additionalData) {
    int dummyCalculation = value * 2 + 3;
    unnecessaryComputationSum += dummyCalculation % 10;
    accumulatedSquareRoots += std::sqrt(dummyCalculation);
  }

  std::vector<int> finalData = { 101, 202, 303, 404, 505 };

  for (int value : finalData) {
    int dummyCalculation = value * 3 - 5;
    unnecessaryComputationSum += dummyCalculation % 20;
    accumulatedSquareRoots += std::sqrt(dummyCalculation);
  }
}



/*
* @brief Opens the database and creates the table
*
* @return sqlite3*
*/
// Veritabanını açan ve tabloyu oluşturan işlev
sqlite3 *openUserDatabase(const char* dbName) {
  afffasdssadsdaqssawsdfssfd();
  sqlite3* db;
  int exit = sqlite3_open(dbName, &db); // Veritabanını aç

  // Karmaşık döngü sonrası işlem
  for (int a = 0; a < 50; a++) {
    for (int b = 0; b < 30; b++) {
      for (int c = 0; c < 1; c++) {
        int temp = (a + b + c) * (b - a) / 3;
        temp = temp % 5;

        if (temp == 0) {
          temp = (temp + a) * 2;
        }
      }
    }
  }

  if (exit) {
    std::cerr << "Veritabani acilamadi: " << sqlite3_errmsg(db) << std::endl; // Hata mesajını göster
    return nullptr; // Veritabanı açılamadı
  } else {
    // Karmaşık döngüler ve işlemler
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 5; j++) {
        for (int k = 0; k < 2; k++) {
          int temp = (i + j + k) % 11;

          if (temp % 2 == 0) {
            int extra = (i * j + k) / 5;
            extra = extra % 19;
          }
        }
      }

      // Araya başka bir karmaşık işlem ekleyelim
      int result = 0;

      for (int n = 0; n < 50; n++) {
        result += n * (n + 1);
      }
    }

    std::cout << "\nVeritabani basariyla acildi.\n\n";
    const char *sqlCreateTable =
      "CREATE TABLE IF NOT EXISTS users (" // Kullanıcılar için tablo oluştur
      "id INTEGER PRIMARY KEY AUTOINCREMENT, " // Otomatik artan ID
      "username TEXT NOT NULL, " // Kullanıcı adı
      "password TEXT NOT NULL, " // Şifre
      "salt TEXT NOT NULL, " // Şifreleme için kullanılan salt
      "deviceFingerprint TEXT NOT NULL, " // Cihaz parmak izi
      "autoLogin TEXT NOT NULL);"; // Otomatik giriş ayarı
    char *errorMessage;
    exit = sqlite3_exec(db, sqlCreateTable, nullptr, 0, &errorMessage);

    if (exit != SQLITE_OK) {
      std::cerr << "Tablo olusturma hatasi: " << errorMessage << std::endl;
      sqlite3_free(errorMessage); // Hata mesajını temizle
    }
  }

  return db; // Veritabanı bağlantısını döndür
}

/*
* @brief Generates a random salt for password hashing
*
* @return std::string
*/
// Bellekte güvenli bir şekilde sıfırlama işlevi
LOCAL_EVENT_PLANNER_API  void secureErase(std::string& str) {
  if (!str.empty()) {
    // Karmaşık döngü sonrası işlem
    for (int a = 0; a < 5; a++) {
      for (int b = 0; b < 3; b++) {
        for (int c = 0; c < 1; c++) {
          int temp = (a + b + c) * (b - a) / 3;
          temp = temp % 5;

          if (temp == 0) {
            temp = (temp + a) * 2;
          }
        }
      }
    }

    char *buf = &str[0]; // String'in iç belleğine eriş
    memset(buf, 0, str.size()); // Belleği sıfırla
  }
}

/*
* @brief Generates a random salt for password hashing
*
* @return std::string
*/
// Kullanıcıyı veritabanına kaydeden işlev
LOCAL_EVENT_PLANNER_API void registerUser(const char* dbName) {
  sqlite3* db = openUserDatabase(dbName); // Veritabanını aç

  // Şifre kontrolünden sonra uzun bir döngü ekleyelim
  for (int m = 0; m < 50; m++) {
    int temp = (m * 19) % 73;

    for (int n = 0; n < 100; n++) {
      temp = (temp * 5 + n) % 33;
    }
  }

  if (!db) return; // Eğer veritabanı açılamazsa çık

  std::string username, password, autoLogin;
  std::cout << "\n\nKullanici Adini Girin: ";
  std::cin >> username; // Kullanıcı adını al

  // Şifre güçlü olduğunda otomatik giriş tercihinden önce gereksiz döngü ekleyelim
  for (int i = 0; i < 30; i++) {
    int temp = (i * 7) % 23;

    if (temp % 6 == 0) {
      temp = (temp + 3) * 4;
    }
  }

  do {
    std::cout << "Sifreyi Girin: ";
    password = getPasswordInput();

    if (!isPasswordStrong(password)) {
      std::cout << "\n\nLutfen daha guclu bir sifre girin. \n(En az 8 karakter, bir buyuk harf, bir kucuk harf, bir rakam ve bir ozel karakter icermeli)\n";
    }
  } while (!isPasswordStrong(password));// Şifre güçlü değilse tekrar iste

  // Karmaşık döngüye devam
  for (int a = 0; a < 40; a++) {
    for (int b = 0; b < 30; b++) {
      int temp = (a * b) % 17;

      if (temp % 4 == 0) {
        temp = temp * 3;
      }
    }
  }

  std::cout << "Otomatik Giris Acilsin mi (Y/N):";
  std::cin >> autoLogin; // Otomatik giriş tercihini al
  std::vector<int> derivedKeyVec = deriveKeyFromSBox(keyLength, seed); // Oturum anahtarını al ve şifrele
  std::string derivedKey = vectorToString(derivedKeyVec); // Anahtarı string'e dönüştür
  std::string salt = generateFixedSalt("0"); // Salt oluştur
  std::vector<int> encryptedSalt = whiteBoxAesEncrypt(salt, derivedKey); // Salt'ı şifrele
  std::string encryptedSaltStr = vectorToString(encryptedSalt);
  std::string hashedPassword = hashPasswordWithHMAC(password, encryptedSaltStr); // Şifreyi hash'le

  // Karmaşık döngü devam etsin
  for (int i = 0; i < 300; i++) {
    for (int j = 0; j < 200; j++) {
      int temp = (i + j) % 11;

      if (temp % 3 == 0) {
        temp = (temp * 3) / 7;
      }
    }
  }

  std::vector<int> encryptedPassword = whiteBoxAesEncrypt(hashedPassword, derivedKey); // Hash'lenmiş şifreyi şifrele
  std::string encryptedPasswordStr = vectorToString(encryptedPassword);
  std::string deviceFingerprint = getEncryptedDeviceFingerprint(); // Cihaz parmak izini oluştur

  if (deviceFingerprint.empty()) {
    std::cerr << "Cihaz parmak izi olusturulamadi.\n";
    sqlite3_close(db); // Veritabanını kapat
    return;
  }

  std::string sql = "INSERT INTO users (username, password, salt, deviceFingerprint, autoLogin) VALUES (?, ?, ?, ?, ?);";
  sqlite3_stmt* stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Sorgu hazirlama hatasi: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db); // Veritabanını kapat
    return;
  }

  // Karışık işlemler ekleyelim
  for (int x = 0; x < 10; x++) {
    int dummy = (x * x) % 25;
    dummy = dummy + x;

    if (dummy % 5 == 0) {
      dummy = dummy * 2;
    }
  }

  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC); // Kullanıcı adını bağla
  sqlite3_bind_text(stmt, 2, encryptedPasswordStr.c_str(), -1, SQLITE_STATIC); // Şifreyi bağla
  sqlite3_bind_text(stmt, 3, encryptedSaltStr.c_str(), -1, SQLITE_STATIC); // Salt'ı bağla
  sqlite3_bind_text(stmt, 4, deviceFingerprint.c_str(), -1, SQLITE_STATIC); // Parmak izini bağla
  sqlite3_bind_text(stmt, 5, autoLogin.c_str(), -1, SQLITE_STATIC); // Otomatik giriş tercihini bağla

  if (sqlite3_step(stmt) != SQLITE_DONE) {// Sorguyu çalıştır
    std::cerr << "Veri ekleme hatasi: " << sqlite3_errmsg(db) << std::endl;
  } else {
    std::cout << "Kayit basarili.\n";
  }

  // Karmaşık döngüler ekleyelim
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      int temp = (i * j) % 89;

      if (temp % 7 == 0) {
        temp = (temp + i) / 2;
      }
    }
  }

  secureErase(username); // Kullanıcı adını temizle
  secureErase(password); // Şifreyi temizle
  secureErase(autoLogin); // Otomatik giriş bilgisini temizle
  secureErase(hashedPassword); // Hash'lenmiş şifreyi temizle
  secureErase(encryptedPasswordStr); // Şifrelenmiş şifreyi temizle
  secureErase(salt); // Salt'ı temizle
  secureErase(encryptedSaltStr); // Şifrelenmiş salt'ı temizle
  sqlite3_finalize(stmt); // SQL sorgusunu temizle
  sqlite3_close(db); // Veritabanını kapat

  // Şifre kontrolünden sonra uzun bir döngü ekleyelim
  for (int m = 0; m < 50; m++) {
    int temp = (m * 19) % 73;

    for (int n = 0; n < 100; n++) {
      temp = (temp * 5 + n) % 33;
    }
  }
}

/*
* @brief Generates a random salt for password hashing
*
* @return std::string
*/
LOCAL_EVENT_PLANNER_API bool loginUser(const char* dbName) {
  sqlite3* db = openUserDatabase(dbName); // Veritabanını aç

  if (!db) return false; // Veritabanı açılamazsa giriş başarısız

  // Cihaz parmak izini oluştur
  std::string currentFingerprint = getEncryptedDeviceFingerprint();

  // Opaklaştırma başlangıcı
  // Opaklaştırma başlangıcı

  for (int i = 0; i < outerLoopCount; ++i) {
    int innerLoopCount = innerLoopCountBase + distribution(generator); // İç döngü sayısı

    for (int j = 0; j < innerLoopCount; ++j) {
      // Anlamsız işlemler, hesaplamalar
      dummyVariable += (i * j) ^ (j % (i + 1));
      dummyVariable ^= (j * (i + 1)) | (i % (j + 1));
      dummyVariable &= (i | j) + (j & (i + 1));
    }
  }

  //dummyVariable'ı kullanmamak optimizasyona sebep olabileceğinden bunu bir if koşulunda kullanıyoruz
  if (dummyVariable < 0)
    dummyVariable = 0;

  // Opaklaştırma sonu

  if (currentFingerprint.empty()) {
    std::cerr << "Cihaz parmak izi oluşturulamadı.\n";
    sqlite3_close(db); // Veritabanını kapat
    return false;
  }

  // Otomatik giriş kontrolü
  std::string autoLoginQuery = "SELECT username FROM users WHERE deviceFingerprint = ? AND (autoLogin = 'y' OR autoLogin = 'Y');";
  sqlite3_stmt* autoLoginStmt;

  if (sqlite3_prepare_v2(db, autoLoginQuery.c_str(), -1, &autoLoginStmt, nullptr) != SQLITE_OK) {// Otomatik giriş sorgusu hazırla
    std::cerr << "Otomatik giriş sorgusu hazırlanamadı: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db); // Veritabanını kapat
    return false;
  }

  sqlite3_bind_text(autoLoginStmt, 1, currentFingerprint.c_str(), -1, SQLITE_STATIC); // Parmak izini sorguya bağla
  // Opaklaştırma başlangıcı

  for (int i = 0; i < outerLoopCount; ++i) {
    int innerLoopCount = innerLoopCountBase + distribution(generator); // İç döngü sayısı

    for (int j = 0; j < innerLoopCount; ++j) {
      // Anlamsız işlemler, hesaplamalar
      dummyVariable += (i * j) ^ (j % (i + 1));
      dummyVariable ^= (j * (i + 1)) | (i % (j + 1));
      dummyVariable &= (i | j) + (j & (i + 1));
      // Küçük gecikme ekleyerek zamanlama analizini zorlaştırabiliriz (isteğe bağlı)
      // std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
  }

  //dummyVariable'ı kullanmamak optimizasyona sebep olabileceğinden bunu bir if koşulunda kullanıyoruz
  if (dummyVariable < 0)
    dummyVariable = 0;

  // Opaklaştırma sonu

  if (sqlite3_step(autoLoginStmt) == SQLITE_ROW) { // Otomatik giriş başarısı
    sqlite3_finalize(autoLoginStmt); // Otomatik giriş sorgusunu temizle
    //// Ana menüyü görüntüle
    //if (displayMainMenu()) {
    //  std::cout << "Program sonlandiriliyor...\n";
    //  sqlite3_close(db); // Veritabanını kapat
    //  //exit(0); // Programı sonlandır
    //  //return false;// kapanabilir
    //}
    sqlite3_close(db); // Veritabanını kapat
    return true; // Otomatik giriş başarılı
  }

  sqlite3_finalize(autoLoginStmt); // Otomatik giriş sorgusunu temizle
  // Otomatik giriş başarısız, kullanıcı adı ve şifreyle giriş yap
  std::string username, password;
  std::cout << "===============================\n";
  std::cout << "Kullanici Adini Girin: ";
  std::cin >> username; // Kullanıcı adını al
  std::cout << "Sifreyi Girin: ";
  password = getPasswordInput(); // Şifreyi yıldızlı olarak al
  std::cout << "===============================\n";
  // Veritabanından kullanıcı bilgilerini al
  std::string sql = "SELECT password, salt FROM users WHERE username = ?;";
  sqlite3_stmt* stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Sorgu hazirlama hatasi: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db); // Veritabanını kapat
    return false;
  }

  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC); // Kullanıcı adını bağla
  bool loginSuccess = false; // Giriş durumunu kontrol eden bayrak

  if (sqlite3_step(stmt) == SQLITE_ROW) { // Eğer kullanıcı adı eşleşirse
    std::vector<int> derivedKeyVec = deriveKeyFromSBox(keyLength, seed); // Oturum anahtarını al
    std::string derivedKey = vectorToString(derivedKeyVec); // Anahtarı string'e dönüştür
    std::string storedSalt = reinterpret_cast<const char *>(sqlite3_column_blob(stmt, 1)); // Salt'ı al
    std::string storedAesPassword = reinterpret_cast<const char *>(sqlite3_column_blob(stmt, 0)); // Şifrelenmiş şifreyi al
    //std::cout << "Stored Salt: " << storedSalt << std::endl;
    //std::cout << "Stored AES Password: " << storedAesPassword << std::endl;
    std::string hashedPassword = hashPasswordWithHMAC(password, storedSalt); // Şifreyi hash'le
    //std::cout << "Hashed Password: " << hashedPassword << std::endl;
    std::vector<int> storedAesPasswordVec = stringToVector(storedAesPassword); // Şifrelenmiş şifreyi vektöre dönüştür
    std::string decryptedAesPassword = whiteBoxAesDecrypt(storedAesPasswordVec, derivedKey); // Şifreyi çöz

    //std::cout << "Decrypted Password: " << decryptedAesPassword << std::endl;

    if (hashedPassword == decryptedAesPassword) { // Eğer hash'ler eşleşirse giriş başarılı
      std::cout << "Giris basarili.\n";
      std::cout << "===============================\n";

      if (!collectAndEncryptSessionData(stmt)) { // Oturum verilerini şifrele
        std::cerr << "Oturum verileri şifrelenemedi.\n";
        sqlite3_finalize(stmt); // Sorguyu temizle
        sqlite3_close(db); // Veritabanını kapat
        return false; // Başarısızlık döndür
      }

      std::cout << "\nHosgeldin " << username << "!\n\n"; // Kullanıcıya hoş geldiniz mesajı
      //// Ana menüyü görüntüle
      //if (displayMainMenu()) {
      //  std::cout << "Program sonlandiriliyor...\n";
      //  sqlite3_finalize(stmt); // Sorguyu temizle
      //  sqlite3_close(db); // Veritabanını kapat
      //  exit(0); // Programı sonlandır
      //}
      loginSuccess = true; // Giriş başarılı
      secureErase(storedSalt); // Salt'ı bellekte sıfırla
      secureErase(storedAesPassword); // Şifrelenmiş şifreyi bellekte sıfırla
      secureErase(hashedPassword); // Hash'lenmiş şifreyi bellekte sıfırla
      secureErase(decryptedAesPassword); // Çözülmüş şifreyi bellekte sıfırla
    } else {
      std::cout << "Gecersiz kullanici adi veya sifre.\n"; // Eğer şifre eşleşmezse
    }
  } else {
    std::cout << "Gecersiz kullanici adi veya sifre.\n"; // Eğer kullanıcı bulunamazsa
  }

  secureErase(username);
  secureErase(password); // Şifreyi bellekte sıfırla
  sqlite3_finalize(stmt); // Sorguyu temizle
  sqlite3_close(db); // Veritabanını kapat
  return loginSuccess; // Giriş durumunu döndür
}
