#include <PS2X_lib.h>
#include <Servo.h>

Servo Servo_UpDown;
Servo Servo_OpenClose;

#define PS2_DATA      12
#define PS2_CMD       11
#define PS2_ATT       10
#define PS2_CLK       13
#define enA           6
#define in1           7
#define in2           4
#define enB           5
#define in3           3
#define in4           2
#define Shoot         8

#define pressures   true
#define rumble      true

PS2X ps2x;

int error = 0,
    motorSpeedA = 0,
    motorSpeedB = 0;

byte type = 0,
     vibrate = 0,
     KecepatanMaju = 255,
     KecepatanMaju1 = 80,
     KecepatanMundur = 255,
     KecepatanBelokKanan = 200,
     KecepatanBelokKiri = 200,
     kecepatan_servo = 0;


unsigned long hitungan_milis;
unsigned long milis_sekarang;
const unsigned long nilai = 50;

#define pin_Servo_UpDown 8
#define pin_Servo_OpenClose A4

int posisiOpenClose = 90;  // Posisi awal servo
unsigned long waktuServo = 0;
const unsigned long intervalServo = 1; // Kecepatan gerak

#define LIMIT_A0 A2
#define LIMIT_A1 A3

#define Open 0
#define Close 130

#define Up 90  // buka
#define Down 20  // tutup

int x, y;
int loop_up = 0,
    loop_down = 0,
    loop_open = 0,
    loop_close = 0;

void Servo_Open() {
  if (posisiOpenClose > Open) {
    posisiOpenClose--;
    Servo_OpenClose.write(posisiOpenClose);
    Serial.println(posisiOpenClose);
  }
}

void Servo_Close() {
  if (posisiOpenClose < Close) {
    posisiOpenClose++;
    Servo_OpenClose.write(posisiOpenClose);
    Serial.println(posisiOpenClose);
  }
}

void Servo_Up() {
  while (loop_up <= 1) {
    for (y = Down; y <= Up; y++) {
      Servo_UpDown.write(y);
      delay(kecepatan_servo);
      Serial.println(y);
    }
    loop_up = 2;
    loop_down = 1;
    Serial.println("L1 Pressed");
  }

}

void Servo_Down() {
  while (loop_down <= 1) {
    for (y = Up; y >= Down; y--) {
      Servo_UpDown.write(y);
      delay(1);
      Serial.println(y);
    }
    loop_up = 1;
    loop_down = 2;
  }
  Serial.println("L2 Pressed");
}

void setup() {

  Serial.begin(57600);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(Shoot, OUTPUT);

  pinMode(LIMIT_A0, INPUT_PULLUP);
  pinMode(LIMIT_A1, INPUT_PULLUP);

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  Servo_Up();
  Servo_UpDown.attach(pin_Servo_UpDown);
  Servo_OpenClose.attach(pin_Servo_OpenClose);


  delay(100);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DATA, pressures, rumble);

  if (error == 0) {
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //  Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
    case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
  }
}

