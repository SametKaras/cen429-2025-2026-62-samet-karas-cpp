## LocalEventPlanner (C++ Konsol Uygulaması)

### Genel Bakış

Yerel etkinliklerinizi planlamanızı sağlayan, C++ ile geliştirilmiş modüler bir konsol uygulaması. Kullanıcı kimlik doğrulama, katılımcı yönetimi, etkinlik oluşturma/listeleme, zamanlama, geri bildirim toplama gibi işlevlerin yanında; oturum şifreleme, imza doğrulama, sürüm/cihaz bağlama ve anti-debug gibi güvenlik mekanizmaları içerir. Veriler SQLite üzerinde saklanır.

### Temel Özellikler

- Kullanıcı girişi / kayıt ve misafir modu
- Etkinlik oluşturma, yönetme ve listeleme
- Katılımcı kaydı ve takibi
- Zamanlama (takvim) düzenleyici
- Geri bildirim toplama
- Oturum şifreleme (AES/HMAC), dinamik varlık koruma
- Sürüm ve cihaz uyumluluğu kontrolü, imza doğrulama, anti-debug kontrolleri
- SQLite tabanlı kalıcı saklama

## Gereksinimler

- CMake >= 3.12
- C++ standardı >= 11
- OpenSSL (SSL/TLS, HMAC vb. için)
- SQLite3 (proje, gömülü `sqlite3.c` ile birlikte derlenir)
- GoogleTest (testler için)
- Windows: Visual Studio Community (MSVC) veya Ninja
- WSL/Linux: build-essential, Ninja (script kurar)

## Ortam Kurulumu

### Windows

1. `1-configure-git-hooks.bat`: Git hook'larını kopyalar, astyle ile kod formatlar.
2. `2-create-git-ignore.bat`: `.gitignore` oluşturur (gerekiyorsa).
3. `3-install-package-manager.bat`: Paket yöneticilerini kurar (choco, scoop).
4. `4-install-windows-enviroment.bat`: Gerekli araç ve kütüphaneleri kurar (CMake, Ninja, OpenSSL vb.).

Not: OpenSSL eksik ise CMake yapılandırması sırasında anahtar üretimi aşamasında hata alabilirsiniz.

### WSL/Linux

PowerShell'i yönetici olarak açın, WSL'ye geçin, proje klasörüne gidin ve:

```
./4-install-wsl-environment.sh
```

Gerekli paketleri kurar ve ortamı hazırlar.

## Hızlı Başlangıç

### Windows'ta Visual Studio projesi üretimi

```
./9-clean-configure-app-windows.bat
```

Visual Studio çözümünü oluşturur (MSVC ile Debug/Release yapılandırmaları).

### Windows'ta derle, test et, paketle

```
./7-build-app-windows.bat
```

- Temizleme, yapı klasörlerini hazırlama
- Doxygen dokümantasyonu ve dokümantasyon kapsam raporu
- Projenin Debug/Release derlenmesi
- Testlerin çalıştırılması ve kapsam raporlarının üretilmesi
- MkDocs ile web sayfası üretimi
- Çıktıların `release` klasörüne paketlenmesi

Yalnızca dokümantasyon: `7-build-doc-windows.bat`

Yalnızca testler: `8-build-test-windows.bat`

### WSL/Linux'ta derle, test et, paketle

```
./7-build-app-linux.sh
```

Windows ile benzer adımları çalıştırır, rapor ve kütüphaneleri `release` klasöründe toplar.

## Uygulamayı Çalıştırma

Derleme sonrası yürütülebilir dosya adı: `LocalEventPlannerapp`

- Windows: `build/Debug/LocalEventPlannerapp.exe` veya `build/Release/LocalEventPlannerapp.exe`
- Linux/WSL: `build/LocalEventPlannerapp`

