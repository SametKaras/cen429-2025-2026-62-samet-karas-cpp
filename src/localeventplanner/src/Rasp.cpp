
/**
 * @file Rasp.cpp
 * @brief Cihaz/emülatör tespiti, hook tespiti, CRC bütünlük kontrolü ve TLS istemci başlatma örnekleri.
 *
 * Bu dosya; emülatör kontrolü, Windows API hook tespiti, CRC32 ile kod bloğu bütünlük doğrulaması
 * ve OpenSSL üzerinden TLS istemci bağlamı kurulumu örneklerini içerir.
 */

#include <iostream>     ///< Standart giriş/çıkış akışları (std::cout, std::cerr).
#include <string>       ///< std::string veri tipi.
#include <vector>       ///< std::vector konteyneri.
#include <fstream>      ///< Dosya okuma/yazma (std::ifstream).
#include "Rasp.h"       ///< CRC_BLOCK_* makroları gibi proje-özel başlık (varsayılan).
#include <sys/stat.h>   ///< Dosya izinleri/stat yapılarına erişim.
#include <openssl/ssl.h>///< OpenSSL SSL/TLS API.
#include <openssl/err.h>///< OpenSSL hata okuma (ERR_get_error vb.).
#include "UserAuthentication.h"        ///< Kullanıcı kimlik doğrulama modülü (proje-özel).
#include "SessionEncryption.h"         ///< Oturum şifreleme modülü (proje-özel).
#include "DynamicAssetProtection.h"    ///< Dinamik varlık koruma modülü (proje-özel).
#include "VersionAndDeviceBinding.h"   ///< Sürüm-cihaz bağlama modülü (proje-özel).
#include "DebugCheck.h"                ///< Debug/anti-debug kontrol modülü (proje-özel).
#include "SignatureVerification.h"     ///< İmza doğrulama modülü (proje-özel).
#include <cstdio>       ///< C stdio (printf, FILE vb.) — bu dosyada sınırlı kullanılır.

#ifdef _WIN32
  #ifndef S_IWUSR
    #define S_IWUSR S_IWRITE ///< Windows'ta eksik olabilecek S_IWUSR tanımını S_IWRITE ile eşler.
  #endif
#endif

/**
 * @brief Emülatörlerde sık görülen üretici isimleri.
 *
 * Cihaz üreticisi bu listede geçiyorsa emülatör olma olasılığı artar.
 */
const std::vector<std::string> EMULATOR_MANUFACTURERS = {
  "Genymotion", "BlueStacks", "Google", "Android Emulator", "VirtualBox" ///< Bilinen emülatör üreticileri.
};

/**
 * @brief Emülatörlerde sık görülen model isimleri.
 *
 * Cihaz modeli bu listede geçiyorsa emülatör olma olasılığı artar.
 */
const std::vector<std::string> EMULATOR_MODELS = {
  "Emulator", "sdk", "google_sdk", "vbox", "VirtualBox" ///< Bilinen emülatör model anahtarları.
};

/**
 * @brief Cihaz üreticisini döndürür.
 *
 * Android/Linux benzeri ortamlarda `/system/build.prop` içinden `ro.product.manufacturer` değerini okur.
 *
 * @return Üretici bilgisi; bulunamazsa `"Unknown"`.
 */
std::string getDeviceManufacturer() {
  std::ifstream file("/system/build.prop"); ///< Android sistem özelliklerinin tipik konumu (her cihazda garanti değil).
  std::string line;                         ///< Dosyadan okunacak satır geçici değişkeni.

  while (std::getline(file, line)) {        ///< Dosya satır satır okunur.
    if (line.find("ro.product.manufacturer") != std::string::npos) { ///< Üretici anahtarı yakalanır.
      return line.substr(line.find("=") + 1); ///< '=' sonrası değer ilgili property değeridir.
    }
  }

  return "Unknown"; ///< Anahtar bulunamazsa varsayılan.
}

/**
 * @brief Cihaz modelini döndürür.
 *
 * Android/Linux benzeri ortamlarda `/system/build.prop` içinden `ro.product.model` değerini okur.
 *
 * @return Model bilgisi; bulunamazsa `"Unknown"`.
 */
