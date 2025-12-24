# Güvenlik Mimarisi Belgesi
## Local Event Planner - Security Architecture Document

**Versiyon:** 1.0  
**Tarih:** 2024-12-24  
**Proje:** Local Event Planner (CEN429 Güvenli Programlama)

---

## 1. Genel Bakış

Bu belge, Local Event Planner uygulamasının güvenlik mimarisini detaylı olarak açıklamaktadır.

### 1.1 Sistem Bileşenleri

```
┌─────────────────────────────────────────────────────────────────┐
│                    LOCAL EVENT PLANNER                          │
├─────────────────────────────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────────┐  │
│  │   User      │  │   Event     │  │   Attendee              │  │
│  │   Auth      │  │   Details   │  │   Management            │  │
│  └──────┬──────┘  └──────┬──────┘  └───────────┬─────────────┘  │
│         │                │                      │                │
│  ┌──────┴──────┐  ┌──────┴──────┐  ┌───────────┴─────────────┐  │
│  │  Schedule   │  │  Feedback   │  │   Security Layer        │  │
│  │  Organizer  │  │  Collection │  │   (RASP/Crypto)         │  │
│  └──────┬──────┘  └──────┬──────┘  └───────────┬─────────────┘  │
│         │                │                      │                │
│  ┌──────┴────────────────┴──────────────────────┴─────────────┐ │
│  │                    SQLite Database Layer                    │ │
│  │              (Encrypted Data Storage)                       │ │
│  └─────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

---

## 2. Kriptografi Mimarisi

### 2.1 White-Box AES (WBAES)

**Dosya:** `src/LocalEventPlanner/src/WBAES.cpp`

**Amaç:** Anahtar gizleme ve tersine mühendisliğe karşı koruma

```
┌─────────────────────────────────────────────────────────────┐
│                    WHITE-BOX AES                            │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   Plaintext ──► S-Box Transform ──► Key Derivation ──►     │
│                       │                    │                │
│                       ▼                    ▼                │
│               ┌───────────────┐    ┌──────────────┐        │
│               │ 256-byte      │    │ Seed-based   │        │
│               │ S-Box Table   │    │ Key Gen      │        │
│               └───────────────┘    └──────────────┘        │
│                       │                    │                │
│                       └────────┬───────────┘                │
│                                ▼                            │
│                         Ciphertext                          │
└─────────────────────────────────────────────────────────────┘
```

**Fonksiyonlar:**
- `createWhiteBoxTable()` - S-Box tablosu oluşturma
- `whiteBoxAesEncrypt()` - Şifreleme
- `whiteBoxAesDecrypt()` - Şifre çözme
- `deriveKeyFromSBox()` - Anahtar türetme

### 2.2 HMAC-SHA256 ile Şifre Hashleme

**Dosya:** `src/LocalEventPlanner/src/SaltAndHMAC.cpp`

```
┌─────────────────────────────────────────────────────────────┐
│                  PASSWORD HASHING FLOW                       │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   Password ──────┐                                          │
│                  │                                          │
│   Salt ──────────┼──► HMAC-SHA256 ──► Hashed Password      │
│                  │         │                                │
│   Secret Key ────┘         │                                │
│                            ▼                                │
│                    ┌──────────────┐                         │
│                    │ 64-char hex  │                         │
│                    │ output       │                         │
│                    └──────────────┘                         │
└─────────────────────────────────────────────────────────────┘
```

**Fonksiyonlar:**
- `generateFixedSalt()` - Salt oluşturma
- `hashPasswordWithHMAC()` - HMAC ile hashleme

### 2.3 Oturum Şifreleme (Session Encryption)

**Dosya:** `src/LocalEventPlanner/src/SessionEncryption.cpp`

- AES-256-CBC ile oturum verisi şifreleme
- RSA ile anahtar şifreleme
- Base64 encoding/decoding

---

## 3. Kimlik Doğrulama Mimarisi

### 3.1 Kullanıcı Kimlik Doğrulama Akışı

```
┌─────────────────────────────────────────────────────────────┐
│                 AUTHENTICATION FLOW                          │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌──────────┐    ┌──────────────┐    ┌──────────────────┐  │
│  │  User    │───►│  Password    │───►│  HMAC-SHA256     │  │
│  │  Input   │    │  Validation  │    │  Verification    │  │
│  └──────────┘    └──────────────┘    └────────┬─────────┘  │
│                                               │             │
│       ┌───────────────────────────────────────┘             │
│       │                                                     │
│       ▼                                                     │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐  │
│  │   SQLite     │───►│   Session    │───►│   Access     │  │
│  │   Lookup     │    │   Creation   │    │   Granted    │  │
│  └──────────────┘    └──────────────┘    └──────────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### 3.2 Şifre Güvenliği Gereksinimleri

