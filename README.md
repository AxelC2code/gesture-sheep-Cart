# Gesture Controlled Car with ESP8266 & ESP-NOW

Este proyecto implementa un vehículo robótico controlado por gestos de la mano. Utiliza dos microcontroladores **ESP8266** que se comunican de forma directa y con baja latencia mediante el protocolo **ESP-NOW**, eliminando la necesidad de módulos de radiofrecuencia externos o routers Wi-Fi.

## Tecnologías y Hardware Utilizado
* **Microcontroladores:** 2x ESP8266 (NodeMCU / Wemos D1 Mini)
* **Sensores:** Acelerómetro y Giroscopio MPU6050
* **Visualización:** Pantalla OLED SSD1306 (128x64) I2C
* **Actuadores:** Driver puente H L298N y 4x Motores DC
* **Comunicación:** Protocolo ESP-NOW (Direcciones MAC)

## Estructura del Proyecto

El repositorio se divide en tres componentes principales:

1.  `get_mac_address`: Script de utilidad para obtener la dirección MAC del ESP8266 receptor.
2.  `receiver_car`: Código para el vehículo. Recibe las instrucciones (F, B, L, R, S) y acciona los motores a través del puente H. Incluye un sistema *Failsafe* que detiene el coche si se pierde la conexión.
3.  `transmitter_glove`: Código para el mando vestible. Lee las inclinaciones del MPU6050, determina la dirección, muestra el estado en la pantalla OLED y envía el comando al receptor.

## Guía de Instalación y Uso

### Paso 1: Obtener la dirección MAC del Receptor
1. Conecta el ESP8266 que usarás en el vehículo.
2. Sube el código de la carpeta `get_mac_address`.
3. Abre el Monitor Serie (115200 baudios) y anota la dirección MAC impresa (ej. `24:D7:EB:EF:A3:C8`).

### Paso 2: Configurar el Transmisor (Guante)
1. Abre el archivo `transmitter_glove.ino`.
2. Reemplaza la variable `receiverAddress[]` con la dirección MAC que obtuviste en el Paso 1.
   ```cpp
   uint8_t receiverAddress[] = {0x24, 0xD7, 0xEB, 0xEF, 0xA3, 0xC8};
   
3. Verifica las conexiones I2C:

- SCL: D1

- SDA: D2

4. Sube el código al ESP8266 transmisor.

### Paso 3: Configurar el Receptor (Vehículo)
1. Abre el archivo receiver_car.ino.

2. Asegúrate de que los pines correspondan a tu conexión con el L298N (IN1=D1, IN2=D2, IN3=D3, IN4=D4).

3. Sube el código al ESP8266 del vehículo.

### ⚙️ Características Destacadas
- Cero Lag en Pantalla: Lógica de refresco optimizada que actualiza la pantalla OLED solo cuando hay un cambio de estado.

- Alta Frecuencia de Envío: Retardo ajustado a 20ms en el transmisor para enviar datos 50 veces por segundo.

- Perro Guardián (Failsafe): El coche se detiene automáticamente si no recibe paquetes en 500 milisegundos, evitando choques por pérdida de señal.
