# 🎄 Christmas Clock — Arduino Nano Restoration

> A broken vintage Christmas decoration brought back to life with
> an Arduino Nano, featuring a rotating base, holiday melodies,
> and a decorative LED.

This project started as a repair job on an old Christmas ornament
that had a dead clock mechanism, a burned motor solder joint, and
a completely fried sound module. Instead of throwing it away, I
replaced all the broken electronics with an Arduino Nano and a
handful of cheap components — turning it into a fully automated
holiday decoration that plays "We Wish You a Merry Christmas"
every 3 minutes while spinning its base in sync with the melody.

---

## 🎯 The Problem

The original ornament had four issues:

- **Clock mechanism** — the hands would not move, the quartz
  motor was dead
- **Rotating base** — the DC motor had a burned solder joint
  and no power
- **Sound module** — the original PCB was burned and
  non-functional
- **No controller** — everything was hardwired with no way
  to reprogram behavior

---

## ✅ The Solution

| Problem | Fix |
|---------|-----|
| Dead clock mechanism | Replaced with new quartz movement (AA battery, fully independent) |
| Burned motor solder | Cleaned pad, re-tinned wires, re-soldered |
| Dead sound module | Replaced with active 5V buzzer |
| No controller | Arduino Nano as the brain |
| Motor killing Arduino | L293D driver + 1N4007 diode + capacitors for protection |

---

## 🎵 Features

- Rotating base spins continuously
- Every 3 minutes plays **"We Wish You a Merry Christmas"**
- DC motor syncs to the melody rhythm (starts/stops with each note)
- LED stays on during normal operation
- Clock hands run completely independent on their own AA battery

---

## 🔧 Components

| Component | Qty | Purpose |
|-----------|-----|---------|
| Arduino Nano | 1 | Main controller |
| L293D motor driver | 1 | Controls DC motor safely |
| DC Motor (RF-300) | 1 | Rotates the base platform |
| Active buzzer 5V | 1 | Plays melodies |
| LED | 1 | Decorative light |
| 220Ω resistor | 1 | LED current limiting |
| 1N4007 diode | 1 | Motor back-EMF protection |
| 100µF capacitor | 1 | Motor startup stabilization |
| 100nF capacitor | 1 | High-frequency noise filter |
| Breadboard | 1 | Circuit assembly |
| 5V DC adapter | 1 | Power supply |
| Quartz clock movement | 1 | Independent clock hands |

---

## 📐 Circuit Diagram

![Circuit Diagram](diagrama/tinkercad_diagram.png)

> Full interactive circuit available on Tinkercad: [link here]

---

## 📸 Photos

| Original interior | Motor | Rotating base |
|-------------------|-------|---------------|
| ![](fotos/interior.jpg) | ![](fotos/motor.jpg) | ![](fotos/base.jpg) |

| Burned solder joint | PCB board | Finished |
|---------------------|-----------|----------|
| ![](fotos/solder.jpg) | ![](fotos/pcb.jpg) | ![](fotos/finished.jpg) |

---

## 💻 Code

// =============================================
//   Reloj Navideño - Arduino Nano
//   We Wish You a Merry Christmas
// =============================================

const int MOT_A  = 3;
const int MOT_B  = 4;
const int LED    = 6;
const int BUZZER = 9;

// 180000 = 3 minutos | 10000 = 10 seg para probar
const unsigned long INTERVALO = 180000UL;

unsigned long ultimoSonido = 0;

// =============================================
//   Notas (G mayor — tiene F#)
// =============================================
#define D4   294
#define E4   330
#define FS4  370   // F# (fa sostenido)
#define G4   392
#define A4   440
#define B4   494
#define C5   523
#define D5   587
#define E5   659
#define SIL  0

// Duraciones base (ms) — tempo ~100 BPM
#define Q    300   // negra (quarter)
#define H    600   // blanca (half)
#define HD   900   // blanca con punto (dotted half)
#define E8   150   // corchea (eighth)

