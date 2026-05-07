/*
  This example connects to an unencrypted WiFi network.
  Then it prints the MAC address of the WiFi module,
  the IP address obtained, and other network details.

  created 13 July 2010
  by dlf (Metodo2 srl)
  modified 31 May 2012
  by Tom Igoe

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#connect-with-wpa
 */
#include <NTPClient.h>
#include <WiFiS3.h>
//#include <ArduinoJson.h>
//#include <ArduinoGraphics.h>
//#include <Arduino_LED_Matrix.h>

WiFiUDP ntpUDP;

// Ireland is UTC+1 during summer.
// Change 3600 to 0 during winter if needed.
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600);

// =========================
// LED PIN
// =========================
// Built-in LED on most Arduino boards
const int ledPin = 7;

int status = WL_IDLE_STATUS;

#include "arduinosecrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
  

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
    
    pinMode(ledPin, OUTPUT);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();


timeClient.begin();}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();
  printWifiData();
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 0; i < 6; i++) {
    if (i > 0) {
      Serial.print(":");
    }
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
  }
  Serial.println();



// Update internet time
timeClient.update();

// Get current time
int currentHour = timeClient.getHours();
int currentMinute = timeClient.getMinutes();
int currentSecond = timeClient.getSeconds();

// Print time to Serial Monitor
Serial.print("Time: ");
Serial.print(currentHour);
Serial.print(":");
Serial.print(currentMinute);
Serial.print(":");
Serial.println(currentSecond);

// =========================
// TURN LIGHT ON AT 7:00 AM
// =========================
if (currentHour == 16) {
digitalWrite(ledPin, HIGH);
Serial.println("LED ON");
}
else {
digitalWrite(ledPin, LOW);
Serial.println("LED OFF");
}

delay(1000);
}