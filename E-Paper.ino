/*
 * E-Paper Photo Frame - Main File
 * Hardware: ESP32 + Waveshare 5.65" 7-color e-paper display
 * Features: Web interface, WiFi with AP fallback, QR code display, SPIFFS storage
 *
 *
 * Repository: https://github.com/9carlo6/E-Paper
 * @author 9carlo6
 * @date 2025
 * @version 1.0
 *
 * @copyright
 * Copyright (c) 2025 9carlo6  
 * All rights reserved.
 */

#include <GxEPD2_7C.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <SPIFFS.h>
#include <QRCode_Library.h>

#include "display_config.h"
#include "web_interface.h"

/* ========================================
   CONFIGURATION
   ======================================== */

#define AP_SSID "E-Paper WiFi"
#define AP_PASS "epaper2025"

#define IMAGE_WIDTH 448
#define IMAGE_HEIGHT 600
#define IMAGE_FILE "/current.bin"

/* ========================================
   GLOBAL VARIABLES
   ======================================== */

Preferences preferences;
WebServer server(80);
bool wifiConfigured = false;
String savedSSID = "";
String savedPass = "";
File uploadFile;

/* ========================================
   FUNCTION DECLARATIONS
   ======================================== */

void initDisplay();
void showBootScreen();
void displayImageFromSPIFFS();
void drawImageFromSPIFFS(File& file);
uint16_t mapColorValue(uint8_t pixel_value);
void loadWiFiCredentials();
void saveWiFiCredentials(String ssid, String pass);
void connectToWiFi();
void startAPMode();
void setupWebServer();
void handleUpload();
void handleUploadComplete();

/* ========================================
   SETUP - INITIALIZATION
   ======================================== */

void setup() {
  Serial.begin(115200);
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║   E-Paper Photo Frame v2.1         ║");
  Serial.println("╚════════════════════════════════════╝");
  
  if (!SPIFFS.begin(true)) {
    Serial.println("✗ ERROR: SPIFFS initialization failed!");
    return;
  }
  Serial.printf("✓ SPIFFS initialized: %d / %d bytes\n", 
                SPIFFS.usedBytes(), SPIFFS.totalBytes());
  
  initDisplay();
  
  preferences.begin("epaper", false);
  loadWiFiCredentials();
  
  if (savedSSID.length() > 0) {
    connectToWiFi();
  } else {
    Serial.println("⚠ No WiFi configured - Starting AP mode");
    startAPMode();
  }
  
  showBootScreen();
  setupWebServer();
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║           SETUP COMPLETE           ║");
  Serial.println("╠════════════════════════════════════╣");
  if (wifiConfigured) {
    Serial.printf("║ IP Address: %-22s ║\n", WiFi.localIP().toString().c_str());
    Serial.println("║                                    ║");
    Serial.println("║ Frame connected. Open browser at:  ║");
    Serial.printf("║ http://%-27s ║\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.printf("║ AP SSID: %-25s ║\n", AP_SSID);
    Serial.printf("║ AP Password: %-19s ║\n", AP_PASS);
    Serial.printf("║ AP IP: %-27s ║\n", WiFi.softAPIP().toString().c_str());
    Serial.println("║                                    ║");
    Serial.println("║ Scan QR code on display or go to:  ║");
    Serial.printf("║ http://%-27s ║\n", WiFi.softAPIP().toString().c_str());
  }
  Serial.println("╚════════════════════════════════════╝\n");
}

/* ========================================
   MAIN LOOP
   ======================================== */

void loop() {
  server.handleClient();
  delay(10);
}

/* ========================================
   DISPLAY FUNCTIONS
   ======================================== */

void initDisplay() {
  Serial.print("Initializing display... ");
  display.init(115200, true, 2, false);
  display.setRotation(1);
  Serial.println("✓ Done");
}

void showBootScreen() {
  Serial.print("Showing boot screen... ");
  
  display.setFullWindow();
  display.firstPage();
  
  do {
    display.fillScreen(GxEPD_WHITE);

    display.setCursor(120, 550);
    display.setTextSize(1);
    display.setTextColor(GxEPD_GREEN);
    display.println("https://github.com/9carlo6/E-Paper");

    display.setCursor(100, 80);
    display.setTextColor(GxEPD_BLACK);
    display.setTextSize(3);
    display.println("E-Paper Frame");
    
    String url;
    if (wifiConfigured) {
      url = WiFi.localIP().toString();
    } else {
      url = WiFi.softAPIP().toString();
    }
    
    int boxX = 50;
    int boxW = 348;
    int boxY, boxH;
    
    if (wifiConfigured) {
      boxY = 150;
      boxH = 180;
    } else {
      boxY = 150;
      boxH = 300;
    }
    
    display.fillRect(boxX, boxY, boxW, boxH, GxEPD_WHITE);
    display.drawRect(boxX, boxY, boxW, boxH, GxEPD_BLACK);
    display.drawRect(boxX+1, boxY+1, boxW-2, boxH-2, GxEPD_BLACK);
    
    display.setTextColor(GxEPD_BLACK);
    
    if (wifiConfigured) {
      /* ====== MODE: Station (Connected to existing network) ====== */
      
      display.setCursor(boxX + 20, boxY + 30);
      display.setTextSize(2);
      display.println("Connected to Wi-Fi:"); 
      
      display.setCursor(boxX + 20, boxY + 65);
      display.setTextSize(2);
      display.println(savedSSID);
      
      display.setCursor(boxX + 20, boxY + 110);
      display.setTextSize(1);
      display.println("Open in browser:");
      
      display.setCursor(boxX + 30, boxY + 135);
      display.setTextSize(2);
      display.setTextColor(GxEPD_RED);
      display.println(url);
      
    } else {
      /* ====== MODE: Access Point (Show AP credentials + QR Code) ====== */
      
      display.setCursor(boxX + 20, boxY + 20);
      display.setTextSize(2);
      display.println("Connect to Wi-Fi:"); 
      
      display.setCursor(boxX + 20, boxY + 50);
      display.setTextSize(2);
      display.setTextColor(GxEPD_RED);
      display.println("Name: " + String(AP_SSID));
      
      display.setCursor(boxX + 20, boxY + 80);
      display.setTextSize(2);
      display.setTextColor(GxEPD_RED);
      display.println("Password: " + String(AP_PASS));

      display.setCursor(boxX + 20, boxY + 120);
      display.setTextSize(2);
      display.setTextColor(GxEPD_BLACK);
      display.println("Scan QR:");
      
      /* ====== QR CODE GENERATION (Inside box) ====== */
      
      String qrData = "http://" + url; 
      
      const int qr_version = 3;
      const int qr_scale = 3;
      const int qr_margin = 2;
      
      int qr_side = (29 + 2 * qr_margin) * qr_scale; 
      int qr_x = boxX + (boxW - qr_side) / 2; 
      int qr_y = boxY + 170; 
      
      if (qr_y + qr_side > boxY + boxH) {
          display.setCursor(boxX + 20, boxY + 360);
          display.setTextSize(1);
          display.setTextColor(GxEPD_RED);
          display.println("QR too big!");
      }
      
      QRCode qrcode;
      uint8_t qrcodeBytes[qrcode_getBufferSize(qr_version)];
      qrcode_initText(&qrcode, qrcodeBytes, qr_version, 0, qrData.c_str());

      display.fillRect(qr_x, qr_y, qr_side, qr_side, GxEPD_WHITE);
      
      for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
          if (qrcode_getModule(&qrcode, x, y)) {
            display.fillRect(
              qr_x + (x + qr_margin) * qr_scale, 
              qr_y + (y + qr_margin) * qr_scale, 
              qr_scale, 
              qr_scale, 
              GxEPD_BLACK
            );
          }
        }
      }
      
      display.drawRect(qr_x, qr_y, qr_side, qr_side, GxEPD_BLACK);
    }
    
  } while (display.nextPage());
  
  Serial.println("✓ Done");
}

