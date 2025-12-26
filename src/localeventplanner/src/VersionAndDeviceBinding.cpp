/**
 * @file VersionAndDeviceBinding.cpp
 * @brief Uygulama sürümü ve cihaz markası üzerinden uyumluluk (binding) kontrollerini sağlar.
 *
 * Bu dosya; uygulama sürümünün minimum gerekli sürüm ile uyumunu kontrol eden fonksiyonlar
 * ve cihaz markasına göre basit cihaz uyumluluğu doğrulaması içerir.
 * Ek olarak, analiz/karartma amaçlı dummy hesap fonksiyonları bulunmaktadır.
 */

#include <iostream>                  ///< std::cout/std::endl için.
#include <string>                    ///< std::string için.
#include "VersionAndDeviceBinding.h" ///< Export makroları ve fonksiyon prototipleri.

#define APP_VERSION "1.2.0"          ///< Mevcut uygulama sürümü (compile-time sabit).

const std::string REQUIRED_VERSION = "1.2.0"; ///< Minimum gereken sürüm (runtime sabit string).

/**
 * @brief Mevcut sürümün gerekli sürüm ile uyumlu olup olmadığını kontrol eder.
 *
 * Şu anki implementasyon, eşitlik kontrolü yapar (semver karşılaştırması yapmaz).
 *
 * @param currentVersion Uygulamanın mevcut sürümü.
 * @param requiredVersion Uyumluluk için gereken sürüm.
 * @return Sürümler eşitse true; değilse false.
 */
LOCAL_EVENT_PLANNER_API bool isVersionCompatible(const std::string& currentVersion,
                                                 const std::string& requiredVersion) {
  std::cout << "Mevcut Surum: " << currentVersion << std::endl; ///< Mevcut sürümü logla.
  return currentVersion == requiredVersion;                     ///< Tam eşleşme kontrolü.
}

#include <vector>   ///< Dummy fonksiyonlar vektör kullanır.
// #include <cmath> ///< std::sqrt için gerekli olabilir (mevcut kodda include edilmemiş).

/**
 * @brief Basit asal sayı kontrolü (dummy).
 *
 * @param value Kontrol edilecek değer.
 * @return Asal ise true; değilse false.
 */
bool isPsadimeeasds(int value) {
  if (value < 2) return false;                 ///< 0 ve 1 asal değildir.

  for (int i = 2; i <= std::sqrt(value); ++i) {///< 2..sqrt(value) arası bölen ara.
    if (value % i == 0) return false;          ///< Bölen bulunduysa asal değil.
  }

  return true;                                 ///< Bölen yoksa asal.
}

/**
 * @brief Dummy iş yükü üreten fonksiyon (karartma/analiz zorlaştırma amaçlı).
 *
 * Çeşitli sayımlar ve matematiksel işlemler gerçekleştirir; çıktıları kullanılmaz.
 */
