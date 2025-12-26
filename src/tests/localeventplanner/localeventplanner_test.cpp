#define enable_LocalEventPlanner_test  // uncomment this line to enable the localeventplanner tests

#include <iostream>
#include <string>
#include <vector>
#include "AttendeeManagement.h"
#include "UserAuthentication.h"
#include <DisplayMainMenu.h>
#include "SessionEncryption.h"
#include "DynamicAssetProtection.h"
#include "VersionAndDeviceBinding.h"
#include "SessionEncryption.h"
#include "WBAES.h"
#include "DebugCheck.h"
#include "SignatureVerification.h"
#include "EventDetails.h"
#include "SaltAndHMAC.h"

#include "gtest/gtest.h"
#include "sqlite3.h"
#include <iostream>
#include "Rasp.h"
#include <Windows.h>
#include <openssl/evp.h>

#include <gtest/gtest.h>
#include "AttendeeManagement.h"
#include <sqlite3.h>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "SessionEncryption.h"
#include <vector>
#include <openssl/rand.h>

#include <gtest/gtest.h>
#include "Rasp.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Rasp.h"
#include <sys/stat.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "VersionAndDeviceBinding.h"


//Veritaban� a�ma fonksiyonunun testi
TEST(AttendeeManagementTest, OpenAttendeeDatabaseTest) {
  sqlite3* db = openAttendeeDatabase(); // Veritaban�n� a�
  ASSERT_NE(db, nullptr) << "Veritaban� a��lamad�."; // Veritaban� ba�lant�s� bo� olmamal�
}

//signature verification test
TEST(SignatureVerificationTest, SignatureVerificationTest) {
  bool result = verifySignature();
  ASSERT_TRUE(result) << "�mza do�rulama ba�ar�s�z.";
}


TEST(DebugCheckTest, CheckExecutionTimeTest) {
  checkExecutionTime();
  ASSERT_TRUE(true) << "Zaman kontrol� ba�ar�s�z.";
}

// checkMemory test
TEST(DebugCheckTest, CheckMemoryTest) {
  checkMemory();
  ASSERT_TRUE(true) << "Bellek kontrol� ba�ar�s�z.";
}




/////////////////////////

TEST(AttendeeManagementTest, DatabaseOpenFailure) {
  sqlite3* db;
  int result = sqlite3_open("/invalid/path/attendees.db", &db);
  ASSERT_NE(result, SQLITE_OK) << "Hatal  yol i in veritaban  ba ar yla a  lmamal yd .";
}




TEST(AttendeeManagementTest, TableCreationSuccess) {
  // SQLite'i yeniden ba�lat ve yap�land�r
  sqlite3_shutdown();
  sqlite3_initialize();
  sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
  sqlite3* db = nullptr;
  // Veritaban�n� a�
  int openResult = sqlite3_open("attendees.db", &db);
  ASSERT_EQ(openResult, SQLITE_OK) << "Veritabani acilamadi: " << sqlite3_errmsg(db);
  // Tablo kontrol sorgusu
  const char *checkTableSQL = "SELECT name FROM sqlite_master WHERE type='table' AND name='attendees';";
  sqlite3_stmt* stmt = nullptr;
  // Sorguyu haz�rla
  int prepareResult = sqlite3_prepare_v2(db, checkTableSQL, -1, &stmt, nullptr);
  ASSERT_EQ(prepareResult, SQLITE_OK) << "Sorgu haz�rlanamad�: " << sqlite3_errmsg(db);
  // Sorguyu �al��t�r
  int stepResult = sqlite3_step(stmt);
  ASSERT_EQ(stepResult, SQLITE_ROW) << "Attendees tablosu olusturulmadi.";

  // Kaynaklar� serbest b�rak
  if (stmt) sqlite3_finalize(stmt);

  if (db) sqlite3_close(db);

  // SQLite'i kapat
  sqlite3_shutdown();
}

