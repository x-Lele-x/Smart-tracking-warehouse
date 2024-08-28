#include <Arduino.h>
#include <BLEDevice.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>


// Definizioni UUID
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define HUMIDITY_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define TEMPERATURE_UUID    "2A6E"
#define GYROSCOPE_UUID "2A35"
#define HIGH_VALUES_UUID "4845e9f7-c0f2-475c-a2cb-2d2859ed6f39"

#define RH_MESH_MAX_MESSAGE_LEN 50

// In this small artifical network of 4 nodes,
#define BRIDGE_ADDRESS 1  // address of the bridge ( we send our data to, hopefully the bridge knows what to do with our data )
#define NODE_ADDRESS 4    // address of this node

// lilygo T3 v2.1.6
// lora SX1276/8
#define LLG_SCK 5
#define LLG_MISO 19
#define LLG_MOSI 27
#define LLG_CS  18
#define LLG_RST 23
#define LLG_DI0 26
#define LLG_DI1 33
#define LLG_DI2 32

#define LLG_LED_GRN 25

// oled
#define LLG_OLED_SDA 21
#define LLG_OLED_SCL 22

// tfcard
#define LLG_SD_CS   13
#define LLG_SD_MISO 2
#define LLG_SD_MOSI 15
#define LLG_SD_SCK  14

#define TXINTERVAL 3000 

//status conn = WAITING_BLE_CONNECTION;
BLEScan* pBLEScan;
bool deviceFound = false;
std::string message = "";
//uint8_t* data;


// Oggetto BLEAdvertisedDevice per memorizzare il dispositivo trovato
BLEAdvertisedDevice foundDevice;


unsigned long nextTxTime;

// Singleton instance of the radio driver
RH_RF95 rf95(LLG_CS, LLG_DI0); // slave select pin and interrupt pin, [heltec|ttgo] ESP32 Lora OLED with sx1276/8

// Class to manage message delivery and receipt, using the driver declared above
RHMesh manager(rf95, NODE_ADDRESS);



// Gestione connessione BLE
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // Controlla se il nome del dispositivo corrisponde al nome desiderato
    if (advertisedDevice.getName() == "Smartband ESP32") {
      BLEDevice::getScan()->stop();
      deviceFound = true;
      foundDevice = advertisedDevice; // Memorizza il dispositivo trovato
      Serial.println("Device found");
    }
  }
};

void setup(){

  Serial.begin(115200);

  Serial.printf("STARTING BLE CONNECTION \n");
  // Inizializzazione di BLE
  BLEDevice::init("Anchor 2");
  

  //Inizializzazione della rete radio Lora
  Serial.print(F("initializing node "));
  Serial.print(NODE_ADDRESS);
  SPI.begin(LLG_SCK,LLG_MISO,LLG_MOSI,LLG_CS);
  if (!manager.init())
    {Serial.println(" init failed");} 
  else
    {Serial.println(" done");}  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36 

  rf95.setTxPower(10, false); // with false output is on PA_BOOST, power from 2 to 20 dBm, use this setting for high power demos/real usage
  //rf95.setTxPower(1, true); // true output is on RFO, power from 0 to 15 dBm, use this setting for low power demos ( does not work on lilygo lora32 )
  rf95.setFrequency(868.0);
  rf95.setCADTimeout(500);

  // long range configuration requires for on-air time
  boolean longRange = false;
  if (longRange) 
    {
    // custom configuration
    RH_RF95::ModemConfig modem_config = {
      0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
      0xC4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
      0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
      };
    rf95.setModemRegisters(&modem_config);
    }
  else
    {
    if (!rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128))
      {Serial.println(F("set config failed"));}
    }
  Serial.println("RF95 ready");
  nextTxTime = millis();

}

