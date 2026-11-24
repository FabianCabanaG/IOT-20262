# IOT-20262

Entrega previa 2 - Escenario 5: sistema de comunicación Bluetooth con Arduino UNO, simulado en [Wokwi](https://wokwi.com/).

Presentado por: Diego Mauricio Ortiz Urriago, Santiago Rodriguez Camen, Jonathan Lopez Betancourt, Fabian Andres Cabana Gutierrez O

## Descripción

El Arduino UNO recibe comandos de un carácter y controla dos LEDs (verde y rojo) en función del comando recibido. Los comandos pueden llegar por dos canales equivalentes:

1. **Bluetooth (HC-05)** — vía `SoftwareSerial` en los pines D10 (RX) y D11 (TX). El HC-05 actúa como puente transparente: lo que se envía desde el celular por Bluetooth (perfil SPP) llega al Arduino como bytes UART.
2. **Monitor serial** — UART por hardware (D0/D1, vía USB). Como Wokwi no simula el módulo HC-05, este canal reemplaza al celular durante las pruebas.

Se usa `SoftwareSerial` para el HC-05 en vez de D0/D1 porque esos pines están conectados al conversor USB: compartirlos impediría cargar el programa y usar el monitor serial al mismo tiempo.

### Comandos disponibles

| Comando | Acción |
|---|---|
| `V` | Enciende LED verde (D2), apaga el rojo |
| `R` | Enciende LED rojo (D3), apaga el verde |
| `A` | Enciende ambos LEDs |
| `0` | Apaga ambos LEDs |
| Otro | Se notifica como comando inválido, los LEDs conservan su estado |

Los comandos aceptan mayúsculas y minúsculas. Después de cada comando, el Arduino responde por ambos canales con el número de comando, la acción ejecutada y el estado actual de los LEDs.

## Configuración de pines

| Pin | Función |
|---|---|
| D2 | LED verde (con resistencia de 220 Ω) |
| D3 | LED rojo (con resistencia de 220 Ω) |
| D10 | RX del `SoftwareSerial` (conectado al TX del HC-05) |
| D11 | TX del `SoftwareSerial` (conectado al RX del HC-05, vía divisor de tensión) |

Comunicación serial: 9600 baudios, 8 bits de datos, sin paridad, 1 bit de parada.

## Archivos del repositorio

- `sketch.ino` — código fuente del Arduino.
- `diagram.json` — diagrama del circuito para la simulación en Wokwi (Arduino UNO, 2 LEDs y 2 resistencias de 220 Ω).

## Cómo probarlo

### En Wokwi
1. Abrir el proyecto en [wokwi.com](https://wokwi.com/projects/481321369351005185).
2. Iniciar la simulación.
3. En el monitor serial, enviar un carácter (`V`, `R`, `A` o `0`) y observar la respuesta y el cambio de estado de los LEDs.

### En hardware real
1. Armar el circuito según `diagram.json`: LED verde en D2 y LED rojo en D3, cada uno con resistencia de 220 Ω a GND.
2. Conectar el módulo HC-05: TX del HC-05 a D10, RX del HC-05 a D11 (con divisor de tensión, ya que el HC-05 trabaja a 3.3V).
3. Cargar `sketch.ino` en el Arduino UNO.
4. Emparejar el HC-05 con un celular y enviar los comandos desde una app de terminal Bluetooth (perfil SPP), o usar el monitor serial por USB.
