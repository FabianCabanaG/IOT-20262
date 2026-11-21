/*
 * Entrega previa 2 - Escenario 5
 * Implementación de un sistema de comunicación Bluetooth mediante Arduino
 *
 * Descripción:
 *   El Arduino UNO recibe comandos de un carácter por comunicación serial
 *   UART a 9600 baudios y controla dos LEDs:
 *     V -> enciende LED verde (D2) y apaga el rojo
 *     R -> enciende LED rojo (D3) y apaga el verde
 *     A -> enciende ambos LEDs
 *     0 -> apaga ambos LEDs
 *     Cualquier otro carácter -> se notifica como comando inválido
 *
 * Canales de entrada de comandos (ambos se procesan igual):
 *   1. Bluetooth HC-05 por SoftwareSerial en D10 (RX) y D11 (TX).
 *      El HC-05 es un puente transparente: lo que el celular envía por
 *      Bluetooth (perfil SPP) llega al Arduino como bytes UART.
 *   2. Monitor serial (UART por hardware, D0/D1 vía USB). En Wokwi, que no
 *      tiene módulo HC-05, este canal reemplaza al celular para las pruebas.
 *
 * Se usa SoftwareSerial para el HC-05 y no D0/D1 porque esos pines están
 * conectados al conversor USB del Arduino: compartirlos impediría cargar el
 * programa y ver el monitor serial al mismo tiempo.
 */

#include <SoftwareSerial.h>

// ---------------- Configuración de pines ----------------
const int PIN_LED_VERDE = 2;   // D2: LED verde (con resistencia de 220 Ω)
const int PIN_LED_ROJO  = 3;   // D3: LED rojo (con resistencia de 220 Ω)
const int PIN_BT_RX     = 10;  // D10: recibe datos del pin TX del HC-05
const int PIN_BT_TX     = 11;  // D11: envía datos al pin RX del HC-05 (con divisor de tensión)

// Velocidad de comunicación: 9600 baudios (valor por defecto del HC-05)
const long BAUDIOS = 9600;

// Puerto serial por software dedicado al módulo Bluetooth
SoftwareSerial bluetooth(PIN_BT_RX, PIN_BT_TX);

// Estado actual de los LEDs (se usa para reportarlo después de cada comando)
bool verdeEncendido = false;
bool rojoEncendido  = false;

// Contador de comandos recibidos, útil para numerar las pruebas en el monitor
unsigned long numeroComando = 0;

// Envía un texto a los dos canales (monitor serial y Bluetooth)
void responder(const String &mensaje) {
  Serial.println(mensaje);
  bluetooth.println(mensaje);
}

// Aplica el estado deseado a los LEDs y lo guarda en las variables de estado
void fijarLeds(bool verde, bool rojo) {
  verdeEncendido = verde;
  rojoEncendido  = rojo;
  digitalWrite(PIN_LED_VERDE, verde ? HIGH : LOW);
  digitalWrite(PIN_LED_ROJO,  rojo  ? HIGH : LOW);
}

// Devuelve el estado de los LEDs en el formato "D2: ON | D3: OFF"
String estadoLeds() {
  return String("Estado -> D2 (verde): ") + (verdeEncendido ? "ON" : "OFF") +
         " | D3 (rojo): " + (rojoEncendido ? "ON" : "OFF");
}

// Interpreta un comando recibido por cualquiera de los dos canales
void procesarComando(char comando, const char *origen) {
  // Se ignoran los fines de línea que agregan el monitor serial y las apps
  if (comando == '\r' || comando == '\n' || comando == ' ') {
    return;
  }

  numeroComando++;
  responder(String("[") + numeroComando + "] Comando recibido por " + origen +
            ": '" + comando + "'");

  // Se aceptan mayúsculas y minúsculas
  switch (toupper(comando)) {
    case 'V':
      fijarLeds(true, false);
      responder("Accion: LED verde ENCENDIDO");
      break;
    case 'R':
      fijarLeds(false, true);
      responder("Accion: LED rojo ENCENDIDO");
      break;
    case 'A':
      fijarLeds(true, true);
      responder("Accion: AMBOS LEDs ENCENDIDOS");
      break;
    case '0':
      fijarLeds(false, false);
      responder("Accion: AMBOS LEDs APAGADOS");
      break;
    default:
      // Validación de comandos incorrectos: los LEDs conservan su estado
      responder("ERROR: comando invalido. Use V, R, A o 0");
      break;
  }

  responder(estadoLeds());
  responder("UART: OK");
  responder("----------------------------------------");
}

void setup() {
  // Pines de los LEDs como salidas, inicialmente apagados
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_ROJO, OUTPUT);
  fijarLeds(false, false);

  // Inicialización de los dos puertos seriales a 9600 baudios
  Serial.begin(BAUDIOS);
  bluetooth.begin(BAUDIOS);

  responder("=== Sistema Bluetooth Arduino UNO listo ===");
  responder("UART a 9600 baudios, 8 bits de datos, sin paridad, 1 bit de parada");
  responder("Comandos: V = verde | R = rojo | A = ambos | 0 = apagar");
  responder(estadoLeds());
  responder("----------------------------------------");
}

void loop() {
  // Lectura no bloqueante: solo se lee cuando hay bytes en el buffer

  // Comandos que llegan desde el celular a través del HC-05
  if (bluetooth.available() > 0) {
    procesarComando((char)bluetooth.read(), "Bluetooth");
  }

  // Comandos que llegan desde el monitor serial (simulación en Wokwi)
  if (Serial.available() > 0) {
    procesarComando((char)Serial.read(), "Monitor serial");
  }
}
