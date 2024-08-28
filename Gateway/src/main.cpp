#define RH_MESH_MAX_MESSAGE_LEN 50

//#include <Arduino.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ctype.h>

// In this small artifical network of 4 nodes,
#define BRIDGE_ADDRESS 1  // address of the bridge ( we send our data to, hopefully the bridge knows what to do with our data )

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

#define RXTIMEOUT 3000  // it is roughly the delay between successive transmissions

// Singleton instance of the radio driver
RH_RF95 rf95(LLG_CS, LLG_DI0); // slave select pin and interrupt pin, [heltec|ttgo] ESP32 Lora OLED with sx1276/8

// Class to manage message delivery and receipt, using the driver declared above
RHMesh manager(rf95, BRIDGE_ADDRESS);

//Di seguito vanno inserite ssid e password della rete wifi a cui è connesso il server e di conseguenza anche l'ESP
const char* ssid = "Lele";
const char* password = "iefr5972";
const char* serverAddress = "192.168.90.142"; //equivale all'indirizzo IP privato del pc su cui gira il server
const uint16_t serverPort = 8080;

String message;


void setup() 
{
  Serial.begin(115200);
  
  Serial.print(F("initializing node "));
  Serial.print(BRIDGE_ADDRESS);
  SPI.begin(LLG_SCK,LLG_MISO,LLG_MOSI,LLG_CS);
  if (!manager.init())
    {Serial.println(" init failed");} 
  else
    {Serial.println(" done");}  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36 

  rf95.setTxPower(2, false); // with false output is on PA_BOOST, power from 2 to 20 dBm, use this setting for high power demos/real usage
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
    // Predefined configurations( bandwidth, coding rate, spread factor ):
    // Bw125Cr45Sf128     Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
    // Bw500Cr45Sf128     Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
    // Bw31_25Cr48Sf512   Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
    // Bw125Cr48Sf4096    Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, low data rate, CRC on. Slow+long range
    // Bw125Cr45Sf2048    Bw = 125 kHz, Cr = 4/5, Sf = 2048chips/symbol, CRC on. Slow+long range
    if (!rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128))
      {Serial.println(F("set config failed"));}
    }
  
  //sending to server using http
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    
  }
  if(WiFi.status() == WL_CONNECTED){
      Serial.println("Connected to WiFi");
  }
  Serial.println("RF95 ready");
}

uint8_t data[] = "Hello back from bridge";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
uint8_t res;

void loop()
{
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager.recvfromAck(buf, &len, &from)){
    Serial.print("request from node n.");
    Serial.print(from);
    Serial.print(": ");
    Serial.print((char*)buf);
    Serial.print(" rssi: ");
    Serial.println(rf95.lastRssi()); 

    //converto la stringa ricevuta per inviarla al server
    char msg[RH_MESH_MAX_MESSAGE_LEN];
    memcpy(msg, buf, RH_MESH_MAX_MESSAGE_LEN);
    message = (String)msg;
    Serial.println("Conversione: ");
    Serial.print(message);
  }


  if(message.length() > 0){
  

    if(WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      
      if(http.begin(serverAddress, serverPort, "/dashboard.php") == true){
        Serial.println("\n Connected to server\n");
      }else{
        Serial.println("Error: couldn't connect to server\n");
      }
      
      int httpResponseCode = http.POST(message);
      
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        //queste due righe servono a stampare l'intero html della pagina web a cui ci siamo connessi
        //String response = http.getString(); 
        //Serial.println(response); 
        //ho commentato perchè riempie inutilmente ed eccessivamente il terminale
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      
      message = "";
      //delay(5000);  // Attendi 5 secondi prima di inviare una nuova richiesta
      http.end();
    }
    
  }

}