std::string getDeviceModel() {
  std::ifstream file("/system/build.prop"); ///< Sistem özellik dosyası.
  std::string line;                         ///< Satır buffer'ı.

  while (std::getline(file, line)) {        ///< Dosya satır satır okunur.
    if (line.find("ro.product.model") != std::string::npos) { ///< Model anahtarı yakalanır.
      return line.substr(line.find("=") + 1); ///< '=' sonrası değer döndürülür.
    }
  }

  return "Unknown"; ///< Bulunamadıysa varsayılan.
}

/**
 * @brief Uygulamanın emülatörde çalışıp çalışmadığını tespit eder.
 *
 * Üretici ve model string'leri, bilinen emülatör üretici/model anahtarlarıyla karşılaştırılır.
 *
 * @return Emülatör tespit edilirse true; aksi halde false.
 */
bool isEmulator() {
  std::string manufacturer = getDeviceManufacturer(); ///< Üretici bilgisi okunur.
  std::string model = getDeviceModel();               ///< Model bilgisi okunur.

  for (const auto& emulatorManufacturer : EMULATOR_MANUFACTURERS) { ///< Üretici listesi taranır.
    if (manufacturer.find(emulatorManufacturer) != std::string::npos) { ///< Eşleşme varsa emülatör kabul edilir.
      return true; ///< Emülatör üretici eşleşmesi.
    }
  }

  for (const auto& emulatorModel : EMULATOR_MODELS) { ///< Model listesi taranır.
    if (model.find(emulatorModel) != std::string::npos) { ///< Eşleşme varsa emülatör kabul edilir.
      return true; ///< Emülatör model eşleşmesi.
    }
  }

  return false; ///< Hiçbir eşleşme yok.
}

#ifdef _WIN32
#include <windows.h> ///< Windows API fonksiyonları (GetModuleHandleA, VirtualQuery vb.).
#include <iostream>  ///< Ek i/o.
#include <vector>    ///< Ek konteyner.

/**
 * @brief Belirtilen modül içindeki fonksiyonun hook edilip edilmediğini kaba şekilde kontrol eder.
 *
 * Mantık: GetProcAddress ile fonksiyon adresi alınır; VirtualQuery ile bu adresin bellek bölgesi incelenir.
 * Eğer bölge tipi `MEM_IMAGE` değilse (yani bir image mapping değilse), adres başka bir alana yönlendirilmiş olabilir.
 *
 * @param moduleName DLL modül adı (örn. "kernel32.dll").
 * @param functionName Fonksiyon adı (örn. "CreateFileA").
 * @return Hook şüphesi varsa true; aksi halde false.
 */
bool isFunctionHooked(const char* moduleName, const char* functionName) {
  HMODULE moduleHandle = GetModuleHandleA(moduleName); ///< Modül handle alınır (yüklü değilse null).
  if (!moduleHandle) return false;                     ///< Modül yoksa hook kontrolü yapılamaz.

  void *originalAddress = GetProcAddress(moduleHandle, functionName); ///< Fonksiyon adresi çözülür.
  if (!originalAddress) return false;                                 ///< Fonksiyon bulunamazsa kontrol yok.

  MEMORY_BASIC_INFORMATION mbi; ///< Bellek bölgesi bilgisi.
  VirtualQuery(originalAddress, &mbi, sizeof(mbi)); ///< Adresin bellek bölgesi özellikleri okunur.

  return mbi.Type != MEM_IMAGE; ///< Image değilse olası hook / trampoline / patch yönlendirmesi.
}

/**
 * @brief Bir dizi kritik API fonksiyonunda hook kontrolü yapar.
 *
 * @return Hook tespit edilirse true; hiçbiri tespit edilmezse false.
 */
