#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <MPU6050.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 // Usamos -1 porque compartimos el reset con el ESP8266
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;

// Actualiza tu direccion MAC:
uint8_t receiverAddress[] = {0x24, 0xD7, 0xEB, 0xEF, 0xA3, 0xC8}; 

// Estructura para enviar el dato
typedef struct struct_message {
  char gesture;
} struct_message;

struct_message msg;

char ultimoGesto = ' ';

void actualizarPantalla(char gesto) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 5);
  display.print("Gesto Activo:");
  display.setTextSize(2);
  display.setCursor(0, 25);

  switch (gesto) {
    case 'F': // Adelante
      display.print("ADELANTE");
      // Dibuja la punta de la flecha hacia ARRIBA y su cuerpo
      display.fillTriangle(96, 15, 81, 35, 111, 35, SSD1306_WHITE);
      display.fillRect(91, 35, 11, 20, SSD1306_WHITE);
      break;
      
    case 'B': // Atras
      display.print("ATRAS");
      // Dibuja la punta de la flecha hacia ABAJO y su cuerpo
      display.fillTriangle(96, 55, 81, 35, 111, 35, SSD1306_WHITE);
      display.fillRect(91, 15, 11, 20, SSD1306_WHITE);
      break;
      
    case 'L': // Izquierda
      display.print("IZQ");
      // Dibuja la punta de la flecha hacia la IZQUIERDA y su cuerpo
      display.fillTriangle(70, 35, 90, 20, 90, 50, SSD1306_WHITE);
      display.fillRect(90, 30, 25, 11, SSD1306_WHITE);
      break;
      
    case 'R': // Derecha
      display.print("DER");
      // Dibuja la punta de la flecha hacia la DERECHA y su cuerpo
      display.fillTriangle(115, 35, 95, 20, 95, 50, SSD1306_WHITE);
      display.fillRect(70, 30, 25, 11, SSD1306_WHITE);
      break;
      
    case 'S': // Stop
      display.print("STOP");
      // Dibuja un cuadrado central imitando una señal de alto
      display.fillRect(81, 20, 30, 30, SSD1306_WHITE);
      break;
  }
  
  display.display();
}
// =========================================================================

void setup() {
  Serial.begin(115200); //ajusta los baudios
  
  // Definimos explicitamente los pines D2 (SDA) y D1 (SCL)
  Wire.begin(D2, D1); 

  delay(500); 

  mpu.initialize();
  bool mpuConectado = mpu.testConnection();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED no encontrada. Revisa las conexiones."));
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    if(!mpuConectado) {
      // Si el giroscopio falla, mostramos el error en la pantalla OLED
      Serial.println("¡MPU6050 NO conectado! Revisa los cables.");
      display.setCursor(0, 15);
      display.print("ERROR I2C:");
      display.setCursor(0, 35);
      display.print("MPU6050 No Detectado");
      display.setCursor(0, 50);
      display.print("Revisa SDA(D2)/SCL(D1)");
      display.display();
      
      while (1) {
        yield(); // El comando yield() evita que el ESP8266 se reinicie mostrando letras raras
      }
    } else {
      // Si ambos estan bien, muestra un mensaje de bienvenida
      display.setCursor(15, 20);
      display.setTextSize(2);
      display.print("Iniciando");
      display.display();
      delay(2000);
    }
  }

  // Iniciar el WiFi para ESP-NOW
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Registrar emparejamiento con el carro
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);   // Leer los valores fisicos del giroscopio

  // Detectar la inclinacion
  if (ax > 10000)       msg.gesture = 'F';  // Forward
  else if (ax < -10000) msg.gesture = 'B';  // Backward
  else if (ay > 10000)  msg.gesture = 'L';  // Left
  else if (ay < -10000) msg.gesture = 'R';  // Right
  else                  msg.gesture = 'S';  // Stop

  esp_now_send(receiverAddress, (uint8_t *)&msg, sizeof(msg));

  // OPTIMIZACION DE VELOCIDAD: Solo actualiza la pantalla si el gesto ha cambiado.
  if (msg.gesture != ultimoGesto) {
    actualizarPantalla(msg.gesture);
    ultimoGesto = msg.gesture;
  }

  Serial.print("Gesture sent: ");
  Serial.println(msg.gesture);

  delay(20); 
}