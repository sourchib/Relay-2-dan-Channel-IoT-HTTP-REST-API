#include <SoftwareSerial.h>
SoftwareSerial SIM800L(D1, D2);
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

//const char* ssid = "DSMNEW";
//const char* password = "bctm0306";

//const char* ssid = "pong";
//const char* password = "saya2133";

const char *ssid = "Dwi Dayak 2";
const char *password = "linalevi";

//Your Domain name with URL path or IP address with path
String serverName = "http://47.254.248.173/iot/service_lamp.php";

int _timeout;
String _buffer;
String textMessage;

String incomingByte;
String inputString;

//Relay 2 Channel WiFi dan SMS
uint8_t relay1 = D0;
uint8_t relay2 = D3;

String status1, status2;

void setup()
{
  // Set relay as OUTPUT
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW); // Initial state of the relay
  digitalWrite(relay2, LOW); // Initial state of the relay
  delay(1000);               //delay for 7 seconds to make sure the modules get the signal
  Serial.begin(115200);
  //  WiFi.begin(ssid, password);
  setup_wifi();
  _buffer.reserve(50);
  Serial.println("Sistem Started...");
  SIM800L.begin(115200);
  delay(100);
  Serial.println("SIM800L Ready");
  RecieveMessage();
}

void loop()
{

  //  if (Serial.available() > 0)
  //    switch (Serial.read())
  //    {
  //      case 1:
  //      Relaysms();
  //      break;
  //    }

   Relaysms();
  
  if (SIM800L.available() > 0)
    Serial.write(SIM800L.read());
  // HttpSend();
}

void RecieveMessage()
{
  Serial.println("SIM800L Read an SMS");
  delay(500);
  SIM800L.println("AT+CMGF=1\r");
  // Set module to send SMS data to serial out upon receipt
  SIM800L.println("AT+CNMI=2,2,0,0,0\r");
  Serial.write("Unread Message done");
}

void Relaysms()
{
  if (SIM800L.available() > 0)
  {
    status1 = SIM800L.read();
    Serial.println(inputString);
    status1.toUpperCase(); // Uppercase the Received Message
    Serial.println("status 1 : " + status1);
    if(status1 == "on1") {
      Serial.println("relay 1 ON");
    }
  }
}

void HttpSend()
{
  //Send an HTTP POST request every 10 minutes
  //  if ((millis() - lastTime) > timerDelay) {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClient client;
    HTTPClient http;
    String serverPath = serverName;
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
      char json[200];
      String payload = http.getString();
      payload.toCharArray(json, 200);
      StaticJsonDocument<2048> doc;
      DeserializationError error = deserializeJson(doc, json);

      const char *token = doc[0]["status_lampu"];
      const char *token1 = doc[1]["status_lampu"];

      String kondisilamp = String(token);
      String kondisilamp1 = String(token1);

      if (kondisilamp == "off")
      {
        digitalWrite(relay1, LOW);
      }
      else if (kondisilamp == "on")
      {
        digitalWrite(relay1, HIGH);
      }
      if (kondisilamp1 == "off")
      {
        digitalWrite(relay2, LOW);
      }
      else if (kondisilamp1 == "on")
      {
        digitalWrite(relay2, HIGH);
      }
      delay(100);
    }
    else
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
  //    lastTime = millis();
  //  }
}

void setup_wifi()
{
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 5 seconds");
}
