/**
 * @file SignatureVerification.cpp
 * @brief DLL içinde imza doğrulama ve çağıran proses yolunu alma yardımcıları.
 *
 * Bu dosya; çağıran prosesin dosya yolunu tespit eder, imza dosyası üretir (openssl komut satırı),
 * ardından OpenSSL EVP ile SHA-256 imza doğrulaması gerçekleştirir.
 * Ek olarak, analiz/karartma amaçlı “dummy/opaque” fonksiyonlar içerir.
 */

// SignatureVerification.cpp : Defines the exported functions for the DLL application. ///< Visual Studio şablon açıklaması.
#include "SignatureVerification.h" ///< Proje başlığı: export makroları ve fonksiyon prototipleri.
#include <iostream>                ///< std::cout/std::cerr için.
#include <fstream>                 ///< Dosya okuma (std::ifstream) için.
#include <openssl/evp.h>           ///< OpenSSL EVP (DigestVerify) API.
#include <openssl/pem.h>           ///< PEM anahtar okuma/yazma fonksiyonları.
#include <openssl/bio.h>           ///< BIO bellek buffer/IO katmanı.
#include <windows.h>               ///< Windows API (process handle, PID vb.).
#include <Psapi.h>                 ///< GetModuleFileNameEx için (Psapi.lib link gerekir).

/**
 * @brief Bir dosyayı tamamını ikili olarak okuyup std::string döndürür.
 *
 * Dosya boyutunu `std::ios::ate` ile sonda konumlanarak alır, sonra başa sarıp buffer'a okur.
 *
 * @param filePath Okunacak dosyanın yolu.
 * @return Dosya içeriği (binary-safe).
 *
 * @note Dosya açma hatası için exception bloğu yorum satırına alınmış; bu nedenle hata durumunda
 *       `tellg()` başarısız olabilir ve sonuç tanımsız davranışa yaklaşabilir.
 */
LOCAL_EVENT_PLANNER_API std::string readFile(const std::string& filePath) {
  std::ifstream file(filePath, std::ios::binary | std::ios::ate); ///< Dosyayı binary + sonda aç.
  /*if (!file) {                                                   ///< Dosya açılamazsa (şu an kapalı).
    throw std::runtime_error("Failed to open file: " + filePath);  ///< Hata fırlat (kapalı).
  }*/                                                              ///< Hata kontrolü devre dışı.
  std::streamsize size = file.tellg();                             ///< Dosya boyutunu oku (konum sondadır).
  file.seekg(0, std::ios::beg);                                    ///< Okuma konumunu dosya başına al.
  std::string buffer(size, '\0');                                  ///< Boyut kadar null ile initialize buffer.
  file.read(&buffer[0], size);                                     ///< Dosya içeriğini buffer'a oku.
  return buffer;                                                   ///< Okunan içeriği döndür.
}

/**
 * @brief Çağıran uygulamanın dosyasına ilişkin imzayı doğrular.
 *
 * İş akışı:
 * - Çağıran prosesin dosya yolu alınır.
 * - `openssl dgst -sha256 -sign ...` ile imza dosyası (*.sig) üretilir (komut satırı).
 * - Public key PEM içeriği yüklenir.
 * - İmza dosyası ve hedef dosya içeriği okunur.
 * - OpenSSL EVP_DigestVerify ile doğrulama yapılır.
 *
 * @return Doğrulama başarılıysa true; aksi halde false.
 *
 * @warning Bu fonksiyon private key kullanarak imza üretmektedir. Normal dağıtım senaryosunda private key
 *          istemci tarafında bulunmamalıdır (güvenlik mimarisi açısından risk).
 */
