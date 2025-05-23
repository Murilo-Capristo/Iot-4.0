#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


const char* ssid = "Wokwi-GUEST";
const char* password = "";

IPAddress mqtt_server(104, 41, 50, 188);
WiFiClient espClient;
PubSubClient client(espClient);

const int buttonPin = 4;
bool lastState = HIGH;

void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("esp32-client", "admin", "otm-password-VM#")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(". Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando...");
  pinMode(buttonPin, INPUT_PULLUP);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}



void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  bool state = digitalRead(buttonPin);
  if (lastState == HIGH && state == LOW) {
    String uid = "TAG123456"; // Simula leitura RFID
    client.publish("rfid-moto/leituras", uid.c_str());
    Serial.println("UID enviado: " + uid);

  }

  lastState = state;
  delay(50);
}
