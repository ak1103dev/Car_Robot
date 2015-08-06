#include <ESP8266WiFi.h>
#include <pt.h>
#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_WHILE(pt, millis()-ts < (ms));

struct pt Myclient;

const char* ssid     = "Robot-staff";
const char* password = "12345678";
const char* host = "192.168.1.117";
const int httpPort = 80;
String url = "/";

 WiFiClient client;
 
int motorValue = 0;
int motorR = 9;
int motorL = 10;

PT_THREAD(MyClient(struct pt* pt))
{
  static uint32_t ts;
  PT_BEGIN(pt);
  while (1)
  {
    PT_DELAY(pt, 100, ts);
  
  // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" + 
                "Connection: close\r\n\r\n");
    PT_DELAY(pt, 100, ts);

    int count = 0;
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      if (count == 6) {
        line.replace("\n", "");
        line.replace("\r", "");
        int micValue = line.toInt();
        Serial.print(micValue);
        motorValue = map(micValue, 0, 1024, 0, 255);
        analogWrite(motorL,motorValue);
        analogWrite(motorR,motorValue);
      }
      count++;
    }
  
    Serial.println();
    Serial.println("closing connection");
  }
  PT_END(pt);
}

void setup() {
  PT_INIT(&Myclient);
  Serial.begin(115200);
  pinMode(motorR, OUTPUT);
  pinMode(motorL, OUTPUT);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.connect(host, httpPort);
}

void loop() {
  MyClient(&Myclient);
}