LOCAL_EVENT_PLANNER_API bool verifySignature() {
  const std::string publicKeyPath = PUBLIC_KEY_PATH;  ///< Public key dosya yolu (makrodan).
  const std::string privateKeyPath = PRIVATE_KEY_PATH;///< Private key dosya yolu (makrodan).
  std::string callerPath = getCallerProcessPath();    ///< İmzalanacak/doğrulanacak hedef: çağıran exe/dll yolu.
  std::string signaturePath = callerPath + ".sig";    ///< İmza dosyası yolu: hedefin yanına .sig eklenir.
  std::string command =                               ///< OpenSSL komut satırı ile imza üretme komutu.
      "openssl dgst -sha256 -sign \"" + privateKeyPath +
      "\" -out \"" + signaturePath + "\" \"" + callerPath + "\"";
  int resultCommand = std::system(command.c_str());   ///< Komutu çalıştır (exit code resultCommand).
  (void)resultCommand;                                ///< Şu an kullanılmıyor; uyarıyı bastırmak için.

  // Load public key                                                   ///< Public key yükleme bölümü.
  std::string publicKeyContent = readFile(publicKeyPath);              ///< PEM dosya içeriğini oku.
  BIO* bio = BIO_new_mem_buf(publicKeyContent.data(), -1);             ///< BIO üzerinden belleğe bağla (-1 = null-terminated varsayımı).
  EVP_PKEY* publicKey = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr); ///< BIO'dan public key oku.
  BIO_free(bio);                                                       ///< BIO nesnesini serbest bırak.

  if (!publicKey) {                                                    ///< Public key yüklenemediyse.
    // std::cerr << "Failed to load public key\n";                      ///< Debug mesajı (kapalı).
    return false;                                                      ///< Başarısız doğrulama.
  }

  // Load signature                                                     ///< İmza içeriğini yükle.
  std::string signatureContent = readFile(signaturePath);              ///< .sig dosyasını oku.
  // Load data                                                          ///< İmzalanan veriyi yükle.
  std::string dataContent = readFile(callerPath);                      ///< Hedef dosyanın içeriğini oku.

  // Create a verification context                                      ///< Doğrulama context'i oluştur.
  EVP_MD_CTX* ctx = EVP_MD_CTX_new();                                  ///< EVP message digest context.

  if (!ctx) {                                                          ///< Context oluşturulamadıysa.
    // std::cerr << "Failed to create EVP_MD_CTX\n";                    ///< Debug mesajı (kapalı).
    EVP_PKEY_free(publicKey);                                          ///< Public key'i serbest bırak.
    return false;                                                      ///< Başarısız doğrulama.
  }

  // Initialize verification                                            ///< Doğrulama algoritmasını init et.
  if (EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, publicKey) <= 0) { ///< SHA-256 ile verify init.
    //std::cerr << "Failed to initialize verification\n";               ///< Debug mesajı (kapalı).
    EVP_MD_CTX_free(ctx);                                              ///< Context'i serbest bırak.
    EVP_PKEY_free(publicKey);                                          ///< Public key'i serbest bırak.
    return false;                                                      ///< Başarısız doğrulama.
  }

  // Verify the signature                                               ///< İmzayı doğrula.
  bool result = EVP_DigestVerify(                                      ///< Tek-shot verify çağrısı.
                                 ctx,                                 ///< Context.
                                 reinterpret_cast<const unsigned char *>(signatureContent.data()), ///< İmza buffer'ı.
                                 signatureContent.size(),             ///< İmza boyutu.
                                 reinterpret_cast<const unsigned char *>(dataContent.data()),      ///< Veri buffer'ı.
                                 dataContent.size()) == 1;            ///< Veri boyutu; ==1 ise doğrulama OK.

  EVP_MD_CTX_free(ctx);                                                ///< Context'i serbest bırak.
  EVP_PKEY_free(publicKey);                                            ///< Public key'i serbest bırak.
  return result;                                                       ///< Sonucu döndür.
}

#include <vector> ///< Dummy fonksiyonlarda vektör kullanılacağı için.

/**
 * @brief Basit asal sayı kontrolü (dummy).
 * @param value Kontrol edilecek değer.
 * @return Asal ise true; değilse false.
 */
bool isPazzximeeasds(int value) {
  if (value < 2) return false;                         ///< 0 ve 1 asal değildir.

  for (int i = 2; i <= std::sqrt(value); ++i) {        ///< 2..sqrt(value) arası bölen ara.
    if (value % i == 0) return false;                  ///< Bölen bulunduysa asal değil.
  }

  return true;                                         ///< Bölen yoksa asal.
}

/**
 * @brief Sahte boolean fonksiyonu (opaque predicate benzeri).
 * @param input Girdi değeri.
 * @return Belirsiz/karartma amaçlı koşul sonucu.
 */
bool sasdasfffvhhjk(int input) {
  int temp = (input * 3) + 7 - 5;                      ///< Gereksiz aritmetik dönüşüm.
  return temp % 2 == 0;                                ///< Pariteye göre boolean.
}