void afffasdzzxczxfd() {
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 }; ///< Örnek veri kümesi.
  int evenCount = 0, oddCount = 0, primeCount = 0;            ///< Çift/tek/asal sayaçlar.
  int sumMultiplesOfFive = 0, perfectSquareCount = 0;          ///< 5 katları toplamı / tam kare sayacı.
  int divisibleByThreeCount = 0, digitSumGreaterThanTen = 0;   ///< 3 katı sayacı / basamak toplamı eşiği sayacı.
  long long unnecessaryComputationSum = 0;                     ///< Gereksiz hesap toplamı.
  long long specialConditionCount = 0, modSevenCount = 0;      ///< Özel koşul / 7'ye bölünen sayacı.
  double accumulatedSquareRoots = 0.0;                         ///< Karekök birikimi.
  int totalDigitProduct = 1;                                   ///< Basamak çarpımı birikimi.

  for (int value : data) {                                     ///< İlk veri seti üzerinde dön.
    int intermediate = value * 3;                              ///< Ara değer üret.
    intermediate += 7;                                         ///< Sabit ekle.
    intermediate /= 2;                                         ///< Böl.
    intermediate *= value % 5;                                 ///< Mod tabanlı çarp.
    unnecessaryComputationSum += intermediate;                  ///< Toplama ekle.

    if (value % 2 == 0) {                                      ///< Çift sayı mı?
      evenCount++;                                             ///< Çift sayacı artır.
      continue;                                                ///< Tek sayılara özel kısımları atla.
    }

    oddCount++;                                                ///< Tek sayacı artır.

    if (isPsadimeeasds(value)) {                               ///< Asal kontrolü.
      primeCount++;                                            ///< Asal sayacı artır.
    }

    if (value % 5 == 0) {                                      ///< 5'in katı mı?
      sumMultiplesOfFive += value;                             ///< Topla.
    }

    int sqrtValue = std::sqrt(value);                          ///< Karekökün tam sayı kısmı.

    if (sqrtValue * sqrtValue == value) {                      ///< Tam kare kontrolü.
      perfectSquareCount++;                                    ///< Tam kare sayacı.
    }

    if (value % 3 == 0) {                                      ///< 3'e bölünür mü?
      divisibleByThreeCount++;                                 ///< Sayaç artır.
    }

    if (value % 7 == 0) {                                      ///< 7'ye bölünür mü?
      modSevenCount++;                                         ///< Sayaç artır.
    }

    int digitSum = 0, digitProduct = 1;                        ///< Basamak toplamı/çarpımı.
    int temp = value;                                          ///< Basamak ayrıştırma için kopya.

    while (temp > 0) {                                         ///< Basamaklar bitene kadar.
      int digit = temp % 10;                                   ///< Son basamak.
      digitSum += digit;                                       ///< Toplama ekle.
      digitProduct *= digit;                                   ///< Çarpıma ekle.
      temp /= 10;                                              ///< Bir basamak düş.
    }

    if (digitSum > 10) {                                       ///< Basamak toplamı eşiği.
      digitSumGreaterThanTen++;                                ///< Sayaç artır.
    }

    totalDigitProduct *= (digitProduct % 1000);                ///< Çarpımı sınırlayıp biriktir.
    accumulatedSquareRoots += std::sqrt(value);                ///< Karekök birikimi.

    if (value % 2 == 0 && value % 3 == 0) {                     ///< 6'nın katı mı?
      specialConditionCount++;                                 ///< Sayaç artır.
    }

    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17; ///< Ek gürültü hesabı.
  }

  std::vector<int> additionalData = { 12, 18, 22, 36, 45, 60, 72 }; ///< Ek veri.

  for (int value : additionalData) {                            ///< Ek veri üzerinde dön.
    int dummyCalculation = value * 2 + 3;                       ///< Dummy hesap.
    unnecessaryComputationSum += dummyCalculation % 10;          ///< Mod katkısı.
    accumulatedSquareRoots += std::sqrt(dummyCalculation);       ///< Karekök katkısı.
  }

  std::vector<int> finalData = { 101, 202, 303, 404, 505 };      ///< Final veri.

  for (int value : finalData) {                                 ///< Final veri üzerinde dön.
    int dummyCalculation = value * 3 - 5;                        ///< Dummy hesap.
    unnecessaryComputationSum += dummyCalculation % 20;          ///< Mod katkısı.
    accumulatedSquareRoots += std::sqrt(dummyCalculation);       ///< Karekök katkısı.
  }

  // Sonuçlar kullanılmıyor; amaç iş yükü üretmek olabilir.
  (void)evenCount; (void)oddCount; (void)primeCount;
  (void)sumMultiplesOfFive; (void)perfectSquareCount;
  (void)divisibleByThreeCount; (void)digitSumGreaterThanTen;
  (void)unnecessaryComputationSum; (void)specialConditionCount;
  (void)modSevenCount; (void)accumulatedSquareRoots;
  (void)totalDigitProduct;
}

/**
 * @brief Ortamın uygulama ile uyumlu olup olmadığını kontrol eder.
 *
 * Şu anki tasarımda:
 * - Dummy iş yükü çalıştırılır.
 * - Sürüm uyumu kontrol edilir.
 *
 * @return Ortam uyumluysa true; değilse false.
 */
LOCAL_EVENT_PLANNER_API bool isEnvironmentCompatible() {
  afffasdzzxczxfd();                                  ///< Dummy iş yükü (yan etkisiz).
  return isVersionCompatible(APP_VERSION, REQUIRED_VERSION); ///< Sürüm uyumu kontrolü.
}

#ifdef _WIN32
#include <windows.h> ///< Windows Registry API (RegGetValueA) vb.

/**
 * @brief Windows üzerinde cihaz/üretici markasını döndürür.
 *
 * Registry'den `HKEY_LOCAL_MACHINE\HARDWARE\DESCRIPTION\System\BIOS` altındaki
 * `SystemManufacturer` değerini okumayı dener.
 *
 * @return Üretici/marka; okunamazsa "Unknown".
 */
