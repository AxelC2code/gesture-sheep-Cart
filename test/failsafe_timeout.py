import matplotlib.pyplot as plt
import numpy as np

# Tiempos de simulación en milisegundos
tiempo = np.linspace(0, 2500, 1000)
corte_wifi = 1000 # El WiFi se cae en el milisegundo 1000

# Comportamiento Original (Sigue avanzando para siempre)
estado_original = np.where(tiempo >= 0, 1, 1)

# Comportamiento Optimizado (Failsafe actúa 500ms después del corte)
estado_optimizado = np.where(tiempo <= corte_wifi + 500, 1, 0)

plt.figure(figsize=(8, 4))
plt.step(tiempo, estado_original, label='Original (Sin Failsafe)', color='#d62728', linewidth=2, where='post')
plt.step(tiempo, estado_optimizado, label='Optimizado (Failsafe 500ms)', color='#2ca02c', linewidth=2, where='post')

plt.axvline(x=corte_wifi, color='gray', linestyle='--', label='Pérdida de Señal (Corte)')

plt.title('Respuesta del Vehículo ante Pérdida de Conexión', fontsize=12)
plt.xlabel('Tiempo (ms)', fontsize=10)
plt.ylabel('Estado del Motor (1 = Avanzando, 0 = Detenido)', fontsize=10)
plt.yticks([0, 1])
plt.legend(loc='lower left')
plt.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('./src/failsafe_test.png', dpi=300)
print("Gráfica generada: failsafe_test.png")