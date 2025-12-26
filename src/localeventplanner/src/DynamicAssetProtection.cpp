/**
 * @file DynamicAssetProtection.cpp
 * @brief Dinamik varlık koruma: cihaz parmak izi üretimi, hashleme, şifreleme ve yardımcı kodlar.
 *
 * Bu dosya; platforma göre cihaz bilgisi toplar, SHA-256 ile parmak izi oluşturur,
 * oturum anahtarıyla şifreler ve bazı yardımcı dönüşümler (Base64) sağlar.
 */

#include "DynamicAssetProtection.h"  ///< Bu modülün dışa açılan prototipleri / export makroları.
#include "SessionEncryption.h"       ///< encryptSessionData gibi oturum şifreleme fonksiyonları.
#include <iostream>                  ///< std::cout/std::cerr için.
#include <iomanip>                   ///< std::setw/std::setfill gibi formatlayıcılar için.
#include <sstream>                   ///< std::ostringstream için.
#include <vector>                    ///< std::vector için.
#include <cstring>                   ///< memset vb. için (bu parçada doğrudan kullanılmıyor olabilir).
#include <openssl/buffer.h>          ///< BUF_MEM gibi OpenSSL buffer tipleri.
#include <openssl/sha.h>             ///< SHA256 fonksiyonu ve SHA256_DIGEST_LENGTH.
#include <openssl/evp.h>             ///< EVP API (genel kripto altyapısı; bu parçada sınırlı).
#include <openssl/rand.h>            ///< Rastgele veri API (bu parçada doğrudan kullanılmıyor olabilir).
#include <iostream>                  ///< Tekrar include (gereksiz ama zararsız).
#include <sstream>                   ///< Tekrar include (gereksiz ama zararsız).
#include <iomanip>                   ///< Tekrar include (gereksiz ama zararsız).
#include <vector>                    ///< Tekrar include (gereksiz ama zararsız).
#include <cstring>                   ///< Tekrar include (gereksiz ama zararsız).
#include "DisplayMainMenu.h"         ///< clearConsole gibi UI yardımcıları.

#ifdef _WIN32
#include <windows.h>                 ///< Windows API (Registry, _popen vb. dahil olmak üzere).

/**
 * @brief Verilen komutu çalıştırır ve stdout çıktısını döndürür (Windows).
 *
 * @param cmd Çalıştırılacak komut satırı.
 * @return Komut çıktısı; pipe açılamazsa "ERROR".
 *
 * @note Windows tarafında `_popen/_pclose` kullanır.
 */
std::string executeCommand(const char* cmd) {
  char buffer[128];                  ///< Komut çıktısını satır bazında almak için buffer.
  std::string result;                ///< Biriktirilecek çıktı.
  FILE* pipe = _popen(cmd, "r");     ///< Komutun stdout’unu okumak için pipe aç.

  if (!pipe) return "ERROR";         ///< Pipe açılamazsa hata string’i döndür.

  while (fgets(buffer, sizeof(buffer), pipe) != nullptr) { ///< Çıktıyı satır satır oku.
    result += buffer;              ///< Her satırı sonuca ekle.
  }

  _pclose(pipe);                    ///< Pipe’ı kapat.
  return result;                    ///< Toplanan çıktıyı döndür.
}

/**
 * @brief Windows üzerinde cihaz bilgisi toplar.
 *
 * - OS bilgisi: `systeminfo | findstr /B /C:"OS"`
 * - CPU bilgisi: Registry üzerinden `ProcessorNameString`
 *
 * @return OS ve CPU bilgilerini birleştiren string.
 */