bool checkHooks() {
  std::vector<std::pair<std::string, std::string>> apiList = { ///< İzlenecek modül-fonksiyon çiftleri.
    {"kernel32.dll", "CreateFileA"},   ///< Dosya erişimi.
    {"kernel32.dll", "ReadFile"},      ///< Dosya okuma.
    {"kernel32.dll", "WriteFile"},     ///< Dosya yazma.
    {"user32.dll", "MessageBoxA"},     ///< UI mesaj kutusu.
    {"advapi32.dll", "RegOpenKeyExA"}  ///< Registry erişimi.
  };

  for (const auto& api : apiList) {                    ///< Liste gezilir.
    const char *moduleName = api.first.c_str();        ///< Modül adı.
    const char *functionName = api.second.c_str();     ///< Fonksiyon adı.

    if (isFunctionHooked(moduleName, functionName)) {  ///< Hook şüphesi kontrolü.
      return true;                                     ///< Herhangi bir hook şüphesinde true döner.
    }
  }

  return false; ///< Hiçbir hook şüphesi yok.
}

#else
// Windows dışı platformlarda çalışacak alternatif (mevcut tasarım Windows'a özel).
#include <iostream> ///< Hata mesajı basmak için.

/// @brief Windows dışı platformlarda çalıştırıldığında bilgi mesajı verip çıkar.
int main() {
  std::cerr << "Bu program sadece Windows platformunda calisabilir.\n"; ///< Platform uyarısı.
  return 1; ///< Hata kodu ile çıkış.
}
#endif

#define _CRT_SECURE_NO_WARNINGS ///< MSVC'de güvenli olmayan fonksiyon uyarılarını bastırır.
#define WIN32_LEAN_AND_MEAN     ///< Windows başlıklarının daha minimal dahil edilmesini sağlar.
#define CRC_POLY      0xEDB88320L ///< CRC32 için kullanılan ters polinom.
#define CRC_TABLE_LEN 256         ///< CRC tablosu uzunluğu.

static unsigned long crc32_table[CRC_TABLE_LEN] = { 0 }; ///< CRC32 lookup tablosu (başlatma sonrası dolacak).

/**
 * @brief CRC32 lookup tablosunu başlatır.
 *
 * Tablo, her byte için CRC dönüşümünü önceden hesaplayarak hızlı CRC hesaplamaya imkan verir.
 */
void crc32_table_init() {
  unsigned long crc; ///< Geçici CRC akümülatörü.

  for (int i = 0; i < CRC_TABLE_LEN; ++i) { ///< 0..255 tüm byte değerleri için.
    crc = i; ///< Başlangıç değeri olarak indeks kullanılır.

    for (int j = 8; j > 0; --j) { ///< Her byte için 8 bit işlenir.
      if (crc & 1) {              ///< LSB 1 ise polinom XOR uygulanır.
        crc = (crc >> 1) ^ CRC_POLY; ///< Shift + polinom.
      } else {
        crc >>= 1; ///< Sadece shift.
      }
    }

    crc32_table[i] = crc; ///< Sonuç tabloya yazılır.
  }
}

/**
 * @brief Verilen buffer için CRC32 checksum hesaplar.
 *
 * @param buf CRC hesaplanacak veri başlangıcı.
 * @param buf_len Veri uzunluğu (byte).
 * @return CRC32 sonucu.
 */
unsigned long crc32_calc(unsigned char* buf, int buf_len) {
  unsigned long crc = 0xFFFFFFFF; ///< CRC başlangıç değeri.

  for (int x = 0; x < buf_len; ++x) { ///< Tüm byte'lar üzerinde iterasyon.
    crc = (crc >> 8) ^ crc32_table[(crc ^ buf[x]) & 0xFF]; ///< Lookup tablosu ile güncelleme.
  }

  return crc ^ 0xFFFFFFFF; ///< Final XOR.
}

/**
 * @def CRC_START_BLOCK
 * @brief CRC ile bütünlüğü kontrol edilecek kod bloğu başlangıcı (proje-özel makro).
 *
 * Not: Bu makrolar "Rasp.h" içinde tanımlı varsayılmaktadır.
 */

/**
 * @brief Örnek fonksiyon (CRC bloğu içinde).
 *
 * Bu fonksiyonun derlenmiş kodu CRC kontrolüne dahil edilir.
 */
CRC_START_BLOCK(exampleBlock)
void exampleFunction() {
  std::cout << "Example function asdasdrunning...\n"; ///< Örnek çıktı.
}
CRC_END_BLOCK(exampleBlock)

