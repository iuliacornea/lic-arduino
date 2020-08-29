#include <ESP8266WiFi.h>

WiFiClient client;

const char* ssid     = "Martin router king";
const char* password = "zacuscabuna";

const char* host = "b33227b1-f656-4710-9e7d-a2d658015935.mock.pstmn.io";
const int httpPort = 80;
const char* url = "/stats";


void setup() {
  Serial.begin(115200);
  delay(10);
  // connect to WiFi network
  WiFi.begin(ssid, password);
  // wait for connection to WiFi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  if (!client.connect(host, httpPort)) {
    return;
  }
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
      return;
    }
  }
  Serial.println("Received response from client");
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

}