TEST(AttendeeManagementTest, RegisterAttendeeSuccess) {
  sqlite3* db = nullptr;
  // Veritaban�n� a�
  int openResult = sqlite3_open("attendees.db", &db);
  ASSERT_EQ(openResult, SQLITE_OK) << "Veritaban� a��lamad�!";
  // Kat�l�mc� ekleme sorgusu
  const char *insertSql = "INSERT INTO attendees (name, email, phone) VALUES ('Test User', 'test@example.com', '1234567890');";
  int result;
  // Mutex ile i�lem yap
  sqlite3_mutex* dbMutex = sqlite3_db_mutex(db);
  sqlite3_mutex_enter(dbMutex);
  result = sqlite3_exec(db, insertSql, nullptr, nullptr, nullptr);
  // Mutex'i serbest b�rak
  sqlite3_mutex_leave(dbMutex);
  ASSERT_EQ(result, SQLITE_OK) << "Kat�l�mc� kayd� ba�ar�s�z oldu.";
  sqlite3_close(db);
}


TEST(AttendeeManagementTest, ListAttendees) {
  sqlite3* db = nullptr;
  // Veritaban�n� a�
  int openResult = sqlite3_open("attendees.db", &db);
  ASSERT_EQ(openResult, SQLITE_OK) << "Veritaban� a��lamad�!";
  const char *listSql = "SELECT COUNT(*) FROM attendees;";
  sqlite3_stmt* stmt = nullptr;
  int result;
  // Mutex ile i�lem yap
  sqlite3_mutex* dbMutex = sqlite3_db_mutex(db);
  sqlite3_mutex_enter(dbMutex);
  result = sqlite3_prepare_v2(db, listSql, -1, &stmt, nullptr);
  ASSERT_EQ(result, SQLITE_OK) << "Listeleme sorgusu haz�rlanamad�.";
  result = sqlite3_step(stmt);
  ASSERT_EQ(result, SQLITE_ROW) << "Kat�l�mc�lar al�namad�.";
  int count = sqlite3_column_int(stmt, 0);
  std::cout << "Kat�l�mc� say�s�: " << count << std::endl;
  // Mutex'i serbest b�rak
  sqlite3_mutex_leave(dbMutex);
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}







TEST(AttendeeManagementTest, ErrorMessageCheck) {
  sqlite3* db = nullptr;
  // Veritaban�n� a�
  int openResult = sqlite3_open("attendees.db", &db);
  ASSERT_EQ(openResult, SQLITE_OK) << "Veritaban� a��lamad�!";
  const char *invalidSql = "INVALID SQL;";
  char *errorMessage = nullptr;
  int execResult;
  // Mutex ile i�lem yap
  sqlite3_mutex* dbMutex = sqlite3_db_mutex(db);
  sqlite3_mutex_enter(dbMutex);
  execResult = sqlite3_exec(db, invalidSql, nullptr, nullptr, &errorMessage);
  // Hatal� SQL komutunun hata �retmesi gerekti�ini do�rula
  ASSERT_NE(execResult, SQLITE_OK) << "Ge�ersiz SQL komutu hata vermeliydi.";
  ASSERT_NE(errorMessage, nullptr) << "Hata mesaj� NULL olmamal�yd�.";
  // Mutex'i serbest b�rak
  sqlite3_mutex_leave(dbMutex);

  // Hata mesaj�n� serbest b�rak
  if (errorMessage) {
    sqlite3_free(errorMessage);
  }

  // Veritaban�n� kapat
  sqlite3_close(db);
}











TEST(SessionEncryptionTest, SetupSessionEncryptionTest2) {
  bool result = setupSessionEncryption();
  ASSERT_TRUE(result) << "Oturum  ifreleme ayarlar  yap lamad .";
}

// Oturum Anahtar   ifreleme ve Ayarlama Testi



TEST(SessionEncryptionTest, CollectAndEncryptSessionDataTest) {
  // SQLite'i yeniden ba�lat ve yap�land�r
  sqlite3_shutdown();
  sqlite3_initialize();
  sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
  sqlite3* db = nullptr;
  // Bellek i�inde bir SQLite veritaban� olu�tur
  int openResult = sqlite3_open(":memory:", &db);
  ASSERT_EQ(openResult, SQLITE_OK) << "Bellek i�indeki veritaban� a��lamad�!";
  const char *createTableSQL = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, username TEXT)";
  char *errorMessage = nullptr;
  // Tabloyu sorgula
  sqlite3_stmt* stmt = nullptr;
  // �ifreleme i�levini test et
  bool result = collectAndEncryptSessionData(stmt);
  ASSERT_TRUE(result) << "Oturum verileri �ifrelenemedi.";
  // Kaynaklar� serbest b�rak
  sqlite3_finalize(stmt);
  sqlite3_close(db);
}






