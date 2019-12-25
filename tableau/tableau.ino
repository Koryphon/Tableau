/*
 * Tableau d'activité
 */

#include <Bounce2.h>
#include <LightDimmer.h>
#include <Key.h>
#include <Keypad.h>
#include <RingBuf.h>
#include<avr/sleep.h>

//#define DEBUG

/*
 * LED d'activité
 */
const byte pinLedON = 22;
LightDimmerSoft ledON;
const word ledOnPeriode= 2000;
const word ledOnAllumee = 500;
const word ledOnMonte = 750;
const word ledOnDescend = 750;

/*
 * LED de mode numérique
 */
const byte pinLedModeNumerique = 24;
LightDimmerSoft ledModeNumerique;

/*
 * LED de mode analogique
 */
const byte pinLedModeAnalogique = 26;
LightDimmerSoft ledModeAnalogique;

/*
 * LED de mode son
 */
const byte pinLedModeSon = 28;
LightDimmerSoft ledModeSon;

/*
 *  Afficheur 7 segments
 */
const byte pinSegmentE = 12;
const byte pinSegmentD = 11;
const byte pinSegmentC = 10;
const byte pinDP = 9;
const byte pinSegmentB = 39;
const byte pinSegmentA = 41;
const byte pinSegmentF = 43;
const byte pinSegmentG = 45;

const byte segments[7] = {
  pinSegmentA,
  pinSegmentB,
  pinSegmentC,
  pinSegmentD,
  pinSegmentE,
  pinSegmentF,
  pinSegmentG  
};

const byte segState[10][7] = {
  /* 0 */ { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW },
  /* 1 */ {  LOW, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW },
  /* 2 */ { HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, HIGH },
  /* 3 */ { HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, HIGH },
  /* 4 */ {  LOW, HIGH, HIGH,  LOW,  LOW, HIGH, HIGH },
  /* 5 */ { HIGH,  LOW, HIGH, HIGH,  LOW, HIGH, HIGH },
  /* 6 */ { HIGH,  LOW, HIGH, HIGH, HIGH, HIGH, HIGH },
  /* 7 */ { HIGH, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW },
  /* 8 */ { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH },
  /* 9 */ { HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH },
};

/*
 * LEDs bargraph
 */
const byte pinBL[] = { A0, A1, 1, 2, 3, 4, 5, 6, 7 };

/*
 * Feu tricolore
 */
const byte pinVert = 40;
const byte pinJaune = 42;
const byte pinRouge = 44;

LightDimmerSoft feuVert;
LightDimmerSoft feuJaune;
LightDimmerSoft feuRouge;


/*
 * Fils et sockets
 */
const byte filVert = A6;
const byte filJaune = A4;
const byte filRouge = A2;
const byte socketVert = A3;
const byte socketJaune = A5;
const byte socketRouge = A7;

Bounce brVert;
Bounce brJaune;
Bounce brRouge;

/*
 * Bouton
 */
const byte pinBoutonMusique = A9;
Bounce boutonMusique;

/*
 * Potentiomètre
 */
const byte pinPotentiometre = A8;

/*
 * Clé
 */
const byte pinCle = A10;
Bounce cle;

/*
 * Clavier
 */
const byte rangees = 4;
const byte colonnes = 3;
char touches [rangees][colonnes] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
const byte pinsRangees[rangees] = { 23, 25, 27, 29 };
const byte pinsColonnes[colonnes] = { 31, 33, 35 };

Keypad clavier(makeKeymap(touches), pinsRangees, pinsColonnes, rangees, colonnes);

/*
 * Buzzer
 */
const byte pinBuzzer = 38;

/*
 * Son
 */
#include "pitches.h"

typedef struct {
  word frequence;
  word duree;
} Note;

const word DEMI_CROCHE = 250;
const word DEMI_CROCHE_POINTEE = 375;
const word CROCHE = 500;
const word CROCHE_POINTEE = 750;
const word NOIRE = 1000;
const word NOIRE_POINTEE = 1500;
const word BLANCHE = 2000;
const word BLANCHE_POINTEE = 3000;
const word RONDE = 4000;
const word RONDE_POINTEE = 6000;