std::string getDeviceInfo() {
  std::string osInfo =
      executeCommand("systeminfo | findstr /B /C:\"OS\""); ///< İşletim sistemi bilgilerini çek.
  std::string cpuInfo;                                     ///< CPU bilgisini tutacak string.

  HKEY hKey;                                               ///< Registry anahtar handle’ı.
  char cpuName[256];                                       ///< CPU adını tutmak için buffer.
  DWORD bufferSize = sizeof(cpuName);                      ///< Buffer boyutu.

  // Registry'den CPU ismini al                                        ///< CPU adını Registry’den okuma.
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,                                ///< HKLM hive.
                   "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",///< CPU bilgisi key’i.
                   0,                                                 ///< Reserved.
                   KEY_READ,                                          ///< Okuma izni.
                   &hKey) == ERROR_SUCCESS) {                         ///< Key açma başarılı mı?
    RegQueryValueEx(hKey,                                             ///< Açık key.
                    "ProcessorNameString",                            ///< Value adı.
                    NULL,                                             ///< Reserved.
                    NULL,                                             ///< Type out param kullanılmıyor.
                    (LPBYTE)cpuName,                                  ///< Çıktı buffer’ı.
                    &bufferSize);                                     ///< Boyut.
    RegCloseKey(hKey);                                                ///< Registry anahtarını kapat.
    cpuInfo = std::string(cpuName);                                   ///< CPU adını string’e çevir.
  } else {
    cpuInfo = "Unknown CPU";                                          ///< Hata durumunda varsayılan.
  }

  return osInfo + "CPU: " + cpuInfo;                                  ///< OS + CPU bilgisini birleştir.
}

#else // Linux ve diğer platformlar

#include <cstdlib>                    ///< popen/pclose bazı platformlarda buradan gelir.
#include <cstdio>                     ///< FILE*, popen, fgets, pclose için.

/**
 * @brief Verilen komutu çalıştırır ve stdout çıktısını döndürür (POSIX).
 *
 * @param cmd Çalıştırılacak komut satırı.
 * @return Komut çıktısı; pipe açılamazsa "ERROR".
 */
std::string executeCommand(const char* cmd) {
  char buffer[128];                   ///< Komut çıktısı için satır buffer’ı.
  std::string result;                 ///< Biriktirilecek çıktı.
  FILE* pipe = popen(cmd, "r");       ///< Komutu çalıştır ve stdout’u okumak için pipe aç.

  if (!pipe) return "ERROR";          ///< Pipe açılamazsa hata.

  while (fgets(buffer, sizeof(buffer), pipe) != nullptr) { ///< Satır satır oku.
    result += buffer;               ///< Çıktıyı birleştir.
  }

  pclose(pipe);                      ///< Pipe’ı kapat.
  return result;                     ///< Çıktıyı döndür.
}

/**
 * @brief Linux üzerinde cihaz bilgisi toplar.
 *
 * - OS bilgisi: `uname -a`
 * - CPU bilgisi: `lscpu | grep 'Model name:'`
 *
 * @return OS ve CPU bilgilerini birleştiren string.
 */
std::string getDeviceInfo() {
  std::string osInfo = executeCommand("uname -a");                    ///< OS kernel/host bilgisi.
  std::string cpuInfo = executeCommand("lscpu | grep 'Model name:'"); ///< CPU model adı.
  return osInfo + "CPU: " + cpuInfo;                                  ///< Birleştir ve döndür.
}

#endif

/**
 * @brief SHA-256 hash üretir.
 *
 * @param input Hashlenecek giriş string’i.
 * @return Girişin SHA-256 hash’inin hex string gösterimi.
 */
std::string sha256(const std::string& input) {
  unsigned char hash[SHA256_DIGEST_LENGTH]; ///< Hash çıktısı için 32-byte buffer.
  SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), ///< Girişin byte pointer’ı.
         input.size(),                                          ///< Giriş uzunluğu.
         hash);                                                 ///< Çıkış buffer.

  std::ostringstream result;                                    ///< Hex string üretmek için stream.

  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {              ///< 32 byte üzerinde dolaş.
    result << std::hex << std::setw(2) << std::setfill('0')
           << (int)hash[i];                                     ///< Her byte’ı 2 haneli hex olarak ekle.
  }

  return result.str();                                          ///< Hex string’i döndür.
}

/**
 * @brief Gereksiz/deneme amaçlı asal kontrol fonksiyonu (dummy).
 *
 * @param value Kontrol edilecek değer.
 * @return Asal ise true; değilse false.
 */
bool sxslioufdw(int value) {
  if (value < 2) return false;                    ///< 0 ve 1 asal değildir.

  for (int i = 2; i <= std::sqrt(value); ++i) {   ///< 2..sqrt(value) arası bölen ara.
    if (value % i == 0) return false;             ///< Bölen bulunduysa asal değil.
  }

  return true;                                    ///< Bölen yoksa asal.
}

