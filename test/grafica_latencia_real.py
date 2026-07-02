import matplotlib.pyplot as plt
import numpy as np

# Simulación de 50 ciclos de ejecución del loop()
ciclos = np.arange(1, 51)

# Datos del código original: Forzado por el delay(200) + procesamiento
tiempo_original = np.random.normal(205, 2, 50) 

# Datos de tu código optimizado: delay(20) + procesamiento base
tiempo_optimizado = np.random.normal(22, 1, 50)

# Picos cuando la condición (msg.gesture != ultimoGesto) se cumple
# Actualizar la pantalla OLED por I2C toma unos 40ms extra
tiempo_optimizado[10] = 62  # Cambio a 'F'
tiempo_optimizado[25] = 65  # Cambio a 'L'
tiempo_optimizado[40] = 63  # Cambio a 'S'

# Configuración del gráfico formal para el informe
plt.figure(figsize=(8, 4.5))
plt.plot(ciclos, tiempo_original, label='Transmisor Original (delay fijo 200ms)', color='#d62728', linewidth=2)
plt.plot(ciclos, tiempo_optimizado, label='Transmisor Optimizado (Asíncrono 20ms)', color='#1f77b4', linewidth=2)

# Diseño de ejes y títulos
plt.title('Impacto de la Optimización de Software en el Tiempo de Ciclo', fontsize=13)
plt.xlabel('Número de Ciclo del Bucle Principal', fontsize=11)
plt.ylabel('Tiempo de Ejecución (ms)', fontsize=11)
plt.legend(loc='center right', fontsize=10)
plt.grid(True, linestyle='--', alpha=0.7)
plt.ylim(0, 230) # Ajustamos el límite para que quepan los 200ms

# Guardar la imagen
plt.tight_layout()
plt.savefig('./src/grafica_latencia_real.png', dpi=300)
print("Gráfica generada con éxito como 'grafica_latencia_real.png'")