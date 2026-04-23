#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <math.h>
#include "esp_adc_cal.h" 

// --- Network ---
const char* ssid = "tiburon";
const char* password = "teamtiburon";
const char* udpAddress = "10.175.11.105"; 
const int udpPort = 8888;

WiFiUDP udp;

// --- TFT PINS ---
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC   4   
#define TFT_RST  2   

SPIClass hspi(HSPI);
Adafruit_ILI9341 tft = Adafruit_ILI9341(&hspi, TFT_DC, TFT_CS, TFT_RST);

// --- SENSOR PINS ---
#define PIN_ZMPT 34 
#define PIN_DC   32 
#define PIN_ACS  33 

#define SCREEN_W 320 
#define SCREEN_H 240 
#define ROTATION 1

#define BUFFER_SIZE 200
#define SAMPLE_INTERVAL_US 100

// --- CALIBRATION ---
float CAL_DC_VOLTS = 6.6; 
float CAL_AC_VOLTS = 600; 
float CAL_CURRENT  = 1.0; 

// --- BUFFERS ---
volatile int bufferZMPT[BUFFER_SIZE];
volatile int bufferDC[BUFFER_SIZE];
volatile int bufferACS[BUFFER_SIZE];

volatile float acVoltsRMS = 0.0;
volatile float dcVolts = 0.0;
volatile float currentRMS = 0.0;

volatile bool bufferReadyForNetwork = false;

esp_adc_cal_characteristics_t adc1_chars; 

TaskHandle_t TaskAcquisition;
TaskHandle_t TaskNetwork;

