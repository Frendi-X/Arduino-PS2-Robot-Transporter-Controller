# 🤖 Arduino PS2 Robot Controller

Sistem kendali robot berbasis **Arduino dengan joystick PlayStation 2 (PS2)** untuk mengontrol pergerakan motor DC, mekanisme servo buka-tutup, serta motor tambahan yang dilengkapi limit switch sebagai pembatas gerakan.

Proyek ini dikembangkan untuk mendukung sistem robotika berbasis mikrokontroler yang membutuhkan kendali manual, pengaturan kecepatan motor, dan kontrol aktuator.

## ✨ Fitur Utama

- 🎮 **PS2 Controller:** Kendali robot menggunakan joystick PS2.
- 🚗 **Motor DC:** Kontrol gerakan maju, mundur, belok kanan, dan belok kiri.
- ⚡ **Pengaturan Kecepatan:** Pengaturan PWM motor dari 0–255.
- 🦾 **Servo Open-Close:** Mengatur mekanisme buka dan tutup pada rentang 0°–130°.
- ⬆️ **Servo Up-Down:** Mengatur mekanisme naik dan turun pada rentang 20°–90°.
- 🛑 **Limit Switch:** Membatasi gerakan motor tambahan melalui input A2 dan A3.
- 🖥️ **Serial Monitor:** Menampilkan status tombol, gerakan, dan limit switch secara langsung.

## 🧰 Komponen yang Digunakan

| No. | Komponen | Fungsi |
|---:|---|---|
| 1 | Arduino | Pengendali utama sistem |
| 2 | PS2 Controller | Input kendali robot |
| 3 | PS2 Receiver | Komunikasi joystick dengan Arduino |
| 4 | Driver motor DC | Mengendalikan arah dan kecepatan motor |
| 5 | Motor DC | Penggerak roda robot |
| 6 | Motor tambahan / aktuator | Menggerakkan mekanisme tambahan |
| 7 | Servo motor | Menggerakkan mekanisme buka-tutup dan naik-turun |
| 8 | Limit switch | Membatasi gerakan aktuator |
| 9 | Power supply / baterai | Sumber daya sistem |
| 10 | Kabel jumper | Penghubung komponen |

## 🔌 Konfigurasi Pin Arduino

### 1. PS2 Controller

| Pin PS2 | Pin Arduino |
|---|---|
| DATA | D12 |
| CMD | D11 |
| ATT / CS | D10 |
| CLK | D13 |

### 2. Driver Motor DC

| Pin Arduino | Fungsi |
|---|---|
| D6 | ENA / PWM Motor A |
| D7 | IN1 Motor A |
| D4 | IN2 Motor A |
| D5 | ENB / PWM Motor B |
| D3 | IN3 Motor B |
| D2 | IN4 Motor B |

### 3. Servo dan Aktuator Tambahan

| Pin Arduino | Fungsi |
|---|---|
| D8 | Output Shoot |
| D8 | Servo Up-Down |
| A4 | Servo Open-Close |
| A0 | Output motor tambahan 1 |
| A1 | Output motor tambahan 2 |
| A2 | Limit switch A0 |
| A3 | Limit switch A1 |

> **Penting:** Pada program saat ini, pin D8 digunakan bersama untuk output `Shoot` dan servo `Up-Down`. Gunakan pin terpisah sebelum menghubungkan kedua perangkat secara bersamaan. Pastikan juga konfigurasi A0 dan A1 sesuai dengan driver atau modul aktuator yang digunakan.

## 🎮 Pemetaan Tombol Joystick

| Tombol PS2 | Fungsi | Keterangan |
|---|---|---|
| D-Pad Up | Maju lambat | PWM 80 |
| D-Pad Down | Mundur | PWM 255 |
| D-Pad Left | Belok kiri | Pengaturan arah kedua motor |
| D-Pad Right | Belok kanan | Pengaturan arah kedua motor |
| CROSS (X) | Maju cepat | PWM 255 |
| R1 | Aktuator A0 | Dibatasi limit switch A2 |
| R2 | Aktuator A1 | Dibatasi limit switch A3 |
| L1 | Servo Open | Menuju 0° |
| L2 | Servo Close | Menuju 130° |

**Catatan:** Tombol L1 dan L2 mengendalikan servo `Open-Close` dalam fungsi `loop()`. Fungsi `Servo_Up()` dan `Servo_Down()` tersedia pada kode, tetapi belum terhubung sebagai kontrol tombol aktif dalam `loop()`.

## ⚙️ Pengaturan Kecepatan Motor

Kecepatan motor diatur menggunakan sinyal PWM melalui pin ENA dan ENB.

| Parameter | Nilai PWM |
|---|---:|
| Maju lambat | 80 |
| Maju cepat | 255 |
| Mundur | 255 |
| Belok kanan | 200 |
| Belok kiri | 200 |

Rentang PWM Arduino umumnya adalah 0–255:

