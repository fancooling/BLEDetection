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
static BLEUUID UUIDS_TO_FIND[] = {
  BLEUUID("bc157f34-328e-412e-a45d-8180ff9b401a"),  // fanbl S23 ultra advertiser simulator
  BLEUUID("0000fffd-0000-1000-8000-00805f9b34fb")  // BLE login key
};

static std::string MANUFACTURER_DATA[] = {
  "4c00121990fb76a061d0b35048f1b1bada2b37a15badd29ef06823037c"  // air tag
};  // useless

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

bool isDeviceDetected(BLEAdvertisedDevice& device) {
  if (device.getRSSI() > CUTOFF) {
    ESP_LOGI(TAG, "Nearby device found: %s\n", device.toString().c_str());
    //  Serial.printf("In loop device found %s\n", device.toString().c_str());
    for (BLEUUID& uuid : UUIDS_TO_FIND) {
      if (device.isAdvertisingService(uuid)) {
        ESP_LOGW(TAG, "Detected Device by UUID: %s\n", device.toString().c_str());
        return true;
      }
    }
    for (const std::string& manufactorer_data : MANUFACTURER_DATA) {
      if (device.getManufacturerData() == manufactorer_data) {
        ESP_LOGW(TAG, "Detected Device by manufactorer data: %s\n", device.toString().c_str());
        return true;
      }
    }
  }
  return false;
}

void loop() {
  BLEScan *scan = BLEDevice::getScan();
  scan->setActiveScan(true);
  BLEScanResults results = scan->start(1);
  bool found = false;
  for (int i = 0; i < results.getCount(); i++) {
    BLEAdvertisedDevice device = results.getDevice(i);
    if (isDeviceDetected(device)) {
      found = true;
      break;
    }
  }
  // digitalWrite(PIN, best > CUTOFF ? HIGH : LOW);
  digitalWrite(PIN, found ? HIGH : LOW);
}