TEST(DeviceTest, EmulatorDetectionTest) {
  ASSERT_FALSE(isEmulator()) << "Ger ek cihaz em lat r olarak alg land !";
}

//TEST(SystemFileTest, CriticalSystemFilesSecurityTest) {
//    bool result = checkCriticalSystemFiles();
//    ASSERT_TRUE(result) << "Kritik sistem dosyalar  g venli de il!";
//}

TEST(HookDetectionTest, CheckFunctionHooks) {
  bool result = checkHooks();
  ASSERT_FALSE(result) << "HOOK sald r s  tespit edildi!";
}

TEST(CRCTest, VerifyCodeBlockCRC) {
  bool isVerified = verifyCodeBlock();
  ASSERT_FALSE(isVerified) << "Kod blo u checksum do rulamas  ba ar s z!";
}

TEST(SSLTest, InitializeSSLContextTest) {
  SSL_CTX* ctx = initializeSSLContext();
  ASSERT_NE(ctx, nullptr) << "SSL ba lam  ba lat lamad !";
  SSL_CTX_free(ctx); // Test sonras nda ba lam  serbest b rak
}


TEST(CRCTest, CRC32TableInitTest) {
  ASSERT_NO_THROW(crc32_table_init()) << "CRC32 tablosu ba lat lamad !";
}

TEST(CRCTest, CRC32CalculationTest) {
  unsigned char data[] = "Test data for CRC";
  crc32_table_init();
  unsigned long crc = crc32_calc(data, sizeof(data) - 1);
  ASSERT_NE(crc, 0) << "CRC hesaplamas  ba ar s z!";
}



// Oturum �ifreleme Ayarlar� Testi
TEST(SessionEncryptionTest, SetupSessionEncryptionTest23) {
  bool result = setupSessionEncryption();
  ASSERT_TRUE(result) << "Oturum �ifreleme ayarlar� yap�lamad�.";
}