// ==========================================================
// CORE 1: ACQUISITION + DISPLAY
// ==========================================================
void Task_Acquisition_and_Display(void *pvParameters) {

  for (;;) {

    long sumZMPT = 0;
    long sumDC   = 0;
    long sumACS  = 0;

    uint32_t nextSampleTime = micros(); 
    
    for (int i = 0; i < BUFFER_SIZE; i++) {

      int rawZMPT = analogRead(PIN_ZMPT);
      int rawDC   = analogRead(PIN_DC);
      int rawACS  = analogRead(PIN_ACS);

      uint32_t mvZMPT = esp_adc_cal_raw_to_voltage(rawZMPT, &adc1_chars);
      uint32_t mvDC   = esp_adc_cal_raw_to_voltage(rawDC, &adc1_chars);
      uint32_t mvACS  = esp_adc_cal_raw_to_voltage(rawACS, &adc1_chars);

      bufferZMPT[i] = mvZMPT;
      bufferDC[i]   = mvDC;
      bufferACS[i]  = mvACS;

      sumZMPT += mvZMPT;
      sumDC   += mvDC;
      sumACS  += mvACS;

      nextSampleTime += SAMPLE_INTERVAL_US;
      while (micros() < nextSampleTime);
    }

    // --- OFFSET ---
    float meanZMPT = sumZMPT / (float)BUFFER_SIZE;
    float meanDC   = sumDC   / (float)BUFFER_SIZE;
    float meanACS  = sumACS  / (float)BUFFER_SIZE;

    float sqSumZMPT = 0;
    float sqSumACS  = 0;

    for (int i = 0; i < BUFFER_SIZE; i++) {

      float centeredZMPT = bufferZMPT[i] - meanZMPT;
      sqSumZMPT += centeredZMPT * centeredZMPT;

      float centeredACS = bufferACS[i] - meanACS;
      sqSumACS += centeredACS * centeredACS;
    }

    // --- RMS ---
    float rmsRaw = sqrt(sqSumZMPT / BUFFER_SIZE) / 1000.0;
    acVoltsRMS = (rmsRaw < 0.02) ? 0 : rmsRaw * CAL_AC_VOLTS;

    dcVolts = (meanDC / 1000.0) * CAL_DC_VOLTS;

    float rmsC = sqrt(sqSumACS / BUFFER_SIZE) / 1000.0;
    currentRMS = (rmsC < 0.01) ? 0 : rmsC * CAL_CURRENT;

    bufferReadyForNetwork = true;

    // ======================================================
    // DISPLAY (3 ZONES SETUP)
    // ======================================================
    int zH = SCREEN_H / 3;
    tft.setTextSize(2);

    // --- ZONE 1: AC TEXT & CLEAR ---
    tft.setCursor(5, 5);
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.print("AC: "); tft.print(acVoltsRMS, 2); tft.println(" V   ");

    int z1_top = 25; 
    int z1_bot = zH - 5;
    tft.fillRect(0, z1_top, SCREEN_W, z1_bot - z1_top, ILI9341_BLACK);

    // --- ZONE 2: DC TEXT & CLEAR ---
    tft.setCursor(5, zH + 5);
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft.print("DC: "); tft.print(dcVolts, 2); tft.println(" V   ");

    int z2_top = zH + 25; 
    int z2_bot = (zH * 2) - 5;
    tft.fillRect(0, z2_top, SCREEN_W, z2_bot - z2_top, ILI9341_BLACK);

    // --- ZONE 3: CURRENT TEXT & CLEAR ---
    tft.setCursor(5, (zH * 2) + 5);
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
    tft.print("Cur: "); tft.print(currentRMS, 2); tft.println(" A   ");

    int z3_top = (zH * 2) + 25; 
    int z3_bot = SCREEN_H - 5;
    tft.fillRect(0, z3_top, SCREEN_W, z3_bot - z3_top, ILI9341_BLACK);

    // ======================================================
    //    DRAW 3 SEPARATE WAVEFORMS
    // ======================================================
    for (int i = 0; i < BUFFER_SIZE - 1; i++) {

      int x1 = map(i, 0, BUFFER_SIZE, 0, SCREEN_W);
      int x2 = map(i+1, 0, BUFFER_SIZE, 0, SCREEN_W);

      // --- 1. AC WAVEFORM (Zoomed: -1000 to 1000 mV) ---
      int ac1 = bufferZMPT[i] - meanZMPT;
      int ac2 = bufferZMPT[i+1] - meanZMPT;
      int y1_ac = map(ac1, -1000, 1000, z1_bot, z1_top);
      int y2_ac = map(ac2, -1000, 1000, z1_bot, z1_top);
      tft.drawLine(x1, y1_ac, x2, y2_ac, ILI9341_GREEN);

      // --- 2. DC WAVEFORM (Absolute: 0 to 3300 mV) ---
      int y1_dc = map(bufferDC[i], 0, 3300, z2_bot, z2_top);
      int y2_dc = map(bufferDC[i+1], 0, 3300, z2_bot, z2_top);
      tft.drawLine(x1, y1_dc, x2, y2_dc, ILI9341_YELLOW);

      // --- 3. CURRENT WAVEFORM (Zoomed: -1000 to 1000 mV) ---
      int cur1 = bufferACS[i] - meanACS;
      int cur2 = bufferACS[i+1] - meanACS;
      int y1_cur = map(cur1, -1000, 1000, z3_bot, z3_top);
      int y2_cur = map(cur2, -1000, 1000, z3_bot, z3_top);
      tft.drawLine(x1, y1_cur, x2, y2_cur, ILI9341_CYAN); 
    }

    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

// ==========================================================
// NETWORK
// ==========================================================
void Task_Network_UDP(void *pvParameters) {

  for (;;) {

    if (WiFi.status() != WL_CONNECTED) {
      WiFi.reconnect();
    }

    if (bufferReadyForNetwork) {

     String payload = "{";
payload += "\"rms\":" + String(acVoltsRMS) + ",";
payload += "\"current\":" + String(currentRMS) + ",";
payload += "\"dc_v\":" + String(dcVolts);
payload += "}";

      udp.beginPacket(udpAddress, udpPort);
      udp.print(payload);
      udp.endPacket();

      bufferReadyForNetwork = false;
    }

    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

// ==========================================================
// SETUP
// ==========================================================
void setup() {

  Serial.begin(115200);

  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc1_chars);

  analogReadResolution(12);
  analogSetPinAttenuation(PIN_ZMPT, ADC_11db);
  analogSetPinAttenuation(PIN_ACS,  ADC_11db);
  analogSetPinAttenuation(PIN_DC,   ADC_11db);

  hspi.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  tft.begin();
  tft.setRotation(ROTATION);
  tft.fillScreen(ILI9341_BLACK);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  xTaskCreatePinnedToCore(Task_Acquisition_and_Display, "Task_Acq", 10000, NULL, 1, &TaskAcquisition, 1);
  xTaskCreatePinnedToCore(Task_Network_UDP, "Task_Net", 10000, NULL, 1, &TaskNetwork, 0);
}

void loop() {
  vTaskDelete(NULL);
}