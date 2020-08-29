#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Martin router king";
const char* password = "zacuscabuna";
String host = "b33227b1-f656-4710-9e7d-a2d658015935.mock.pstmn.io";
const int httpPort = 80;
String url = "/stats";

void setup()
{
  Serial.begin(115200);
  delay(10); // We start by connecting to a WiFi network
  //  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default, would try to act as both a client and an access-point and could cause network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}

void loop()
{
  String postData = Serial.readString();
  while (postData == "") {
    postData = Serial.readString();
  }
  WiFiClient client;
  HTTPClient http;

  if (!client.connect(host, httpPort))
  {
    return;
  }

  String address = "http://" + host + url;

  http.begin(client, address);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(postData);
  String payload = http.getString();
  payload.replace("\n", "");
  payload.replace("\r", "");
  payload.replace("\t", "");
  payload.replace(" ", "");
  Serial.println(payload); //Print request response payload
  http.end();
}