/**
 * @brief Dummy iş yükü üreten fonksiyon (test/karartma).
 *
 * Çok sayıda etkisiz aritmetik ve sayaç içerir; çıktılar kullanılmaz.
 */
void ttassqqxcfd() {
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 };      ///< Ana veri.
  std::vector<int> additionalData = { 12, 18, 22, 36, 45, 60, 72 }; ///< Ek veri.
  std::vector<int> finalData = { 101, 202, 303, 404, 505 };         ///< Final veri.

  // Gereksiz değişkenler                                             ///< Dummy sayaçlar/birikimler.
  int evenCount = 0, oddCount = 0, primeCount = 0;                   ///< Çift/tek/asal sayacı.
  int sumMultiplesOfFive = 0, perfectSquareCount = 0;                 ///< 5 katları toplamı / tam kare sayacı.
  int divisibleByThreeCount = 0, digitSumGreaterThanTen = 0;          ///< 3 katı sayacı / basamak toplamı eşiği.
  long long unnecessaryComputationSum = 0, totalIterations = 0;       ///< Gürültü toplamı / iterasyon sayısı.
  long long specialConditionCount = 0, modSevenCount = 0;             ///< Özel koşul / 7'ye bölünen sayacı.
  double accumulatedSquareRoots = 0.0;                                ///< Karekök birikimi.
  int totalDigitProduct = 1;                                          ///< Basamak çarpımı birikimi.

  // Gereksiz değişkenler                                             ///< Ek dummy değişkenler.
  int a = 0;
  int b = 1;
  int c = 2;
  int d = 3;
  int uselessCalculation1 = 0;
  int uselessCalculation2 = 0;
  int uselessCalculation3 = 0;

  for (int value : data) {                                            ///< Ana veri üzerinde dön.
    totalIterations++;                                                ///< Iterasyon say.

    // Gereksiz işlemler                                              ///< İşlemsel gürültü.
    int intermediate = value * 3;                                     ///< Ara hesap.
    intermediate += 7;                                                ///< Sabit ekle.
    intermediate -= 7;                                                ///< Sabiti çıkar (net 0).
    intermediate /= 2;                                                ///< Böl.
    intermediate *= value % 5;                                        ///< Mod ile çarp.
    intermediate += 1 - 1;                                            ///< Net 0.
    unnecessaryComputationSum += intermediate;                         ///< Biriktir.

    int e = 4;
    int f = 5;
    int g = 6;
    int h = 7;
    int i = 8;
    int j = 9;

    a = a + 1 - 1 + 2 - 2;                                            ///< Net 0.
    b = b * 2 / 2 + 5 - 5;                                            ///< Net 0.
    c = c + a - b + 3 - 3;                                            ///< a/b üzerinden etkileniyor gibi.
    d = d * 4 / 4 - c + 6 - 6;                                        ///< d = d - c gibi.

    (void)e; (void)f; (void)g; (void)h; (void)i; (void)j;             ///< Kullanılmayan uyarılarını bastır.

    if (value % 2 == 0) {                                             ///< Çift mi?
      evenCount++;                                                    ///< Çift say.
      continue;                                                       ///< Tek sayılara özel kısımları atla.
    }

    oddCount++;                                                       ///< Tek say.

    if (sxslioufdw(value)) {                                          ///< Asal kontrol.
      primeCount++;                                                   ///< Asal say.
    }

    if (value % 5 == 0) {                                             ///< 5'in katı mı?
      sumMultiplesOfFive += value;                                    ///< Topla.
    }

    int sqrtValue = std::sqrt(value);                                 ///< Karekök (tam sayı kısmı).

    if (sqrtValue * sqrtValue == value) {                             ///< Tam kare mi?
      perfectSquareCount++;                                           ///< Say.
    }

    if (value % 3 == 0) {                                             ///< 3'e bölünür mü?
      divisibleByThreeCount++;                                        ///< Say.
    }

    if (value % 7 == 0) {                                             ///< 7'ye bölünür mü?
      modSevenCount++;                                                ///< Say.
    }

    int digitSum = 0, digitProduct = 1;                               ///< Basamak toplamı/çarpımı.
    int temp = value;                                                 ///< Basamak ayrıştırma kopyası.

    while (temp > 0) {                                                ///< Basamaklar bitene kadar.
      int digit = temp % 10;                                          ///< Son basamak.
      digitSum += digit;                                              ///< Topla.
      digitProduct *= digit;                                          ///< Çarp.
      temp /= 10;                                                     ///< Basamak düş.

      uselessCalculation1 = uselessCalculation1 + digit - digit;      ///< Net 0.
      uselessCalculation2 = uselessCalculation2 * digit / (digit == 0 ? 1 : digit); ///< Sıfıra bölmeyi önle.
      uselessCalculation3 = uselessCalculation3 + digitProduct - digitProduct;      ///< Net 0.
    }

    if (digitSum > 10) {                                              ///< Eşik kontrolü.
      digitSumGreaterThanTen++;                                       ///< Say.
    }

    totalDigitProduct *= (digitProduct % 1000);                       ///< Çarpımı sınırla.
    accumulatedSquareRoots += std::sqrt(value);                       ///< Karekök birikimi.

    if (value % 2 == 0 && value % 3 == 0) {                            ///< 6'nın katı mı?
      specialConditionCount++;                                        ///< Say.
    }

    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17;        ///< Ek gürültü.
  }

  for (int value : additionalData) {                                  ///< Ek veri.
    int dummyCalculation = value * 2 + 3 - 3;                         ///< Net: value*2.
    dummyCalculation *= dummyCalculation % 10 + 1 - 1;                ///< Çarpan: %10.
    accumulatedSquareRoots += std::sqrt(dummyCalculation);            ///< Karekök ekle.
    unnecessaryComputationSum += dummyCalculation % 10 + 4 - 4;        ///< Net: %10.
  }

  for (int value : finalData) {                                       ///< Final veri.
    int dummyCalculation = value * 3 - 5 + 5 - 5;                     ///< Net: value*3 - 5.
    dummyCalculation *= 2;                                            ///< 2 ile çarp.
    accumulatedSquareRoots += std::sqrt(dummyCalculation);            ///< Karekök ekle.
    unnecessaryComputationSum += dummyCalculation % 20 + 8 - 8;        ///< Net: %20.
  }

  // Daha fazla gereksiz değişken                                      ///< Ek gürültü.
  int p = 10;
  int q = 20;
  int r = 30;
  int s = 40;
  int t = 50;
  p = p + q - r + s - t;                                              ///< Karma aritmetik.
  q = q * 2 - p + r - s + t;                                          ///< Karma aritmetik.
  r = r * 3 / 3 + q - p;                                              ///< Karma aritmetik.

  // Kullanılmayanları bastır                                          ///< Uyarı bastırma.
  (void)evenCount; (void)oddCount; (void)primeCount;
  (void)sumMultiplesOfFive; (void)perfectSquareCount;
  (void)divisibleByThreeCount; (void)digitSumGreaterThanTen;
  (void)unnecessaryComputationSum; (void)totalIterations;
  (void)specialConditionCount; (void)modSevenCount;
  (void)accumulatedSquareRoots; (void)totalDigitProduct;
  (void)a; (void)b; (void)c; (void)d;
  (void)uselessCalculation1; (void)uselessCalculation2; (void)uselessCalculation3;
  (void)p; (void)q; (void)r; (void)s; (void)t;
}