/**
 * @brief Sahte aritmetik işlem (karartma amaçlı).
 * @param a Birinci operand.
 * @param b İkinci operand.
 * @return “Anlamsal olarak” gereksiz bir sonuç.
 */
int shithsadmeticOsadperation(int a, int b) {
  return (a * b + 3 - 2) / (a == 0 ? 1 : a);           ///< a=0 ise bölme hatasını önlemek için 1'e sabitler.
}

/**
 * @brief Opaque döngü (kontrol akışı manipülasyonu örneği).
 *
 * Rastgele break/return noktaları içerir; analiz ve optimizasyonu zorlaştırmayı amaçlayabilir.
 */
void shiddasdenLoopOdasperation() {
  std::vector<int> data = { 2, 4, 6, 8, 10, 12, 14, 16, 18 }; ///< Örnek veri dizisi.
  int opaqueResult = 0;                                       ///< Opaque birikim.
  int fakeComputation = 0;                                    ///< Sahte hesaplama birikimi.

  for (int i = 0; i < static_cast<int>(data.size()); ++i) {   ///< Dizi üzerinde indeksli iterasyon.
    // Sahte işlemler ve belirsiz aritmetik                       ///< Satır içi açıklama.
    int intermediate = (data[i] * 2 + 5) - (data[i] / 2);      ///< Ara hesap.
    fakeComputation += (intermediate % 7) * 3;                 ///< Mod + çarpım ile gürültü.

    if (sasdasfffvhhjk(intermediate)) {                        ///< Opaque predicate.
      opaqueResult += shithsadmeticOsadperation(intermediate, i + 1); ///< Sahte aritmetik sonucu ekle.
    }

    // Rastgele çıkış noktaları                                    ///< Kontrol akışı “bozma” örnekleri.
    if (i == 3 && intermediate % 3 == 0) {                     ///< Belirli noktada koşullu break.
      break;                                                   ///< Döngüden çık.
    }

    if (i == 5 && intermediate % 2 == 1) {                     ///< Belirli noktada koşullu return.
      return;                                                  ///< Fonksiyondan çık.
    }
  }

  (void)opaqueResult;                                          ///< Kullanılmıyor; uyarıyı bastır.
  (void)fakeComputation;                                       ///< Kullanılmıyor; uyarıyı bastır.
}

/**
 * @brief Sahte “ölüm dalı” ve rastgele değişken manipülasyonları.
 *
 * Değişkenler üzerinde etkisiz aritmetik dönüşümler uygular.
 */
void sfadasdasfrations() {
  int a = 10;                                                  ///< Dummy değişken.
  int b = 20;                                                  ///< Dummy değişken.
  int c = 30;                                                  ///< Dummy değişken.
  int d = 40;                                                  ///< Dummy değişken.
  // Sahte işlemler                                              ///< İşlemsel gürültü.
  a = a + 1 - 1 + b - b;                                       ///< Net etkisi 0.
  b = b * 2 / 2 - c + c;                                       ///< Net etkisi 0.
  c = c + d - d * 2 / 2;                                       ///< Net etkisi 0.
  d = d * 3 / 3 - a + b;                                       ///< a/b etkisi var gibi görünür (ama önceki satırlara bağlı).
  (void)a; (void)b; (void)c; (void)d;                          ///< Uyarıları bastır.
}

/**
 * @brief Kullanılmayan parametreler içeren sahte fonksiyon.
 * @param x Asıl değer.
 * @param unused1 Kullanılmayan parametre.
 * @param unused2 Kullanılmayan parametre.
 * @param unused3 Kullanılmayan parametre.
 * @return Gürültülü bir aritmetik sonuç.
 */
int sfasdction(int x, int unused1, int unused2, int unused3) {
  return x * 3 + unused1 - unused2 + unused3;                  ///< Parametreler “kullanılıyor gibi”.
}

/**
 * @brief Dummy ana işlem (karartma zinciri).
 *
 * Opaque döngü, sahte varyasyon ve sonuç manipülasyonu yapar.
 */
void ssecusreOperation() {
  shiddasdenLoopOdasperation();                                 ///< Opaque döngüyü çalıştır.
  sfadasdasfrations();                                          ///< Sahte hesapları çalıştır.
  int result = sfasdction(10, 0, 0, 0);                         ///< Dummy fonksiyon çağrısı.
  result += shithsadmeticOsadperation(5, 3);                    ///< Sahte aritmetik ekle.

  if (result % 2 == 0) {                                        ///< Parite kontrolü.
    result += 1;                                                ///< Belirsiz değişiklik.
  }

  (void)result;                                                 ///< Kullanılmıyor; uyarıyı bastır.
}

