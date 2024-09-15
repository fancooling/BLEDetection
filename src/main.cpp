#include <Arduino.h>
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAddress.h>
#include "esp_log.h"

static const char* TAG = "MyScanModule";
const int PIN = 2;
const int CUTOFF = -70;
// const std::string DEVICE_TO_FIND("d0:3f:27:60:3b:30");  // Wyze Scale
// const std::string DEVICE_TO_FIND("ec:a7:cd:8b:af:8c");  // Air tag
// const std::string DEVICE_TO_FIND("51:96:d5:d8:3b:d4");  // fanbl S23 ultra advertiser
const std::string UUID_TO_FIND("bc157f34-328e-412e-a45d-8180ff9b401a");  // fanbl S23 ultra advertiser

void setup() {
  Serial.begin(9600);  
  while (!Serial) {
    ;
  }
  Serial.println("Hello Arduino!");
  pinMode(PIN, OUTPUT);
  BLEDevice::init("");  
  ESP_LOGI(TAG, "start testing");
}

void loop() {
  BLEUUID uuid_to_find(UUID_TO_FIND);
  BLEScan *scan = BLEDevice::getScan();
  scan->setActiveScan(true);
  BLEScanResults results = scan->start(1);
  bool found = false;
  for (int i = 0; i < results.getCount(); i++) {
    BLEAdvertisedDevice device = results.getDevice(i);
    int rssi = device.getRSSI();
    if (rssi > CUTOFF) {
      // ESP_LOGI(TAG, "Device found: %s\n", device.toString().c_str());
      Serial.printf("In loop device found %s\n", device.toString().c_str());       
      if (device.isAdvertisingService(uuid_to_find)) {
        ESP_LOGW(TAG, "Detected Device: %s\n", device.toString().c_str());
        found = true;
      }
    }    
    
      /*if (rssi > CUTOFF) {
        found = true;
      }*/
    //}    
    /*
    if (rssi > best) {
      best = rssi;
      ESP_LOGI(TAG, "ESPlog Device found: %ls, address: %s\n", device.getName().c_str(), device.getAddress().toString().c_str());
      ESP_LOGI(TAG, "ESPlog Device: %s\n", device.toString().c_str());
    }
    */
    //delay(1000);
  }
  // digitalWrite(PIN, best > CUTOFF ? HIGH : LOW);
  digitalWrite(PIN, found ? HIGH : LOW);
}