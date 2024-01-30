/* Copyright (c) 2018 pcbreflux. All Rights Reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>. *
 */
 
/*
   Create a BLE server that will send periodic eddystone frames.
   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create advertising data
   3. Start advertising.
   4. wait
   5. Stop advertising.
   6. deep sleep
   
*/
#include "Arduino.h"
#include "sys/time.h"

#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "esp_sleep.h"

#define GPIO_DEEP_SLEEP_DURATION     1  // sleep 4 seconds and then wake up
RTC_DATA_ATTR static time_t last;        // remember last boot in RTC Memory
RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory
uint32_t chipID = 0x00;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
BLEAdvertising *pAdvertising;
struct timeval now;
  
void setBeacon() {

  uint64_t macAddress = ESP.getEfuseMac();
  uint64_t macAddressTrunc = macAddress << 40;
  chipID = macAddressTrunc >> 40;

  uint16_t beaconUUID = 0xFEAA;
  uint16_t volt = 3300; // 3300mV = 3.3V
  uint16_t temp = (uint16_t)((float)23.00);
  uint32_t tmil = now.tv_sec*10;
  
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  
  oAdvertisementData.setFlags(0x06); // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
  oAdvertisementData.setCompleteServices(BLEUUID(beaconUUID));

  // NAMESPACE is 10 bytes "FPVTEAMGOE" -> 70 80 86 84 69 65 77 71 79 69
  char beacon_data[20] = { // must be 20 bytes
      0x00, // Eddystone Frame Type (Eddystone-UID)
      0x20, // calibrated Tx Power at 0m
      0x70, // 10-byte Namespace
      0x80, // 10-byte Namespace
      0x86, // 10-byte Namespace
      0x84, // 10-byte Namespace
      0x69, // 10-byte Namespace
      0x65, // 10-byte Namespace
      0x77, // 10-byte Namespace
      0x71, // 10-byte Namespace
      0x79, // 10-byte Namespace
      0x69, // 10-byte Namespace
      0x00, // 6-byte Instance
      0x00, // 6-byte Instance
      0x00, // 6-byte Instance
      0x00, // 6-byte Instance
      0x00, // 6-byte Instance
      0x00, // 6-byte Instance
      0x00, // RFU, must be 0x00
      0x00, // RFU, must be 0x00
  };

  // Set macAddress as Instance-ID
  beacon_data[12] = (macAddress >> (8*0)) & 0xff;
  beacon_data[13] = (macAddress >> (8*1)) & 0xff;
  beacon_data[14] = (macAddress >> (8*2)) & 0xff; 
  beacon_data[15] = (macAddress >> (8*3)) & 0xff; 
  beacon_data[16] = (macAddress >> (8*4)) & 0xff; 
  beacon_data[17] = (macAddress >> (8*5)) & 0xff; 
    
  oAdvertisementData.setServiceData(BLEUUID(beaconUUID), std::string(beacon_data, 20));
  
  pAdvertising->setScanResponseData(oAdvertisementData);

}

void setup() {

  Serial.begin(460800);
  delay(1000);

  gettimeofday(&now, NULL);

  Serial.printf("start ESP32 %d\n",bootcount++);

  Serial.printf("deep sleep (%lds since last reset, %lds since last boot)\n",now.tv_sec,now.tv_sec-last);

  last = now.tv_sec;
  
  // // Create the BLE Device
  BLEDevice::init("FPVRaceTracker - Gö");

  // // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();

  pAdvertising = pServer->getAdvertising();
  
  setBeacon();
  //  Start advertising
  pAdvertising->start();
  Serial.println("Advertizing started...");
  // delay(10000);
  // Serial.printf("enter deep sleep\n");
  // esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
  // Serial.printf("in deep sleep\n");
}

void loop() {
  gettimeofday(&now, NULL);
  Serial.printf("alive %lds\n", now.tv_sec-last);
  delay(1000);

}
