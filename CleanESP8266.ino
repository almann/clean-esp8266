#include <FS.h>
#include <Esp.h>
#include <ESP8266WiFi.h>

// ESP8266 SDK headers
extern "C" {
  #include <user_interface.h>
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  bool opened = SPIFFS.begin();
  Serial.print("FS Opened: ");
  Serial.println(opened);
}

void loop() {
  if (Serial.available() > 1) {
    String command = Serial.readStringUntil('\n');
    if (command.length() >= 1) {
      char const code = command.charAt(0);
      switch (code) {
        case 'i': {
          Serial.println("Information");

          Serial.print("SDK: ");
          Serial.println(ESP.getSdkVersion());
          Serial.print("Free Heap: ");
          Serial.println(ESP.getFreeHeap());
          Serial.println();

          Serial.print("Flash Real Size: ");
          Serial.println(ESP.getFlashChipRealSize());
          Serial.print("Flash Firmware Configured Size: ");
          Serial.println(ESP.getFlashChipSize());
          
          FSInfo fsInfo;
          SPIFFS.info(fsInfo);
          Serial.print("FS Bytes: ");
          Serial.print(fsInfo.usedBytes);
          Serial.print(" / ");
          Serial.println(fsInfo.totalBytes);

          Serial.println();
          Serial.println("WiFi Information");
          WiFi.printDiag(Serial);
          Serial.println();

          IPAddress ip = WiFi.localIP();
          Serial.print("IP Address: ");
          ip.printTo(Serial);
          Serial.println();
          
          break;
        }
        case 'w': {
          if (command.length() < 4 || command.charAt(1) != ':') {
            Serial.println("WiFi configuration must have the following form: w:<SSID>:<PASS>");
            break;
          }
          int delim = command.indexOf(':', 2);
          if (delim == -1) {
            Serial.println("WiFi password missing");
            break;
          }
          String ssid = command.substring(2, delim);
          String pass = command.substring(delim + 1);
          if (pass == "") {
            Serial.println("WiFi password missing");
            break;
          }
          Serial.println("Configuring WiFi SSID '" + ssid + "' with password '" + pass + "'");
          WiFi.begin(ssid.c_str(), pass.c_str());
          break;
        }
        case 'f': {
          Serial.println("Formatting SPIFFS");
          SPIFFS.format();
          break;
        }
        case 'c': {
          Serial.println("Clearing WiFi Configuration");
          system_restore();
          break;
        }
        default: {
          Serial.print("Unknown command: ");
          Serial.println(command);
          break;
        }
      }
      Serial.println("Done.");
      Serial.println("====");
    }
  }
}