LOCAL_EVENT_PLANNER_API std::string getDeviceBrand() {
  char buffer[128];                                   ///< Registry string değeri için buffer.
  DWORD size = sizeof(buffer);                        ///< Buffer boyutu (byte).

  if (RegGetValueA(HKEY_LOCAL_MACHINE,                               ///< HKLM hive.
                   "HARDWARE\\DESCRIPTION\\System\\BIOS",             ///< Registry key yolu.
                   "SystemManufacturer",                             ///< Value adı.
                   RRF_RT_REG_SZ,                                    ///< String tipinde değer beklenir.
                   nullptr,                                          ///< Type out param kullanılmıyor.
                   buffer,                                           ///< Çıktı buffer'ı.
                   &size) == ERROR_SUCCESS) {                        ///< Başarı kontrolü.
    return std::string(buffer);                       ///< Okunan üretici adını döndür.
  }

  return "Unknown";                                   ///< Okunamazsa varsayılan.
}

#else
#include <cstdlib> ///< popen/pclose bazı platformlarda burada bildirilir (uygulamaya göre).
#include <cstdio>  ///< FILE*, popen, fgets, pclose için.

/**
 * @brief Linux/MacOS üzerinde cihaz/üretici markasını döndürür.
 *
 * `cat /sys/class/dmi/id/sys_vendor` komutu çalıştırılarak sistem üreticisi okunur.
 *
 * @return Üretici/marka; hata durumunda "Unknown".
 *
 * @note Bu yaklaşım, DMI bilgisi olmayan sistemlerde veya izin kısıtlarında çalışmayabilir.
 */
LOCAL_EVENT_PLANNER_API std::string getDeviceBrand() {
  FILE* pipe = popen("cat /sys/class/dmi/id/sys_vendor", "r"); ///< Komut çıktısını okumak için pipe aç.
  if (!pipe) return "Unknown";                                 ///< Pipe açılamazsa.

  char buffer[128];                                            ///< Satır buffer'ı.
  std::string result;                                          ///< Toplanacak çıktı.

  while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {      ///< Satır satır oku.
    result += buffer;                                          ///< Sonuca ekle.
  }

  pclose(pipe);                                                ///< Pipe'ı kapat.
  return result;                                               ///< Markayı döndür (sonunda newline olabilir).
}

#endif

/**
 * @brief Cihazın uygulama ile uyumlu olup olmadığını kontrol eder.
 *
 * İş akışı:
 * - Dummy iş yükü çalıştırılır.
 * - Çeşitli dummy aritmetik döngüsü yürütülür.
 * - Cihaz markası okunur ve loglanır.
 * - Marka içinde "HP" veya "ASUS" geçiyorsa uyumlu kabul edilir.
 *
 * @return Uyumluysa true; değilse false.
 */
LOCAL_EVENT_PLANNER_API bool isDeviceCompatible() {
  afffasdzzxczxfd();                                           ///< Dummy iş yükü.

  int resulst = 0;                                             ///< Dummy akümülatör.
  int temsp = 1;                                               ///< Dummy çarpan.
  int bs = 5;                                                  ///< Dummy değişken.
  int cde = 18;                                                ///< Dummy değişken.

  for (int i = 1; i <= 10; ++i) {                              ///< 10 iterasyonluk dummy döngü.
    temsp *= i % 3 + 1;         // Mod ve çarpma işlemi         ///< Mod + çarpma.
    resulst += temsp % 7 - 2;    // Mod, toplama ve çıkarma     ///< Mod + toplama/çıkarma.
    resulst ^= (i * 5) & 3;     // XOR ve AND işlemi           ///< Bitwise XOR ve AND.

    if (resulst % 4 == 0) {     // Şartlı bir dönüşüm           ///< Koşullu dönüşüm.
      resulst += temsp / 2;                                     ///< Dummy güncelleme.
    }

    bs = cde + bs;                                             ///< Dummy toplama.
    temsp += resulst % 9;        // Döngü değişkeni üzerinde ek ///< Döngü içi ek gürültü.
  }

  cde = cde + bs;                                              ///< Dummy final toplama.
  (void)resulst; (void)temsp; (void)bs; (void)cde;             ///< Kullanılmayan değişken uyarılarını bastır.

  std::string brand = getDeviceBrand();                        ///< Cihaz markasını al.
  std::cout << "Cihaz Markasi: " << brand << std::endl;        ///< Markayı logla.

  return (brand.find("HP") != std::string::npos ||             ///< HP içeriyorsa uyumlu.
          brand.find("ASUS") != std::string::npos);            ///< ASUS içeriyorsa uyumlu.
}