/**
 * @brief CRC bloğunun bütünlüğünü doğrular.
 *
 * Hesaplanan CRC ile beklenen CRC karşılaştırılır.
 *
 * @return CRC eşleşirse true; aksi halde false.
 */
bool verifyCodeBlock() {
  unsigned long computedCRC = crc32_calc(CRC_BLOCK_ADDR(exampleBlock), CRC_BLOCK_LEN(exampleBlock)); ///< CRC hesapla.
  const unsigned long expectedCRC = 0x267ecd13; ///< Test/derleme zamanında elde edilmiş beklenen CRC (placeholder).
  std::cout << "Computed CRC: " << std::hex << computedCRC << std::endl; ///< Hesaplanan CRC yazdır.
  std::cout << "Expected CRC: " << std::hex << expectedCRC << std::endl; ///< Beklenen CRC yazdır.
  return computedCRC == expectedCRC; ///< Karşılaştırma sonucu döndür.
}

#define STRINGIFY(x) #x   ///< Makroyu string literal'e çevirir.
#define TOSTRING(x) STRINGIFY(x) ///< İki aşamalı stringify için yardımcı.

const std::string PRIVATE_KEY_FILE = PRIVATE_KEY_FILE_PATH; ///< Özel anahtar dosya yolu (derleme tanımı).
const std::string CERTIFICATE_FILE = CERTIFICATE_FILE_PATH; ///< İstemci sertifika dosya yolu (derleme tanımı).
const std::string CA_CERTIFICATE_FILE = CA_CERTIFICATE_FILE_PATH; ///< CA sertifikası dosya yolu (derleme tanımı).

#define HOSTNAME "localhost" ///< TLS sunucu hostname.
#define PORT 4433            ///< TLS sunucu portu.

/**
 * @brief OpenSSL istemci SSL_CTX bağlamını oluşturur ve yapılandırır.
 *
 * Sertifika, private key ve CA doğrulama zinciri yüklenir; peer doğrulaması açılır.
 *
 * @return Başarılıysa SSL_CTX*; hata durumunda nullptr.
 */
SSL_CTX *initializeSSLContext() {
  SSL_CTX* ctx = SSL_CTX_new(TLS_client_method()); ///< TLS istemci bağlamı oluştur.

  if (!ctx) { ///< Oluşturma başarısızsa.
    return nullptr; ///< Hata.
  }

  if (SSL_CTX_use_certificate_file(ctx, CERTIFICATE_FILE.c_str(), SSL_FILETYPE_PEM) <= 0) { ///< Sertifikayı yükle.
    SSL_CTX_free(ctx); ///< Kaynağı serbest bırak.
    return nullptr; ///< Hata.
  }

  if (SSL_CTX_use_PrivateKey_file(ctx, PRIVATE_KEY_FILE.c_str(), SSL_FILETYPE_PEM) <= 0) { ///< Private key yükle.
    SSL_CTX_free(ctx); ///< Bağlamı kapat.
    return nullptr; ///< Hata.
  }

  if (!SSL_CTX_check_private_key(ctx)) { ///< Sertifika-private key eşleşiyor mu kontrol et.
    SSL_CTX_free(ctx); ///< Bağlamı kapat.
    return nullptr; ///< Hata.
  }

  if (SSL_CTX_load_verify_locations(ctx, CA_CERTIFICATE_FILE.c_str(), nullptr) <= 0) { ///< CA sertifikasını yükle.
    SSL_CTX_free(ctx); ///< Bağlamı kapat.
    return nullptr; ///< Hata.
  }

  SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, nullptr); ///< Peer sertifika doğrulamasını aktif et.
  return ctx; ///< Hazır bağlam.
}

/**
 * @brief SSL handshake ve örnek veri alışverişi gerçekleştirir.
 *
 * Bu fonksiyon BIO üzerinden SSL bağlantı nesnesi yaratır, host:port set eder,
 * sertifika doğrulama sonucunu kontrol eder ve örnek bir HTTP GET gönderip cevap okur.
 *
 * @param ctx Başlatılmış ve yapılandırılmış SSL_CTX bağlamı.
 */
