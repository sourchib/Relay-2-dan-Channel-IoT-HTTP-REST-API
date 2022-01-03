#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
SoftwareSerial mySerial(D6,D7);  // (Rx,Tx  > Tx,Rx) 
#include <ArduinoJson.h>

////Delay Relay HTTP
//unsigned long lastTime = 0;
//unsigned long timerDelay = 5000;

char incomingByte; 
String inputString = "";
uint8_t relay1 = D5;
uint8_t relay2 = D4;
//#define relay2 D4

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//const char* ssid = "DSMNEW";
//const char* password = "bctm0306";

const char* ssid = "pong";
const char* password = "saya2133";

//Your Domain name with URL path or IP address with path
String serverName = "http://47.254.248.173/iot/service_lamp.php";

void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 5 seconds");
}

void SetupSim(){
   while(!mySerial.available()){
        mySerial.println("AT");
        delay(1000); 
        Serial.println("Connecting...");
        }
      Serial.println("Connected!");  
      mySerial.println("AT+CMGF=1");  //Set SMS to Text Mode 
      delay(5000);  
      mySerial.println("AT+CNMI=1,2,0,0,0");  //Procedure to handle newly arrived messages(command name in text: new message indications to TE) 
      delay(5000);
      mySerial.println("AT+CMGL=\"REC UNREAD\""); // Read Unread Messages
}

void setup() 
{
      pinMode(relay1, OUTPUT);
      pinMode(relay2, OUTPUT);
//      digitalWrite(relay1, LOW); // Initial state of the relay
      Serial.begin(9600);
      mySerial.begin(4800); 
      setup_wifi();
      SetupSim();
}

void loop()
{  
  SM800LSMS();
  SendHTTPGET();
}

void SendHTTPGET(){
  //Send an HTTP POST request every 10 minutes
//  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      String serverPath = serverName ;
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        char json[500];
        String payload = http.getString();
        payload.toCharArray(json, 500);
        StaticJsonDocument<384> doc;
        DeserializationError error = deserializeJson(doc, json);
        const char* token1=doc[1]["status_lampu"];
        String kondisilamp1 = String(token1);
        
        const char* token=doc[0]["status_lampu"];
        
        String kondisilamp = String(token);
        
        if (kondisilamp=="yes"){
        digitalWrite(relay1,HIGH);
        Serial.println("nyala");
        }
        else if (kondisilamp=="no"){
        digitalWrite(relay1,LOW);
        Serial.println("mati");
        }
        else if (kondisilamp1=="yes1"){
        digitalWrite(relay2,HIGH);
        Serial.println("nyala");
        }
        else if (kondisilamp1=="no1"){
        digitalWrite(relay2,LOW);
        Serial.println("mati");
        }
//        delay(100);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
//    lastTime = millis();
//  }
}

void SM800LSMS(){
    if(mySerial.available()){
      delay(100);

      // Serial Buffer
      while(Serial.available()){
        incomingByte = mySerial.read();
        inputString += incomingByte; 
        }

        delay(10);      
        mySerial.println(inputString);
        inputString.toUpperCase(); // Uppercase the Received Message

        //turn RELAY ON or OFF
        if (inputString.indexOf("ON_1") > -1){
          digitalWrite(relay1, HIGH);
        }
        else if (inputString.indexOf("OFF_1") > -1){
          digitalWrite(relay1, LOW);
        }      
        else if (inputString.indexOf("ON_2") > -1){
          digitalWrite(relay2, HIGH);
        }  
        else if (inputString.indexOf("OFF_2") > -1){
          digitalWrite(relay2, LOW);
        }else{
        }
        delay(100);
        //Delete Messages & Save Memory
        if (inputString.indexOf("OK") == -1){
        mySerial.println("AT+CMGDA=\"DEL ALL\"");
        delay(5000);}
        inputString = "";
  } 
}
