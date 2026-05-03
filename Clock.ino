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