/**
 * @brief Daha uzun dummy hesap fonksiyonu.
 *
 * Çeşitli sayımlar, karekökler ve mod işlemleriyle “iş” üretir.
 */
void afffxxzzddddzcwdfssfd() {
  std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 16, 25, 30 };  ///< Örnek veri.
  int evenCount = 0, oddCount = 0, primeCount = 0;             ///< Sayaçlar.
  int sumMultiplesOfFive = 0, perfectSquareCount = 0;           ///< Ek sayaçlar.
  int divisibleByThreeCount = 0, digitSumGreaterThanTen = 0;    ///< Ek sayaçlar.
  long long unnecessaryComputationSum = 0;                      ///< Gereksiz toplam.
  long long specialConditionCount = 0, modSevenCount = 0;       ///< Ek sayımlar.
  double accumulatedSquareRoots = 0.0;                          ///< Karekök birikimi.
  int totalDigitProduct = 1;                                    ///< Basamak çarpımı birikimi.

  for (int value : data) {                                      ///< Veriler üzerinde dön.
    int intermediate = value * 3;                               ///< Ara değer.
    intermediate += 7;                                          ///< Sabit ekle.
    intermediate /= 2;                                          ///< Böl.
    intermediate *= value % 5;                                  ///< Mod ile çarp.
    unnecessaryComputationSum += intermediate;                   ///< Biriktir.

    if (value % 2 == 0) {                                       ///< Çift mi?
      evenCount++;                                              ///< Çift say.
      continue;                                                 ///< Aşağıdaki tek-odaklı işleri atla.
    }

    oddCount++;                                                 ///< Tek say.

    if (isPazzximeeasds(value)) {                               ///< Asal kontrol.
      primeCount++;                                             ///< Asal say.
    }

    if (value % 5 == 0) {                                       ///< 5'in katı mı?
      sumMultiplesOfFive += value;                              ///< Topla.
    }

    int sqrtValue = std::sqrt(value);                           ///< Tam sayıya indirgenmiş karekök.

    if (sqrtValue * sqrtValue == value) {                       ///< Tam kare mi?
      perfectSquareCount++;                                     ///< Tam kare say.
    }

    if (value % 3 == 0) {                                       ///< 3'e bölünür mü?
      divisibleByThreeCount++;                                  ///< Say.
    }

    if (value % 7 == 0) {                                       ///< 7'ye bölünür mü?
      modSevenCount++;                                          ///< Say.
    }

    int digitSum = 0, digitProduct = 1;                         ///< Basamak toplamı/çarpımı.
    int temp = value;                                           ///< Geçici kopya.

    while (temp > 0) {                                          ///< Basamaklar bitene kadar.
      int digit = temp % 10;                                    ///< Son basamak.
      digitSum += digit;                                        ///< Toplam.
      digitProduct *= digit;                                    ///< Çarpım.
      temp /= 10;                                               ///< Basamak düş.
    }

    if (digitSum > 10) {                                        ///< Eşik kontrolü.
      digitSumGreaterThanTen++;                                 ///< Say.
    }

    totalDigitProduct *= (digitProduct % 1000);                 ///< Çarpımı sınırlayarak biriktir.
    accumulatedSquareRoots += std::sqrt(value);                 ///< Karekök birikimi.

    if (value % 2 == 0 && value % 3 == 0) {                      ///< 6'nın katı mı?
      specialConditionCount++;                                  ///< Say.
    }

    unnecessaryComputationSum += digitSum * 5 - value / 3 + 17;  ///< Ek gürültü.
  }

  std::vector<int> additionalData = { 12, 18, 22, 36, 45, 60, 72 }; ///< Ek veri.

  for (int value : additionalData) {                            ///< Ek veri üzerinde dön.
    int dummyCalculation = value * 2 + 3;                       ///< Dummy hesap.
    unnecessaryComputationSum += dummyCalculation % 10;          ///< Mod ekle.
    accumulatedSquareRoots += std::sqrt(dummyCalculation);       ///< Karekök ekle.
  }

  std::vector<int> finalData = { 101, 202, 303, 404, 505 };      ///< Final veri.

  for (int value : finalData) {                                 ///< Final veri üzerinde dön.
    int dummyCalculation = value * 3 - 5;                        ///< Dummy hesap.
    unnecessaryComputationSum += dummyCalculation % 20;          ///< Mod ekle.
    accumulatedSquareRoots += std::sqrt(dummyCalculation);       ///< Karekök ekle.
  }

  // Sonuç değişkenleri kullanılmıyor; gürültü amaçlı olabilir.
  (void)evenCount; (void)oddCount; (void)primeCount;
  (void)sumMultiplesOfFive; (void)perfectSquareCount;
  (void)divisibleByThreeCount; (void)digitSumGreaterThanTen;
  (void)unnecessaryComputationSum; (void)specialConditionCount;
  (void)modSevenCount; (void)accumulatedSquareRoots;
  (void)totalDigitProduct;
}

