#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi/MQTT parameters
#define WLAN_SSID       "Fios-C5TUP"
#define WLAN_PASS       "ski0000gong8656run"
#define BROKER_IP       "192.168.1.222"

//pins
#define BUTTON 4
#define LED 5 //define 
bool buttonstate;
bool clickedflag = false;
bool ledon = false;

WiFiClient client;
PubSubClient mqttclient(client);

void callback (char* topic, byte* payload, unsigned int length) {
  Serial.println(topic);
  Serial.write(payload, length); //print incoming messages
  Serial.println("");

  payload[length] = '\0'; // add null terminator to byte payload so we can treat it as a string

  if (strcmp(topic, "/led") == 0){
     if (strcmp((char *)payload, "on") == 0){
        digitalWrite(LED, HIGH);
     } else if (strcmp((char *)payload, "off") == 0){
        digitalWrite(LED, LOW);
     }
  } 

  if(strcmp(topic, "/button") == 0){
    if(strcmp((char*)payload, "on") == 0){
      digitalWrite(LED, HIGH);
    } else if (strcmp((char *)payload, "off") == 0){
      digitalWrite(LED, LOW);
    }
  }

}


void setup() {
  Serial.begin(115200);
  
  // connect to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // connect to mqtt server
  mqttclient.setServer(BROKER_IP, 1883);
  mqttclient.setCallback(callback);
  connect();

  //setup pins
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT); // setup pin for input

}

void loop() {
  if (!mqttclient.connected()) {
    connect();
  }
  
  mqttclient.loop();

  buttonstate = digitalRead(BUTTON);

  if(buttonstate and not clickedflag){
    clickedflag = true;
  }

  if(not buttonstate and clickedflag){

    clickedflag = false;
    Serial.println("clicked");
    if(not ledon)
    {
      mqttclient.publish("/led", "on", false);
      ledon = true;
    }
    else
    {
      mqttclient.publish("/led", "off", false);
      ledon = false;
    }
  }
  
}


void connect() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("Wifi issue"));
    delay(3000);
  }
  Serial.print(F("Connecting to MQTT server... "));
  while(!mqttclient.connected()) {
    if (mqttclient.connect(WiFi.macAddress().c_str())) {
      Serial.println(F("MQTT server Connected!"));

       mqttclient.subscribe("/led");
       mqttclient.subscribe("/button");
      
    } else {
      Serial.print(F("MQTT server connection failed! rc="));
      Serial.print(mqttclient.state());
      Serial.println("try again in 10 seconds");
      // Wait 5 seconds before retrying
      delay(20000);
    }
  }
}
