#include <Arduino.h>
#include <Wire.h>

// ================== TELEMETRY CONFIG ==================
#define PLEM_I2C_ADDR 0x3F
#define PLEM_SDA      13
#define PLEM_SCL      2

#define BTN_MEASURE   14
#define BTN_POWER     15

#define BTN_CONT 12
const unsigned long BTN_DEBOUNCE_MS = 40;
int btnLastReading = HIGH;
int btnStableState = HIGH;
unsigned long btnLastChangeTime = 0;

bool prevBtn = HIGH;

// UART2 (TX2 to the camera+TFT board)
#define UART2_TX_PIN  16
#define UART2_RX_PIN  17   // unused but required

static const int BUFSIZE = 200;
static volatile int  in_idx       = 0;
static volatile bool packet_ready = false;
static uint8_t packet_incoming[BUFSIZE];
static uint8_t packet_last[BUFSIZE];

String lastDecodedValue = "";
String lastSentValue    = "";

bool continuousMode = false;
bool keepAlive      = false;
unsigned long lastKeepAliveMs = 0;

// ================== TELEMETRY ISR ==================
void IRAM_ATTR onReceive(int len) {
  while (Wire.available()) {
    if (in_idx < BUFSIZE) {
      packet_incoming[in_idx++] = Wire.read();
    } else {
      (void)Wire.read(); // overflow discard
    }
  }

  if (in_idx >= 2) {
    if (!(packet_incoming[0] == 0x80 && packet_incoming[1] == 0xB0)) {
      in_idx = 0;
      return;
    }
  }

  if (in_idx >= 198) {
    memcpy(packet_last, packet_incoming, 198);
    memset(packet_incoming, 0, BUFSIZE);
    in_idx = 0;
    packet_ready = true;
  }
}

// Fetch last ready packet
bool fetch_packet(uint8_t out[]) {
  if (!packet_ready) return false;
  noInterrupts();
  memcpy(out, packet_last, BUFSIZE);
  packet_ready = false;
  interrupts();
  return true;
}

// ================== DECODING ==================
void decode_digit_last_line(char result[], const uint8_t d1, const uint8_t d2) {
  const char *s;
  switch ((d1 << 8) | d2) {
    case 0x0000: s="";   break;
    case 0x0400: s="-";  break;
    case 0x121E: s="0";  break;
    case 0x1A1E: s=".0"; break;
    case 0x0006: s="1";  break;
    case 0x0806: s=".1"; break;
    case 0x061C: s="2";  break;
    case 0x0E1C: s=".2"; break;
    case 0x041E: s="3";  break;
    case 0x0C1E: s=".3"; break;
    case 0x1406: s="4";  break;
    case 0x1C06: s=".4"; break;
    case 0x141A: s="5";  break;
    case 0x1C1A: s=".5"; break;
    case 0x161A: s="6";  break;
    case 0x1E1A: s=".6"; break;
    case 0x0016: s="7";  break;
    case 0x0816: s=".7"; break;
    case 0x161E: s="8";  break;
    case 0x1E1E: s=".8"; break;
    case 0x141E: s="9";  break;
    case 0x1C1E: s=".9"; break;
    default:     s="?";  break;
  }
  strcat(result, s);
}

void decode_last_line(char result[], const uint8_t *line) {
  result[0] = '\0';
  decode_digit_last_line(result, line[1],  line[3]);
  decode_digit_last_line(result, line[5],  line[7]);
  decode_digit_last_line(result, line[9],  line[11]);
  decode_digit_last_line(result, line[13], line[15]);
  decode_digit_last_line(result, line[17], line[19]);
  decode_digit_last_line(result, line[21], line[23]);
}

void decode_unit(char result[], uint8_t unit) {
  switch (unit) {
    case 0x0A: strcat(result, "m"); break;
    case 0x10: strcat(result, "ft"); break;
    case 0x06: strcat(result, "in"); break;
  }
}

// ================== BUTTON EMULATION ==================
void pressMeasure() {
  pinMode(BTN_MEASURE, OUTPUT);
  digitalWrite(BTN_MEASURE, LOW);
  delay(120);
  pinMode(BTN_MEASURE, INPUT);
}

void pressPower() {
  pinMode(BTN_POWER, OUTPUT);
  digitalWrite(BTN_POWER, LOW);
  delay(350);
  pinMode(BTN_POWER, INPUT);
}

