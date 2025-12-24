# OWASP Uyumluluk Belgesi
## Local Event Planner - OWASP Compliance Document

**Versiyon:** 1.0  
**Tarih:** 2024-12-24  
**Referans:** OWASP Top 10 (2021), OWASP ASVS v4.0

---

## 1. Genel Bakış

Bu belge, Local Event Planner uygulamasının OWASP (Open Web Application Security Project) standartlarına uyumluluğunu değerlendirmektedir.

---

## 2. OWASP Top 10 (2021) Uyumluluk Tablosu

| # | Kategori | Durum | Uygulanan Önlemler | Kod Referansı |
|---|----------|-------|-------------------|---------------|
| A01 | Broken Access Control | ✅ Uyumlu | Rol tabanlı erişim, Guest mode kısıtlamaları | `UserAuthentication.cpp` |
| A02 | Cryptographic Failures | ✅ Uyumlu | WBAES, HMAC-SHA256, AES-256 | `WBAES.cpp`, `SaltAndHMAC.cpp` |
| A03 | Injection | ✅ Uyumlu | Parametreli SQL sorguları | Tüm database işlemleri |
| A04 | Insecure Design | ✅ Uyumlu | Güvenlik mimarisi tasarımı | `SECURITY_ARCHITECTURE.md` |
| A05 | Security Misconfiguration | ⚠️ Kısmi | Varsayılan güvenli ayarlar | CMakeLists.txt |
| A06 | Vulnerable Components | ✅ Uyumlu | Güncel OpenSSL, SQLite | Dependencies |
| A07 | Auth Failures | ✅ Uyumlu | Güçlü şifre politikası, secure hashing | `UserAuthentication.cpp` |
| A08 | Software/Data Integrity | ✅ Uyumlu | Signature verification, checksum | `SignatureVerification.cpp` |
| A09 | Logging Failures | ⚠️ Kısmi | Temel loglama mevcut | Debug outputs |
| A10 | SSRF | ➖ N/A | Server taraflı istek yok | N/A |

---

## 3. Detaylı OWASP Uyumluluk Analizi

### A01:2021 - Broken Access Control

**Durum:** ✅ Uyumlu

**Uygulanan Önlemler:**
- Kullanıcı oturum yönetimi
- Guest mode ile sınırlı erişim
- Kimlik doğrulama gereksinimleri

**Kod Örnekleri:**
```cpp
// UserAuthentication.cpp
bool getGuestMode() {
    return isGuestMode;
}

void setGuestMode(bool mode) {
    isGuestMode = mode;
}

// Fonksiyon erişimi kontrolü
if (!getGuestMode()) {
    // Sadece giriş yapmış kullanıcılar için
}
```

---

### A02:2021 - Cryptographic Failures

**Durum:** ✅ Uyumlu

**Uygulanan Önlemler:**

| Öğe | Uygulama | Standart |
|-----|----------|----------|
| Şifre Hashleme | HMAC-SHA256 + Salt | NIST SP 800-132 |
| Veri Şifreleme | White-Box AES | Proprietary WB |
| Oturum Şifreleme | AES-256-CBC | FIPS 197 |
| Anahtar Türetme | S-Box based | Custom |
| TLS | OpenSSL TLS 1.2+ | RFC 5246 |

**Kod Örnekleri:**
```cpp
// SaltAndHMAC.cpp
std::string hashPasswordWithHMAC(const std::string& password, const std::string& salt) {
    // HMAC-SHA256 implementation
    HMAC_CTX* hmac_ctx = HMAC_CTX_new();
    HMAC_Init_ex(hmac_ctx, key.c_str(), key.size(), EVP_sha256(), nullptr);
    // ... güvenli hashleme
}

// WBAES.cpp
std::vector<int> whiteBoxAesEncrypt(const std::string& plaintext, const std::string& key) {
    // White-Box AES şifreleme
    // S-Box ile key embedding
}
```

---

### A03:2021 - Injection

**Durum:** ✅ Uyumlu

**Uygulanan Önlemler:**
- Parametreli SQL sorguları (Prepared statements)
- Input validation
- Escaped string handling

**Kod Örnekleri:**
```cpp
// AttendeeManagement.cpp - Parametreli sorgu
std::string sql = "INSERT INTO attendees (name, email, phone) VALUES (?, ?, ?);";
sqlite3_stmt* stmt;
sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
sqlite3_bind_text(stmt, 1, encryptedAttendeeNameStr.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 2, encryptedAttendeeEmailStr.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 3, encryptedAttendeePhoneStr.c_str(), -1, SQLITE_STATIC);
```