/**
 * @brief İmza doğrulamayı başlatır ve sonucu loglar.
 *
 * @return Başarılıysa true/1 benzeri, başarısızsa false/0 beklenir; ancak mevcut implementasyon tutarsızdır.
 *
 * @note Fonksiyon dönüş tipi bool iken başarısızlıkta `return 1;` dönüyor ve başarıda return etmiyor.
 *       Bu, derleyici uyarısı/UB doğurabilir. Davranışı değiştirmeden dokümante edilmiştir.
 */
bool startVerification() {
  if (verifySignature()) {                                      ///< Doğrulama ok ise.
    std::cout << "Signature verification successful!\n";        ///< Başarı logu.
    //   std::cout << "===============================\n";       ///< Ek çıktı (kapalı).
  } else {                                                      ///< Doğrulama başarısız ise.
    //  std::cerr << "Signature verification failed!\n";         ///< Hata logu (kapalı).
    return 1;                                                   ///< bool bağlamında true döner; muhtemelen istenmeyen.
  }
  // Burada return yok; bool fonksiyon için problemli.
}

/**
 * @brief Ana uygulamanın (çağıran prosesin) dosya yolunu döndürür.
 *
 * Windows'ta:
 * - PID alınır.
 * - OpenProcess ile handle açılır.
 * - GetModuleFileNameEx ile path okunur.
 *
 * Ek olarak, çeşitli dummy hesaplar ve karartma fonksiyonları çağrılır.
 *
 * @return Proses yürütülebilirinin tam yolu; başarısızlıkta boş string.
 */
LOCAL_EVENT_PLANNER_API std::string getCallerProcessPath() {
  int resulst = 0;                                              ///< Dummy akümülatör (adlandırma korunmuş).
  int temsp = 1;                                                ///< Dummy çarpan.
  int bs = 5;                                                   ///< Dummy değişken.
  int cde = 18;                                                 ///< Dummy değişken.

  for (int i = 1; i <= 10; ++i) {                               ///< Dummy döngü.
    temsp *= i % 3 + 1;                                         ///< Mod + çarpma.
    resulst += temsp % 7 - 2;                                   ///< Mod + toplama/çıkarma.
    resulst ^= (i * 5) & 3;                                     ///< XOR + AND.

    if (resulst % 4 == 0) {                                     ///< Koşullu dönüşüm.
      resulst += temsp / 2;                                     ///< Güncelleme.
    }

    bs = cde + bs;                                              ///< Dummy toplama.
    temsp += resulst % 9;                                       ///< Döngü değişkenini etkileyen ek işlem.
  }

  cde = cde + bs;                                               ///< Dummy final toplama.
  ssecusreOperation();                                          ///< Dummy karartma zinciri.
  afffxxzzddddzcwdfssfd();                                      ///< Dummy iş yükü.

  char processPath[MAX_PATH] = { 0 };                           ///< Proses yolunu tutacak buffer.
  DWORD processId = GetCurrentProcessId();                      ///< Mevcut proses PID.
  HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                     FALSE,
                                     processId);                ///< Proses handle aç (sorgu + VM read).

  if (processHandle) {                                          ///< Handle başarıyla açıldıysa.
    if (GetModuleFileNameEx(processHandle, NULL, processPath, MAX_PATH)) { ///< Ana modül yolunu al.
      CloseHandle(processHandle);                               ///< Handle kapat (başarı).
      return std::string(processPath);                          ///< Yol string olarak döndür.
    }

    CloseHandle(processHandle);                                 ///< Handle kapat (başarısız).
  }

  return "";                                                    ///< Yol alınamadıysa boş döndür.
}
