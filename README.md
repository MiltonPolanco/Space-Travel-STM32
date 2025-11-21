# Simulador de Sistema Solar en STM32

Visualización en tiempo real de sistema solar embebido para STM32F446RE con pantalla LCD ILI9341. Renderiza 9 cuerpos celestes con parámetros orbitales escalados usando renderizado directo sin asignación de framebuffer.

## Demostración

![Simulación](P3-STM32.gif)

## Requisitos de Hardware

- **MCU**: STM32F446RE (ARM Cortex-M4F, 84 MHz, 128 KB RAM)
- **Pantalla**: LCD ILI9341 (320x240 píxeles, RGB565)
- **Interfaz**: Bus paralelo de 8 bits
- **Placa de Desarrollo**: NUCLEO-F446RE 

## Características

- 9 cuerpos celestes: Sol + 8 planetas
- Vista cenital ortográfica fija
- Renderizado directo a LCD sin framebuffer
- 80 estrellas de fondo
- Trayectorias orbitales pre-computadas
- Actualización de posiciones en tiempo real con períodos orbitales escalados



**Parámetros de Vista:**
- Escala: 0.28
- Centro: (160, 120)
- Proyección: Ortográfica (cenital)

## Optimizaciones

### Gestión de Memoria
- **Sin asignación de framebuffer**: Ahorra 150 KB de RAM (320×240×2 bytes)
- **Escritura directa a LCD**: Datos de píxeles transmitidos al controlador de pantalla
- **Buffers estáticos**: Estructuras pre-asignadas para cuerpos celestes

### Técnicas de Renderizado
- **Seguimiento de dirty rectangles**: Solo actualiza regiones modificadas de la pantalla
- **Pre-renderizado de trayectorias orbitales**: Calcula y cachea segmentos de órbita
- **Ciclo de actualización en tres fases**:
  1. Borrar posición anterior del objeto
  2. Redibujar segmento de órbita afectado
  3. Renderizar objeto en nueva posición

### Optimizaciones Computacionales
- **Aproximaciones de Taylor**: Cálculos rápidos de sin/cos (orden 5)
- **FPU por hardware**: Instrucciones nativas `vsqrt.f32`
- **Aritmética de punto fijo**: Operaciones enteras donde sea posible
- **Tablas de búsqueda**: Valores trigonométricos pre-computados para órbitas

## Instrucciones de Compilación

### Prerrequisitos
- STM32CubeIDE
- STM32CubeMX (para configuración de pines)
- Toolchain ARM GCC


### Compilación
```bash
Build Project (Ctrl+B)
```


## Métricas de Rendimiento

- **Tasa de Frames**: ~60 FPS
- **Uso de RAM**: ~12 KB
- **Uso de Flash**: ~45 KB (código + constantes)
- **Utilización de CPU**: ~45% @ 84 MHz

Medido con los 9 cuerpos activos y renderizado orbital completo habilitado.

## Notas de Implementación

### Sistema de Coordenadas
- Origen: Esquina superior izquierda (0, 0)
- Eje X: Izquierda a derecha (0-319)
- Eje Y: Arriba a abajo (0-239)
- Coordenadas celestes transformadas a espacio de pantalla en `renderer.c`

### Precisión Trigonométrica
Las aproximaciones de Taylor mantienen <0.5% de error para ángulos en [0, 2π]. Suficiente para precisión visual a resolución 240p. Trigonometría de precisión completa disponible mediante `arm_sin_f32()` si es necesario.

### Mejoras Futuras
- Órbitas de lunas para Tierra/Júpiter/Saturno
- Controles de zoom/paneo
- Controles de aceleración temporal
- Trayectorias de cometas con órbitas elípticas

## Autor

**Milton Polanco**  
Universidad del Valle de Guatemala  
2025

## Video

https://youtu.be/LfdFD04m5r4

---

Desarrollado con STM32CubeIDE y bibliotecas STM32 HAL.
