/*
  WiFi Web Server with Sensor Readings (US Customary Units, Larger Font)

  This code sets up an access point and serves sensor data via HTTP.
*/

#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define TRIGGER_PIN 9
#define ECHO_PIN 8
#define IR_SENSOR_PIN A0

const char* ssid = "ArduinoAP";
const char* password = "";
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  dht.begin();

  WiFi.beginAP(ssid);
  Serial.println("AP Started");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

// Function to read HC-SR04 sensor and return distance in inches
float readHCSR04() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.0133; // Convert cm to inches
}

// Function to read temperature in Fahrenheit
float readTemperatureF() {
  float tempC = dht.readTemperature();
  return (tempC * 9.0 / 5.0) + 32.0; // Convert Celsius to Fahrenheit
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();
    String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    
    // Add CSS styles for larger font
    response += "<html><head><style>";
    response += "body { font-family: Arial, sans-serif; text-align: center; }";
    response += "h1 { font-size: 24px; }";
    response += "p { font-size: 20px; }";
    response += "a { font-size: 22px; text-decoration: none; color: blue; }";
    response += "a:hover { color: darkblue; }";
    response += "</style></head><body>";

    response += "<h1>Sensor Data</h1>";

    if (request.indexOf("GET /hc-sr04") >= 0) {
      response += "<p>Ultrasonic Sensor: <strong>" + String(readHCSR04()) + " in</strong></p>";
    } else if (request.indexOf("GET /hw-488") >= 0) {
      response += "<p>IR Sensor: <strong>" + String(analogRead(IR_SENSOR_PIN)) + "</strong></p>";
    } else if (request.indexOf("GET /dht-11") >= 0) {
      response += "<p>Temperature: <strong>" + String(readTemperatureF()) + " F</strong></p>";
      response += "<p>Humidity: <strong>" + String(dht.readHumidity()) + " %</strong></p>";
    } else {
      // Home page with sensor links
      response += "<p><a href='/hc-sr04'>Ultrasonic Sensor</a></p>";
      response += "<p><a href='/hw-488'>IR Sensor</a></p>";
      response += "<p><a href='/dht-11'>Temperature & Humidity</a></p>";
    }

    response += "</body></html>";

    client.print(response);
    client.stop();
  }
}