/**
 * @brief Binary veriyi Base64 string’e encode eder.
 *
 * OpenSSL BIO zinciri kullanır: memory BIO + base64 filter BIO.
 *
 * @param buffer Binary veri buffer’ı.
 * @param length Buffer uzunluğu (byte).
 * @return Base64 encoded string; hata durumunda boş string.
 */
LOCAL_EVENT_PLANNER_API std::string base64Encode(const unsigned char* buffer, size_t length) {
  ttassqqxcfd();                                                    ///< Dummy iş yükü (yan etkisiz).

  BIO* bio = BIO_new(BIO_s_mem());                                  ///< Bellek tabanlı BIO.
  BIO* b64 = BIO_new(BIO_f_base64());                               ///< Base64 filter BIO.

  if (!bio || !b64) {                                               ///< BIO oluşturulamazsa.
    std::cerr << "Base64 encode icin BIO olusturulamadi." << std::endl; ///< Hata logu.
    return "";                                                      ///< Boş dön.
  }

  int resulst = 0;                                                  ///< Dummy akümülatör.
  int temsp = 1;                                                    ///< Dummy çarpan.
  int bs = 5;                                                       ///< Dummy değişken.
  int cde = 18;                                                     ///< Dummy değişken.

  for (int i = 1; i <= 10; ++i) {                                   ///< Dummy döngü.
    temsp *= i % 3 + 1;         // Mod ve çarpma işlemi              ///< Mod + çarpma.
    resulst += temsp % 7 - 2;    // Mod, toplama ve çıkarma          ///< Mod + toplama/çıkarma.
    resulst ^= (i * 5) & 3;     // XOR ve AND işlemi                ///< Bitwise XOR/AND.

    if (resulst % 4 == 0) {     // Şartlı bir dönüşüm                ///< Koşullu dönüşüm.
      resulst += temsp / 2;                                           ///< Dummy güncelleme.
    }

    bs = cde + bs;                                                  ///< Dummy toplama.
    temsp += resulst % 9;        // Döngü değişkeni üzerinde ek iş    ///< Ek gürültü.
  }

  cde = cde + bs;                                                   ///< Dummy final toplama.
  (void)resulst; (void)temsp; (void)bs; (void)cde;                  ///< Kullanılmayan uyarılarını bastır.

  bio = BIO_push(b64, bio);                                         ///< Base64 filtresini zincire ekle.
  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);                        ///< Base64 çıktısında newline’ı kapat.
  BIO_write(bio, buffer, length);                                   ///< Binary veriyi BIO’ya yaz.
  BIO_flush(bio);                                                   ///< BIO’yu flush et.

  BUF_MEM* bufferPtr;                                               ///< Memory BIO içeriğine erişim için.
  BIO_get_mem_ptr(bio, &bufferPtr);                                 ///< BIO’nun içerik pointer’ını al.
  std::string encodedData(bufferPtr->data, bufferPtr->length);      ///< Base64 çıktısını string’e çevir.

  BIO_free_all(bio);                                                ///< BIO zincirini serbest bırak.
  return encodedData;                                               ///< Encoded string’i döndür.
}

