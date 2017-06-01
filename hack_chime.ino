// Hack Chime 1.0
//
// Detect BIOS POST beeps and play the Mac boot sound
//
// Copyright (c) 2017, Matthias Grosser
//
// This work is licensed under a Creative Commons
// Attribution-ShareAlike 4.0 International License
// http://creativecommons.org/licenses/by-sa/4.0/

const boolean DEBUG = true;

// BIOS beep detect pin
const byte DETECT_PIN = 2;

// Output pin indexes, addresses and timers
const byte CHIME  = 0;
const byte ALERT  = 1;
const byte FUNK   = 2;
const byte BOINK  = 3;
const byte SOUND_PIN_ADDR[] = { 4, 6, 8, 10 };
unsigned long pinTicks[] = { 0, 0, 0, 0 };
const char* pinNames[] = { "chime", "alert", "funk", "boink" };

// Power-On Self Test times
const unsigned long POST_WAIT_TIME = 3000;
const unsigned long POST_TIMEOUT = 5000;

// Beep detection
volatile unsigned long cycles = 0;
unsigned long beeps = 0;

void setup() {
  pinMode(DETECT_PIN, INPUT_PULLUP);
  
  pinMode(SOUND_PIN_ADDR[CHIME], OUTPUT);
  pinMode(SOUND_PIN_ADDR[FUNK],  OUTPUT);
  pinMode(SOUND_PIN_ADDR[BOINK], OUTPUT);
  pinMode(SOUND_PIN_ADDR[ALERT], OUTPUT);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  if (DEBUG) {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("\n\nHack Chime\n\n");
  }

  attachInterrupt(digitalPinToInterrupt(DETECT_PIN), detectCycles, LOW);
}

void detectCycles() {
  cycles++;
}

void ledOn() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void ledOff() {
  digitalWrite(LED_BUILTIN, LOW);
}

void pullUpPin(byte pinIdx, unsigned long duration) {
  pinTicks[pinIdx] += duration;
  if (pinTicks[pinIdx] > 0) digitalWrite(SOUND_PIN_ADDR[pinIdx], HIGH);
}

void elapsePins(unsigned long elapsed) {
  byte pinIdx;
  for (pinIdx = 0; pinIdx <= 3; pinIdx++) {
    if (pinTicks[pinIdx] < elapsed) {
      pinTicks[pinIdx] = 0;
    } else {
      pinTicks[pinIdx] -= elapsed;
    }
    if (pinTicks[pinIdx] == 0) digitalWrite(SOUND_PIN_ADDR[pinIdx], LOW);
  }
}

void play(byte pinIdx) {
  debug("Playing " + String(pinNames[pinIdx]));
  pullUpPin(pinIdx, 100);
}

void playPostSound() {
  static boolean done = false;

  if (done) return;
  
  switch (beeps) {
    case 0:
      debug("No beep after wait time, playing chime anyway");
      play(CHIME);
      break;
    case 1:
      debug("One beep after wait time - playing chime");
      play(CHIME);
      break;
    default:
      debug("Multiple beeps after wait time: " + String(beeps));
      play(ALERT);
  }
  
  done = true;
}

void debug(String message) {
  if(DEBUG) Serial.println(message);
}

void loop() {
  static unsigned long lastMillis = 0;
  static unsigned long previousCycles = 0;
  static boolean beeping = false;

  unsigned long currentMillis = millis();
  unsigned long elapsed = currentMillis - lastMillis;

  boolean boinkIt = false;

  if (elapsed < 10) return;
  lastMillis = currentMillis;

  elapsePins(elapsed);
  
  if (beeping && cycles == previousCycles) {
    beeping = false;
    ledOff();
    debug("Beep finished: " + String(beeps));
  } else if (!beeping && cycles > previousCycles) {
    beeping = true;
    beeps++;
    ledOn();
    debug("Beep started: " + String(beeps));
    if (currentMillis > POST_TIMEOUT) play(BOINK);
  }
  previousCycles = cycles;
  
  if (currentMillis <= POST_TIMEOUT && currentMillis >= POST_WAIT_TIME) {
    playPostSound();
  }
}
