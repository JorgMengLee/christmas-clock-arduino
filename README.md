#  Christmas Clock — Arduino Nano Restoration

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

##  The Problem

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

##  The Solution

| Problem | Fix |
|---------|-----|
| Dead clock mechanism | Replaced with new quartz movement (AA battery, fully independent) |
| Burned motor solder | Cleaned pad, re-tinned wires, re-soldered |
| Dead sound module | Replaced with active 5V buzzer |
| No controller | Arduino Nano as the brain |
| Motor killing Arduino | L293D driver + 1N4007 diode + capacitors for protection |

---

##  Features

- Rotating base spins continuously
- Every 3 minutes plays **"We Wish You a Merry Christmas"**
- DC motor syncs to the melody rhythm (starts/stops with each note)
- LED stays on during normal operation
- Clock hands run completely independent on their own AA battery

---

## Components

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

## Circuit Diagram

<img width="1158" height="649" alt="trloj navidad" src="https://github.com/user-attachments/assets/ab73bb1e-7df1-4694-bdfa-21afe2a1a938" />

---

##  Photos
<img width="1200" height="1600" alt="44fdeb18-0eed-41c4-9b6e-53e59f34c12e" src="https://github.com/user-attachments/assets/c808a9c7-bfcb-4981-bbe1-cc200e4e2af9" />
<img width="1600" height="1200" alt="2dce2429-2696-46ff-a61e-1bc6c6d30448" src="https://github.com/user-attachments/assets/9f857c31-5dc3-4de1-894d-cee3f6c3f721" />
<img width="1600" height="1200" alt="37e4ddae-b38e-411b-9915-d9b02938667a" src="https://github.com/user-attachments/assets/fef91371-9ac6-4a4b-9f7b-4206a90242df" />




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

##  Important Notes

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

##  Tools Used

- Arduino IDE 2.x
- Tinkercad (circuit diagram)
- Soldering iron + solder + flux
- Multimeter
- Hot glue gun (motor to platform)