std::string BLEconnection(BLEClient* pClient){

    std::string humidityValue;
    std::string tempValue;
    std::string gyroscopeValue;
    std::string highValues;

    //Lettura delle caratteristiche
    BLERemoteService* pRemoteService = pClient->getService(BLEUUID(SERVICE_UUID));
    if(pRemoteService != nullptr){
      //Lettura della caratteristica frequenza cardiaca
      BLERemoteCharacteristic* pHumidityCharacteristic = pRemoteService->getCharacteristic(BLEUUID(HUMIDITY_UUID));
      if(pHumidityCharacteristic != nullptr){
        humidityValue = pHumidityCharacteristic->readValue();
        Serial.print("BLE, HEART RATE: ");
        Serial.println(humidityValue.c_str());
      }
    }
    // Lettura caratteristica temperatura corporea
    BLERemoteCharacteristic* pTemperatureCharacteristic = pRemoteService->getCharacteristic(BLEUUID(TEMPERATURE_UUID));
    if (pTemperatureCharacteristic != nullptr) {
        tempValue = pTemperatureCharacteristic->readValue();
        Serial.print("BLE, BODY TEMPERATURE: ");
        Serial.println(tempValue.c_str());
    }
    // Lettura caratteristica pressione sanguigna
    BLERemoteCharacteristic* pGyroscopeCharacteristic = pRemoteService->getCharacteristic(BLEUUID(GYROSCOPE_UUID));
    if (pGyroscopeCharacteristic != nullptr) {
        gyroscopeValue = pGyroscopeCharacteristic->readValue();
        Serial.print("BLE, BLOOD PRESSURE: ");
        Serial.println(gyroscopeValue.c_str());
    }

    BLERemoteCharacteristic* pHighValuesCharacteristic = pRemoteService->getCharacteristic(BLEUUID(HIGH_VALUES_UUID));
    if (pHighValuesCharacteristic != nullptr) {
        highValues = pHighValuesCharacteristic->readValue();
        Serial.print("BLE, HIGH VALUE: ");
        Serial.println(highValues.c_str());
    }

    // Invio del messaggio
    if((humidityValue.length() > 0) && (tempValue.length() > 0) && (gyroscopeValue.length() > 0)){
      message = std::to_string(NODE_ADDRESS) + ":" + humidityValue + ":" + tempValue + ":" + gyroscopeValue + ":" + highValues + "\0";
      Serial.printf("BLE, SENDING MESSAGE TO ESP: %s \n", message.c_str());
    }

  return message;
}

//uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
uint8_t res;

void loop(){

  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(2, false);
  BLEClient* pClient;

  //Connessione al server BLE e lettura dei dati
  if(deviceFound){

    pClient = BLEDevice::createClient();
    pClient->connect(&foundDevice);
    delay(1000);
    message = BLEconnection(pClient);

    if(!pClient->isConnected()){
      deviceFound = false;
    }

  }
 

  //Gestione invio messaggio con LORA
  // send message every TXINTERVAL millisecs
  if (millis() > nextTxTime){
    nextTxTime += TXINTERVAL;
    Serial.print("Sending to bridge n.");
    Serial.print(BRIDGE_ADDRESS);
    Serial.print(" res=");

    if(deviceFound){
      Serial.println(message.c_str());
      String msg(message.c_str());
      uint8_t messageChar[msg.length()+1];

      memcpy(messageChar, message.c_str(), sizeof(messageChar));
      messageChar[sizeof(messageChar)]='\0';
      Serial.println((char*)messageChar);
      //Serial.printf("Srncpy: %s \n", messageChar);
      res = manager.sendtoWait(messageChar, sizeof(messageChar), BRIDGE_ADDRESS);

      //res = manager.sendtoWait(data, sizeof(data), BRIDGE_ADDRESS);
      Serial.println(res);

      if (res == RH_ROUTER_ERROR_NONE){
      // Data has been reliably delivered to the next node.
      // now we do...
        Serial.println("msg successfully shared"); 
      }
      else{
        // Data not delivered to the next node.
        Serial.println("sendtoWait failed. Are the bridge/intermediate mesh nodes running?");
      }
    }else{
      pClient = BLEDevice::createClient();
      pClient->connect(&foundDevice);
      delay(1000);
      message = BLEconnection(pClient);
      if(!pClient->isConnected()){
        deviceFound = false;
      }
    }
  }
  pClient->disconnect();
  
  // radio needs to stay always in receive mode ( to process/forward messages )
  // uint8_t len = sizeof(buf)/sizeof(uint8_t);
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager.recvfromAck(buf, &len, &from)){
    Serial.print("message from node n.");
    Serial.print(from);
    Serial.print(": ");
    Serial.print((char*)buf);
    Serial.print(" rssi: ");
    Serial.println(rf95.lastRssi()); 
  }

}