- Minimum 8 karakter
- En az 1 büyük harf
- En az 1 küçük harf
- En az 1 rakam
- En az 1 özel karakter

**Fonksiyon:** `isPasswordStrong()`

---

## 4. Veri Koruma Katmanları

### 4.1 Veritabanı Güvenliği

| Veritabanı | Şifreleme | Açıklama |
|------------|-----------|----------|
| `users.db` | HMAC-SHA256 | Şifre hash'leri |
| `events.db` | WBAES | Etkinlik bilgileri |
| `attendees.db` | WBAES | Katılımcı verileri |
| `schedule.db` | WBAES | Program verileri |
| `feedbacks.db` | WBAES | Geri bildirimler |

### 4.2 Bellek Güvenliği

**Fonksiyon:** `secureErase()`

```cpp
// Hassas verilerin bellekten güvenli silinmesi
void secureErase(std::string& str) {
    volatile char* p = &str[0];
    for (size_t i = 0; i < str.size(); ++i) {
        p[i] = 0;
    }
    str.clear();
}
```

---

## 5. İletişim Güvenliği

### 5.1 SSL/TLS Implementasyonu

**Dosya:** `src/LocalEventPlanner/src/Rasp.cpp`

```
┌─────────────────────────────────────────────────────────────┐
│                    SSL/TLS STACK                             │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   ┌─────────────────────────────────────────────────────┐   │
│   │              Application Layer                      │   │
│   └───────────────────────┬─────────────────────────────┘   │
│                           │                                 │
│   ┌───────────────────────┴─────────────────────────────┐   │
│   │           OpenSSL (TLS 1.2/1.3)                     │   │
│   │   - SSL_CTX_new(TLS_client_method())                │   │
│   │   - Certificate verification                         │   │
│   │   - Private key authentication                       │   │
│   └───────────────────────┬─────────────────────────────┘   │
│                           │                                 │
│   ┌───────────────────────┴─────────────────────────────┐   │
│   │              Transport Layer                         │   │
│   └─────────────────────────────────────────────────────┘   │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### 5.2 Sertifika Dosyaları

| Dosya | Amaç |
|-------|------|
| `ca_cert.pem` | CA sertifikası |
| `certificate.crt` | Sunucu sertifikası |
| `private.key` | Özel anahtar |
| `public_key.pem` | Genel anahtar |

---

## 6. Runtime Koruma (RASP)

### 6.1 Anti-Debug Mekanizmaları

**Dosya:** `src/LocalEventPlanner/src/DebugCheck.cpp`

| Platform | Yöntem | Fonksiyon |
|----------|--------|-----------|
| Windows | IsDebuggerPresent() | `checkDebugger()` |
| Linux | ptrace() | `checkDebugger()` |
| Tüm | Zamanlama kontrolü | Execution time analysis |

### 6.2 İmza Doğrulama

**Dosya:** `src/LocalEventPlanner/src/SignatureVerification.cpp`

- `verifySignature()` - Dijital imza doğrulama
- `getCallerProcessPath()` - Çağıran proses kontrolü

### 6.3 Cihaz ve Versiyon Bağlama

**Dosya:** `src/LocalEventPlanner/src/VersionAndDeviceBinding.cpp`

- `isVersionCompatible()` - Versiyon kontrolü
- `isDeviceCompatible()` - Cihaz uyumluluk
- `getDeviceBrand()` - Cihaz tanımlama

---

## 7. Kod Sertleştirme Teknikleri

### 7.1 Obfuscation Teknikleri

```cpp
// Opaque predicates
bool isPimeeasds(int value) { ... }

// Dummy functions
void afffasdssadsdaqssawsdfssfd() { ... }

// Random delay loops
for (int outer = 0; outer < outerLoopCount; ++outer) {
    for (int inner = 0; inner < innerLoopCount; ++inner) {
        dummyVariable += (outer * inner) % 17;
    }
}
```

### 7.2 Volatile Değişkenler

Optimizasyon engellemek için volatile kullanımı:
```cpp
volatile int dummyVariable = 0;
```

---

## 8. Güvenlik Kontrol Listesi

| Kontrol | Durum | Dosya |
|---------|-------|-------|
| Şifre hashleme | ✅ | SaltAndHMAC.cpp |
| Veri şifreleme | ✅ | WBAES.cpp |
| SSL/TLS | ✅ | Rasp.cpp |
| Anti-debug | ✅ | DebugCheck.cpp |
| Bellek temizleme | ✅ | UserAuthentication.cpp |
| İmza doğrulama | ✅ | SignatureVerification.cpp |
| Cihaz bağlama | ✅ | VersionAndDeviceBinding.cpp |

---

## 9. Referanslar

- OpenSSL Documentation
- OWASP Application Security Verification Standard (ASVS)
- NIST Cryptographic Standards
- ETSI TS 103 097 (Security)