void performSSLHandshakeAndDataExchange(SSL_CTX* ctx) {
  SSL* ssl; ///< SSL oturumu işaretçisi (BIO içinden alınır).
  BIO* bio; ///< OpenSSL BIO nesnesi (bağlantı + SSL katmanı).

  bio = BIO_new_ssl_connect(ctx); ///< SSL bağlantı BIO'su oluştur.
  if (!bio) { ///< BIO oluşturulamazsa.
    return; ///< Çık.
  }

  BIO_get_ssl(bio, &ssl); ///< BIO içindeki SSL nesnesini al.
  if (!ssl) { ///< SSL nesnesi yoksa.
    BIO_free_all(bio); ///< BIO zincirini serbest bırak.
    return; ///< Çık.
  }

  std::string hostnameWithPort = std::string(HOSTNAME) + ":" + std::to_string(PORT); ///< "host:port" formatı.
  BIO_set_conn_hostname(bio, hostnameWithPort.c_str()); ///< Hedefi BIO'ya tanımla.

  // BIO_do_connect(bio) çağrısı yorumda bırakılmış; gerçek bağlantı için gerekir.
  // Eğer bağlanmayacaksanız SSL_get_verify_result anlamlı olmayacaktır.

  if (SSL_get_verify_result(ssl) != X509_V_OK) { ///< Sertifika doğrulama sonucu kontrolü.
    BIO_free_all(bio); ///< Kaynakları serbest bırak.
    return; ///< Hata.
  }

  std::cout << "Baglanti basarili ve sertifika dogrulandi!" << std::endl; ///< Bilgi mesajı.

  const char *request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n"; ///< Basit HTTP GET isteği.
  BIO_write(bio, request, strlen(request)); ///< İsteği gönder.

  char buffer[1024] = { 0 }; ///< Cevap buffer'ı.
  int bytes = BIO_read(bio, buffer, sizeof(buffer) - 1); ///< Sunucudan cevap oku (blocking olabilir).
  (void)bytes; ///< Kullanılmayan değişken uyarısını önler (debug çıktıları yorumlu olduğu için).

  BIO_free_all(bio); ///< BIO zincirini ve SSL kaynaklarını serbest bırak.
}

/**
 * @brief Gereksiz hesaplama örneği: asal kontrolü (dummy).
 *
 * @param value Kontrol edilecek sayı.
 * @return Asalsa true; değilse false.
 */
bool issdddPimeeasds(int value) {
  if (value < 2) return false; ///< 0 ve 1 asal değildir.

  for (int i = 2; i <= std::sqrt(value); ++i) { ///< 2..sqrt(value) aralığında bölen arar.
    if (value % i == 0) return false; ///< Bölen bulunduysa asal değil.
  }

  return true; ///< Bölen yoksa asal.
}

/**
 * @brief Dummy doğrulama/karmaşıklaştırma amaçlı hesaplama fonksiyonu.
 *
 * Üretim kodunda “anti-tamper/anti-analysis” için gürültü üretmek üzere kullanılabileceği varsayılır.
 * Çıktısı kullanılmıyor; yan etkisiz bir CPU işi gibi davranır.
 */