- `0` — duty cycle PWM 0%.
- `80` — duty cycle PWM sekitar 31%.
- `200` — duty cycle PWM sekitar 78%.
- `255` — duty cycle PWM 100%.

Nilai PWM tidak selalu berbanding lurus dengan kecepatan putaran motor karena dipengaruhi beban, tegangan, dan karakteristik motor.

## 🛑 Sistem Limit Switch

Sistem menggunakan dua limit switch dengan konfigurasi `INPUT_PULLUP`.

| Input | Pin | Kondisi Aktif |
|---|---|---|
| Limit A0 | A2 | LOW |
| Limit A1 | A3 | LOW |

Cara kerja:

1. Arduino membaca status kedua limit switch.
2. Ketika R1 ditekan, output A0 dikendalikan berdasarkan status limit switch A2.
3. Ketika R2 ditekan, output A1 dikendalikan berdasarkan status limit switch A3.
4. Jika limit switch terkait aktif, output penggerak ditetapkan HIGH sesuai logika program.

> Sistem penghentian ini bergantung pada karakteristik rangkaian penggerak. Untuk perlindungan mekanis yang andal, sesuaikan logika output dengan driver yang digunakan dan pertimbangkan pemutusan perangkat keras apabila diperlukan.

## 📚 Library yang Dibutuhkan

Instal library berikut melalui Arduino IDE:

- **PS2X_lib** — untuk membaca tombol dan berkomunikasi dengan joystick PS2.
- **Servo** — untuk mengontrol posisi servo.

Library `Servo` umumnya sudah tersedia melalui instalasi Arduino IDE.

### Instalasi

1. Unduh atau clone repositori ini.
2. Buka file `.ino` menggunakan Arduino IDE.
3. Pastikan library PS2X dan Servo sudah terpasang.
4. Sesuaikan pin dan koneksi perangkat keras.
5. Pilih board Arduino dan port yang benar.
6. Upload program ke Arduino.
7. Buka Serial Monitor pada baud rate `57600`.
8. Uji setiap tombol joystick sebelum menjalankan robot.

## 🖥️ Serial Monitor

Program menyediakan informasi status untuk membantu proses pengujian dan troubleshooting.

Contoh pesan yang ditampilkan:

```text
Found Controller, configured successful
R1 DITEKAN | MOTOR A0 AKTIF
LIMIT A0 AKTIF - MOTOR BERHENTI
R2 DITEKAN | MOTOR A1 AKTIF
L1 DITEKAN | SERVO OPEN
L2 DITEKAN | SERVO CLOSE
Maju
Mundur
Belok Kanan
Belok Kiri
```

Pesan aktual bergantung pada tombol yang ditekan dan kondisi perangkat.

## 🔋 Catatan Instalasi dan Keselamatan

- Gunakan catu daya motor sesuai spesifikasi motor dan driver.
- Jangan mengambil daya motor langsung dari pin 5V Arduino.
- Pastikan GND Arduino, driver motor, dan rangkaian kontrol memiliki referensi bersama apabila diperlukan.
- Periksa konflik pin sebelum menghubungkan semua perangkat.
- Uji fungsi limit switch tanpa beban terlebih dahulu.
- Pastikan servo tidak dipaksa bergerak melewati batas mekanisnya.
- Hindari mengubah kabel ketika sistem masih bertegangan.

## 🛠️ Pengembangan Selanjutnya

Beberapa pengembangan yang dapat dilakukan:

- [ ] Memisahkan pin servo dan output Shoot.
- [ ] Mengoptimalkan pembacaan joystick agar lebih responsif.
- [ ] Mengatur gerakan servo secara non-blocking menggunakan `millis()`.
- [ ] Memperbaiki algoritma belok dan pengaturan PWM.
- [ ] Menambahkan indikator status koneksi joystick.
- [ ] Meningkatkan sistem pengamanan limit switch.
- [ ] Menambahkan dokumentasi wiring dan diagram rangkaian.

## 👨‍💻 Teknologi

- Arduino / Embedded Systems
- C++ (Arduino)
- PS2 Controller Interface
- DC Motor Control
- PWM Speed Control
- Servo Motor Control
- Limit Switch

## 📧 Contact Us

* 📷 Instagram : https://www.instagram.com/frendi.co/
* 💬 WhatsApp : https://wa.me/+6287888227410
* 📧 Email : [frendirobotech@gmail.com](mailto:frendirobotech@gmail.com) atau [frisnova.solutions@gmail.com](mailto:frisnova.solutions@gmail.com)
* 📧 Email Alternatif : [frendix45@gmail.com](mailto:frendix45@gmail.com)

---

## 👨‍💻 Author

**Dikembangkan oleh:** Imam Sa'id Nurfrendi

**Komunitas:** FRISNOVA Innovative Solutions OR Robotech Electronics

**Lisensi:** MIT License