void displayImageFromSPIFFS() {
  Serial.println("\n=== Updating Display ===");
  File imageFile = SPIFFS.open(IMAGE_FILE, FILE_READ);
  if (!imageFile) {
    Serial.println("✗ ERROR: Cannot open image file!");
    return;
  }
  
  Serial.printf("Image file size: %d bytes\n", imageFile.size());
  
  display.setFullWindow();
  display.firstPage();
  
  unsigned long startTime = millis();
  do {
    display.fillScreen(GxEPD_WHITE);
    drawImageFromSPIFFS(imageFile);
  } while (display.nextPage());
  
  imageFile.close();
  
  unsigned long elapsed = millis() - startTime;
  Serial.printf("✓ Display updated in %lu ms\n", elapsed);
  
  display.hibernate();
}

void drawImageFromSPIFFS(File& file) {
  file.seek(0);
  uint8_t lineBuffer[IMAGE_WIDTH / 2];
  
  for (int y = 0; y < IMAGE_HEIGHT; y++) {
    size_t bytesRead = file.read(lineBuffer, IMAGE_WIDTH / 2);
    if (bytesRead != IMAGE_WIDTH / 2) {
      Serial.printf("⚠ Warning: Line %d - only %d bytes read\n", y, bytesRead);
      break;
    }
    
    for (int x = 0; x < IMAGE_WIDTH; x += 2) {
      int buffer_index = x / 2;
      uint8_t byte_data = lineBuffer[buffer_index];
      
      uint8_t left_pixel = (byte_data >> 4) & 0x0F;
      uint8_t right_pixel = byte_data & 0x0F;
      
      display.drawPixel(x, y, mapColorValue(left_pixel));
      if (x + 1 < IMAGE_WIDTH) {
        display.drawPixel(x + 1, y, mapColorValue(right_pixel));
      }
    }
  }
}

uint16_t mapColorValue(uint8_t pixel_value) {
  switch (pixel_value) {
    case 0: return GxEPD_BLACK;
    case 1: return GxEPD_WHITE;
    case 2: return GxEPD_GREEN;
    case 3: return GxEPD_BLUE;
    case 4: return GxEPD_RED;
    case 5: return GxEPD_YELLOW;
    case 6: return GxEPD_ORANGE;
    default: return GxEPD_WHITE;
  }
}

