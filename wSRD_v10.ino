#define DEBUG true  //set to true for debug output, false for no debug output
#define DEBUG_SERIAL \
  if (DEBUG) Serial

#define VSPI_MISO 12
#define VSPI_MOSI 14
#define VSPI_SCLK 13
#define VSPI_SS 21
#define LED_GPIO 19
#define LED_COUNT 15

#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define VSPI FSPI
#endif

#define DEBUG_PRINT (1)

const float mcuFirmware = 1.51;
const float reqNexFirmware = 1.51;
String updateUrl = "http://update.racevolution.com/wsrde/update.json";
String hostNex = "update.racevolution.com/wsrde";
String tftNex = "/151.tft";

#include <SPIFFS.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <EasyNextionLibrary.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ESPNexUpload.h>
#include <math.h>
#include <CircularBuffer.h>
#include <NeoPixelBus.h>

TaskHandle_t Task1;
TaskHandle_t Task2;

WiFiManager wm;
WiFiUDP udp;
WiFiClient client;
HTTPClient http;
EasyNex myNex(Serial2);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, LED_GPIO);
SPIClass *vspi = NULL;

#include "TelemetryData.h"
#include "GameAdapter.h"
#include "F1Structs2022.h"
#include "F1Structs2023.h"
#include "F1Structs2024.h"
#include "ACCStructs.h"
#include "Variables.h"
#include "UpdateFW.h"
#include "UpdateNex.h"
#include "MultiGameHandlers.h"
#include "Functions.h"

void setup() {

  Serial.begin(115200);
  DEBUG_SERIAL.println("Serial0 interface initialized.");
  myNex.begin(921600);
  DEBUG_SERIAL.println("Nextion Serial2 interface initialized.");
  strip.Begin();
  DEBUG_SERIAL.println("WS2812B initialized.");

  wm.setAPCallback(SetPortalPage);
  
  vspi = new SPIClass(VSPI);
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS);  //SCLK, MISO, MOSI, SS
  pinMode(vspi->pinSS(), OUTPUT);                         //VSPI SS
  DEBUG_SERIAL.print("VSPI initialized. ");
  DEBUG_SERIAL.print(" CLK:");
  DEBUG_SERIAL.print(VSPI_SCLK);
  DEBUG_SERIAL.print(" MISO:");
  DEBUG_SERIAL.print(VSPI_MISO);
  DEBUG_SERIAL.print(" MOSI:");
  DEBUG_SERIAL.print(VSPI_MOSI);
  DEBUG_SERIAL.print(" CS:");
  DEBUG_SERIAL.println(VSPI_SS);

  if (!SD.begin(VSPI_SS, *vspi)) {
    DEBUG_SERIAL.println("-------------------------");
    DEBUG_SERIAL.println("SD-Card Mount Failed");
    DEBUG_SERIAL.println("-------------------------");
  } else {
    DEBUG_SERIAL.println("-------------------------");
    DEBUG_SERIAL.println("SD-Card Mount Ok!");
    DEBUG_SERIAL.println("-------------------------");
  }

  LoadSettings();

  WiFi.mode(WIFI_STA);

  bool res;
  res = wm.autoConnect("wSRD", "simracing");
  if (!res) {
    // myNex.writeStr("page start");
    // delay(100);
    // myNex.writeStr("h.txt", "Failed to connect");
    // myNex.writeStr("c.txt", "Could not connect to your wifi.\\rReset the wifi configuration and try again.");
    // myNex.writeStr("vis breset,1");
  } else {
    CheckNexFirmware();
    myNex.writeStr("page main");
  }

  udp.begin(F1_UDP_PORT);

  xTaskCreatePinnedToCore(
    Task1code, /* Function to implement the task */
    "Task1",   /* Name of the task */
    10000,     /* Stack size in words */
    NULL,      /* Task input parameter */
    0,         /* Priority of the task */
    &Task1,    /* Task handle. */
    0);        /* Core where the task should run */

  xTaskCreatePinnedToCore(
    Task2code, /* Function to implement the task */
    "Task2",   /* Name of the task */
    10000,     /* Stack size in words */
    NULL,      /* Task input parameter */
    1,         /* Priority of the task */
    &Task2,    /* Task handle. */
    1);        /* Core where the task should run */
}

void Task1code(void *parameter) {  //core 0
  for (;;) {
    delay(1);
    int packetSize = udp.parsePacket();
    if (packetSize > 0) {
      int packet_length = udp.read(packetBuffer, sizeof(packetBuffer));
      if (packet_length > 0) {
        // Route packet to appropriate game handler
        ProcessGamePacket(packetBuffer, packet_length);
      }
    }
  }
}


void Task2code(void *parameter) {  //core 1
  for (;;) {
    delay(1);
    myNex.NextionListen();
    if (currentPage != myNex.currentPageId) {
      currentPage = myNex.currentPageId;
      switch (currentPage) {
        case 2:
          myNex.writeStr("s.ip.txt", WiFi.localIP().toString());
          break;
        default:
          break;
      }
    }
  }
}

void loop() {
}
