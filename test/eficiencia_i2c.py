import matplotlib.pyplot as plt
import numpy as np

# Constantes de hardware (tiempos en milisegundos)
T_MPU = 2      # Tiempo de lectura del giroscopio
T_WIFI = 1     # Tiempo de transmisión ESP-NOW
T_OLED = 40    # Tiempo de dibujado en pantalla I2C
T_SIMULACION = 60000 # 60 segundos de operación

# ==========================================
# MODELO 1: Código Original
# loop = MPU + WIFI + OLED (siempre) + Delay(200)
# ==========================================
tiempo_ciclo_original = T_MPU + T_WIFI + T_OLED + 200 
ciclos_originales_en_60s = int(T_SIMULACION / tiempo_ciclo_original)
llamadas_oled_original = ciclos_originales_en_60s

# ==========================================
# MODELO 2: Código Optimizado (Axel & Pamela)
# loop = MPU + WIFI + Delay(20) + OLED (Solo si cambia)
# ==========================================
# Asumimos 15 cambios de gesto en 1 minuto de uso real
cambios_gesto = 15 
tiempo_gastado_en_cambios = cambios_gesto * (T_MPU + T_WIFI + T_OLED + 20)
tiempo_restante = T_SIMULACION - tiempo_gastado_en_cambios

tiempo_ciclo_vacio = T_MPU + T_WIFI + 20
ciclos_vacios = int(tiempo_restante / tiempo_ciclo_vacio)

ciclos_optimizados_totales = ciclos_vacios + cambios_gesto
llamadas_oled_optimizado = cambios_gesto

# ==========================================
# IMPRESIÓN DE DATOS DUROS (Para tu análisis)
# ==========================================
print(f"--- RESULTADOS DEL MODELO (60 Segundos) ---")
print(f"Frecuencia de Muestreo Original: {1000/tiempo_ciclo_original:.2f} Hz")
print(f"Frecuencia de Muestreo Optimizada: {1000/tiempo_ciclo_vacio:.2f} Hz")
print(f"Llamadas I2C redundantes evitadas: {llamadas_oled_original - llamadas_oled_optimizado}")

# ==========================================
# GENERACIÓN DE GRÁFICA DE EFICIENCIA
# ==========================================
labels = ['Código Original', 'Código Optimizado']
llamadas_oled = [llamadas_oled_original, llamadas_oled_optimizado]

plt.figure(figsize=(7, 5))
barras = plt.bar(labels, llamadas_oled, color=['#d62728', '#2ca02c'], width=0.5)

plt.title('Saturación del Bus I2C (Actualizaciones de Pantalla en 60s)', fontsize=12, fontweight='bold')
plt.ylabel('Cantidad de Actualizaciones OLED', fontsize=11)

# Añadir el número exacto sobre cada barra
for barra in barras:
    yval = barra.get_height()
    plt.text(barra.get_x() + barra.get_width()/2, yval + 5, int(yval), ha='center', va='bottom', fontweight='bold')

plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.tight_layout()
plt.savefig('./src/eficiencia_i2c.png', dpi=300)