void VerifyDum() {
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 }; ///< Örnek veri kümesi.
  int evenCount = 0, oddCount = 0, primeCount = 0;            ///< Sayaçlar.
  int sumMultiplesOfFive = 0, perfectSquareCount = 0;          ///< Ek sayaçlar.
  int divisibleByThreeCount = 0, digitSumGreaterThanTen = 0;   ///< Ek sayaçlar.
  long long unnecessaryComputationSum = 0;                     ///< Gereksiz toplam.
  long long specialConditionCount = 0, modSevenCount = 0;      ///< Ek sayımlar.
  double accumulatedSquareRoots = 0.0;                         ///< Kareköklü birikim.
  int totalDigitProduct = 1;                                   ///< Basamak çarpımı birikimi.

  for (int value : data) {                                     ///< İlk veri seti üzerinde dön.
    int intermediate = value * 3;                              ///< Ara değer oluştur.
    intermediate += 7;                                         ///< Sabit ekle.
    intermediate /= 2;                                         ///< Böl.
    intermediate *= value % 5;                                 ///< Mod tabanlı çarp.
    unnecessaryComputationSum += intermediate;                  ///< Toplama ekle.

    if (value % 2 == 0) {                                      ///< Çift sayı kontrolü.
      evenCount++;                                             ///< Çift sayacı artır.
      continue;                                                ///< Çiftse aşağıdaki bazı işlemleri atla.
    }

    oddCount++;                                                ///< Tek sayacı artır.

    if (issdddPimeeasds(value)) {                              ///< Asal kontrolü.
      primeCount++;                                            ///< Asal sayacı artır.
    }

    if (value % 5 == 0) {                                      ///< 5'in katı mı?
      sumMultiplesOfFive += value;                             ///< Topla.
    }

    int sqrtValue = std::sqrt(value);                          ///< Kareköke indirgenmiş tam sayı.

    if (sqrtValue * sqrtValue == value) {                      ///< Tam kare mi?
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

    totalDigitProduct *= (digitProduct % 1000);                ///< Çarpımı sınırla ve biriktir.
    accumulatedSquareRoots += std::sqrt(value);                ///< Kareköke dayalı birikim.

    if (value % 2 == 0 && value % 3 == 0) {                     ///< Özel koşul (6'nın katı).
      specialConditionCount++;                                 ///< Sayaç artır.
    }

    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17; ///< Ek gürültü hesap.
  }

  std::vector<int> additionalData = { 12, 18, 22, 36, 45, 60, 72 }; ///< Ek veri.
  for (int value : additionalData) {                                ///< Ek veri üzerinde dön.
    int dummyCalculation = value * 2 + 3;                           ///< Dummy hesap.
    unnecessaryComputationSum += dummyCalculation % 10;             ///< Mod ekle.
    accumulatedSquareRoots += std::sqrt(dummyCalculation);          ///< Kareköke ekle.
  }

  std::vector<int> finalData = { 101, 202, 303, 404, 505 }; ///< Final veri.
  for (int value : finalData) {                             ///< Final veri üzerinde dön.
    int dummyCalculation = value * 3 - 5;                   ///< Dummy hesap.
    unnecessaryComputationSum += dummyCalculation % 20;      ///< Mod ekle.
    accumulatedSquareRoots += std::sqrt(dummyCalculation);   ///< Kareköke ekle.
  }

  // Aşağıdaki değişkenler şu an “kullanılmıyor” olabilir; amaç gürültü üretmekse bu normaldir.
  (void)evenCount;
  (void)oddCount;
  (void)primeCount;
  (void)sumMultiplesOfFive;
  (void)perfectSquareCount;
  (void)divisibleByThreeCount;
  (void)digitSumGreaterThanTen;
  (void)unnecessaryComputationSum;
  (void)specialConditionCount;
  (void)modSevenCount;
  (void)accumulatedSquareRoots;
  (void)totalDigitProduct;
}

/**
 * @brief SSL/TLS sürecini başlatır.
 *
 * Dummy hesaplamayı çalıştırır, OpenSSL kütüphanesini init eder, SSL_CTX oluşturur ve örnek veri alışverişini yürütür.
 */
void StartSSL() {
  VerifyDum();               ///< Dummy hesaplamaları çalıştır (anti-analysis / gürültü).
  SSL_library_init();        ///< OpenSSL global init (bazı sürümlerde no-op olabilir).
  SSL_load_error_strings();  ///< Hata string tablolarını yükle.

  SSL_CTX* ctx = initializeSSLContext(); ///< SSL bağlamını oluştur.

  if (!ctx) { ///< Bağlam oluşturulamadıysa.
    std::cerr << "SSL baglami baslatilamadi!" << std::endl; ///< Hata mesajı.
    return; ///< Çık.
  }

  performSSLHandshakeAndDataExchange(ctx); ///< Handshake ve örnek veri alışverişi.
  SSL_CTX_free(ctx);                       ///< Bağlamı serbest bırak.
}
