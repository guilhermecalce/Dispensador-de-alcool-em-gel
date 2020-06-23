
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#include <Servo.h>

// Update these with values suitable for your network.

int pos=0;

Servo myservo; 

const char* ssid = "Mateus";  // nome da rede de wifi
const char* password = "mateus18"; // senha da rede de wifi
const char* mqtt_server = "broker.mqtt-dashboard.com"; // url do broker

SoftwareSerial s(16,14);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String inString = "";
bool enviado = true;
    
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  inString="";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    inString+=(char)payload[i];
  }
  Serial.println();

   pos=inString.toInt();   
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("mateus/hello", "ESP Conectado");
      // ... and resubscribe
      client.subscribe("mateus/sen/disp");
      client.subscribe("mateus/sen/respin");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(16,OUTPUT);  //D6
  myservo.attach(14);//D5
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    digitalWrite(16,1);
    snprintf (msg, 75, "%d", pos);   
    if(enviado){
      client.publish("mateus/sen/disp", "2");
      client.publish("mateus/sen/respin","baixo");
      enviado = false;
    }
  }
}
