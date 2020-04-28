#include "MAX30105.h"
#include "heartRate.h"
#include <Wire.h>
  
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi/MQTT parameters
#define WLAN_SSID       "Fios-C5TUP"
#define WLAN_PASS       "ski0000gong8656run"
#define BROKER_IP       "192.168.1.225"

//Pin parameters
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2

WiFiClient client;
PubSubClient mqttclient(client);

//MAX30102 sensor and related values.
MAX30105 fingerSensor;
int ahr; // Average heart rate.
float bpm; // Heartbeats per minute.
long lastBeat = 0; // Time since last detected heartbeat.
const byte SAMPLE_SIZE = 8;
byte samples[SAMPLE_SIZE];
byte s = 0; // Index for samples.

void setup() {
  Serial.begin(115200);

  // init pins
  //pinMode(BUTTON,INPUT);
  //pinMode(LED,OUTPUT);

  // init finger sensor
  // Defaults to GPIO4 for SDA and GPIO5 for SCL.
  fingerSensor.begin(Wire, I2C_SPEED_FAST);
  fingerSensor.setup();
  fingerSensor.setPulseAmplitudeRed(0x0A);

 
}

void loop() {
  if (!mqttclient.connected()) {
    connect();
  }
  
  long ir_value;
  int i;
  int sensor_value;
  char buffer[5];

  ir_value = fingerSensor.getIR(); // Read IR value.

  // Read and publish sensor value
  if (ir_value > 7000){ // Finger detected.
    if (checkForBeat(ir_value) == true) {
      long delta = millis() - lastBeat;
      lastBeat = millis();

      bpm = 60 / (delta/1000.0);
      /*
      Serial.print("heartbeat: ");
      Serial.print(bpm);
      Serial.println("");
      */
      if (bpm < 255 && bpm > 16) {
        samples[s++] = (byte) bpm;
        s %= SAMPLE_SIZE;
        // Take average of readings.
        ahr = 0; // Reset average heart rate.
        for (i = 0; i < SAMPLE_SIZE; i++) {
          ahr += samples[i];
        }
        ahr /= SAMPLE_SIZE;
      }
    }
    Serial.println(ahr);
    data = Integer.toString(ahr);
    mqttclient.publish("/data", data, false);
  } else { // No finder detected.
    //Serial.println(F("No finger detected..."));
    ahr = 0;
  }
 

}
