#include <Arduino.h>
#include "esp_camera.h"
#include <TFT_eSPI.h>

// ====================================================
// TFT DISPLAY
// ====================================================
TFT_eSPI tft = TFT_eSPI();
#define TFT_ROTATION 1

// ====================================================
// UART2 – receives distance from telemeter board
// ====================================================

// NOTE: GPIO4 is used here.
// This ONLY works if the flash LED on GPIO4 is removed.
// If you want RX2 on GPIO13 instead, tell me.

#define UART2_RX_PIN 13
#define UART2_TX_PIN -1   // unused

String lastDistance = "0m";   // <-- default value

// ====================================================
// ESP32-CAM AI THINKER PIN DEFINITIONS
// ====================================================

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5

#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ====================================================
// CAMERA INIT
// ====================================================

bool init_camera() {
  camera_config_t c;

  c.ledc_channel = LEDC_CHANNEL_0;
  c.ledc_timer   = LEDC_TIMER_0;

  c.pin_d0 = Y2_GPIO_NUM;
  c.pin_d1 = Y3_GPIO_NUM;
  c.pin_d2 = Y4_GPIO_NUM;
  c.pin_d3 = Y5_GPIO_NUM;
  c.pin_d4 = Y6_GPIO_NUM;
  c.pin_d5 = Y7_GPIO_NUM;
  c.pin_d6 = Y8_GPIO_NUM;
  c.pin_d7 = Y9_GPIO_NUM;

  c.pin_xclk = XCLK_GPIO_NUM;
  c.pin_pclk = PCLK_GPIO_NUM;
  c.pin_vsync = VSYNC_GPIO_NUM;
  c.pin_href  = HREF_GPIO_NUM;

  c.pin_sccb_sda = SIOD_GPIO_NUM;
  c.pin_sccb_scl = SIOC_GPIO_NUM;

  c.pin_pwdn  = PWDN_GPIO_NUM;
  c.pin_reset = RESET_GPIO_NUM;

  c.xclk_freq_hz = 20000000;

  c.frame_size   = FRAMESIZE_QVGA;     // 320x240
  c.pixel_format = PIXFORMAT_RGB565;   // for TFT
  c.fb_count     = 2;
  c.grab_mode    = CAMERA_GRAB_LATEST;
  c.fb_location  = psramFound() ? CAMERA_FB_IN_PSRAM : CAMERA_FB_IN_DRAM;

  esp_err_t err = esp_camera_init(&c);
  return (err == ESP_OK);
}

// ====================================================
// DRAW OVERLAY - White text bottom-center, no box
// ====================================================

void drawDistanceOverlay(const String &s) {
  tft.setTextColor(TFT_WHITE, TFT_BLACK /*ignored unless background enabled*/);
  tft.setTextSize(2);

  int16_t textW = tft.textWidth(s);
  int16_t x = (320 - textW) / 2;
  int16_t y = 240 - 26 + 4;  // bottom area

  // Draw text only, no background
  tft.setCursor(x, y);
  tft.print(s);
}

String normalizeDistance(const String &raw) {
  String s = raw;
  s.trim();

  // Case: 255 (no unit)
  if (s == "255") {
    return lastDistance;
  }

  // Case: telemeter sends "---" or empty
  if (s == "---" || s.length() == 0) {
    return lastDistance;
  }

  // Case: must end with 'm'
  if (!s.endsWith("m")) {
    return lastDistance;
  }

  // Remove the trailing 'm'
  String numberPart = s.substring(0, s.length() - 1);

  // Validate the number part: digits or digits+dot
  bool valid = true;
  bool dotSeen = false;

  for (int i = 0; i < numberPart.length(); i++) {
    char c = numberPart[i];
    if (c >= '0' && c <= '9') continue;
    if (c == '.' && !dotSeen) {
      dotSeen = true;
      continue;
    }
    valid = false;
    break;
  }

  if (!valid || numberPart.length() == 0) {
    return lastDistance;
  }

  // Everything valid, return "[number]m"
  return numberPart + "m";
}


// ====================================================
// SETUP
// ====================================================

void setup() {
  Serial.begin(115200);
  delay(200);

  // UART from telemeter ESP32
  Serial2.begin(115200, SERIAL_8N1, UART2_RX_PIN, UART2_TX_PIN);

  // GPIO4 flash LED must stay OFF
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

  // Camera
  if (!init_camera()) {
    Serial.println("Camera init FAILED");
    while (true) delay(500);
  }

  // TFT
  tft.init();
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(TFT_BLACK);

  Serial.println("Camera + TFT board ready.");
}

// ====================================================
// LOOP
// ====================================================

void loop() {

  // ---- 1. Read distance from telemeter board ----
  while (Serial2.available()) {
    String line = Serial2.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) {

      // Normalize/filter the incoming distance
      String formatted = normalizeDistance(line);

      lastDistance = formatted;

      Serial.print("rx distance raw: ");
      Serial.println(line);
      Serial.print("rx distance normalized: ");
      Serial.println(lastDistance);
    }
  }

  // ---- 2. Capture camera frame ----
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) return;

  // ---- 3. Draw image ----
  if (fb->format == PIXFORMAT_RGB565 &&
      fb->width  == 320 &&
      fb->height == 240) {

    tft.pushImage(0, 0, 320, 240, (uint16_t*)fb->buf);

    // ---- 4. Draw distance ----
    drawDistanceOverlay(lastDistance);
  }

  esp_camera_fb_return(fb);
}
