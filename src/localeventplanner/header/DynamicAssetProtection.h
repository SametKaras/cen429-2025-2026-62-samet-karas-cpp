/**
 * @file DynamicAssetProtection.h
 * @brief Dinamik varlık koruma arayüzü: cihaz parmak izi ve Base64 yardımcıları.
 *
 * Bu başlık; DLL import/export makrolarını ve dışa açılan API fonksiyon prototiplerini içerir.
 */

#ifndef DYNAMIC_ASSET_PROTECTION_H              ///< Header include guard başlangıcı.
#define DYNAMIC_ASSET_PROTECTION_H              ///< Header include guard tanımı.

#ifdef LOCAL_EVENT_PLANNER_LIB_EXPORTS          ///< DLL derlenirken export modu.
  #define LOCAL_EVENT_PLANNER_API __declspec(dllexport)  /**< DLL oluşturulurken sembolleri dışa aktarır. */
#else                                           ///< DLL kullanıcı tarafında import modu.
  #define LOCAL_EVENT_PLANNER_API __declspec(dllimport)  /**< DLL kullanılırken sembolleri içe aktarır. */
#endif

#include <string>                               ///< std::string tipi için gerekli header.

#ifdef __cplusplus                              ///< C++ derleyicisi ile derleniyorsa.
extern "C" {                                   ///< C bağlantı uyumluluğu (name mangling kapatılır).
#endif

/**
 * @brief Cihaz parmak izini üretir ve şifreleyerek döndürür.
 *
 * Uygulama, platforma özgü cihaz bilgisini toplar, SHA-256 ile hash’ler ve oturum anahtarıyla şifreler.
 *
 * @return Şifrelenmiş cihaz parmak izi (string). Hata durumunda boş string dönebilir.
 *
 * @note Bu prototip `extern "C"` bloğunda tanımlandığı için C-linkage ile export edilir.
 *       Ancak dönüş tipi `std::string` olduğu için pratikte C tüketicileri için uygun değildir.
 */
LOCAL_EVENT_PLANNER_API std::string getEncryptedDeviceFingerprint(); ///< Şifrelenmiş cihaz parmak izi API'si.

#ifdef __cplusplus                              ///< C++ bloğu kapanışı.
}                                              ///< extern "C" kapsam sonu.
#endif

/**
 * @brief Binary veriyi Base64 string’e encode eder.
 *
 * @param buffer Encode edilecek binary veri pointer’ı.
 * @param length Buffer uzunluğu (byte).
 * @return Base64 encoded string; hata durumunda boş string dönebilir.
 *
 * @warning Bu prototip `size_t` kullanır; bu header'da `<cstddef>` dahil edilmemiştir.
 *          Bazı derleyici/konfigürasyonlarda `size_t` için `<cstddef>` gerekebilir.
 */
LOCAL_EVENT_PLANNER_API std::string base64Encode(const unsigned char* buffer, size_t length); ///< Base64 encode API.

#endif // DYNAMIC_ASSET_PROTECTION_H             ///< Header include guard sonu.