// ================== COMMAND HELP ==================
void printHelp() {
  Serial.println();
  Serial.println("Available commands:");
  Serial.println("  measure");
  Serial.println("  continuous start");
  Serial.println("  continuous stop");
  Serial.println("  power");
  Serial.println("  keepalive start");
  Serial.println("  keepalive stop");
  Serial.println("  help");
  Serial.println();
}

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, UART2_RX_PIN, UART2_TX_PIN);

  pinMode(BTN_MEASURE, INPUT);
  pinMode(BTN_POWER, INPUT);

  pinMode(BTN_CONT, INPUT_PULLUP);

  memset(packet_incoming, 0, BUFSIZE);
  memset(packet_last, 0, BUFSIZE);
  in_idx = 0;
  packet_ready = false;

  pinMode(PLEM_SDA, INPUT);
  pinMode(PLEM_SCL, INPUT);
  delay(200); // let everything settle
  Wire.onReceive(onReceive);
  Wire.begin((uint8_t)PLEM_I2C_ADDR, PLEM_SDA, PLEM_SCL, 400000);

  Serial.println("Telemeter board ready. Type 'help' for commands.");
}

void handleContinuousButton() {
  int reading = digitalRead(BTN_CONT);
  unsigned long now = millis();

  // If the raw reading changed, reset debounce timer
  if (reading != btnLastReading) {
    btnLastChangeTime = now;
    btnLastReading = reading;
  }

  // If it has been stable long enough and differs from stable state,
  // treat it as a real change
  if ((now - btnLastChangeTime) > BTN_DEBOUNCE_MS && reading != btnStableState) {
    btnStableState = reading;

    // Button is wired to GND, so LOW means pressed
    if (btnStableState == LOW) {
      continuousMode = !continuousMode;
      Serial.print("Continuous mode: ");
      Serial.println(continuousMode ? "ON" : "OFF");
    }
  }
}

void handleMeasureButton() {
  int reading = digitalRead(BTN_CONT);
  unsigned long now = millis();

  // Debounce raw changes
  if (reading != btnLastReading) {
    btnLastChangeTime = now;
    btnLastReading = reading;
  }

  // If stable long enough and changed from stable state
  if ((now - btnLastChangeTime) > BTN_DEBOUNCE_MS && reading != btnStableState) {
    btnStableState = reading;

    // LOW = button pressed
    if (btnStableState == LOW) {
      // Perform a single measure action
      pressMeasure();
      Serial.println("Button measure");
    }
  }
}

// ================== LOOP ==================
void loop() {

  // ---- READ TELEMETRY PACKET ----
  uint8_t pkt[BUFSIZE];
  if (fetch_packet(pkt)) {
    char v[16] = "";
    decode_last_line(v, pkt + 94);
    decode_unit(v, pkt[119]);

    String decoded = String(v);

    // Only send if changed
    if (decoded.length() > 0 && decoded != lastSentValue) {
      lastSentValue = decoded;

      Serial.println(decoded);
      Serial2.println(decoded);
    }
  }

  // ---- SERIAL COMMANDS ----
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toLowerCase();

    if (cmd == "help") {
      printHelp();
    }
    else if (cmd == "measure") {
      pressMeasure();
    }
    else if (cmd == "continuous start") {
      continuousMode = true;
      Serial.println("Continuous mode ON");
    }
    else if (cmd == "continuous stop") {
      continuousMode = false;
      Serial.println("Continuous mode OFF");
    }
    else if (cmd == "power") {
      pressPower();
    }
    else if (cmd == "keepalive start") {
      keepAlive = true;
      lastKeepAliveMs = millis();
      Serial.println("Keepalive ON");
    }
    else if (cmd == "keepalive stop") {
      keepAlive = false;
      Serial.println("Keepalive OFF");
    }
    else {
      Serial.println("Unknown command. Type 'help'.");
    }
  }

  // ---- CONTINUOUS MODE ----
  if (continuousMode) {
    pressMeasure();
    delay(400);
  }

  // ---- KEEPALIVE MODE ----
  if (keepAlive && millis() - lastKeepAliveMs > 30000) {
    pressMeasure();
    lastKeepAliveMs = millis();
  }

  // ---- BUTTON HANDLER (debounced) ----
  handleMeasureButton();
}
