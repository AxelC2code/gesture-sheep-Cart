# Gesture Controlled Car with ESP8266 & ESP-NOW

Este proyecto implementa un vehículo robótico controlado por gestos de la mano. Utiliza dos microcontroladores ESP8266 que se comunican de forma directa y con baja latencia mediante el protocolo ESP-NOW, eliminando la necesidad de módulos de radio adicionales o routers Wi-Fi.

## Tecnologías y hardware utilizado

- Microcontroladores: 2x ESP8266 (NodeMCU / Wemos D1 Mini)
- Sensores: MPU6050 (acelerómetro + giroscopio)
- Visualización: pantalla OLED SSD1306 de 128x64 con interfaz I2C
- Actuadores: driver L298N y 4 motores DC
- Comunicación: ESP-NOW con direcciones MAC

## ¿Qué hace este proyecto?

El sistema está dividido en dos partes:

- Un transmisor montado en un guante, que detecta la inclinación de la mano y envía un gesto al vehículo.
- Un receptor instalado en el carro, que interpreta el gesto y activa los motores correspondientes.

Los gestos reconocidos son:

- F: adelante
- B: atrás
- L: izquierda
- R: derecha
- S: stop

## Estructura del repositorio

- code/get_mac_address/get_mac_address.ino: utilidad para obtener la dirección MAC del ESP8266 receptor.
- code/receiver_car/receiver_car.ino: firmware del vehículo, control de motores y sistema failsafe.
- code/transmitter_glove/transmitter_glove.ino: firmware del transmisor, lectura del MPU6050, pantalla OLED y envío por ESP-NOW.
- test/eficiencia_i2c.py: script para modelar la eficiencia del bus I2C y generar una gráfica de comparación.
- test/grafica_latencia_real.py: script para simular y visualizar la mejora de latencia del código optimizado.
- src/main.tex: fuente LaTeX para el informe o documentación del proyecto.
- requirements.txt: dependencias de Python necesarias para los scripts de análisis.
- output/: carpeta destinada al PDF generado.

## Requisitos

### Hardware

- 2 ESP8266
- MPU6050
- OLED SSD1306
- Driver L298N
- 4 motores DC
- Fuente de alimentación adecuada para el carro y guante

### Software

- Arduino IDE
- Librerías Arduino:
  - ESP8266WiFi
  - espnow
  - Wire
  - MPU6050
  - Adafruit_GFX
  - Adafruit_SSD1306
- Python 3.x

Instala las dependencias de Python con:

```bash
pip install -r requirements.txt
```

## Guía de instalación y uso

### Paso 1: obtener la dirección MAC del receptor

1. Conecta el ESP8266 que usarás en el vehículo.
2. Sube el código de la carpeta code/get_mac_address.
3. Abre el monitor serial a 115200 baudios y anota la dirección MAC que se imprima.

### Paso 2: configurar el transmisor (guante)

1. Abre el archivo code/transmitter_glove/transmitter_glove.ino.
2. Reemplaza la variable receiverAddress[] con la dirección MAC obtenida en el paso anterior.

```cpp
uint8_t receiverAddress[] = {0x24, 0xD7, 0xEB, 0xEF, 0xA3, 0xC8};
```

3. Verifica las conexiones I2C:
   - SDA: D2
   - SCL: D1

4. Sube el firmware al ESP8266 transmisor.

### Paso 3: configurar el receptor (vehículo)

1. Abre el archivo code/receiver_car/receiver_car.ino.
2. Asegúrate de que los pines del L298N coincidan con la configuración actual:
   - IN1 = D1
   - IN2 = D2
   - IN3 = D3
   - IN4 = D4
3. Sube el firmware al ESP8266 del vehículo.

## Características destacadas

- Baja latencia en la comunicación por ESP-NOW.
- Pantalla OLED actualizada solo cuando cambia el gesto, reduciendo carga innecesaria en I2C.
- Frecuencia de envío ajustada para mantener un control más fluido.
- Failsafe que detiene el vehículo si se pierde la comunicación durante un tiempo determinado.

## Análisis y gráficas

Los scripts de la carpeta test permiten evaluar el rendimiento del sistema:

```bash
python test/eficiencia_i2c.py
python test/grafica_latencia_real.py
```

Estos scripts generan gráficas que ayudan a comparar el comportamiento del sistema original y el sistema optimizado.
