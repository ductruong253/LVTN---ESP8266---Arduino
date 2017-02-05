#include <DHT.h>
#include <ESP8266WiFi.h>

#define DHTPIN 5
#define DHTTYPE DHT22

const char* ssid     = "Truong";
const char* password = "12345678";

const char* host = "192.168.1.107";
const char* streamId   = "....................";
const char* privateKey = "....................";
WiFiClient client;
DHT dht(DHTPIN,DHTTYPE);
void setup() {
  Serial.begin(115200);
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
  dht.begin();
}
void loop(){
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int temp = (int) (t*100);
  int humid = (int) (h*100);
  String tempchar = String(temp);
  String humidchar = String(humid);

  /*debugging
  int t = 25;
  int h = 75;
  */

  
  delay(100);
  digitalWrite(D4, LOW);
  Serial.print("connecting to ");
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 3000;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String url = "/sensor/";
  url += tempchar;
  url += "/";
  url += humidchar;
  if ((t <100) && (h<100)){
    Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  }
  
  digitalWrite(D4, HIGH);
  ESP.deepSleep(5000000, WAKE_RF_DEFAULT);
}