void loop() {

  ps2x.read_gamepad(false, vibrate);
  vibrate = ps2x.Analog(PSAB_CROSS);

  // ===============================
  // CEK LIMIT SWITCH
  // ===============================
  bool limitA0Aktif = (digitalRead(LIMIT_A0) == LOW);
  bool limitA1Aktif = (digitalRead(LIMIT_A1) == LOW);

  // ===============================
  // KONTROL R1 DAN R2
  // MOTOR A0 DAN A1
  // ===============================
  if (ps2x.Button(PSB_R1) && !ps2x.Button(PSB_R2)) {
    digitalWrite(A0, limitA0Aktif ? HIGH : LOW);
    digitalWrite(A1, HIGH);

    Serial.println("R1 DITEKAN | MOTOR A0 AKTIF");
    if (limitA0Aktif) {
      Serial.println("LIMIT A0 AKTIF - MOTOR BERHENTI");
    }
  }
  else if (ps2x.Button(PSB_R2) && !ps2x.Button(PSB_R1)) {
    digitalWrite(A0, HIGH);
    digitalWrite(A1, limitA1Aktif ? HIGH : LOW);

    Serial.println("R2 DITEKAN | MOTOR A1 AKTIF");
    if (limitA1Aktif) {
      Serial.println("LIMIT A1 AKTIF - MOTOR BERHENTI");
    }
  }
  else {
    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
  }

  // ===============================
  // KONTROL L1 DAN L2
  // SERVO OPEN-CLOSE
  // ===============================

  if (millis() - waktuServo >= intervalServo) {
    waktuServo = millis();

    if (ps2x.Button(PSB_L1) && !ps2x.Button(PSB_L2)) {
      if (posisiOpenClose > Open) {
        posisiOpenClose--;
        Servo_OpenClose.write(posisiOpenClose);
      }

      Serial.println("L1 DITEKAN | SERVO OPEN");
    }
    else if (ps2x.Button(PSB_L2) && !ps2x.Button(PSB_L1)) {
      if (posisiOpenClose < Close) {
        posisiOpenClose++;
        Servo_OpenClose.write(posisiOpenClose);
      }

      Serial.println("L2 DITEKAN | SERVO CLOSE");
    }

    analogWrite(enA, motorSpeedA);
    analogWrite(enB, motorSpeedB);
  }

  ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
  vibrate = ps2x.Analog(PSAB_CROSS);

  if (ps2x.Button(PSB_PAD_DOWN)) {
    // Set Motor A
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    motorSpeedA = KecepatanMundur;
    motorSpeedB = KecepatanMundur;
    Serial.println("Mundur");
    /*
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    */
  }

  else if (ps2x.Button(PSB_PAD_RIGHT)) {
    // Set Motor A
    digitalWrite(in1, 1);
    digitalWrite(in2, 0);
    // Set Motor B
    digitalWrite(in3, 0);
    digitalWrite(in4, 1);
    for (int speedBelka = KecepatanBelokKanan; speedBelka <= 255; speedBelka++) {
      int xMapped = speedBelka;
      motorSpeedA = motorSpeedA + xMapped;
      motorSpeedB = KecepatanBelokKanan; //roda kanan

      if (motorSpeedA > 255) {
        motorSpeedA = KecepatanBelokKiri;//roda kiri
      }
      if (motorSpeedB < 0) {
        motorSpeedB = 0;
      }
      if (!ps2x.Button(PSB_PAD_RIGHT)) break;
    }
    Serial.println("Belok Kanan");
    /*
      Serial.print("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
    */
  }

  else if (ps2x.Button(PSB_PAD_LEFT)) {
    // Set Motor A
    digitalWrite(in1, 0);
    digitalWrite(in2, 1);
    // Set Motor B
    digitalWrite(in3, 1);
    digitalWrite(in4, 0);
    for (int speedBelki = KecepatanBelokKiri; speedBelki <= 255; speedBelki++) {
      int xMapped = speedBelki;
      motorSpeedA = KecepatanBelokKiri; //roda kiri
      motorSpeedB = motorSpeedB + xMapped;

      if (motorSpeedA < 0) {
        motorSpeedA = 0;
      }
      if (motorSpeedB > 255) {
        motorSpeedB = KecepatanBelokKanan; // roda kanan
      }
      if (!ps2x.Button(PSB_PAD_LEFT)) break;
    }
    Serial.println("Belok Kiri");
    /*
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
    */
  }

  else if (ps2x.Button(PSB_PAD_UP)) {
    // Set Motor A
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    motorSpeedA = KecepatanMaju1;
    motorSpeedB = KecepatanMaju1;
    Serial.println("Maju1");

    //  Serial.print("Up held this hard: ");
    //  Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);

  }
  else if (ps2x.Button(PSB_CROSS)) {
    // Set Motor A
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    motorSpeedA = KecepatanMaju;
    motorSpeedB = KecepatanMaju;
    Serial.println("Maju");
  }

  else {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    // Set Motor B
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
    motorSpeedA = 255;
    motorSpeedB = 255;
  }

  delay(10);
}
