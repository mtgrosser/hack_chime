const boolean DEBUG = true;

const byte INTERRUPT_PIN = 2;
const byte PLAY_PIN = 11;
const byte SPEAKER_PIN = 6;

const unsigned long WAIT_TIME = 10 * 10; // 10 seconds
const unsigned long TIMEOUT = 20 * 10;  // 20 seconds
const unsigned int FREQUENCY = 500;

volatile unsigned long cycles = 0;

unsigned long ticks = 0;
unsigned long previousCycles = 0;
boolean beeping = false;
boolean played = false;
boolean notified = false;
byte beeps = 0;
  
void setup() {
  if (DEBUG) {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("\n\nMac Chime\n\n");
  }

  pinMode(PLAY_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), detectCycles, LOW);
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

void debug(String message) {
  if(DEBUG) {
    Serial.println(message);
  }
}

void loop() {
  digitalWrite(PLAY_PIN, LOW);

  if (beeping && cycles == previousCycles) {
    beeping = false;
    beeps++;
    ledOff();
    noTone(SPEAKER_PIN);
    debug("Beep detected: " + String(beeps));
  } else if (!beeping && cycles > previousCycles) {
    beeping = true;
    ledOn();
    if (ticks > TIMEOUT) {
      tone(SPEAKER_PIN, FREQUENCY);
    }
  }
  
  previousCycles = cycles;

  if (!played && ticks >= WAIT_TIME && ticks < TIMEOUT) {
    switch (beeps) {
      case 0:
        if (!notified) {
          debug("No beep after wait time");
          notified = true;
        }
        break;
      case 1:
        debug("One beep after wait time - playing chime!");
        digitalWrite(PLAY_PIN, HIGH);
        played = true;
        break;
      default:
        if (!notified) {
          debug("Multiple beeps after wait time: " + String(beeps));
          tone(SPEAKER_PIN, FREQUENCY, 1000);
          tone(SPEAKER_PIN, FREQUENCY, 1000);
          notified = true;
        }
    }
  }
  
  delay(100);
  ticks++;
}