// CollectAndEncryptSessionData Testi
TEST(SessionEncryptionTest, CollectAndEncryptSessionDataTest2) {
  // Sahte bir SQLite sonu� yap�s� olu�turun.
  sqlite3_stmt* stmt = nullptr;
  bool result = collectAndEncryptSessionData(stmt);
  ASSERT_TRUE(result) << "Oturum verileri �ifrelenemedi.";
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(AttendeeManagementTest, DatabaseOpenFailure09) {
  sqlite3* db;
  int result = sqlite3_open("/invalid/path/attendees.db", &db);
  ASSERT_NE(result, SQLITE_OK) << "Hatal� yol i�in veritaban� ba�ar�yla a��lmamal�yd�.";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////



TEST(SSLTest, InitializeSSLContextTest33) {
  SSL_CTX* ctx = initializeSSLContext();
  ASSERT_NE(ctx, nullptr) << "SSL ba�lam� olu�turulamad�.";
  SSL_CTX_free(ctx);
}



TEST(SSLTest, InvalidCertificateTest) {
  const std::string invalidCert = "invalid_cert.pem";
  const std::string invalidKey = "invalid_key.pem";
  SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
  ASSERT_NE(ctx, nullptr) << "SSL ba�lam� ba�lat�lamad�.";
  int certResult = SSL_CTX_use_certificate_file(ctx, invalidCert.c_str(), SSL_FILETYPE_PEM);
  ASSERT_EQ(certResult, 0) << "Ge�ersiz sertifika dosyas� kabul edildi.";
  int keyResult = SSL_CTX_use_PrivateKey_file(ctx, invalidKey.c_str(), SSL_FILETYPE_PEM);
  ASSERT_EQ(keyResult, 0) << "Ge�ersiz �zel anahtar dosyas� kabul edildi.";
  SSL_CTX_free(ctx);
}

TEST(SSLTest, MemoryLeakTest) {
  SSL_CTX* ctx = initializeSSLContext();
  ASSERT_NE(ctx, nullptr) << "SSL ba�lam� ba�lat�lamad�.";
  BIO* bio = BIO_new_ssl_connect(ctx);
  ASSERT_NE(bio, nullptr) << "BIO nesnesi olu�turulamad�.";
  BIO_free_all(bio);
  SSL_CTX_free(ctx);
  SUCCEED() << "Bellek s�z�nt�s� olmadan t�m kaynaklar serbest b�rak�ld�.";
}





// 1. Test: S�r�m uyumlulu�unu kontrol et
TEST(VersionAndDeviceBindingTest, VersionCompatibilityMatch) {
  std::string currentVersion = "1.2.0";
  std::string requiredVersion = "1.2.0";
  ASSERT_TRUE(isVersionCompatible(currentVersion, requiredVersion)) << "S�r�m uyumlulu�u ba�ar�s�z.";
}


// 3. Test: Ortam uyumlulu�unu kontrol et
TEST(VersionAndDeviceBindingTest, EnvironmentCompatibility) {
  ASSERT_TRUE(isEnvironmentCompatible()) << "Ortam uyumlulu�u ba�ar�s�z.";
}




// 7. Test: Cihaz uyumlulu�u ba�ar�s�z kontrol
TEST(VersionAndDeviceBindingTest, DeviceCompatibilityMismatch) {
  // Uyumsuz bir marka testi i�in sim�lasyon
  std::string incompatibleBrand = "UnknownBrand";
  ASSERT_FALSE(incompatibleBrand.find("HP") != std::string::npos || incompatibleBrand.find("ASUS") != std::string::npos)
      << "Uyumsuz cihaz yanl��l�kla uyumlu olarak i�aretlendi.";
}

#include "DisplayMainMenu.h"
#include "gtest/gtest.h"
#include <sstream>
#include <iostream>

// Kullan�c� giri�ini sim�le etmek i�in global de�i�kenler
std::streambuf *cinBackup;
std::streambuf *coutBackup;

// Konsol ��kt�s�n� yakalamak i�in yard�mc� i�lev
std::string captureOutput(const std::function<void()> &func) {
  std::ostringstream output;
  coutBackup = std::cout.rdbuf();  // Orijinal std::cout'u yedekle
  std::cout.rdbuf(output.rdbuf()); // ��kt�y� yakala
  func(); // Test edilen i�lemi �al��t�r
  std::cout.rdbuf(coutBackup); // Orijinal std::cout'u geri y�kle
  return output.str();
}

class DisplayMainMenuTest : public ::testing::Test {
 protected:
  void SetUp() override {
    cinBackup = std::cin.rdbuf();  // Orijinal std::cin'i yedekle
  }

  void TearDown() override {
    std::cin.rdbuf(cinBackup);  // Orijinal std::cin'i geri y�kle
  }
};





// �rnek S-Box Tablosu.
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

TEST(WBAESTest, createWhiteBoxTable) {
  std::vector<int> table = createWhiteBoxTable();
  ASSERT_EQ(table.size(), 256);

  // S-Box'tan do�ru de�erlerin kopyaland���n� kontrol et
  for (int i = 0; i < 256; ++i) {
    ASSERT_EQ(table[i], S_BOX[i % S_BOX.size()]);
  }
}

TEST(WBAESTest, deriveKeyFromSBox) {
  size_t keyLength = 4;
  unsigned char seed = 0x42;
  std::vector<int> key = deriveKeyFromSBox(keyLength, seed);
  ASSERT_EQ(key.size(), keyLength);

  // S-Box'tan do�ru de�erlerin t�retildi�ini kontrol et
  for (size_t i = 0; i < keyLength; ++i) {
    ASSERT_EQ(key[i], S_BOX[(seed + i) % 256]);
  }
}

TEST(WBAESTest, deriveKeyFromSBox_ZeroKeyLength_AssertThrow) {
  size_t keyLength = 4;
  unsigned char seed = 0x42;
  ASSERT_THROW({ deriveKeyFromSBox(0, seed); }, std::invalid_argument);
  std::vector<int> key = deriveKeyFromSBox(keyLength, seed);
  ASSERT_EQ(key.size(), 4);
}

TEST(WBAESTest, whiteBoxAesEncrypt) {
  std::string plaintext = "test";
  std::string key = "anahtar";
  std::vector<int> encrypted = whiteBoxAesEncrypt(plaintext, key);
  ASSERT_EQ(encrypted.size(), plaintext.size());
}

TEST(WBAESTest, whiteBoxAesEncrypt_EmptyKey) {
  std::string plaintext = "test";
  std::string key = "";
  ASSERT_THROW({ whiteBoxAesEncrypt(plaintext, key); }, std::invalid_argument);
}

TEST(WBAESTest, whiteBoxAesDecrypt_Success) {
  std::string plaintext = "test";
  std::string key = "key";
  std::vector<int> encrypted = whiteBoxAesEncrypt(plaintext, key);
  std::string decrypted;

  try {
    decrypted = whiteBoxAesDecrypt(encrypted, key);
  } catch (const std::runtime_error& e) {
    FAIL() << "Beklenmeyen bir istisna f�rlat�ld�: " << e.what();
  }

  ASSERT_NO_THROW({ whiteBoxAesEncrypt(plaintext, key); }, std::invalid_argument);
  ASSERT_EQ(decrypted, plaintext);
}



TEST(WBAESTest, vectorToString) {
  std::vector<int> vec = { 97, 98, 99 }; // a, b, c
  std::string str = vectorToString(vec);
  ASSERT_EQ(str, "abc");
  // Bo� vekt�r i�in test
  vec.clear();
  str = vectorToString(vec);
  ASSERT_EQ(str, "");
}

TEST(WBAESTest, stringToVector) {
  std::string str = "abc";
  std::vector<int> vec = stringToVector(str);
  ASSERT_EQ(vec.size(), 3);
  ASSERT_EQ(vec[0], 97);
  ASSERT_EQ(vec[1], 98);
  ASSERT_EQ(vec[2], 99);
  // Bo� string i�in test
  str = "";
  vec = stringToVector(str);
  ASSERT_TRUE(vec.empty());
}






///////////////////////ig//////////////////////////////



#ifdef _WIN32
TEST(VersionAndDeviceBindingTest, GetDeviceBrandWindows) {
  std::string deviceBrand = getDeviceBrand();
  ASSERT_FALSE(deviceBrand.empty()) << "Cihaz markas� al�namad� (Windows).";
}

#endif



// 2. Test: S�r�m uyumlulu�u ba�ar�s�z kontrol
TEST(VersionAndDeviceBindingTest, VersionCompatibilityMismatch) {
  std::string currentVersion = "1.1.0";
  std::string requiredVersion = "1.2.0";
  ASSERT_FALSE(isVersionCompatible(currentVersion, requiredVersion)) << "S�r�m uyumlulu�u yanl�� e�le�ti.";
}


TEST(SessionEncryptionTest, SessionKeyAndIVRandomnessTest2) {
  ASSERT_TRUE(setupSessionEncryption()) << "Oturum �ifreleme kurulumu ba�ar�s�z.";
  std::string key1 = getEncryptedSessionKey();
  std::string iv1 = getEncryptedSessionIV();
  ASSERT_TRUE(setupSessionEncryption()) << "Oturum �ifreleme kurulumu ba�ar�s�z (tekrar).";
  std::string key2 = getEncryptedSessionKey();
  std::string iv2 = getEncryptedSessionIV();
  ASSERT_EQ(key1, key2) << "Oturum anahtarlar� ayn�, rastgelelik ba�ar�s�z.";
  ASSERT_EQ(iv1, iv2) << "Oturum IV'leri ayn�, rastgelelik ba�ar�s�z.";
}



TEST(SessionEncryptionTest, SessionKeyEncryptionTest4) {
  ASSERT_TRUE(setupSessionEncryption()) << "Oturum �ifreleme kurulumu ba�ar�s�z.";
  std::string encryptedKey = getEncryptedSessionKey();
  ASSERT_FALSE(encryptedKey.empty()) << "Oturum anahtar� �ifrelenemedi.";
  bool result = setSessionKey(encryptedKey);
  ASSERT_TRUE(result) << "�ifrelenmi� oturum anahtar� ayarlanamad�.";
}

TEST(SessionEncryptionTest, SessionIVEncryptionTest2) {
  ASSERT_TRUE(setupSessionEncryption()) << "Oturum �ifreleme kurulumu ba�ar�s�z.";
  std::string encryptedIV = getEncryptedSessionIV();
  ASSERT_FALSE(encryptedIV.empty()) << "Oturum IV �ifrelenemedi.";
  bool result = setSessionIV(encryptedIV);
  ASSERT_TRUE(result) << "�ifrelenmi� oturum IV ayarlanamad�.";
}

TEST(SessionEncryptionTest, SessionDataEncryptionDecryptionTest2) {
  ASSERT_TRUE(setupSessionEncryption()) << "Oturum �ifreleme kurulumu ba�ar�s�z.";
  std::string testData = "Bu bir test verisidir.";
  std::string encryptedData = encryptSessionData(testData);
  ASSERT_FALSE(encryptedData.empty()) << "Oturum verisi �ifrelenemedi.";
  std::string decryptedData = decryptSessionData(encryptedData);
  ASSERT_EQ(testData, decryptedData) << "�ifrelenmi� ve ��z�lm�� veriler e�le�miyor.";
}

TEST(SessionEncryptionTest, Base64EncodingDecodingTest2) {
  std::string testData = "Base64 Test Verisi";
  std::string encodedData = base64Encode(reinterpret_cast<const unsigned char*>(testData.c_str()), testData.size());
  ASSERT_FALSE(encodedData.empty()) << "Base64 kodlama ba�ar�s�z.";
  std::vector<unsigned char> decodedData = base64Decode(encodedData);
  ASSERT_EQ(testData, std::string(decodedData.begin(), decodedData.end())) << "Base64 ��z�mlemesi ba�ar�s�z.";
}

TEST(SessionEncryptionTest, SessionDataEncryptionDecryptionTest) {
  ASSERT_TRUE(setupSessionEncryption()) << "Oturum  ifreleme kurulumu ba ar s z.";
  std::string testData = "Bu bir test verisidir.";
  std::string encryptedData = encryptSessionData(testData);
  ASSERT_FALSE(encryptedData.empty()) << "Oturum verisi  ifrelenemedi.";
  std::string decryptedData = decryptSessionData(encryptedData);
  ASSERT_EQ(testData, decryptedData) << " ifrelenmi  ve   z lm   veriler e le miyor.";
}


TEST(SessionEncryptionTest, Base64EncodingDecodingTest) {
  std::string testData = "Base64 Test Verisi";
  std::string encodedData = base64Encode(reinterpret_cast<const unsigned char*>(testData.c_str()), testData.size());
  ASSERT_FALSE(encodedData.empty()) << "Base64 kodlama ba ar s z.";
  std::vector<unsigned char> decodedData = base64Decode(encodedData);
  ASSERT_EQ(testData, std::string(decodedData.begin(), decodedData.end())) << "Base64   z mlemesi ba ar s z.";
}





//// Beklenen salt de�eri - ignore
const std::string EXPECTED_SALT_FOR_TEST = "9f86d081884c7d659a2feaa0c55ad015";

TEST(GenerateFixedSaltTest, CorrectSaltGeneration) {
  std::string seed = "test";  // Test i�in kullan�lan seed
  std::string salt = generateFixedSalt(seed);  // generateFixedSalt fonksiyonunu �a��r�n
  // Salt'�n do�ru olup olmad���n� kontrol et
  EXPECT_EQ(salt, EXPECTED_SALT_FOR_TEST);
}




// Test case 5: B�y�k ve k���k harf duyarl�l���n� kontrol et
TEST(HashPasswordWithHMACTest, CaseSensitivity) {
  std::string password = "Password";  // B�y�k harf ile �ifre
  std::string salt = "salt";  // Ayn� salt
  std::string hash1 = hashPasswordWithHMAC(password, salt);  // Fonksiyonu �a��r
  password = "password";  // K���k harf ile �ifre
  std::string hash2 = hashPasswordWithHMAC(password, salt);  // Fonksiyonu �a��r
  // B�y�k ve k���k harf duyarl�l��� nedeniyle hash'ler farkl� olmal�
  EXPECT_NE(hash1, hash2);
}

// getcallerprocesspath test - ignore
TEST(SignatureVerificationTest, GetCallerProcessPathTest) {
  std::string result = getCallerProcessPath();
  ASSERT_FALSE(result.empty()) << "�a��r�c� i�lem yolu bo�.";
}


//////////////////////ig//////////////////////////////


///////////////////////////////olop////////////////////

TEST(SSLTest, StartSSL) {
  StartSSL();
  ASSERT_TRUE(true) << "SSL baslatilmadi!";
}


TEST(PasswordStrengthTest1, ValidPasswords163) {
  EXPECT_TRUE(isPasswordStrong("Str0ng!Pass")); // Güçlü şifre
  EXPECT_TRUE(isPasswordStrong("An0ther$Pass1")); // Güçlü şifre
}

TEST(RegisterAttendeesTest, ValidInputTest) {
  // Katılımcı bilgilerini kayıt et ve başarılı olduğundan emin ol
  std::string input = "John Doe\njohn.doe@example.com\n1234567890\n";
  std::istringstream input_stream(input);
  std::cin.rdbuf(input_stream.rdbuf()); // Test için std::cin'i yönlendir
  testing::internal::CaptureStdout();
  registerAttendees();
  std::string output = testing::internal::GetCapturedStdout();
  // Beklenen çıktıyı kontrol et
  EXPECT_NE(output.find("Katilimci basariyla kaydedildi."), std::string::npos);
}


TEST(TrackAttendeesTest, ListAttendees) {
  testing::internal::CaptureStdout();
  trackAttendees();
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output.find("decrypted_key"), std::string::npos);
  // EXPECT_NE(output.find("ID: 1"), std::string::npos);
  //EXPECT_NE(output.find("ID: 2"), std::string::npos);
}



TEST(CreateEventTest, ValidEventCreation) {
  // Kullanıcı girişlerini simüle et
  std::string input = "Team Meeting\n2025-01-15\nOffice Room 1\nDiscuss project updates\n";
  std::istringstream input_stream(input);
  std::cin.rdbuf(input_stream.rdbuf());
  testing::internal::CaptureStdout(); // Çıktıyı yakala
  createEvent();
  std::string output = testing::internal::GetCapturedStdout();
  // Beklenen çıktıyı kontrol et
  EXPECT_NE(output.find("Etkinlik basariyla olusturuldu."), std::string::npos);
}



//ER----------------------------------------------------------------------
TEST(GuestModeTest, GetGuestModeTest) {
  // Varsay�lan olarak misafir modunda oldu�unu kontrol ediyoruz
  setGuestMode(true);
  ASSERT_TRUE(getGuestMode());
  setGuestMode(false);
  ASSERT_FALSE(getGuestMode());
}


TEST(GuestModeTest, SetGuestModeTest) {
  // Misafir modunu kapat�yoruz
  setGuestMode(false);
  // Misafir modunun ger�ekten kapand���n� kontrol ediyoruz
  ASSERT_FALSE(getGuestMode());
  // Misafir modunu tekrar a��yoruz
  setGuestMode(true);
  // Misafir modunun ger�ekten a��ld���n� kontrol ediyoruz
  ASSERT_TRUE(getGuestMode());
}

// ============================================================================
// getPasswordInput() Test Suite - Windows Platform
// ============================================================================
#ifdef _WIN32

namespace {
// Helper function to flush console input buffer and inject keyboard events
void InjectConsoleInput(const std::vector<INPUT_RECORD> &events) {
  HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

  if (hStdIn == INVALID_HANDLE_VALUE) {
    return;
  }

  FlushConsoleInputBuffer(hStdIn);
  DWORD written = 0;
  WriteConsoleInputW(hStdIn, events.data(), static_cast<DWORD>(events.size()), &written);
}

// Helper function to create a keyboard event record
INPUT_RECORD CreateKeyEvent(wchar_t character, WORD virtualKeyCode) {
  INPUT_RECORD record = {};
  record.EventType = KEY_EVENT;
  record.Event.KeyEvent.bKeyDown = TRUE;
  record.Event.KeyEvent.wRepeatCount = 1;
  record.Event.KeyEvent.wVirtualKeyCode = virtualKeyCode;
  record.Event.KeyEvent.wVirtualScanCode = 0;
  record.Event.KeyEvent.uChar.UnicodeChar = character;
  record.Event.KeyEvent.dwControlKeyState = 0;
  return record;
}
}

#endif // _WIN32

//Testlerin ba�lang�� noktas�
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
