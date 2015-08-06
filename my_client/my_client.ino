#include <ESP8266WiFi.h>

const char* ssid     = "Robot-staff";
const char* password = "12345678";
const char* host = "192.168.1.117";

int motorValue = 0;
int motorR = 9;
int motorL = 10;

void setup() {
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
}

void loop() {
  delay(100);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/";
  
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);

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

