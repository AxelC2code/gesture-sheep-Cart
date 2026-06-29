#include <ESP8266WiFi.h>
#include <espnow.h>

// Pines de los motores
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4

// Estructura de recepción
typedef struct struct_message {
  char gesture;
} struct_message;

struct_message msg;

// Variables globales volátiles para el control del bucle
volatile char gestoActual = 'S'; 
unsigned long ultimoTiempoDatos = 0;

// =========================================================================
// CALLBACK: Atrapa el dato a la velocidad de la luz y sale. Sin bloqueos.
// =========================================================================
void onDataReceive(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&msg, incomingData, sizeof(msg));
  gestoActual = msg.gesture;        // Actualiza el gesto al instante
  ultimoTiempoDatos = millis();     // Reinicia el cronómetro de seguridad
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Modo estación (Receptor)
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Registrar el callback de recepción
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataReceive);
}

void loop() {
  // 1. Monitoreo Inteligente (Solo imprime cuando cambia el gesto para no crear lag)
  static char ultimoGestoImpreso = ' ';
  if (gestoActual != ultimoGestoImpreso) {
    Serial.print("Gesto ejecutando: ");
    Serial.println(gestoActual);
    ultimoGestoImpreso = gestoActual;
  }

  // 2. Sistema Failsafe (Perro Guardián anti-choques)
  // Si pasan más de 500 milisegundos sin recibir un paquete WiFi, forzamos un STOP
  if (millis() - ultimoTiempoDatos > 500) {
    gestoActual = 'S'; 
  }

  // 3. Ejecución fluida en el bucle principal
  switch (gestoActual) {
    case 'F': forward();  break;
    case 'B': backward(); break;
    case 'L': left();     break;
    case 'R': right();    break;
    case 'S': stop();     break;
  }

  // Pausa minúscula para dar estabilidad al procesador
  delay(10); 
}

// Funciones de control de motores (Intactas)
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