#define HUITIEME_SOUPIR   0, 125
#define QUART_SOUPIR   0, DEMI_CROCHE
#define DEMI_SOUPIR   0, CROCHE
#define SOUPIR        0, NOIRE
#define DEMI_PAUSE    0, BLANCHE

Note totoro[16] = {
  { NOTE_D4, NOIRE },
  { NOTE_B3, CROCHE },
  { NOTE_G3, CROCHE },
  { DEMI_SOUPIR },
  { NOTE_D4, NOIRE },
  { NOTE_C4, NOIRE},
  { NOTE_A3, CROCHE},
  { DEMI_PAUSE },
  { NOTE_C4, NOIRE },
  { NOTE_A3, CROCHE},
  { NOTE_FS3, CROCHE},
  { DEMI_SOUPIR },
  { NOTE_C4, NOIRE },
  { NOTE_B3, NOIRE },
  { NOTE_G3, CROCHE },
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note ascendante[4] = {
  { NOTE_C3, DEMI_CROCHE },
  { NOTE_D3, DEMI_CROCHE },
  { NOTE_E3, DEMI_CROCHE },
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note descendante[4] = {
  { NOTE_E2, DEMI_CROCHE },
  { NOTE_D2, DEMI_CROCHE },
  { NOTE_C2, DEMI_CROCHE },
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note victory[15] = {
  { SOUPIR },
  { NOTE_A4, DEMI_CROCHE},
  { NOTE_A4, DEMI_CROCHE},
  { NOTE_A4, DEMI_CROCHE},
  { NOTE_A4, CROCHE},
  { QUART_SOUPIR },
  { NOTE_F4, CROCHE},
  { 0, 100 },
  { NOTE_G4, CROCHE_POINTEE},
  { 0, 100 },
  { NOTE_A4, DEMI_CROCHE},
  { 0, 180 },
  { NOTE_G4, DEMI_CROCHE},
  { NOTE_A4, NOIRE_POINTEE},
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note DO[2] = {
  { NOTE_C3, NOIRE },
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note RE[2] = {
  { NOTE_D3, NOIRE },
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note MI[2] = {
  { NOTE_E3, NOIRE },
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note FA[2] = {
  { NOTE_F3, NOIRE },
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note SOL[2] = {
  { NOTE_G3, NOIRE },
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note LA[2] = {
  { NOTE_A3, NOIRE },
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note SI[2] = {
  { NOTE_B3, NOIRE },
  { 0xFFFF, 0 } /* Fin de melodie */
};

Note *melodies[10] = {
  totoro,
  DO,
  RE,
  MI,
  FA,
  SOL,
  LA,
  SI,
  victory,
  NULL
};

RingBuf<Note *, 10> bufferMelodies;

/*
 * Modes de fonctionnement
 */
typedef enum {
  MODE_NUMERIQUE,
  MODE_ANALOGIQUE,
  MODE_SON,
  AUCUN_MODE
} Mode;

Mode modeDeFonctionnement = MODE_NUMERIQUE;

/*
 * Date de dernière manipulation pour la veille
 */
unsigned long dateActivite = 0;
const unsigned long delaiVeille = 60000;

/*
 *  Autorisation de jouer du son
 */
bool sonPermis = true;

/*
 * Affiche un chiffre sur l'afficheur 7 segments
 */
void affiche(const char chiffre)
{
  if (chiffre >= '0' && chiffre <= '9') {
    for (byte seg = 0; seg < 7; seg++) {
      digitalWrite(segments[seg], segState[chiffre - '0'][seg]);
    }
  }
}

/*
 * Efface l'afficheur 7 segments
 */
void efface(void)
{
  for (byte seg = 0; seg < 7; seg++) digitalWrite(segments[seg], LOW);
}

/*
 * bargraph
 */
void bargraph(const char chiffre)
{
  if (chiffre >= '0' && chiffre <= '9') {
    byte rank = chiffre - '0';
    for (byte ledBL = 0; ledBL < rank; ledBL++) digitalWrite(pinBL[ledBL], HIGH);
    for (byte ledBL = rank; ledBL < 9; ledBL++) digitalWrite(pinBL[ledBL], LOW);  
  }
}

/*
 * Chenillard
 */
void chenillard()
{
  static byte pos = 0;
  static bool dir = true;
  static unsigned long date = 0;

  if (millis() - date > 100) {
    date = millis();
    for (byte i = 0; i < 9; i++) digitalWrite(pinBL[i], LOW);
    digitalWrite(pinBL[pos], HIGH);
    if (dir) {
      if (pos == 8) {
        dir = false; 
      }
    }
    else {
      if (pos == 0) {
        dir = true; 
      }
    }
    pos = dir ? pos + 1 : pos - 1;
  }
}

/*
 * 
 */
void joueMelodie()
{
  static int pos = 0;
  static Note *melodieCourante = NULL;
  static unsigned long date = 0;
  static word duree = 0;
  bool interNote = false;

  if (melodieCourante == NULL) {
    pos = 0;
    if (bufferMelodies.pop(melodieCourante));
  }
  
  if (millis() - date > duree) {
    /* temps écoulé, on joue la note suivante */
    if (melodieCourante != NULL) {
      if (interNote) {
        date = millis();
        duree = duree / 2;
        interNote = false;
      }
      else {
        /* une melodie est en cours */
        if (melodieCourante[pos].frequence != 0xFFFF) {
          /* On n'est pas au bout */
          duree = melodieCourante[pos].duree * 2 / 3;
          if (sonPermis) {
            tone(pinBuzzer, melodieCourante[pos].frequence, duree);
          }
          date = millis();
          duree = melodieCourante[pos].duree * 2 / 3;
          interNote = true;
          pos++;
        }
        else {
          /* melodie terminée */
          melodieCourante = NULL;
          pos = 0;
        }
      }
    }
  }
}

void setup()
{
  pinMode(pinSegmentE, OUTPUT);
  pinMode(pinSegmentD, OUTPUT);
  pinMode(pinSegmentC, OUTPUT);
  pinMode(pinDP, OUTPUT);
  pinMode(pinSegmentB, OUTPUT);
  pinMode(pinSegmentA, OUTPUT);
  pinMode(pinSegmentF, OUTPUT);
  pinMode(pinSegmentG, OUTPUT);

  pinMode(socketVert, OUTPUT);
  pinMode(socketJaune, OUTPUT);
  pinMode(socketRouge, OUTPUT);

  pinMode(filVert, INPUT_PULLUP);
  pinMode(filJaune, INPUT_PULLUP);
  pinMode(filRouge, INPUT_PULLUP);

  brVert.attach(filVert);
  brVert.interval(5);
  brJaune.attach(filJaune);
  brJaune.interval(5);
  brRouge.attach(filRouge);
  brRouge.interval(5);


  for (byte i = 0; i < 9; i++) {
    pinMode(pinBL[i], OUTPUT);
  }

  pinMode(pinBoutonMusique, INPUT_PULLUP);
  boutonMusique.attach(pinBoutonMusique);
  boutonMusique.interval(5);

  pinMode(pinCle, INPUT_PULLUP);
  cle.attach(pinCle);
  cle.interval(5);

  ledON.begin(pinLedON, HIGH);
  ledON.setPeriod(ledOnPeriode);
  ledON.setOnTime(ledOnAllumee);
  ledON.setBrighteningTime(ledOnMonte);
  ledON.setFadingTime(ledOnDescend);
  ledON.startBlink();

  ledModeNumerique.begin(pinLedModeNumerique, HIGH);
  ledModeAnalogique.begin(pinLedModeAnalogique, HIGH);
  ledModeSon.begin(pinLedModeSon, HIGH);

  feuVert.begin(pinVert, HIGH);
  feuJaune.begin(pinJaune, HIGH);
  feuRouge.begin(pinRouge, HIGH);
  
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void gereChangementDeMode(const char touche)
{
  static Mode modePrecedent = AUCUN_MODE;
  
  boutonMusique.update();
  if (boutonMusique.fell()) {
    modeDeFonctionnement = MODE_SON;
    dateActivite = millis();
  }

  if (touche == '*')      modeDeFonctionnement = MODE_NUMERIQUE;
  else if (touche == '#') modeDeFonctionnement = MODE_ANALOGIQUE;

  if (modeDeFonctionnement != modePrecedent) {
    switch (modeDeFonctionnement) {
      case MODE_NUMERIQUE:
        ledModeNumerique.on();
        ledModeAnalogique.off();
        ledModeSon.off();
        affiche('0');
        bargraph('0');
       break;
      case MODE_ANALOGIQUE:
        ledModeNumerique.off();
        ledModeAnalogique.on();
        ledModeSon.off();
        efface();
        bargraph('0');
        break;
      case MODE_SON:
        ledModeNumerique.off();
        ledModeAnalogique.off();
        ledModeSon.on();
        efface();
        bargraph('0');
        break;
    }
    dateActivite = millis();
  }

  modePrecedent = modeDeFonctionnement;
}

void gereModeNumerique(const char touche)
{
  affiche(touche);
  bargraph(touche);
}

void gereModeAnalogique(const char touche)
{
  static word anciennePositionPotentiometre = 0;
  word positionPotentiometre = analogRead(pinPotentiometre);

  if (abs(positionPotentiometre - anciennePositionPotentiometre) < 2) dateActivite = millis();
  
  char digit = map(positionPotentiometre, 0, 1023, '0', '9');

  affiche(digit);
  bargraph(digit);
}

void gereModeSon(const char touche)
{
  if (touche >= '0' && touche <= '9') {
    bufferMelodies.push(melodies[touche - '0']);
  }
  chenillard();
}

void gereMode(const char touche)
{
  switch (modeDeFonctionnement) {
    case MODE_NUMERIQUE:
      gereModeNumerique(touche);
      break;
    case MODE_ANALOGIQUE:
      gereModeAnalogique(touche);
      break;
    case MODE_SON:
      gereModeSon(touche);
      break;
  }
}

void gereBranchements(void)
{
  static bool vertOk = false;
  static bool jauneOk = false;
  static bool rougeOk = false;

  static bool toutOk = false;

  bool vertBranche = false;
  bool jauneBranche = false;
  bool rougeBranche = false;
  
  digitalWrite(socketVert, LOW);
  digitalWrite(socketJaune, HIGH);
  digitalWrite(socketRouge, HIGH);

  brVert.update();
  if (brVert.read() == LOW) vertBranche = true;

  digitalWrite(socketVert, HIGH);
  digitalWrite(socketJaune, LOW);
  digitalWrite(socketRouge, HIGH);

  brJaune.update();
  if (brJaune.read() == LOW) jauneBranche = true;
  
  digitalWrite(socketVert, HIGH);
  digitalWrite(socketJaune, HIGH);
  digitalWrite(socketRouge, LOW);

  brRouge.update();
  if (brRouge.read() == LOW) rougeBranche = true;

  bool toutBranche = vertBranche && jauneBranche && rougeBranche;
  
  if (!vertOk && vertBranche) {
    dateActivite = millis();
    bufferMelodies.push(ascendante);
  }
  if (!jauneOk && jauneBranche) {
    dateActivite = millis();
    bufferMelodies.push(ascendante);
  }
  if (!rougeOk && rougeBranche) {
    dateActivite = millis();
    bufferMelodies.push(ascendante);
  }

  if (!toutOk && toutBranche) {
    dateActivite = millis();
    bufferMelodies.push(victory);
  }

  vertOk = vertBranche;
  jauneOk = jauneBranche;
  rougeOk = rougeBranche;
  toutOk = toutBranche;

  if (vertOk) feuVert.on(); else feuVert.off();
  if (jauneOk) feuJaune.on(); else feuJaune.off();
  if (rougeOk) feuRouge.on(); else feuRouge.off();
}

void veille()
{
  efface();
  digitalWrite(pinVert,LOW);
  digitalWrite(pinJaune,LOW);
  digitalWrite(pinRouge,LOW);
  for (byte i = 0; i < 9; i++) {
    digitalWrite(pinBL[i], LOW);
  }
  digitalWrite(pinLedON,LOW);
  digitalWrite(pinLedModeNumerique,LOW);
  digitalWrite(pinLedModeAnalogique,LOW);
  digitalWrite(pinLedModeSon,LOW);

  while(1) set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop()
{
  cle.update();
  sonPermis = (cle.read() == HIGH);

  char touche = clavier.getKey();

  if (touche) dateActivite = millis();

  gereChangementDeMode(touche);
  gereMode(touche);
  gereBranchements();
  joueMelodie();

  if (millis() - dateActivite > delaiVeille) veille();

  LightDimmer::update();
}
