#include <Wire.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <EloquentTinyML.h>
// #include "model.h"

// #define TENSOR_SIZE color_model_tflite_len

// Eloquent::TinyML::TfLite<3, 1, TENSOR_SIZE> ml;

BLEServer* pServer = NULL;
BLECharacteristic* pHumidityCharacteristic = NULL;
BLECharacteristic* pTemperatureCharacteristic = NULL;
BLECharacteristic* pGyroscopeCharacteristic = NULL;
BLECharacteristic* pHighValuesCharacteristic = NULL;
BLEService *pService = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t value = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define HUMIDITY_UUID       "2A6F" 
#define TEMPERATURE_UUID    "2A6E"
#define GYROSCOPE_UUID      "A3C87502-8ED3-4BDF-8A39-A01BE548C8D6" 
#define HIGH_VALUES_UUID    "4845e9f7-c0f2-475c-a2cb-2d2859ed6f39"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("Device connected");
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("Device disconnected");
    }
};

void setup() {
    Serial.begin(115200);
    BLEDevice::init("Tracker ESP32");

    // if (!ml.begin(color_model_tflite)) {
    //   Serial.println("Failed to initialize model!");
    //   while (true){ }
    // }

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    pService = pServer->createService(SERVICE_UUID);

    pHumidityCharacteristic = pService->createCharacteristic(
        HUMIDITY_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    BLEDescriptor *pHumidityDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
    pHumidityDescriptor->setValue("Humidity");
    pHumidityCharacteristic->addDescriptor(pHumidityDescriptor);

    pTemperatureCharacteristic = pService->createCharacteristic(
        TEMPERATURE_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    BLEDescriptor *pTemperatureDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
    pTemperatureDescriptor->setValue("Temperature");
    pTemperatureCharacteristic->addDescriptor(pTemperatureDescriptor);

    pGyroscopeCharacteristic = pService->createCharacteristic(
        GYROSCOPE_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    BLEDescriptor *pGyroscopeDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901)); // Usa lo stesso UUID del descrittore standard
    pGyroscopeDescriptor->setValue("Gyroscope");
    pGyroscopeCharacteristic->addDescriptor(pGyroscopeDescriptor);

    pHighValuesCharacteristic = pService->createCharacteristic(
        HIGH_VALUES_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    BLEDescriptor *pHighValuesDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
    pHighValuesDescriptor->setValue("HighValues");
    pHighValuesCharacteristic->addDescriptor(pHighValuesDescriptor);

    pService->start();
    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify...");
}

float inputs[3];
// float outputs[2] = {0,0};

void loop() {
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("Start advertising");
        oldDeviceConnected = deviceConnected;
    }
    
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }

    if (deviceConnected) {
        float humidity = random(0, 101); // Valore random umidità tra 0 e 100
        String humidityStr = String(humidity);
        pHumidityCharacteristic->setValue(humidityStr.c_str());
        pHumidityCharacteristic->notify();
        Serial.printf("Humidity: %.1f%% \n", humidity);
        inputs[0] = humidity;

        float temperature = random(20, 40); // Valore random temperatura tra 20 e 40 gradi Celsius
        String temperatureStr = String(temperature);
        pTemperatureCharacteristic->setValue(temperatureStr.c_str());
        pTemperatureCharacteristic->notify();
        Serial.printf("Temperature: %.1f°C \n", temperature);
        inputs[1] = temperature;

        float gyroscopeValue = random(0, 360); // Valore random giroscopio tra 0 e 360 degrees
        String gyroscopeStr = String(gyroscopeValue);
        pGyroscopeCharacteristic->setValue(gyroscopeStr.c_str());
        pGyroscopeCharacteristic->notify();
        Serial.printf("Gyroscope: %.1f° \n", gyroscopeValue);
        inputs[2] = gyroscopeValue;

        // ml.predict(inputs, outputs);
        // Serial.println(outputs[0]);
        // Serial.println(outputs[1]);

        // bool damaged = outputs[0] > 0.8 ? true : false;
        bool damaged = random(0,2);
        pHighValuesCharacteristic->setValue(std::to_string(damaged));
        pHighValuesCharacteristic->notify();
        Serial.printf("Damage value: %d \n", damaged);

        delay(5000);
    }
}
