/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE"
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second.
*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// init cloudPlatform
Adafruit_PWMServoDriver cloudPlatform = Adafruit_PWMServoDriver(0x40);

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLE_NAME "AITracker"

// define variavles for cloudplatfrom(pca9685)
#define SERVOMIN  150 
#define SERVOMAX  600 
#define USMIN  500  
#define USMAX  2500 
#define SERVO_FREQ 50
#define BOTTOMSEVROID 0
#define TOPSEVROID 1


int angleToPulse(int ang){
   int pulse = map(ang,0, 270, SERVOMIN, SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max
   Serial.print("Angle: ");Serial.print(ang);
   Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}

void setBottomServoAngle(int angle){
  Serial.print("Bottom: ");
  cloudPlatform.setPWM(BOTTOMSEVROID, 0, angleToPulse(angle));
}

void setTopServoAngle(int angle){
  Serial.print("Top: ");
  cloudPlatform.setPWM(TOPSEVROID, 0, angleToPulse(angle));
}


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    int bottomAngle=0;
    int topAngle=0;

    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);
          
        Serial.println();
        Serial.println("*********");
      }
      
      // check is angle info, example: "angle:180,180"
      if (rxValue.rfind("angle:", 0)==0){
        Serial.println("input angle info");
        // std::string angleStr = rxValue.substr(6,rxValue.length());
       int pos= rxValue.find(",");

       std::string  bottomAngleStr = rxValue.substr(6,pos);
       std::string  topAngleStr = rxValue.substr(pos+1,rxValue.length());

       Serial.print("bottomAngleStr:");  Serial.println(bottomAngleStr.c_str());
       Serial.print("topAngleStr:");  Serial.println(topAngleStr.c_str());

       bottomAngle=atoi(bottomAngleStr.c_str());
       topAngle=atoi(topAngleStr.c_str());

       Serial.print("bottomAngle:");  Serial.println(bottomAngle);
       Serial.print("topAngle:");  Serial.println(topAngle);

        setBottomServoAngle(bottomAngle);
        setTopServoAngle(topAngle);
      }

    }
};

void initCloudPlatform() {
  Serial.println("CloudPlatform init start");
  
  cloudPlatform.begin();
  cloudPlatform.setOscillatorFrequency(27000000);
  cloudPlatform.setPWMFreq(SERVO_FREQ); 
  
  Serial.println("CloudPlatform init finish");
}

void initBLE(){
  // Create the BLE Device
  BLEDevice::init(BLE_NAME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void setup() {
  Serial.begin(115200);

  initCloudPlatform();
  initBLE();
}


void loop() {

    if (deviceConnected) {
        pTxCharacteristic->setValue(&txValue, 1);
        pTxCharacteristic->notify();
        txValue++;
		    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	}

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}