# Varlık Yönetimi Belgesi
## Local Event Planner - Asset Management Document

**Versiyon:** 1.0  
**Tarih:** 2024-12-24

---

## 1. Varlık Envanteri

### 1.1 Statik Varlıklar

| Varlık ID | Adı | Türü | Konumu | Koruma Şeması |
|-----------|-----|------|--------|---------------|
| SA-001 | S-Box Tablosu | Kriptografik | WBAES.cpp | Kod içi gömülü |
| SA-002 | CA Sertifikası | Sertifika | keys/ca_cert.pem | Dosya sistemi |
| SA-003 | Sunucu Sertifikası | Sertifika | keys/certificate.crt | Dosya sistemi |
| SA-004 | Özel Anahtar | Anahtar | keys/private.key | Dosya sistemi (korumalı) |
| SA-005 | Genel Anahtar | Anahtar | keys/public_key.pem | Dosya sistemi |
| SA-006 | HMAC Secret | Gizli | SaltAndHMAC.cpp | Kod içi gömülü |

### 1.2 Dinamik Varlıklar

| Varlık ID | Adı | Türü | Oluşturulma | Koruma Şeması |
|-----------|-----|------|-------------|---------------|
| DA-001 | Oturum Anahtarı | Anahtar | Runtime | RSA şifreleme |
| DA-002 | Oturum IV | IV | Runtime | RSA şifreleme |
| DA-003 | Kullanıcı Salt | Salt | Kayıt | WBAES şifreleme |
| DA-004 | Cihaz Parmak İzi | Tanımlayıcı | Runtime | Şifreli |

### 1.3 Veritabanı Varlıkları

| Varlık ID | Veritabanı | Tablo | İçerik | Koruma |
|-----------|------------|-------|--------|--------|
| DB-001 | users.db | users | Kullanıcı bilgileri | HMAC hash |
| DB-002 | events.db | events | Etkinlik detayları | WBAES |
| DB-003 | attendees.db | attendees | Katılımcı bilgileri | WBAES |
| DB-004 | schedule.db | schedule | Program verileri | WBAES |
| DB-005 | feedbacks.db | feedbacks | Geri bildirimler | WBAES |

---

## 2. Varlık Özellikleri

### 2.1 SA-001: S-Box Tablosu

| Özellik | Değer |
|---------|-------|
| **Adı** | S-Box (Substitution Box) |
| **Açıklaması** | AES şifreleme için ikame tablosu |
| **Konumu** | src/LocalEventPlanner/src/WBAES.cpp |
| **Kaynağı** | Statik tanımlı |
| **Boyutu** | 256 byte |
| **Oluşturulma Zamanı** | Derleme zamanı |
| **Silinme Zamanı** | N/A (kalıcı) |
| **Varsayılan Değeri** | AES standart S-Box |
| **Koruma Şeması** | Gizlilik: White-Box gömme |

### 2.2 SA-004: Özel Anahtar

| Özellik | Değer |
|---------|-------|
| **Adı** | RSA Private Key |
| **Açıklaması** | SSL/TLS ve imza için özel anahtar |
| **Konumu** | src/keys/private.key |
| **Kaynağı** | OpenSSL ile oluşturulmuş |
| **Boyutu** | 1675 byte |
| **Oluşturulma Zamanı** | Kurulum |
| **Silinme Zamanı** | Manuel |
| **Koruma Şeması** | Gizlilik: Dosya izinleri, Bütünlük: Hash |

### 2.3 DA-001: Oturum Anahtarı

| Özellik | Değer |
|---------|-------|
| **Adı** | Session Key |
| **Açıklaması** | AES-256 oturum şifreleme anahtarı |
| **Konumu** | Bellek (RAM) |
| **Kaynağı** | RAND_bytes() ile oluşturulmuş |
| **Boyutu** | 32 byte (256 bit) |
| **Oluşturulma Zamanı** | Oturum başlangıcı |
| **Silinme Zamanı** | Oturum sonu (secureErase) |
| **Koruma Şeması** | Gizlilik: RSA şifreleme, Bütünlük: N/A |

---

## 3. Koruma Şemaları

### 3.1 Gizlilik (Confidentiality)

| Varlık | Yöntem | Açıklama |
|--------|--------|----------|
| Şifreler | HMAC-SHA256 | Tek yönlü hash |
| Kullanıcı verileri | WBAES | White-Box şifreleme |
| Oturum anahtarları | RSA | Asimetrik şifreleme |
| Özel anahtarlar | Dosya izinleri | OS seviyesi koruma |

### 3.2 Bütünlük (Integrity)

| Varlık | Yöntem | Açıklama |
|--------|--------|----------|
| Binary | Checksum | Çalışma zamanı doğrulama |
| Sertifikalar | Dijital imza | X.509 |
| Veritabanı | SQLite integrity | Dahili kontrol |

### 3.3 Kimlik Doğrulama (Authentication)

| Varlık | Yöntem | Açıklama |
|--------|--------|----------|
| Kullanıcılar | Şifre + HMAC | İki faktör benzeri |
| SSL bağlantısı | Sertifika | TLS mutual auth |
| Cihaz | Device fingerprint | Binding |

---

## 4. Erişim Kontrol Matrisi

| Varlık | Okuma | Yazma | Silme | Şifreleme |
|--------|-------|-------|-------|-----------|
| SA-001 (S-Box) | Uygulama | ❌ | ❌ | N/A |
| SA-004 (Private Key) | Uygulama | Admin | Admin | PEM |
| DA-001 (Session Key) | Uygulama | Uygulama | Uygulama | RSA |
| DB-001 (Users) | Uygulama | Uygulama | Admin | HMAC |
| DB-002-005 | Uygulama | Uygulama | Uygulama | WBAES |

---

## 5. Varlık Yaşam Döngüsü

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│   Oluştur   │────►│   Kullan    │────►│    Sil      │
└─────────────┘     └─────────────┘     └─────────────┘
      │                   │                   │
      ▼                   ▼                   ▼
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│  Şifrele    │     │  Eriş/Güncele│    │ SecureErase │
└─────────────┘     └─────────────┘     └─────────────┘
```

### 5.1 Oturum Anahtarı Yaşam Döngüsü

1. **Oluşturma:** `RAND_bytes()` ile rastgele üretim
2. **Şifreleme:** RSA public key ile şifreleme
3. **Depolama:** Şifreli olarak bellekte tutma
4. **Kullanım:** Oturum verisi şifreleme
5. **Silme:** `secureErase()` ile güvenli silme

---

## 6. Risk Değerlendirmesi

| Varlık | Tehdit | Olasılık | Etki | Risk | Mitigasyon |
|--------|--------|----------|------|------|------------|
| SA-004 | Key theft | Düşük | Kritik | Yüksek | Dosya izinleri |
| DA-001 | Memory dump | Düşük | Yüksek | Orta | Şifreleme, secureErase |
| DB-001 | SQLi | Çok Düşük | Yüksek | Düşük | Prepared statements |
| SA-001 | Reverse eng. | Orta | Orta | Orta | White-Box |

---

## 7. Sonuç

Tüm kritik varlıklar uygun koruma şemaları ile korunmaktadır. Dinamik varlıklar çalışma zamanında şifrelenmekte, statik varlıklar ise kod içine gömülü veya güvenli dosya sistemi koruması altındadır.

---