Uygulama başlatıldığında ana menüden Giriş Yap, Kayıt Ol veya Misafir Modu seçenekleri ile ilerleyebilirsiniz. Misafir modunda bazı işlemler (ör. etkinlik/katılımcı oluşturma) kısıtlıdır.

## Proje Yapısı

- `src/localeventplanner`: Çekirdek kütüphane (güvenlik, şifreleme, iş kuralları, SQLite entegrasyonu)
- `src/localeventplannerapp`: Konsol uygulaması (ana akış ve menüler)
- `src/utility`: Ortak yardımcılar ve gömülü `sqlite3.c/.h`
- `src/tests`: GoogleTest birim testleri ve yardımcı script'ler
- `assets`: Rozetler, görseller
- `report_*` klasörleri: Test ve dokümantasyon kapsam geçmişleri
- `Doxyfile*`: Doxygen yapılandırmaları (Windows/Linux, kütüphane/test)
- `mkdocs.yml`: Web dokümantasyonu yapılandırması

## Anahtarlar ve Güvenlik

Kütüphane yapılandırma aşamasında OpenSSL ile RSA anahtarları üretilir ve derleme sırasında ilgili yollar derleyici tanımlarına geçilir. OpenSSL kurulu değilse yapılandırma başarısız olur.

Başlıca güvenlik kontrolleri:

- İmza doğrulama ve kod bütünlüğü kontrolleri
- Anti-debug ve hook tespiti
- Sürüm/cihaz uyumluluğu kontrolü
- Oturum şifreleme (AES) ve HMAC tabanlı doğrulama

## Temizlik

```
./9-clean-project.bat
```

Proje çıktılarını temizler.

## Desteklenen Platformlar

![Ubuntu badge](assets/badge-ubuntu.svg)

![macOS badge](assets/badge-macos.svg)

![Windows badge](assets/badge-windows.svg)

### Test Kapsamı Rozetleri

> Not: Doxygen'de aynı isimli görsellerin çıktıda çakışması ile ilgili bilinen bir hata vardır; README ve web sayfasındaki rozetler doğrudur. Ayrıntı: `https://github.com/doxygen/doxygen/issues/8362`.

| Coverage Type | Windows OS                                                             | Linux OS (WSL-Ubuntu 20.04)                                              |
| ------------- | ---------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| Line Based    | ![Line Coverage](assets/codecoveragelibwin/badge_linecoverage.svg)     | ![Line Coverage](assets/codecoverageliblinux/badge_linecoverage.svg)     |
| Branch Based  | ![Branch Coverage](assets/codecoveragelibwin/badge_branchcoverage.svg) | ![Branch Coverage](assets/codecoverageliblinux/badge_branchcoverage.svg) |
| Method Based  | ![Method Coverage](assets/codecoveragelibwin/badge_methodcoverage.svg) | ![Method Coverage](assets/codecoverageliblinux/badge_methodcoverage.svg)  |

### Dokümantasyon Kapsamı

|                    | Windows OS                                                        | Linux OS (WSL-Ubuntu 20.04)                                         |
| ------------------ | ----------------------------------------------------------------- | ------------------------------------------------------------------- |
| Coverage Ratio     | ![Line Coverage](assets/doccoveragelibwin/badge_linecoverage.svg) | ![Line Coverage](assets/doccoverageliblinux/badge_linecoverage.svg) |

## Test Sonuçlarını HTML'e Çevirme (Opsiyonel)

JUnit XML test sonuçlarını HTML rapora çevirmek için aşağıdaki aracı kullanabilirsiniz:

`https://github.com/inorton/junit2html`

```
pip install junit2html
```

> İpucu: Windows ve Linux script'leri kapsamlı bir şekilde yapı/test/dokümantasyon adımlarını otomatikleştirir. Eksik adım veya kurulum problemi yaşarsanız ilgili `.bat`/`.sh` script'lerini sırasıyla çalıştırmanız genellikle yeterlidir.

---

$End-Of-File$