/**
 * @brief Cihaz parmak izini üretir ve oturum anahtarıyla şifreleyerek döndürür.
 *
 * - Platforma göre cihaz bilgisi toplanır.
 * - SHA-256 ile hashlenerek parmak izi üretilir.
 * - encryptSessionData ile şifrelenir.
 * - Konsol temizlenip bilgilendirici çıktılar basılır.
 *
 * @return Şifrelenmiş cihaz parmak izi; hata durumunda boş string.
 */
LOCAL_EVENT_PLANNER_API std::string getEncryptedDeviceFingerprint() {
  std::string deviceInfo = getDeviceInfo();                         ///< Platforma göre cihaz bilgilerini al.

  if (deviceInfo.empty()) {                                         ///< Cihaz bilgisi yoksa.
    std::cerr << "Cihaz bilgisi alinamadi." << std::endl;            ///< Hata logu.
    return "";                                                      ///< Boş dön.
  }

  std::string fingerprint = sha256(deviceInfo);                     ///< Cihaz bilgisini SHA-256 ile hashle.
  std::string encryptedFingerprint = encryptSessionData(fingerprint);///< Hash’i oturum anahtarıyla şifrele.

  if (encryptedFingerprint.empty()) {                               ///< Şifreleme başarısızsa.
    std::cerr << "Cihaz parmak izi sifrelenemedi." << std::endl;     ///< Hata logu.
    return "";                                                      ///< Boş dön.
  }

  clearConsole();                                                   ///< Konsolu temizle (UI).
  std::cout << "===============================\n";                  ///< Ayraç.
  std::cout << "Cihaz Parmak Izi\n\n";                               ///< Başlık.
  std::cout << "Cihaz Parmak Izi (Hashed): " << fingerprint << std::endl;        ///< Hashlenmiş parmak izi.
  std::cout << "Cihaz Parmak Izi (Sifreli): " << encryptedFingerprint << std::endl; ///< Şifreli parmak izi.

  return encryptedFingerprint;                                      ///< Şifrelenmiş değeri döndür.
}
