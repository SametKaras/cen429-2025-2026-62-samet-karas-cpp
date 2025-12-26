/**
 * @file SaltAndHmac.h
 * @brief Salt üretimi ve HMAC tabanlı parola hashleme için API tanımları.
 *
 * Bu başlık dosyası; DLL import/export makrolarını ve salt/HMAC ile ilgili
 * dışa açık fonksiyon prototiplerini içerir.
 */

#ifndef SALT_AND_HMAC_H                      ///< Header include guard başlangıcı.
#define SALT_AND_HMAC_H                      ///< Header include guard tanımı.

#ifdef LOCAL_EVENT_PLANNER_LIB_EXPORTS       ///< DLL derlenirken (export modu).
  #define LOCAL_EVENT_PLANNER_API __declspec(dllexport)  /**< DLL oluşturulurken sembolleri dışa aktarır. */
#else                                        ///< DLL kullanılırken (import modu).
  #define LOCAL_EVENT_PLANNER_API __declspec(dllimport)  /**< DLL kullanılırken sembolleri içe aktarır. */
#endif

#include <string>                            ///< std::string tipi için gerekli başlık.

#ifdef __cplusplus                           ///< C++ derleyicisi kullanılıyorsa.
extern "C" {                                ///< C bağlantı uyumluluğu (name mangling kapatılır).
#endif

/**
 * @brief Sabit (deterministik) bir salt üretir.
 *
 * Verilen seed değeri üzerinden her çağrıda aynı sonucu üreten bir salt oluşturur.
 *
 * @param seed Salt üretiminde kullanılacak tohum (seed) değeri.
 * @return Üretilmiş salt string’i.
 *
 * @note Deterministik salt kullanımı, bazı güvenlik senaryolarında risklidir.
 */
LOCAL_EVENT_PLANNER_API std::string generateFixedSalt(const std::string& seed); ///< Sabit salt üretim API'si.

/**
 * @brief Parolayı HMAC algoritması kullanarak hashler.
 *
 * @param password Hashlenecek açık metin parola.
 * @param salt HMAC işlemi için kullanılacak salt değeri.
 * @return HMAC ile üretilmiş hash string’i.
 */
LOCAL_EVENT_PLANNER_API std::string hashPasswordWithHMAC(const std::string& password,
                                                         const std::string& salt); ///< HMAC tabanlı parola hashleme API'si.

/**
 * @brief Rastgele (deterministik olmayan) bir salt üretir.
 *
 * @return Rastgele üretilmiş salt string’i.
 */
LOCAL_EVENT_PLANNER_API std::string generateSalt(); ///< Rastgele salt üretim API'si.

/**
 * @brief Alternatif HMAC tabanlı parola hashleme fonksiyonu.
 *
 * Muhtemelen farklı bir implementasyon veya test varyantıdır.
 *
 * @param password Hashlenecek açık metin parola.
 * @param salt HMAC işlemi için kullanılacak salt değeri.
 * @return HMAC ile üretilmiş hash string’i.
 */
LOCAL_EVENT_PLANNER_API std::string hashPasswordWithHMACC(const std::string& password,
                                                          const std::string& salt); ///< Alternatif HMAC API'si.

#ifdef __cplusplus                           ///< C++ bloğu kapanışı.
}                                           ///< extern "C" kapsam sonu.
#endif

#endif // SALT_AND_HMAC_H                    ///< Header include guard sonu.