// =============================================
//   We Wish You a Merry Christmas
//   {nota, duración}
// =============================================
int NOTAS[][2] = {

  // "We wish you a mer-ry Christ-mas,"
  {D4,  Q},
  {G4,  Q}, {G4,  Q}, {A4,  Q},
  {G4,  H}, {FS4, Q},
  {E4,  HD},

  // "we wish you a mer-ry Christ-mas,"
  {E4,  Q}, {E4,  Q}, {E4,  Q},
  {A4,  Q}, {A4,  Q}, {B4,  Q},
  {A4,  H}, {G4,  Q},
  {FS4, H}, {D4,  Q},

  // "we wish you a mer-ry Christ-mas,"
  {D4,  Q},
  {B4,  Q}, {B4,  Q}, {C5,  Q},
  {B4,  H}, {A4,  Q},
  {G4,  Q}, {E4,  Q}, {D4,  Q}, {D4,  Q},

  // "and a hap-py new year."
  {E4,  Q}, {A4,  Q}, {FS4, Q},
  {G4,  HD},

  {SIL, Q},

  // "Good ti-dings we bring"
  {D4,  Q},
  {G4,  Q}, {G4,  Q}, {G4,  Q},
  {FS4, H}, {FS4, Q},

  // "to you and your kin,"
  {A4,  Q}, {A4,  Q}, {A4,  Q},
  {E4,  H}, {E4,  Q},

  // "good ti-dings for Christ-mas"
  {B4,  Q}, {B4,  Q}, {B4,  Q},
  {G4,  H}, {D5,  Q},
  {D5,  Q}, {C5,  Q}, {B4,  Q},
  {A4,  H}, {D4,  Q},

  // "and a hap-py new year!"
  {E4,  Q}, {A4,  Q}, {FS4, Q},
  {G4,  HD},

  {SIL, H}
};

const int LEN = sizeof(NOTAS) / sizeof(NOTAS[0]);

// =============================================
//   Setup
// =============================================
void setup() {
  pinMode(MOT_A,  OUTPUT);
  pinMode(MOT_B,  OUTPUT);
  pinMode(LED,    OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED,   HIGH);
  digitalWrite(MOT_A, HIGH);
  digitalWrite(MOT_B, LOW);

  // Toca al arrancar para probar
  delay(1500);
  tocarYMover();
}

// =============================================
//   Loop
// =============================================
void loop() {
  // Motor siempre girando
  digitalWrite(MOT_A, HIGH);
  digitalWrite(MOT_B, LOW);

  if (millis() - ultimoSonido >= INTERVALO) {
    ultimoSonido = millis();
    tocarYMover();
  }
}

// =============================================
//   Toca la canción con motor sincronizado
// =============================================
void tocarYMover() {
  for (int i = 0; i < LEN; i++) {
    int nota     = NOTAS[i][0];
    int duracion = NOTAS[i][1];

    if (nota == SIL) {
      noTone(BUZZER);
      digitalWrite(MOT_A, LOW);
      digitalWrite(MOT_B, LOW);
      delay(duracion);

    } else {
      tone(BUZZER, nota);
      digitalWrite(MOT_A, HIGH);
      digitalWrite(MOT_B, LOW);
      delay(duracion);

      // Pausa entre notas — da efecto de ritmo al motor
      noTone(BUZZER);
      digitalWrite(MOT_A, LOW);
      delay(40);
    }
  }

  // Motor y LED vuelven a estado normal
  noTone(BUZZER);
  digitalWrite(MOT_A, HIGH);
  digitalWrite(MOT_B, LOW);
  digitalWrite(LED,   HIGH);
}

### Pin mapping

| Arduino Pin | Component |
|-------------|-----------|
| D3 | L293D IN1 (motor) |
| D4 | L293D IN2 (motor) |
| D6 | LED |
| D9 | Buzzer |

### Adjusting the melody interval

```cpp
// 180000  = every 3 minutes  (default)
// 3600000 = every 1 hour
// 10000   = every 10 seconds (for testing)
const unsigned long INTERVALO = 180000UL;
```

### Key notes about the code

- `tone()` conflicts with Timer 2, which also controls pin D3
  — that is why the motor uses D3/D4 and the buzzer uses D9
  (Timer 1), avoiding interference
- The melody is written in G major, so F# (370 Hz) is used
  instead of plain F (349 Hz) — this is intentional
- Motor syncs to the melody: spins on each note,
  brief stop between notes for rhythm effect
- On startup the melody plays once after 1.5s
  so you can verify everything works immediately

---

## ⚠️ Important Notes

- The **100µF electrolytic capacitor** has polarity —
  the stripe marks the negative leg, always connect it to GND
- The **1N4007 diode** must be oriented with the
  cathode (silver stripe) toward the positive motor terminal
- The clock hands mechanism is **completely independent** —
  it runs on its own AA battery and has no connection
  to the Arduino whatsoever
- If the motor spins in the wrong direction,
  swap the two motor wires on OUT1/OUT2 of the L293D
- If the motor stalls under load, connect L293D pin 8 (VCC2)
  to 9V instead of 5V for more torque

---

## 🛠️ Tools Used

- Arduino IDE 2.x
- Tinkercad (circuit diagram)
- Soldering iron + solder + flux
- Multimeter
- Hot glue gun (motor to platform)

---

## 📁 Repository Structure
<img width="1158" height="649" alt="trloj navidad" src="https://github.com/user-attachments/assets/ab73bb1e-7df1-4694-bdfa-21afe2a1a938" />