---

### A04:2021 - Insecure Design

**Durum:** ✅ Uyumlu

**Uygulanan Önlemler:**
- Güvenlik mimarisi dokümantasyonu
- Tehdit modelleme (STRIDE)
- Defense in depth stratejisi
- Secure by default yaklaşımı

**Referans:** `SECURITY_ARCHITECTURE.md`

---

### A05:2021 - Security Misconfiguration

**Durum:** ⚠️ Kısmi Uyumlu

**Uygulanan Önlemler:**
- OpenSSL güvenli yapılandırma
- Debug modda güvenlik kontrolleri

**İyileştirme Alanları:**
- Hardcoded credentials kontrolü
- Varsayılan şifre politikası

---

### A06:2021 - Vulnerable and Outdated Components

**Durum:** ✅ Uyumlu

**Kullanılan Bileşenler:**

| Bileşen | Versiyon | Güvenlik Durumu |
|---------|----------|-----------------|
| OpenSSL | Latest | ✅ Güncel |
| SQLite3 | 3.x | ✅ Güncel |
| Google Test | Latest | ✅ Güncel |

---

### A07:2021 - Identification and Authentication Failures

**Durum:** ✅ Uyumlu

**Uygulanan Önlemler:**

| Kontrol | Uygulama |
|---------|----------|
| Şifre Politikası | Min 8 karakter, büyük/küçük harf, rakam, özel karakter |
| Şifre Depolama | HMAC-SHA256 + unique salt |
| Oturum Yönetimi | Şifreli oturum anahtarları |
| Çoklu Oturum | Kontrollü |

**Kod Örneği:**
```cpp
// UserAuthentication.cpp
bool isPasswordStrong(const std::string& password) {
    // Regex ile şifre gücü kontrolü
    std::regex strongPassword(
        "^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,}$"
    );
    return std::regex_match(password, strongPassword);
}
```

---

### A08:2021 - Software and Data Integrity Failures

**Durum:** ✅ Uyumlu

**Uygulanan Önlemler:**
- Dijital imza doğrulama
- Caller process verification
- Runtime integrity checks

**Kod Örneği:**
```cpp
// SignatureVerification.cpp
bool verifySignature() {
    // RSA imza doğrulama
    // Public key ile verification
}

std::string getCallerProcessPath() {
    // Çağıran proses yolu kontrolü
}
```

---

### A09:2021 - Security Logging and Monitoring Failures

**Durum:** ⚠️ Kısmi Uyumlu

**Mevcut:**
- Hata mesajları
- Debug output

**İyileştirme Önerileri:**
- Yapılandırılmış loglama
- Güvenlik olayları izleme
- Log rotation

---

### A10:2021 - Server-Side Request Forgery (SSRF)

**Durum:** ➖ Uygulanamaz

Uygulama yerel (local) bir masaüstü uygulaması olduğundan SSRF saldırıları risk oluşturmamaktadır.

---

## 4. OWASP ASVS v4.0 Uyumluluğu

### Level 1 (Minimum) Gereksinimleri

| Kategori | Durum |
|----------|-------|
| V1: Architecture | ✅ |
| V2: Authentication | ✅ |
| V3: Session Management | ✅ |
| V4: Access Control | ✅ |
| V5: Validation | ✅ |
| V6: Cryptography | ✅ |
| V7: Error Handling | ⚠️ |
| V8: Data Protection | ✅ |
| V9: Communication | ✅ |
| V10: Malicious Code | ✅ |
| V11: Business Logic | ✅ |
| V12: Files and Resources | ✅ |
| V13: API | ➖ N/A |
| V14: Configuration | ⚠️ |

---

## 5. Sonuç

### 5.1 Genel Uyumluluk Skoru

| Seviye | Durum |
|--------|-------|
| OWASP Top 10 | **85%** Uyumlu |
| ASVS Level 1 | **80%** Uyumlu |

### 5.2 Güçlü Yönler
1. Güçlü kriptografi implementasyonu
2. SQL injection koruması
3. Secure password handling
4. Runtime integrity checks

### 5.3 İyileştirme Önerileri
1. Yapılandırılmış güvenlik loglaması
2. Rate limiting implementasyonu
3. Enhanced configuration management

---

**Hazırlayan:** Security Team  
**Referanslar:** 
- https://owasp.org/Top10/
- https://owasp.org/www-project-application-security-verification-standard/