/* ========================================
   WIFI MANAGEMENT
   ======================================== */

void loadWiFiCredentials() {
  savedSSID = preferences.getString("ssid", "");
  savedPass = preferences.getString("pass", "");
  
  if (savedSSID.length() > 0) {
    Serial.println("✓ WiFi credentials found");
    Serial.printf("   SSID: %s\n", savedSSID.c_str());
  } else {
    Serial.println("⚠ No WiFi credentials stored");
  }
}

void saveWiFiCredentials(String ssid, String pass) {
  preferences.putString("ssid", ssid);
  preferences.putString("pass", pass);
  Serial.println("✓ WiFi credentials saved");
}

void connectToWiFi() {
  Serial.printf("Connecting to WiFi: %s ", savedSSID.c_str());
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(savedSSID.c_str(), savedPass.c_str());
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConfigured = true;
    Serial.println(" ✓ Connected");
    Serial.printf("   IP Address: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("   Signal: %d dBm\n", WiFi.RSSI());
  } else {
    Serial.println(" ✗ Failed");
    Serial.println("⚠ Starting AP mode instead");
    startAPMode();
  }
}

void startAPMode() {
  Serial.print("Starting Access Point... ");
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  
  Serial.println("✓ Done");
  Serial.printf("   SSID: %s\n", AP_SSID);
  Serial.printf("   Password: %s\n", AP_PASS);
  Serial.printf("   IP Address: %s\n", WiFi.softAPIP().toString().c_str());
}

/* ========================================
   WEB SERVER & FILE UPLOAD
   ======================================== */

void setupWebServer() {
  Serial.print("Starting web server... ");
  
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send_P(200, "text/html", HTML_PAGE);
  });
  
  server.on("/upload", HTTP_POST, handleUploadComplete, handleUpload);
  
  server.on("/config", HTTP_GET, []() {
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
    html += "<style>body{font-family:Arial;max-width:400px;margin:50px auto;padding:20px;}";
    html += "input{width:100%;padding:10px;margin:10px 0;box-sizing:border-box;}";
    html += "button{width:100%;padding:15px;background:#007bff;color:white;border:none;cursor:pointer;border-radius:5px;}";
    html += "button:hover{background:#0056b3;}</style></head><body>";
    html += "<h2>⚙️ WiFi Configuration</h2>";
    html += "<form action='/save' method='POST'>";
    html += "<input name='ssid' placeholder='WiFi Name (SSID)' required>";
    html += "<input name='pass' type='password' placeholder='WiFi Password' required>";
    html += "<button type='submit'>Save and Connect</button>";
    html += "</form></body></html>";
    server.send(200, "text/html", html);
  });
  
  server.on("/save", HTTP_POST, []() {
    if (server.hasArg("ssid") && server.hasArg("pass")) {
      String newSSID = server.arg("ssid");
      String newPass = server.arg("pass");
      
      saveWiFiCredentials(newSSID, newPass);
      
      server.send(200, "text/html",
        "<html><body style='font-family:Arial;text-align:center;padding:50px;'>"
        "<h2>✓ Credentials Saved!</h2>"
        "<p>Restarting in 3 seconds...</p>"
        "</body></html>");
      
      delay(3000);
      ESP.restart();
    } else {
      server.send(400, "text/html",
        "<html><body style='font-family:Arial;text-align:center;padding:50px;'>"
        "<h2>✗ Error</h2>"
        "<p>Missing parameters!</p>"
        "</body></html>");
    }
  });
  
  server.on("/upload", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });
  
  server.begin();
  Serial.println("✓ Done");
}

void handleUpload() {
  HTTPUpload& upload = server.upload();
  
  if (upload.status == UPLOAD_FILE_START) {
    Serial.println("\n=== File Upload Started ===");
    Serial.printf("Filename: %s\n", upload.filename.c_str());
    
    uploadFile = SPIFFS.open(IMAGE_FILE, FILE_WRITE);
    if (!uploadFile) {
      Serial.println("✗ ERROR: Cannot open file for writing!");
      return;
    }
  } 
  else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
      if (uploadFile.size() % 10000 < upload.currentSize) {
        Serial.printf("Progress: %d bytes\n", uploadFile.size());
      }
    }
  } 
  else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
      Serial.printf("✓ Upload complete: %d bytes\n", upload.totalSize);
    }
  }
}

void handleUploadComplete() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  
  if (!SPIFFS.exists(IMAGE_FILE)) {
    Serial.println("✗ ERROR: Image file not found!");
    server.send(400, "text/plain", "Error: File not found");
    return;
  }
  
  File f = SPIFFS.open(IMAGE_FILE, FILE_READ);
  size_t fileSize = f.size();
  f.close();
  
  uint32_t expectedSize = IMAGE_WIDTH * IMAGE_HEIGHT / 2;
  
  if (fileSize != expectedSize) {
    Serial.printf("⚠ Warning: File size %d, expected %d\n", fileSize, expectedSize);
  }
  
  server.send(200, "text/plain", "OK");
  displayImageFromSPIFFS();
}